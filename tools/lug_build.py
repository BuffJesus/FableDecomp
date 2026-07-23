#!/usr/bin/env python3
"""lug_build.py — WRITER for Fable TLC `data\\Sound\\*.lug` audio segment banks.

Companion to tools/parse_lug.py (read side; docs/LUG_AUDIO_FORMAT.md) — reuses its
parser and never modifies it. Codec encode side lives in tools/xbadpcm.py (ffmpeg has
no adpcm_ima_xbox encoder). Always work on COPIES of retail files.

Strategy: surgical raw-byte rebuild. Block payloads are carried verbatim except:
  - LHAudioWaveData is re-emitted from the unique RIFF blobs (original offset order,
    replacements/additions spliced in) — offsets ripple deterministically;
  - each 652-byte sample record gets riffSize/riffOffset re-patched (and, on replace/
    add, the wave-format mirror fields at +292 refreshed from the new RIFF fmt chunk);
  - the criteria block, on ADD --tag, gets a new entry appended (count bumped);
    otherwise byte-verbatim.
Alias records (several ids -> one RIFF) follow the blob: replacing an aliased clip
changes it for every id that points at it. Empty slots (riffSize=0) are preserved.

CLI:
  python lug_build.py IDENTITY bank.lug [--out out.lug]     # rebuild, byte-compare
  python lug_build.py IDENTITY-ALL <dir>                    # oracle over all .lug in dir
  python lug_build.py REPLACE bank.lug ID new.wav out.lug [--encode] [--raw]
  python lug_build.py ADD bank.lug new.wav out.lug [--encode] [--raw]
         [--path devpath] [--group name] [--tag CRITERIA;TAG] [--like ID]

new.wav: a RIFF/WAVE. By default it is canonicalized to a minimal retail-style RIFF
(fmt + data only; PCM16 fmt size 16, Xbox ADPCM fmt size 20). --raw embeds the file
verbatim. --encode transcodes a PCM16 input to Xbox ADPCM (xbadpcm.py) first.
Every write is auto-validated by a full re-parse (parse_lug cross-check walk).
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_lug
import xbadpcm

REC = parse_lug.SAMPLE_REC_SIZE           # 652
PATHF = parse_lug.PATH_FIELD              # 260


class Bank:
    """Raw-level model of one .lug: block list + unique blob table."""

    def __init__(self, path):
        self.src = parse_lug.LugBank(path)      # validated read model
        self.src.validate()
        d = self.src.data
        self.block_order = []                   # (name, raw_payload_bytes)
        for name, off, size in self.src.blocks:
            self.block_order.append([name, bytes(d[off:off + size])])
        # unique blobs in original offset order
        keys = sorted(set((s.riff_off, s.riff_size)
                          for s in self.src.samples if s.riff_size))
        self.blob_keys = keys                   # ordered list of original (off,size)
        self.blobs = {k: bytes(d[self.src.wave_off + k[0]:
                                 self.src.wave_off + k[0] + k[1]]) for k in keys}
        # raw 652-byte records, mutable
        to = dict((n, (o, s)) for n, o, s in self.src.blocks)["LHAudioBankSampleTable"][0]
        self.tbl_count, self.tbl_unk = struct.unpack_from("<HH", d, to)
        self.records = [bytearray(d[to + 4 + i * REC:to + 4 + (i + 1) * REC])
                        for i in range(self.tbl_count)]

    # -- record field helpers -------------------------------------------------
    @staticmethod
    def rec_key(rec):
        size, off = struct.unpack_from("<II", rec, PATHF + 8)
        return (off, size)

    @staticmethod
    def rec_id(rec):
        return struct.unpack_from("<I", rec, PATHF)[0]

    def rec_by_id(self, sid):
        for r in self.records:
            if self.rec_id(r) == sid:
                return r
        raise SystemExit(f"no sample with id {sid}")

    # -- mutations -------------------------------------------------------------
    def replace_blob(self, sid, riff):
        rec = self.rec_by_id(sid)
        key = self.rec_key(rec)
        if key[1] == 0:
            raise SystemExit(f"sample {sid} is an empty slot — use ADD semantics instead")
        self.blobs[key] = riff                  # key stays (identity of the slot);
        touched = []                            # sizes/offsets are recomputed on emit
        for r in self.records:                  # refresh fmt mirror on every alias
            if self.rec_key(r) == key:
                self._patch_fmt(r, riff)
                touched.append(self.rec_id(r))
        return touched

    def add_sample(self, riff, dev_path, group, like_id):
        template = self.rec_by_id(like_id) if like_id else \
            next(r for r in reversed(self.records) if self.rec_key(r)[1])
        rec = bytearray(template)
        new_id = max(self.rec_id(r) for r in self.records) + 1
        new_wave_id = max(struct.unpack_from("<I", r, PATHF + 4)[0]
                          for r in self.records) + 1
        rec[0:PATHF] = dev_path.encode("latin1")[:PATHF - 1].ljust(PATHF, b"\0")
        struct.pack_into("<II", rec, PATHF, new_id, new_wave_id)
        if group is not None:
            rec[320:576] = group.encode("latin1")[:255].ljust(256, b"\0")
        # a fresh key that cannot collide with an existing blob key: appended last
        key = (1 << 32, new_id)                 # sorts after all real (off,size)
        struct.pack_into("<II", rec, PATHF + 8, len(riff), 0)  # patched on emit
        self._patch_fmt(rec, riff)
        self.records.append(rec)
        self.blob_keys.append(key)
        self.blobs[key] = riff
        self._synthetic = getattr(self, "_synthetic", {})
        self._synthetic[key] = rec              # emit maps key->rec for offset patch
        self.tbl_count += 1
        # header unk tracks the non-empty count in most retail ambience banks;
        # keep that invariant if it held, else preserve verbatim
        old_nonempty = sum(1 for r in self.records[:-1] if self.rec_key(r)[1])
        if self.tbl_unk == old_nonempty:
            self.tbl_unk = old_nonempty + 1
        return new_id

    def add_criteria(self, tag, sample_ids):
        for i, (name, payload) in enumerate(self.block_order):
            if name == "LHAudioBankCriteiaInfo":
                (count,) = struct.unpack_from("<I", payload, 0)
                t = tag.encode("latin1")
                entry = (struct.pack("<I", len(t)) + t +
                         struct.pack(f"<I{len(sample_ids)}I", len(sample_ids), *sample_ids))
                self.block_order[i][1] = struct.pack("<I", count + 1) + payload[4:] + entry
                return
        raise SystemExit("bank has no criteria block")

    @staticmethod
    def _patch_fmt(rec, riff):
        fmt, _data = xbadpcm.parse_wav(riff)
        struct.pack_into("<HHIIHH", rec, PATHF + 32, fmt["tag"], fmt["channels"],
                         fmt["rate"], fmt["avg_bps"], fmt["block_align"], fmt["bits"])

    # -- emit --------------------------------------------------------------------
    def build(self):
        # wave payload + new offsets
        newoff, wave = {}, bytearray()
        for k in self.blob_keys:
            newoff[k] = len(wave)
            wave += self.blobs[k]
        # patch records
        synth = getattr(self, "_synthetic", {})
        recs = bytearray()
        for r in self.records:
            rr = bytearray(r)
            k = None
            for sk, sr in synth.items():
                if sr is r:
                    k = sk
                    break
            if k is None:
                k = self.rec_key(r)
            if k[1] or k in synth:              # skip empty slots (keep verbatim)
                blob = self.blobs[k]
                struct.pack_into("<II", rr, PATHF + 8, len(blob), newoff[k])
            recs += rr
        table = struct.pack("<HH", self.tbl_count, self.tbl_unk) + bytes(recs)
        out = bytearray(b"LiOnHeAd")
        for name, payload in self.block_order:
            if name == "LHAudioWaveData":
                payload = bytes(wave)
            elif name == "LHAudioBankSampleTable":
                payload = table
            out += name.encode("latin1").ljust(32, b"\0")
            out += struct.pack("<I", len(payload))
            out += payload
        return bytes(out)


# ------------------------------------------------------------------ input WAV prep

def load_input_riff(path, encode=False, raw=False):
    blob = open(path, "rb").read()
    fmt, data = xbadpcm.parse_wav(blob)         # validates RIFF/WAVE + fmt/data
    if encode:
        if fmt["tag"] != 1:
            raise SystemExit("--encode needs a PCM16 input WAV")
        return xbadpcm.encode_wav(blob)
    if raw:
        return blob
    # canonicalize to minimal retail-style RIFF
    if fmt["tag"] == 1:
        if fmt["bits"] != 16:
            raise SystemExit(f"PCM input must be 16-bit (got {fmt['bits']})")
        n = len(data) // 2
        return xbadpcm.build_pcm_riff(struct.unpack(f"<{n}h", data[:n * 2]),
                                      fmt["channels"], fmt["rate"])
    if fmt["tag"] == 0x0069:
        return xbadpcm.build_xadpcm_riff(data, fmt["channels"], fmt["rate"])
    raise SystemExit(f"unsupported input codec {fmt['tag']:#x} (PCM16 or Xbox ADPCM)")


# ------------------------------------------------------------------------ validate

def revalidate(out_path, orig_bank, expect_changed_ids=(), expect_new=0):
    nb = parse_lug.LugBank(out_path)
    nb.validate()                               # zero-slack walk + table/wave cross-check
    print(f"  re-parse: OK ({len(nb.samples)} samples, "
          f"{len(orig_bank.samples)} before, criteria {len(nb.criteria)})")
    assert len(nb.samples) == len(orig_bank.samples) + expect_new
    old = {s.id: s for s in orig_bank.samples}
    changed = []
    for s in nb.samples:
        if s.id in old and s.riff_size:
            a, b = old[s.id], s
            if a.riff_size != b.riff_size or \
               orig_bank.data[orig_bank.wave_off + a.riff_off:
                              orig_bank.wave_off + a.riff_off + a.riff_size] != nb.riff_bytes(b):
                changed.append(s.id)
    print(f"  clip diff vs original: changed={changed} "
          f"(expected {sorted(expect_changed_ids)})")
    assert set(changed) == set(expect_changed_ids), "unexpected clip changes"
    return nb


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 1
    mode = argv[1].upper()
    opts = [a for a in argv[2:] if a.startswith("--")]
    args = [a for a in argv[2:] if not a.startswith("--")]

    def opt_val(name, default=None):
        for i, a in enumerate(argv):
            if a == name and i + 1 < len(argv):
                return argv[i + 1]
        return default

    if mode == "IDENTITY":
        b = Bank(args[0])
        out = b.build()
        ok = out == b.src.data
        print(f"[IDENTITY] {os.path.basename(args[0])}: {len(b.src.data):,} -> "
              f"{len(out):,}  byte-exact={ok}")
        ov = opt_val("--out")
        if ov:
            open(ov, "wb").write(out)
        return 0 if ok else 2

    if mode == "IDENTITY-ALL":
        bad = 0
        for f in sorted(os.listdir(args[0])):
            if not f.lower().endswith(".lug"):
                continue
            b = Bank(os.path.join(args[0], f))
            out = b.build()
            ok = out == b.src.data
            print(f"[IDENTITY] {f:<32} {len(out):>12,} B  byte-exact={ok}")
            bad += 0 if ok else 1
        print(f"IDENTITY-ALL: {'ALL PASS' if not bad else f'{bad} FAILURES'}")
        return 0 if not bad else 2

    if mode == "REPLACE":
        bank_path, sid, wav, out_path = args[0], int(args[1]), args[2], args[3]
        riff = load_input_riff(wav, encode="--encode" in opts, raw="--raw" in opts)
        b = Bank(bank_path)
        touched = b.replace_blob(sid, riff)
        open(out_path, "wb").write(b.build())
        print(f"[REPLACE] id {sid} (aliases touched: {touched}) -> {out_path}")
        nb = revalidate(out_path, b.src, expect_changed_ids=touched)
        new = nb.riff_bytes(next(s for s in nb.samples if s.id == sid))
        print(f"  replaced clip byte-exact to input RIFF: {new == riff} ({len(riff)} B)")
        return 0

    if mode == "ADD":
        bank_path, wav, out_path = args[0], args[1], args[2]
        riff = load_input_riff(wav, encode="--encode" in opts, raw="--raw" in opts)
        b = Bank(bank_path)
        like = opt_val("--like")
        new_id = b.add_sample(
            riff,
            dev_path=opt_val("--path", f"S:\\Forge\\{os.path.basename(wav)}"),
            group=opt_val("--group"),
            like_id=int(like) if like else None)
        tag = opt_val("--tag")
        if tag:
            b.add_criteria(tag, [new_id])
        open(out_path, "wb").write(b.build())
        print(f"[ADD] new sample id {new_id}"
              + (f" + criteria tag {tag!r}" if tag else "") + f" -> {out_path}")
        nb = revalidate(out_path, b.src, expect_new=1)
        new = nb.riff_bytes(next(s for s in nb.samples if s.id == new_id))
        print(f"  added clip byte-exact to input RIFF: {new == riff} ({len(riff)} B)")
        if tag:
            hit = [t for t in nb.criteria if t[0] == tag]
            print(f"  criteria tag resolves: {hit}")
        return 0

    print(f"unknown mode {mode}")
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
