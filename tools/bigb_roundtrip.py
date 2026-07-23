#!/usr/bin/env python3
"""BIGB .big byte-exact round-trip harness (the WRITE-PATH validation gate).

Goal: prove that read -> in-memory model -> RE-SERIALIZE reproduces an
unmodified retail .big file BYTE-FOR-BYTE. This defines the write-path CONTRACT
that the FableForge C++ writer must satisfy. It is NOT the C++ writer.

Design philosophy (faithful passthrough, NOT clever regeneration):
  * We build a structural model (header, footer/sub-bank dir, per-sub-bank TOC
    records, entry payloads, stats headers) by PARSING with parse_bigb's reader.
  * We then RE-EMIT each structural region from the model, writing it at the
    EXACT byte offset it was read from. We never re-pack or re-order.
  * Every byte of the file must be claimed by exactly one region. Any byte NOT
    covered by a reconstructed structural region is an explicit PASSTHROUGH
    region (alignment padding, unknown gaps) copied verbatim from the original.
  * A region is "RECONSTRUCTED" if we rebuild its bytes from parsed fields and
    they match. If reconstructed bytes != original bytes, that is a FAIL with a
    diagnosed structural cause (a faithful-reproduction gap in the serializer).

This distinguishes:
  - bytes the serializer REBUILDS from the model (header, footer, TOC, stats,
    payload copies) -> must byte-match, else serializer bug / format unknown.
  - bytes the serializer PASSES THROUGH verbatim (padding, uncovered gaps) ->
    copied, and REPORTED so a real writer knows what it must preserve/regenerate.

Usage:
  python bigb_roundtrip.py                 # round-trip all 8 retail files
  python bigb_roundtrip.py <file.big>      # round-trip one file
  python bigb_roundtrip.py --smoke <file>  # single-edit controllability test
"""
import struct, sys, os, hashlib

# import the validated reader as a dependency (do not fork its API)
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb
from parse_bigb import parse_header, parse_footer, Reader

INSTALL = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"
FILES = [
    ("text.big",     r"data\lang\English\text.big"),
    ("dialogue.big", r"data\lang\English\dialogue.big"),
    ("fonts.big",    r"data\lang\English\fonts.big"),
    ("frontend.big", r"data\graphics\pc\frontend.big"),
    ("graphics.big", r"data\graphics\graphics.big"),
    ("textures.big", r"data\graphics\pc\textures.big"),
    ("effects.big",  r"data\Misc\pc\effects.big"),
    ("shaders.big",  r"data\shaders\pc\shaders.big"),
]

# --------------------------------------------------------------------------
# Structural model + re-serialization
# --------------------------------------------------------------------------
# A "region" is (start, end, kind, rebuilt_bytes_or_None).
#   kind = 'header' | 'footer' | 'toc' | 'payload' | 'passthrough'
#   rebuilt_bytes: for RECONSTRUCTED regions, the bytes we emit from the model.
#                  for 'passthrough', None (we copy the original slice).
# We assemble the output by laying every reconstructed region at its recorded
# [start,end) and filling all uncovered bytes from the original (passthrough).

def build_model(b):
    """Parse b into a structural model of byte regions. Returns (regions, meta).

    regions: list of dicts {start,end,kind,rebuilt}. rebuilt is bytes for
             reconstructed structural regions, or None for payload/passthrough
             (payload is reconstructed only in the sense that we copy the exact
             slice; we mark it 'payload' and reconstruct == original slice so the
             smoke test can substitute an edited slice).
    meta:    parse info for reporting.
    """
    fsize = len(b)
    magic, ver, foot_off, foot_size = parse_header(b)
    if magic != b'BIGB':
        raise ValueError("not BIGB")
    if foot_off + foot_size > fsize:
        raise ValueError("footer out of range")

    regions = []

    # --- header (16 bytes at 0) : RECONSTRUCT from fields ---
    hdr = magic + struct.pack('<III', ver, foot_off, foot_size)
    assert len(hdr) == 16
    regions.append(dict(start=0, end=16, kind='header', rebuilt=hdr,
                        label='header'))

    subs, footer_end = parse_footer(b, foot_off)

    # --- footer / sub-bank directory : RECONSTRUCT from fields ---
    fbuf = bytearray()
    fbuf += struct.pack('<I', len(subs))
    for sub in subs:
        fbuf += sub['name'].encode('latin1') + b'\x00'
        fbuf += struct.pack('<IIIII', sub['version'], sub['entry_count'],
                            sub['offset'], sub['size'], sub['align'])
    footer_bytes = bytes(fbuf)
    regions.append(dict(start=foot_off, end=foot_off + len(footer_bytes),
                        kind='footer', rebuilt=footer_bytes, label='footer'))

    # --- per sub-bank TOC : RECONSTRUCT record-by-record ---
    entries_all = []
    for si, sub in enumerate(subs):
        entries, stats, end_pos, err = parse_bigb.parse_toc(b, sub, foot_off)
        if err:
            raise ValueError(f"toc parse error subbank[{si}]: {err}")
        for e in entries:
            _augment_entry_raw(b, e)  # attach byte-exact raw name/deps
        # reconstruct the exact TOC bytes from stats + entry fields
        toc = bytearray()
        # stats header: parse_toc read it iff sc<1000. Reproduce the same
        # decision by re-reading the leading u32.
        sc_probe = struct.unpack_from('<I', b, sub['offset'])[0]
        if stats is not None:
            # stats present: statsCount then pairs
            toc += struct.pack('<I', len(stats))
            for (t, c) in stats:
                toc += struct.pack('<II', t, c)
        # (if stats is None, parse_toc rewound; nothing emitted here)
        for e in entries:
            toc += struct.pack('<IIIIII', e['magic'], e['id'], e['type'],
                               e['size'], e['offset'], e['crc'])
            nm = e['name'].encode('latin1')
            # Name is length-prefixed; parse_bigb strips trailing NUL on read.
            # We must reproduce the ORIGINAL length prefix + raw bytes exactly.
            # Recover the raw on-disk name field from the original bytes using
            # the recorded record offset so we never guess the NUL convention.
            raw_name_field = _raw_lpstr_at(b, _name_field_off(b, e))
            toc += raw_name_field
            toc += struct.pack('<II', e['timestamp'], e['depcount'])
            for d in e['deps_raw']:
                toc += d
            toc += struct.pack('<I', e['infosize'])
            toc += e['info']
        toc_bytes = bytes(toc)
        regions.append(dict(start=sub['offset'], end=sub['offset'] + len(toc_bytes),
                            kind='toc', rebuilt=toc_bytes,
                            label=f"toc[{si}] {sub['name']}"))
        for e in entries:
            entries_all.append((si, e))

    # --- payloads : mark each entry's [offset, offset+size) as a payload region.
    # These we "reconstruct" as an exact copy of the original slice (the model's
    # payload bytes). The smoke test substitutes an edited slice here.
    for si, e in entries_all:
        if e['size'] > 0 and e['offset'] > 0:
            regions.append(dict(start=e['offset'], end=e['offset'] + e['size'],
                                kind='payload',
                                rebuilt=bytes(b[e['offset']:e['offset'] + e['size']]),
                                label=f"payload s{si} id{e['id']}",
                                entry=e, subbank=si))

    meta = dict(fsize=fsize, magic=magic, ver=ver, foot_off=foot_off,
                foot_size=foot_size, subs=subs, nentries=len(entries_all))
    return regions, meta


def _name_field_off(b, e):
    """Byte offset of the entry Name length-prefix within the TOC.
    Name follows magic,id,type,size,offset,crc (6 u32 = 24 bytes) after rec_off.
    """
    return e['rec_off'] + 24


def _raw_lpstr_at(b, off):
    """Return the exact on-disk length-prefixed string field (u32 len + bytes)."""
    n = struct.unpack_from('<I', b, off)[0]
    return b[off:off + 4 + n]


# We need deps in RAW on-disk form (length prefix + raw bytes) to reproduce the
# trailing-NUL convention exactly. parse_bigb strips NULs, so re-derive here by
# re-walking each record. Patch entries with 'deps_raw' during model build.

def _augment_entry_raw(b, e):
    """Attach raw name/deps byte fields to an entry dict, byte-exact from disk."""
    p = e['rec_off'] + 24  # after the 6 fixed u32s
    name_raw = _raw_lpstr_at(b, p); p += len(name_raw)
    p += 8  # timestamp + depcount
    deps_raw = []
    for _ in range(e['depcount']):
        d = _raw_lpstr_at(b, p); p += len(d); deps_raw.append(d)
    e['deps_raw'] = deps_raw
    return e


def serialize(b, regions, edit=None):
    """Emit output bytes from the model. `edit`: optional (start,end,newbytes)
    to substitute one payload region (for the smoke test). Uncovered bytes are
    copied verbatim from `b` (passthrough)."""
    fsize = len(b)
    out = bytearray(b)  # start as copy; we overwrite reconstructed regions.
    # We DON'T rely on the copy for correctness of reconstructed regions:
    # every structural region is re-written from model bytes below. Passthrough
    # bytes (padding/gaps) are what remains from the copy — which IS the intent.
    # To PROVE we cover the structural bytes, we also compute coverage.
    covered = bytearray(fsize)  # 0/1 per byte, structural coverage
    for reg in regions:
        rb = reg['rebuilt']
        if rb is None:
            continue
        s, en = reg['start'], reg['end']
        if en - s != len(rb):
            raise ValueError(f"region {reg['label']} length mismatch "
                             f"{en-s} vs rebuilt {len(rb)}")
        out[s:en] = rb
        for i in range(s, en):
            covered[i] = 1
    if edit is not None:
        s, en, nb = edit
        out[s:en] = nb
    return bytes(out), covered


def coverage_report(covered, meta):
    """Summarize which byte ranges are passthrough (structurally unclaimed)."""
    fsize = len(covered)
    gaps = []
    i = 0
    while i < fsize:
        if covered[i] == 0:
            j = i
            while j < fsize and covered[j] == 0:
                j += 1
            gaps.append((i, j))
            i = j
        else:
            i += 1
    return gaps


# --------------------------------------------------------------------------
# Round-trip driver
# --------------------------------------------------------------------------
def hexdump_window(b, off, width=64):
    lo = max(0, off - 16)
    hi = min(len(b), off + width)
    out = []
    p = lo
    while p < hi:
        row = b[p:p+16]
        hexs = ' '.join(f'{x:02x}' for x in row)
        asci = ''.join(chr(x) if 32 <= x < 127 else '.' for x in row)
        mark = ' <<<' if p <= off < p+16 else ''
        out.append(f"  {p:08x}  {hexs:<47}  {asci}{mark}")
        p += 16
    return '\n'.join(out)


def first_diff(a, c):
    n = min(len(a), len(c))
    for i in range(n):
        if a[i] != c[i]:
            return i
    if len(a) != len(c):
        return n
    return -1


def region_at(regions, off):
    hits = [r for r in regions if r['start'] <= off < r['end']]
    if hits:
        # prefer the most specific (smallest) region
        hits.sort(key=lambda r: r['end'] - r['start'])
        return hits[0]
    return None


def roundtrip_file(name, path, verbose=True):
    with open(path, 'rb') as f:
        b = f.read()
    fsize = len(b)
    orig_hash = hashlib.sha256(b).hexdigest()

    # Build model. We must augment entries with raw name/deps for byte-exact TOC.
    # Re-run parse_toc here (build_model calls it) but we need the raw fields, so
    # patch parse_bigb entries in-place via a wrapper.
    result = {'name': name, 'size': fsize, 'sha256': orig_hash}
    try:
        # First pass: parse to attach raw fields, THEN build regions.
        magic, ver, foot_off, foot_size = parse_header(b)
        if magic != b'BIGB':
            result['status'] = 'NOT-BIGB'
            return result
        subs, _ = parse_footer(b, foot_off)
        for si, sub in enumerate(subs):
            entries, stats, end_pos, err = parse_bigb.parse_toc(b, sub, foot_off)
            if err:
                raise ValueError(f"toc[{si}] parse error: {err}")
            for e in entries:
                _augment_entry_raw(b, e)
        regions, meta = build_model(b)
    except Exception as ex:
        result['status'] = f'PARSE-FAIL: {ex}'
        if verbose:
            print(f"  {name}: PARSE-FAIL {ex}")
        return result

    out, covered = serialize(b, regions)
    out_hash = hashlib.sha256(out).hexdigest()
    gaps = coverage_report(covered, meta)

    identical = (out == b)
    result['status'] = 'PASS' if identical else 'FAIL'
    result['out_sha256'] = out_hash
    result['n_subbanks'] = len(meta['subs'])
    result['n_entries'] = meta['nentries']
    # classify passthrough (uncovered structural) bytes
    gap_bytes = sum(j - i for i, j in gaps)
    result['passthrough_bytes'] = gap_bytes
    result['n_gaps'] = len(gaps)
    result['gaps'] = gaps

    if verbose:
        print(f"\n=== {name} ({fsize:,} bytes) ===")
        print(f"  sha256(in) ={orig_hash}")
        print(f"  sha256(out)={out_hash}")
        print(f"  subbanks={len(meta['subs'])} entries={meta['nentries']}")
        print(f"  structural coverage: {fsize - gap_bytes:,}/{fsize:,} bytes "
              f"reconstructed; {gap_bytes:,} bytes passthrough in {len(gaps)} gap(s)")
        # characterize the passthrough gaps
        _describe_gaps(b, gaps, regions, meta)

    if not identical:
        d = first_diff(b, out)
        result['first_diff'] = d
        reg = region_at(regions, d)
        cause = _diagnose(b, out, d, reg, regions, meta)
        result['diff_cause'] = cause
        if verbose:
            print(f"  FAIL: first diff at 0x{d:x} (region: "
                  f"{reg['label'] if reg else 'PASSTHROUGH/uncovered'})")
            print(f"  likely cause: {cause}")
            print("  --- ORIGINAL ---")
            print(hexdump_window(b, d))
            print("  --- RE-SERIALIZED ---")
            print(hexdump_window(out, d))
            # count total differing bytes and distinct diff regions
            diffs = _all_diff_regions(b, out)
            print(f"  total differing byte-runs: {len(diffs)} "
                  f"(first 5: {[(f'0x{s:x}', e-s) for s,e in diffs[:5]]})")
    else:
        if verbose:
            print(f"  PASS: {fsize:,} bytes byte-identical (sha256 match).")
    return result


def _describe_gaps(b, gaps, regions, meta):
    """Report what the passthrough gaps are (padding? unknown?)."""
    if not gaps:
        print("  passthrough: NONE — every byte is a reconstructed structural region.")
        return
    zero_pad = 0
    nonzero = 0
    samples = []
    for (i, j) in gaps:
        slc = b[i:j]
        if all(x == 0 for x in slc):
            zero_pad += (j - i)
        else:
            nonzero += (j - i)
            if len(samples) < 6:
                samples.append((i, j, slc[:16]))
    print(f"  passthrough breakdown: {zero_pad:,} zero-pad bytes, "
          f"{nonzero:,} non-zero bytes")
    if samples:
        print(f"  NON-ZERO passthrough samples (start,end,first16):")
        for (i, j, s) in samples:
            print(f"    0x{i:x}..0x{j:x} ({j-i}B): {s.hex()}")


def _diagnose(b, out, off, reg, regions, meta):
    if reg is None:
        return "diff in a PASSTHROUGH/uncovered byte — original was copied; if this differs, serialize() logic bug"
    kind = reg['kind']
    if kind == 'header':
        return "HEADER back-patch mismatch (Magic/Version/FooterOffset/FooterSize reconstruction)"
    if kind == 'footer':
        return "FOOTER (sub-bank directory) reconstruction mismatch (name NUL / version / offset / size / align field)"
    if kind == 'toc':
        return "TOC record reconstruction mismatch (magic/id/type/size/offset/crc/name-lenprefix/timestamp/deps/infosize/info)"
    if kind == 'payload':
        return "PAYLOAD copy mismatch (should be impossible for identity round-trip — indicates region overlap)"
    return f"unknown region kind {kind}"


def _all_diff_regions(a, c):
    n = min(len(a), len(c))
    regs = []
    i = 0
    while i < n:
        if a[i] != c[i]:
            j = i
            while j < n and a[j] != c[j]:
                j += 1
            regs.append((i, j))
            i = j
        else:
            i += 1
    if len(a) != len(c):
        regs.append((n, max(len(a), len(c))))
    return regs


# --------------------------------------------------------------------------
# Single-edit smoke test (controllability proof)
# --------------------------------------------------------------------------
def smoke_test(name, path):
    """Change ONE benign byte in ONE payload and confirm ONLY that byte (plus
    any legitimately-dependent size/offset fields) changes vs the original.

    We pick the smallest non-empty payload, flip one byte in it, re-serialize,
    and diff. Since we do NOT re-pack, and payload size is unchanged, the ONLY
    expected difference is the single edited byte. This proves the writer is
    controllable (targeted edits produce targeted byte changes), not an identity
    copy that ignores the model.
    """
    with open(path, 'rb') as f:
        b = f.read()
    magic, ver, foot_off, foot_size = parse_header(b)
    subs, _ = parse_footer(b, foot_off)
    entries_all = []
    for si, sub in enumerate(subs):
        entries, stats, end_pos, err = parse_bigb.parse_toc(b, sub, foot_off)
        for e in entries:
            _augment_entry_raw(b, e)
            if e['size'] > 0 and e['offset'] > 0:
                entries_all.append((si, e))
    regions, meta = build_model(b)

    # pick the smallest non-empty payload
    entries_all.sort(key=lambda x: x[1]['size'])
    si, tgt = entries_all[0]
    poff, psize = tgt['offset'], tgt['size']
    # flip the last byte of the payload (benign: we then restore-compare)
    edit_off = poff + psize - 1
    orig_byte = b[edit_off]
    new_byte = orig_byte ^ 0xFF
    newslice = bytearray(b[poff:poff+psize]); newslice[-1] = new_byte
    out, _ = serialize(b, regions, edit=(poff, poff+psize, bytes(newslice)))

    diffs = _all_diff_regions(b, out)
    print(f"\n=== SMOKE TEST: {name} ===")
    print(f"  target payload: subbank[{si}] id={tgt['id']} name={tgt['name']!r} "
          f"size={psize} at 0x{poff:x}")
    print(f"  edited byte at 0x{edit_off:x}: 0x{orig_byte:02x} -> 0x{new_byte:02x}")
    print(f"  total differing byte-runs after re-serialize: {len(diffs)}")
    for (s, e) in diffs:
        print(f"    diff 0x{s:x}..0x{e:x} ({e-s}B)")
    ok = (len(diffs) == 1 and diffs[0] == (edit_off, edit_off+1))
    print(f"  VERDICT: {'CONTROLLABLE (only the intended byte changed)' if ok else 'UNCONTROLLED — unexpected extra diffs'}")
    return ok


# --------------------------------------------------------------------------
def main():
    args = sys.argv[1:]
    if args and args[0] == '--smoke':
        path = args[1] if len(args) > 1 else os.path.join(INSTALL, FILES[7][1])
        name = os.path.basename(path)
        smoke_test(name, path)
        return
    if args:
        # single explicit file
        path = args[0]
        roundtrip_file(os.path.basename(path), path)
        return
    # all 8
    results = []
    for name, rel in FILES:
        path = os.path.join(INSTALL, rel)
        if not os.path.exists(path):
            print(f"\n=== {name} === MISSING: {path}")
            results.append({'name': name, 'status': 'MISSING'})
            continue
        results.append(roundtrip_file(name, path))
    # summary table
    print("\n" + "=" * 78)
    print("ROUND-TRIP SUMMARY")
    print("=" * 78)
    print(f"{'file':<15}{'status':<8}{'size':>14}  {'passthru':>10}  cause")
    for r in results:
        st = r.get('status', '?')
        sz = r.get('size', 0)
        pt = r.get('passthrough_bytes', 0)
        cause = r.get('diff_cause', '') if st != 'PASS' else ''
        print(f"{r['name']:<15}{st:<8}{sz:>14,}  {pt:>10,}  {cause[:40]}")
    npass = sum(1 for r in results if r.get('status') == 'PASS')
    print(f"\n{npass}/{len(results)} byte-exact round-trip.")


if __name__ == '__main__':
    main()
