#!/usr/bin/env python3
"""lut_write.py - controlled WRITE path for Fable: TLC `.lut` audio banks.

Companion to the READ-ONLY parser `tools/parse_lut.py`. Provides:

  * identity round-trip  : read -> (rebuild) -> write, byte-exact on unmodified .lut
  * same-size clip replace: swap one clip's embedded RIFF/WAVE blob for another of
                            IDENTICAL byte length (no offset/TOC ripple - the safe case)
  * size-change replace   : swap a clip for a RIFF of a DIFFERENT length, recomputing
                            the tail LHAudioBankLookupTable directory + TocOffset + all
                            downstream offsets (the full repack)

All layout facts are re-derived from `parse_lut.walk()`; this module NEVER modifies
parse_lut.py. Every write is meant to be re-validated by re-reading through parse_lut.

--------------------------------------------------------------------------------
TAIL DIRECTORY - FULLY DECODED (this run, all 4 retail .lut)
--------------------------------------------------------------------------------
The format doc (docs/LUT_AUDIOBANK_FORMAT.md) flagged the tail LHAudioBankLookupTable
"hash bucket array" as only partially decoded and the main writer risk. That risk is
now CLOSED: the tail is a plain, deterministic (index, record_size, offset) array, not
a hash table. Layout, from `TocOffset`:

  TocOffset (header +0x28) points to a 44-byte PREAMBLE.
    - On the 3 large banks the preamble is 44 zero bytes.
    - On single-clip Dialogue2.lut the LookupOffset field aliases into it; still
      reproduced verbatim because we copy the whole tail region for identity.
  +44  char[32]  "LHAudioBankLookupTable\0..."   (NUL-padded name)
  +32  u32  LookupDataSize = 20 + (Count-1)*12   (VERIFIED exact on all 4 files)
       u32  MaxEntries     (500 Dialogue / 1000 Script) - copied verbatim, engine cap
       u32  Count          = number of clips (== walk count)
       u32  flag           = 1
       u32  LookupOffset   = offset of the SECOND clip's record = clips[1].rec_off-0x2c
       u32  reserved       = 0
  then  (Count-1) x 12-byte records, one per clip EXCEPT the first (index 1 is implicit,
        always at rec_off 0x2c), in ascending file-position order:
            u32 index         (the clip's 1-based ordinal <N>)
            u32 record_size   = 36 (clip header) + riff_span
            u32 offset        = clip rec_off - 0x2c   (relative to first clip record)
  then  a FIXED 560-byte TRAILER, byte-identical across all 4 retail files
        (md5 dc639577347ce41e08b8d9f6656532cd): "LHFileSegmentBankInfo" +
        "\\Generated bank for Fable" + "NoFPNameYet" + padding, ending 04 00 00 00.

Because every tail field is a pure function of the clip layout (offsets/sizes) plus
two verbatim constants (the 44-byte preamble and the 560-byte trailer, both taken from
the source file being edited), a size-changing repack is fully reproducible.

Usage:
  python lut_write.py IDENTITY  <file.lut>
  python lut_write.py REPLACE   <file.lut> <N> <newclip.wav> <out.lut>   # same-size only unless --allow-resize
  python lut_write.py REPLACE   <file.lut> <N> <newclip.wav> <out.lut> --allow-resize
"""
import struct, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_lut as P

CLIP_HDR = P.CLIP_HDR   # 36
FIRST_CLIP_OFF = 0x2c
LUT_NAME = b"LHAudioBankLookupTable"


# ---------------------------------------------------------------------------
# tail-directory model
# ---------------------------------------------------------------------------

def locate_tail(b, toc_off):
    """Return dict describing the tail directory of an on-disk .lut buffer `b`."""
    marker = b.find(LUT_NAME, toc_off if toc_off > 4 else 0)
    if marker < 0:
        raise ValueError("LHAudioBankLookupTable marker not found")
    preamble = b[toc_off:marker]                      # 44 bytes (verbatim)
    name_field = b[marker:marker + 32]                # 32-byte NUL-padded name
    fld = struct.unpack_from('<6I', b, marker + 32)   # 6 header u32
    lookup_data_size, max_entries, count, flag, lookup_off, reserved = fld
    arr_off = marker + 32 + 24
    n_recs = count - 1
    array_bytes = b[arr_off:arr_off + n_recs * 12]
    trailer = b[arr_off + n_recs * 12:]
    return dict(marker=marker, preamble=preamble, name_field=name_field,
                lookup_data_size=lookup_data_size, max_entries=max_entries,
                count=count, flag=flag, lookup_off=lookup_off, reserved=reserved,
                arr_off=arr_off, array_bytes=array_bytes, trailer=trailer)


def build_tail(clips, name_field, max_entries, flag, trailer, single_lookup_off=0):
    """Rebuild the tail directory (name+header+array+trailer) from clip layout.

    `clips` is the parse_lut.walk() list, already in final file-position order, whose
    rec_off / riff_span reflect the (possibly resized) clip region being written.
    Returns bytes. The tail proper begins at the LHAudioBankLookupTable marker, which
    sits immediately after the last clip record (end_of_last_clip). TocOffset is stored
    as (marker_pos - 44): it points 44 bytes *before* the marker, into the final clip's
    data - it is NOT a separate padding region.
    """
    count = len(clips)
    n_recs = count - 1
    lookup_data_size = 20 + n_recs * 12                    # verified formula
    # LookupOffset = second clip's relative offset (rec_off - 0x2c); if only 1 clip,
    # reproduce whatever the source had via preamble aliasing -> use clips[0] fallback.
    if count >= 2:
        lookup_off = clips[1]['rec_off'] - FIRST_CLIP_OFF
    else:
        # single-clip banks (Dialogue2.lut): retail stores LookupOffset == TocOffset
        # (the field aliases into the preamble; there is no 2nd clip to point at).
        lookup_off = single_lookup_off
    hdr = struct.pack('<6I', lookup_data_size, max_entries, count, flag, lookup_off, 0)
    recs = bytearray()
    for c in clips[1:]:                                     # skip implicit first clip
        rec_size = CLIP_HDR + c['riff_span']
        rel_off = c['rec_off'] - FIRST_CLIP_OFF
        recs += struct.pack('<3I', c['index'], rec_size, rel_off)
    return bytes(name_field) + hdr + bytes(recs) + bytes(trailer)


# ---------------------------------------------------------------------------
# whole-file rebuild
# ---------------------------------------------------------------------------

def rebuild(b, replace=None, add=None):
    """Rebuild a full .lut buffer from the parsed clip layout of `b`.

    replace: optional (target_index, new_record_bytes) where new_record_bytes is the
             full clip RECORD (36-byte header + new RIFF). If None, an identity rebuild.
    add:     optional (new_index, new_record_bytes) - APPEND one brand-new clip record
             after the last retail clip. new_index must be > every existing index so the
             tail lookup array (file-position order == index order) stays sorted, and the
             record's 36-byte header +0x00 Index field must already equal new_index.
             Count/LookupDataSize are recomputed. NOTE: retail MaxEntries (500/1000) is
             already < retail Count (12134/5310), so MaxEntries is NOT a count cap;
             engine acceptance of a grown bank is [unproven] until a live test.
    Returns the new bytes.
    """
    clips, stop, toc = P.walk(b)
    if not clips:
        raise ValueError("no clips walked")
    tail = locate_tail(b, toc)

    # The region [0x2c, end_of_last_clip) is the packed clip records. We reconstruct it
    # record-by-record so a resized replacement ripples all downstream offsets.
    header = b[0:FIRST_CLIP_OFF]                     # 44-byte file header (Magic..TocOffset)

    out = bytearray()
    out += header
    new_clips = []                                   # rebuilt layout for tail regen
    cur = FIRST_CLIP_OFF
    for c in clips:
        if replace is not None and c['index'] == replace[0]:
            rec_bytes = replace[1]
            # validate the replacement record: 36-byte header then a valid RIFF/WAVE
            if len(rec_bytes) < CLIP_HDR + 12 or rec_bytes[CLIP_HDR:CLIP_HDR+4] != b'RIFF':
                raise ValueError("replacement record is not a 36-byte header + RIFF/WAVE")
            riff_span = P.riff_size(rec_bytes, CLIP_HDR)
            if riff_span is None:
                raise ValueError("replacement RIFF invalid (missing RIFF/WAVE tags)")
        else:
            # copy the original record verbatim: header (36) + RIFF (span)
            rec_bytes = b[c['rec_off']:c['riff_off'] + c['riff_span']]
            riff_span = c['riff_span']
        out += rec_bytes
        new_clips.append(dict(index=c['index'], rec_off=cur,
                              riff_off=cur + CLIP_HDR, riff_span=riff_span))
        cur += CLIP_HDR + riff_span

    if add is not None:
        new_index, rec_bytes = add
        if any(c['index'] >= new_index for c in clips):
            raise ValueError("add index %d must exceed every existing index (max %d)"
                             % (new_index, max(c['index'] for c in clips)))
        if len(rec_bytes) < CLIP_HDR + 12 or rec_bytes[CLIP_HDR:CLIP_HDR+4] != b'RIFF':
            raise ValueError("added record is not a 36-byte header + RIFF/WAVE")
        if struct.unpack_from('<I', rec_bytes, 0)[0] != new_index:
            raise ValueError("added record header Index field != %d" % new_index)
        riff_span = P.riff_size(rec_bytes, CLIP_HDR)
        if riff_span is None:
            raise ValueError("added RIFF invalid (missing RIFF/WAVE tags)")
        out += rec_bytes
        new_clips.append(dict(index=new_index, rec_off=cur,
                              riff_off=cur + CLIP_HDR, riff_span=riff_span))
        cur += CLIP_HDR + riff_span

    end_of_clips = cur                               # == LHAudioBankLookupTable marker pos
    new_toc = end_of_clips - len(tail['preamble'])   # TocOffset points 44 bytes before marker
    # back-patch TocOffset in the header
    struct.pack_into('<I', out, 0x28, new_toc)

    # rebuild tail from the new clip layout (tail begins AT the marker; the 44-byte
    # preamble is the tail of the final clip, already emitted above)
    new_tail = build_tail(new_clips, tail['name_field'],
                          tail['max_entries'], tail['flag'], tail['trailer'],
                          single_lookup_off=new_toc)
    out += new_tail
    return bytes(out)


# ---------------------------------------------------------------------------
# clip-replacement helpers
# ---------------------------------------------------------------------------

def make_record(orig_record_header36, new_riff_bytes):
    """Assemble a new clip record: keep the original 36-byte clip header verbatim
    (const fields, 3D-audio floats, sentinel - all preserved per the format doc),
    swap in the new RIFF/WAVE blob."""
    assert len(orig_record_header36) == CLIP_HDR
    return orig_record_header36 + new_riff_bytes


def get_record_header(b, clips, index):
    for c in clips:
        if c['index'] == index:
            return b[c['rec_off']:c['rec_off'] + CLIP_HDR], c
    raise KeyError(f"clip index {index} not found")


# ---------------------------------------------------------------------------
# validation
# ---------------------------------------------------------------------------

def diff_clips(a_bytes, b_bytes):
    """Compare two .lut buffers clip-by-clip. Returns (changed_indices, identical_indices,
    structural_ok, notes)."""
    ca, _, _ = P.walk(a_bytes)
    cb, _, _ = P.walk(b_bytes)
    notes = []
    ia = {c['index']: c for c in ca}
    ib = {c['index']: c for c in cb}
    if set(ia) != set(ib):
        notes.append(f"index set differs: only-in-A={sorted(set(ia)-set(ib))[:5]} "
                     f"only-in-B={sorted(set(ib)-set(ia))[:5]}")
    changed, identical = [], []
    for idx in sorted(set(ia) & set(ib)):
        pa = a_bytes[ia[idx]['riff_off']:ia[idx]['riff_off'] + ia[idx]['riff_span']]
        pb = b_bytes[ib[idx]['riff_off']:ib[idx]['riff_off'] + ib[idx]['riff_span']]
        if pa == pb:
            identical.append(idx)
        else:
            changed.append(idx)
    return changed, identical, (len(ca) == len(cb)), notes


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def cmd_identity(path):
    b = open(path, 'rb').read()
    out = rebuild(b)
    ok = (out == b)
    print(f"[IDENTITY] {os.path.basename(path)}: {len(b):,} bytes -> rebuilt {len(out):,} bytes  "
          f"byte-exact={ok}")
    if not ok:
        # locate first diff
        n = min(len(b), len(out))
        for i in range(n):
            if b[i] != out[i]:
                print(f"   first diff at 0x{i:x}: orig={b[i]:02x} new={out[i]:02x}")
                break
        if len(b) != len(out):
            print(f"   length differs: orig={len(b)} new={len(out)}")
    return ok


def cmd_replace(path, index, wav_path, out_path, allow_resize):
    b = open(path, 'rb').read()
    clips, _, _ = P.walk(b)
    hdr36, tgt = get_record_header(b, clips, index)
    new_riff = open(wav_path, 'rb').read()
    # sanity: new blob must be a RIFF/WAVE
    if P.riff_size(new_riff, 0) is None:
        print(f"[REPLACE] ERROR: {wav_path} is not a RIFF/WAVE file"); return False
    old_span = tgt['riff_span']
    new_span = len(new_riff)
    same_size = (new_span == old_span)
    print(f"[REPLACE] clip N={index}: old RIFF span={old_span}  new RIFF span={new_span}  "
          f"same_size={same_size}")
    if not same_size and not allow_resize:
        print("   size differs and --allow-resize not set -> refusing (safe same-size mode only).")
        return False
    new_rec = make_record(hdr36, new_riff)
    out = rebuild(b, replace=(index, new_rec))
    open(out_path, 'wb').write(out)
    print(f"   wrote {out_path} ({len(out):,} bytes)")
    return True


def main():
    if len(sys.argv) < 3:
        print(__doc__); return
    mode = sys.argv[1].upper()
    if mode == 'IDENTITY':
        cmd_identity(sys.argv[2])
    elif mode == 'REPLACE':
        path, index, wav, out = sys.argv[2], int(sys.argv[3]), sys.argv[4], sys.argv[5]
        allow = '--allow-resize' in sys.argv
        cmd_replace(path, index, wav, out, allow)
    else:
        print(f"unknown mode {mode}"); print(__doc__)


if __name__ == '__main__':
    main()
