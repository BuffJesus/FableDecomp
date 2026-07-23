#!/usr/bin/env python3
r"""save_install.py -- install an edited Fable:TLC .sav into a profile so the in-game
LOAD list shows it.

WHY THIS IS NEEDED
------------------
Fable enumerates loadable *manual* saves from the profile registry `Profile.bin`, NOT
by scanning the save directory. `Profile.bin` is a "FablePro" container:

    0x00  8   magic "FablePro"
    0x08  u32 0
    0x0C  u32 total_data_len   (== byte offset of the 4-byte trailer == filelen - 4)
    0x10  ...   plaintext persist-script, `Name value;\r\n` grammar (latin1)
    EOF-4 u32 trailer = CCRC::Calc(seed=0, file[0:total_data_len])   (reflected CRC-32,
          poly 0xEDB88320, NO final inversion -- same primitive as the .sav trailer)

There is NO zlib and NO padding: the payload is raw text and the file ends immediately
after the 4-byte trailer (bytes_after_trailer == 0 on all real profiles).

The manual-save registry inside the text is 50 fixed slots:

    SaveGameNames1  "Manual - Save1";
    SaveGameNames2  "";
    ... SaveGameNames50 "";

A slot is "occupied" (shown in the LOAD list) iff its string is non-empty. The string is
BOTH the on-screen label AND the on-disk filename: `SaveGameNames1 "Manual - Save1"` maps
to the file `<profileDir>\Manual - Save1`. No GUID / slot-id / embedded save-id is used --
the key is the exact filename. (AutoSave / AutoSave.qs are reserved special slots and are
NOT listed here; they show up in-game regardless.)

The per-slot metadata the LOAD screen shows (hero name, chapter/region, playtime,
thumbnail) is read lazily FROM THE .SAV ITSELF at enumerate time -- it is not stored in
Profile.bin. So a valid edited .sav (which keeps a well-formed HEADER section) displays
correctly with no extra work.

Because the trailer is a hard-enforced signature (CUserProfileManager::VerifySignature @
retail 0x00409730 validates it for BOTH FablePro and FableSav containers, unconditionally),
you MUST recompute it after editing the text -- a hand-edited Profile.bin with a stale
trailer is rejected and the whole profile fails to load.

TRANSFER PROCEDURES
-------------------
(a) Overwrite an existing occupied slot: copy the edited .sav over the file named by that
    slot's existing string. Profile.bin is UNCHANGED (its entry still points at the same
    filename; the entry does not checksum the .sav). Simplest, zero registry risk.
(b) New slot: copy the edited .sav to a new filename, add `SaveGameNamesN "<name>"` in the
    first empty slot, recompute the Profile.bin trailer. This script does (b) (and (a) when
    --slot names an occupied slot / existing filename).

USAGE
    python save_install.py inspect  <profileDir>
    python save_install.py install  <edited.sav> <profileDir> <slotName> [--out-dir DIR]
        <slotName> = "new"                 -> first empty SaveGameNames slot, file named
                                              "Manual - SaveN" (N = slot index)
                   = "<existing name>"     -> overwrite that occupied slot's file in place
                                              (Profile.bin only re-signed if unchanged text
                                               -> still rewritten to the staging dir as a copy)
                   = "Some New Name"       -> new slot, file + label = "Some New Name"

By default NOTHING under the real profile dir is written: the tool STAGES a corrected
Profile.bin + the renamed .sav + an INSTALL.txt into --out-dir (default
<profileDir>\..\save_install_staged) for the user to copy in. Pass --apply to write the
real profile dir directly (makes a Profile.bin.bak first).
"""
import sys, os, struct, shutil

MAGIC = b"FablePro"
N_SLOTS = 50

# ---- seed-0 reflected CRC-32 (poly 0xEDB88320, no final inversion) -----------------
def _build_table():
    t = []
    for i in range(256):
        c = i
        for _ in range(8):
            c = (c >> 1) ^ 0xEDB88320 if (c & 1) else (c >> 1)
        t.append(c & 0xFFFFFFFF)
    return t
_TABLE = _build_table()

def crc0(data):
    crc = 0
    for b in data:
        crc = (crc >> 8) ^ _TABLE[(crc ^ b) & 0xFF]
    return crc & 0xFFFFFFFF

# ---- Profile.bin container ---------------------------------------------------------
class Profile:
    def __init__(self, raw):
        if raw[:8] != MAGIC:
            raise ValueError("not a FablePro profile (bad magic %r)" % raw[:8])
        self.dw08 = struct.unpack_from("<I", raw, 0x08)[0]
        self.total_data_len = struct.unpack_from("<I", raw, 0x0C)[0]
        if self.total_data_len + 4 > len(raw):
            raise ValueError("total_data_len (%d) overruns file (%d)" %
                             (self.total_data_len, len(raw)))
        self.stored_trailer = struct.unpack_from("<I", raw, self.total_data_len)[0]
        self.text = raw[0x10:self.total_data_len]      # raw persist-script bytes
        self.trailing = raw[self.total_data_len + 4:]  # normally b"" (no padding)

    def verify(self):
        return crc0(self.text_prefix()) == self.stored_trailer

    def text_prefix(self):
        # everything the CRC covers = file[0:total_data_len] = header(16) + text
        return MAGIC + struct.pack("<II", self.dw08, self.total_data_len) + self.text

    def slots(self):
        """Return dict {index(1..50): value-string} parsed from the text."""
        out = {}
        t = self.text.decode("latin1")
        for line in t.split("\r\n"):
            line = line.strip()
            if line.startswith("SaveGameNames"):
                # SaveGameNamesN "value";
                head, _, rest = line.partition(" ")
                idx = int(head[len("SaveGameNames"):])
                val = rest
                if val.endswith(";"):
                    val = val[:-1]
                val = val.strip()
                if val.startswith('"') and val.endswith('"'):
                    val = val[1:-1]
                out[idx] = val
        return out

    def set_slot(self, idx, value):
        """Replace SaveGameNames<idx>'s value in the text (byte-exact splice)."""
        t = self.text.decode("latin1")
        marker = "SaveGameNames%d " % idx
        p = t.find(marker)
        if p < 0:
            raise ValueError("slot %d not found in profile text" % idx)
        # value runs to the ';' after the marker
        vstart = p + len(marker)
        semi = t.find(";", vstart)
        if semi < 0:
            raise ValueError("malformed slot %d (no ';')" % idx)
        new = t[:vstart] + '"%s"' % value + t[semi:]
        self.text = new.encode("latin1")

    def build(self):
        """Re-serialise: header + text + recomputed trailer. No padding (matches retail)."""
        total_data_len = 0x10 + len(self.text)
        head = MAGIC + struct.pack("<II", self.dw08, total_data_len)
        prefix = head + self.text
        trailer = crc0(prefix)
        return prefix + struct.pack("<I", trailer) + self.trailing


def load_profile(profile_dir):
    path = os.path.join(profile_dir, "Profile.bin")
    with open(path, "rb") as f:
        return Profile(f.read()), path


# ---- commands ----------------------------------------------------------------------
def cmd_inspect(profile_dir):
    prof, path = load_profile(profile_dir)
    print("Profile.bin:", path)
    print("  magic OK | dw08=%d | total_data_len=%d | file trailer=%08X | CRC verify=%s"
          % (prof.dw08, prof.total_data_len, prof.stored_trailer, prof.verify()))
    slots = prof.slots()
    occupied = {i: v for i, v in slots.items() if v}
    print("  manual-save registry: %d/%d slots occupied" % (len(occupied), len(slots)))
    for i in sorted(occupied):
        f = os.path.join(profile_dir, occupied[i])
        exists = os.path.isfile(f)
        sz = os.path.getsize(f) if exists else 0
        print('    SaveGameNames%-2d "%s"   file=%s (%d B)' %
              (i, occupied[i], "OK" if exists else "MISSING", sz))
    first_empty = next((i for i in range(1, N_SLOTS + 1) if not slots.get(i)), None)
    print("  first empty slot: %s" % first_empty)
    # list stray .sav-like files not in the registry
    print("  files in dir:")
    for fn in sorted(os.listdir(profile_dir)):
        fp = os.path.join(profile_dir, fn)
        if os.path.isfile(fp):
            tag = ""
            if fn in ("Profile.bin",):
                tag = "(profile)"
            elif fn.startswith("AutoSave"):
                tag = "(reserved autosave slot)"
            elif fn in occupied.values():
                tag = "(registered manual save)"
            else:
                tag = "(UNREGISTERED -> will NOT show in LOAD list)"
            print("    %-28s %8d B  %s" % (fn, os.path.getsize(fp), tag))


def cmd_install(sav_path, profile_dir, slot_name, out_dir=None, apply=False):
    prof, prof_path = load_profile(profile_dir)
    if not prof.verify():
        print("WARNING: source Profile.bin trailer does NOT verify -- proceeding anyway.")
    slots = prof.slots()

    with open(sav_path, "rb") as f:
        sav = f.read()
    if sav[:8] != b"FableSav":
        raise SystemExit("ERROR: %s is not a FableSave! container (magic %r)"
                         % (sav_path, sav[:10]))
    if len(sav) != 0x4B000:
        print("WARNING: .sav is %d bytes, expected 307200 (0x4B000)." % len(sav))

    # decide target slot + filename
    if slot_name == "new":
        idx = next((i for i in range(1, N_SLOTS + 1) if not slots.get(i)), None)
        if idx is None:
            raise SystemExit("ERROR: no empty manual-save slot free (all 50 used).")
        filename = "Manual - Save%d" % idx
        action = "new"
    else:
        # is it an existing occupied slot value? -> overwrite in place
        existing = {v: i for i, v in slots.items() if v}
        if slot_name in existing:
            idx = existing[slot_name]
            filename = slot_name
            action = "overwrite"
        else:
            # new named slot in first empty
            idx = next((i for i in range(1, N_SLOTS + 1) if not slots.get(i)), None)
            if idx is None:
                raise SystemExit("ERROR: no empty slot for new name '%s'." % slot_name)
            filename = slot_name
            action = "new"

    # build corrected profile (only changes for a new/renamed slot)
    prof.set_slot(idx, filename)
    new_profile_bytes = prof.build()
    # sanity: re-parse & verify
    chk = Profile(new_profile_bytes)
    assert chk.verify(), "rebuilt Profile.bin failed self CRC verify"
    assert chk.slots().get(idx) == filename, "slot write mismatch"

    print("Plan:")
    print("  action        : %s" % ("overwrite existing slot" if action == "overwrite"
                                     else "add new registry slot"))
    print("  slot index    : %d" % idx)
    print("  slot/filename : %r" % filename)
    print("  .sav source   : %s (%d B)" % (sav_path, len(sav)))
    print("  new Profile.bin: %d B, trailer=%08X (verified)"
          % (len(new_profile_bytes), chk.stored_trailer))

    if apply:
        bak = prof_path + ".bak"
        if not os.path.exists(bak):
            shutil.copy2(prof_path, bak)
            print("  backed up real Profile.bin -> %s" % bak)
        dest_sav = os.path.join(profile_dir, filename)
        with open(dest_sav, "wb") as f:
            f.write(sav)
        with open(prof_path, "wb") as f:
            f.write(new_profile_bytes)
        print("APPLIED to real profile dir:")
        print("  wrote %s" % dest_sav)
        print("  wrote %s" % prof_path)
        return

    # stage
    if out_dir is None:
        out_dir = os.path.join(os.path.dirname(os.path.abspath(profile_dir.rstrip("\\/"))),
                               "save_install_staged")
    os.makedirs(out_dir, exist_ok=True)
    staged_profile = os.path.join(out_dir, "Profile.bin")
    staged_sav = os.path.join(out_dir, filename)
    with open(staged_profile, "wb") as f:
        f.write(new_profile_bytes)
    with open(staged_sav, "wb") as f:
        f.write(sav)
    instr = os.path.join(out_dir, "INSTALL.txt")
    with open(instr, "w", encoding="utf-8") as f:
        f.write(
"""Fable TLC -- install edited save into the LOAD list
====================================================
Target profile dir:
  %s

To deploy (game must be CLOSED):
  1. Copy  "%s"  ->  "%s\\%s"
  2. Copy  "Profile.bin"  ->  "%s\\Profile.bin"
     (back up the existing Profile.bin first if you want a rollback)

The corrected Profile.bin adds registry slot %d = "%s" and has a recomputed
signature trailer, so the game accepts it and the save "%s" appears in the
in-game LOAD list. Its hero name / chapter / playtime / thumbnail are read from
the .sav itself, so the edited save displays correctly.

If you instead want to OVERWRITE an already-listed slot, you do NOT need the new
Profile.bin: just copy the edited .sav over the existing file of that name
(e.g. overwrite "Manual - Save1") -- the registry entry is unchanged and does
not checksum the .sav.
""" % (os.path.abspath(profile_dir), filename, os.path.abspath(profile_dir),
       filename, os.path.abspath(profile_dir), idx, filename, filename))
    print("STAGED (real profile dir untouched):")
    print("  %s" % staged_profile)
    print("  %s" % staged_sav)
    print("  %s" % instr)


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 1
    cmd = argv[1]
    if cmd == "inspect":
        cmd_inspect(argv[2])
    elif cmd == "install":
        apply = "--apply" in argv
        out_dir = None
        if "--out-dir" in argv:
            out_dir = argv[argv.index("--out-dir") + 1]
        pos = [a for a in argv[2:] if not a.startswith("--")
               and argv[argv.index(a) - 1] != "--out-dir"]
        if len(pos) < 3:
            raise SystemExit("usage: install <edited.sav> <profileDir> <slotName> "
                             "[--out-dir DIR] [--apply]")
        cmd_install(pos[0], pos[1], pos[2], out_dir=out_dir, apply=apply)
    else:
        print(__doc__)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
