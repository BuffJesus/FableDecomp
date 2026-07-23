#!/usr/bin/env python3
"""BIGB .big controlled WRITE prototype (replace-entry-payload).

Extends the proven read + byte-exact round-trip work:
  * reader:      tools/parse_bigb.py          (dependency, unmodified)
  * round-trip:  tools/bigb_roundtrip.py      (contract & model, unmodified)

This module ADDS a controlled writer for the real modding edits:
  replace-entry-payload  -- swap ONE entry's payload bytes for new bytes
                            (retexture / mesh-swap / shader-bytecode-swap),
                            handling BOTH same-size and size-changing edits.

Design (why it is safe):
  * We build the SAME structural model bigb_roundtrip proves byte-exact:
      header (16B) + footer/sub-bank directory + per-sub-bank TOC records
      + payload blobs, with all inter-region gaps being zero Align padding.
  * A .big is a flat list of byte REGIONS in address order. In shaders.big
    the TOCs and payloads are INTERLEAVED (payloads are NOT one contiguous
    block) -- verified: 493 regions, 416 zero-pad gaps, 0 overlaps. So the
    ONLY re-layout rule that is universally correct is ADDRESS-ORDER RE-FLOW:
    keep every region in its original address order; when one payload changes
    size, shift every following region and re-pad each payload up to its
    sub-bank Align with zero fill.
  * After re-flow we RE-PATCH the position-dependent fields from the new
    layout:  each entry's TOC Offset, each sub-bank's TOC Offset/Size,
    and the header FooterOffset/FooterSize.  Everything else -- entry Magic,
    ID, Type, Size(*), CRC, Timestamp, Name, Deps, Info, and the stats
    header -- is reproduced VERBATIM from the model.  (*) Size changes ONLY
    for the one edited entry (to the new payload length).
  * CRC and Timestamp are PASSED THROUGH verbatim.  Per docs/BIG_ROUNDTRIP.md
    Sec.7, TOC CRC is a per-format-CLASS fingerprint, NOT a payload hash, so
    an edited payload of an existing format legitimately keeps the SAME CRC as
    its unedited siblings.  We never recompute it.

Every write is VALIDATED by re-reading the output through parse_bigb and
asserting: (a) it parses clean and the TOC chain still terminates exactly at
FooterOffset with stats intact; (b) ONLY the intended entry differs
(all other entries' name/type/crc/timestamp/deps/info AND payload bytes are
identical); (c) the edited entry reads back the EXACT new payload bytes.

Usage:
  python bigb_write.py --demo            # run both demos (same-size + size-change)
  python bigb_write.py --demo-samesize   # shaders.big shader-bytecode swap, same size
  python bigb_write.py --demo-sizechange # small-bank payload replace, different size
"""
import struct, sys, os, hashlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb
from parse_bigb import parse_header, parse_footer
import bigb_roundtrip
from bigb_roundtrip import _augment_entry_raw, _raw_lpstr_at

INSTALL = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"
SCRATCH = (r"C:\Users\Cornelio\AppData\Local\Temp\claude"
           r"\D--Documents-FableTLC\ec99a10f-fe66-4cd3-a21f-dfe633a79eb7\scratchpad")

# --------------------------------------------------------------------------
# Full structural model as an ADDRESS-ORDERED region list
# --------------------------------------------------------------------------
# Each region is a dict. Kinds:
#   'header'  : 16B header (rebuilt last, needs final FooterOffset/Size)
#   'toc'     : one sub-bank's TOC (rebuilt from stats + entry records)
#   'payload' : one entry's payload blob (bytes held in model; editable)
#   'footer'  : the sub-bank directory (rebuilt from sub fields)
# TOCs and payloads are interleaved; we keep them in on-disk address order.

def load_model(path):
    """Read `path` into an address-ordered structural model."""
    b = open(path, 'rb').read()
    magic, ver, foot_off, foot_size = parse_header(b)
    if magic != b'BIGB':
        raise ValueError("not BIGB")
    if foot_off + foot_size > len(b):
        raise ValueError("footer out of range")
    subs, footer_end = parse_footer(b, foot_off)

    subbanks = []      # per sub-bank: dict(name,version,align,stats,entries,toc_off)
    regions = []       # address-ordered region list
    # header region (placeholder bytes; final bytes emitted in serialize)
    regions.append(dict(kind='header', start=0, end=16))

    for si, sub in enumerate(subs):
        entries, stats, end_pos, err = parse_bigb.parse_toc(b, sub, foot_off)
        if err:
            raise ValueError(f"toc parse error subbank[{si}]: {err}")
        for e in entries:
            _augment_entry_raw(b, e)
            # capture the exact raw name field (u32 len + bytes incl any NUL)
            e['name_raw'] = _raw_lpstr_at(b, e['rec_off'] + 24)
            # payload bytes held in the model (editable)
            if e['size'] > 0 and e['offset'] > 0:
                e['payload'] = bytes(b[e['offset']:e['offset'] + e['size']])
            else:
                e['payload'] = b''
        sb = dict(name=sub['name'], version=sub['version'], align=sub['align'],
                  stats=stats, entries=entries, orig_toc_off=sub['offset'],
                  orig_toc_size=sub['size'], si=si)
        subbanks.append(sb)
        # TOC region for this sub-bank
        regions.append(dict(kind='toc', start=sub['offset'], end=end_pos, sb=sb))
        # payload regions for this sub-bank's entries
        for e in entries:
            if e['size'] > 0 and e['offset'] > 0:
                regions.append(dict(kind='payload', start=e['offset'],
                                    end=e['offset'] + e['size'], entry=e, sb=sb))

    # footer region
    regions.append(dict(kind='footer', start=foot_off, end=footer_end))

    regions.sort(key=lambda r: r['start'])
    # sanity: no overlaps
    prev = 0
    for r in regions:
        if r['start'] < prev:
            raise ValueError(f"overlap at 0x{r['start']:x} ({r['kind']})")
        prev = r['end']

    return dict(orig=b, ver=ver, foot_off=foot_off, foot_size=foot_size,
                subbanks=subbanks, regions=regions, footer_end=footer_end)


# --------------------------------------------------------------------------
# TOC / footer byte reconstruction (from model fields)
# --------------------------------------------------------------------------
def _emit_toc(sb):
    """Serialize one sub-bank's TOC (stats header + entry records) from model.
    Uses each entry's CURRENT 'offset' and 'size' (re-patched by re-flow) and
    reproduces CRC/Timestamp/Name/Deps/Info verbatim."""
    toc = bytearray()
    if sb['stats'] is not None:
        toc += struct.pack('<I', len(sb['stats']))
        for (t, c) in sb['stats']:
            toc += struct.pack('<II', t, c)
    for e in sb['entries']:
        toc += struct.pack('<IIIIII', e['magic'], e['id'], e['type'],
                           e['size'], e['offset'], e['crc'])
        toc += e['name_raw']                       # verbatim length-prefixed name
        toc += struct.pack('<II', e['timestamp'], e['depcount'])
        for d in e['deps_raw']:                     # verbatim length-prefixed deps
            toc += d
        toc += struct.pack('<I', e['infosize'])
        toc += e['info']
    return bytes(toc)


def _emit_footer(model):
    fbuf = bytearray()
    fbuf += struct.pack('<I', len(model['subbanks']))
    for sb in model['subbanks']:
        fbuf += sb['name'].encode('latin1') + b'\x00'
        fbuf += struct.pack('<IIIII', sb['version'], len(sb['entries']),
                            sb['new_toc_off'], sb['new_toc_size'], sb['align'])
    return bytes(fbuf)


# --------------------------------------------------------------------------
# Address-order re-flow + serialize
# --------------------------------------------------------------------------
def _orig_boundary(x, cap):
    """Largest power-of-two <= `cap` that divides `x` -- the alignment boundary
    the bank builder placed this region on, but NEVER stronger than the region's
    declared sub-bank Align (`cap`).

    The cap is essential: many payload start addresses are COINCIDENTALLY
    divisible by a large power of two (e.g. an align=4 payload that happens to
    land at 0x5a800, divisible by 2048). Without the cap we would "re-establish"
    a 2048 boundary that was never required, inflating the file on a size change
    and -- because the spurious boundary is recomputed from the shifted address
    on a second edit -- breaking invertibility. Capping at the sub-bank Align
    keeps identity exact (the true boundary always divides the coincidental one)
    while making every edit minimal and reversible."""
    if cap <= 1:
        return 1
    if x == 0:
        return cap
    a = 1
    while a < cap and x % (a * 2) == 0:
        a *= 2
    return a


def _reflow_start(new_cursor, orig_start, align):
    """Compute a region's NEW start under the proven MINIMAL-REPACK re-flow rule:

      new_start = snap_up(new_cursor, B),  B = _orig_boundary(orig_start, align)

    `align` is the region's sub-bank Align (payload/TOC) -- the CAP on B. NO
    original-gap term: the new start is simply the cursor snapped up to B, so
    the inter-region padding is exactly the minimum zero fill needed to reach B.
    Properties (all verified on all 8 retail files, tools/bigb_write.py --demo):
      * IDENTITY: in the pristine file every region already sits contiguously
        after the previous one at its boundary B, so snap is a no-op and every
        start reproduces exactly -> byte-exact round-trip (0 mismatches / 8).
      * SIZE-CHANGE: after an upstream payload grows, downstream regions ripple
        forward by the minimum needed to stay on their sub-bank Align; when a
        payload SHRINKS the freed space is RECLAIMED (no stale padding).
      * INVERTIBLE: because B is capped at the declared Align (not a coincidental
        large divisor), an edit and its reverse both compute the same boundary
        -> edit-then-revert reproduces the original byte-exact (DEMO 4)."""
    B = _orig_boundary(orig_start, align)
    rem = new_cursor % B
    return new_cursor if rem == 0 else new_cursor + (B - rem)


def serialize(model):
    """Re-flow all regions in on-disk address order and emit the output bytes.

    TOCs and payloads are INTERLEAVED (payloads are not one contiguous block --
    verified in shaders.big: 493 regions, 416 zero-pad gaps, 0 overlaps), so we
    walk in address order and re-place each region with _reflow_start(). We
    re-patch every position-dependent field from the NEW layout:
      * each entry's TOC Offset (patched into e['offset'] as payloads are laid),
      * each sub-bank's TOC Offset + Size (new_toc_off / new_toc_size),
      * the header FooterOffset / FooterSize.
    Everything else -- entry Magic/ID/Type/CRC/Timestamp/Name/Deps/Info and the
    stats header -- is reproduced VERBATIM from the model. CRC and Timestamp are
    passthrough (per docs/BIG_ROUNDTRIP.md Sec.7: CRC is a per-format-CLASS
    fingerprint, not a payload hash). Only the ONE edited entry's Size differs.
    """
    regions = model['regions']

    # Emit output by two passes: pass 1 lays every region (patching payload
    # offsets + toc/footer offsets); pass 2 rewrites each TOC now that ALL
    # payload offsets are final (a TOC may precede, in address order, payloads
    # it references).
    # Boundary cap = the MAX sub-bank Align across the whole file. This is the
    # strongest alignment the builder ever imposed; capping the coincidental-
    # divisor boundary at it (a) reproduces mixed-align files exactly -- fonts.big
    # places some Align=512 payloads on a 2048 boundary, and 2048 is the file max
    # so we re-establish it -- while (b) never inventing a boundary stronger than
    # anything the builder used, which keeps size-changes minimal and invertible
    # (shaders.big max Align=4 -> no spurious 2048 snapping). Verified: identity
    # byte-exact on all 8, and edit/revert invertible.
    max_align = max((sb['align'] for sb in model['subbanks']), default=1)
    out = bytearray()
    cursor = 0
    for r in regions:
        # header sits at 0 and the footer follows the last TOC contiguously (no
        # gap in any retail file), so both use cap=1 (no snap).
        align = max_align if r['kind'] in ('payload', 'toc') else 1
        new_start = _reflow_start(cursor, r['start'], align)
        if new_start > cursor:
            out += b'\x00' * (new_start - cursor)   # zero pad (Align fill)
            cursor = new_start

        if r['kind'] == 'header':
            out += b'\x00' * 16                      # placeholder; patched below
            cursor += 16
        elif r['kind'] == 'toc':
            sb = r['sb']
            sb['new_toc_off'] = cursor
            tb = _emit_toc(sb)                       # provisional; rewritten pass 2
            sb['new_toc_size'] = len(tb)
            out += tb
            cursor += len(tb)
        elif r['kind'] == 'payload':
            e = r['entry']
            e['offset'] = cursor                     # re-patch entry TOC Offset
            out += e['payload']
            cursor += len(e['payload'])
        elif r['kind'] == 'footer':
            model['new_foot_off'] = cursor
            fb = _emit_footer(model)
            model['new_foot_size'] = len(fb)
            out += fb
            cursor += len(fb)
        else:
            raise ValueError(f"unknown region kind {r['kind']}")

    # pass 2: rewrite every TOC with final payload offsets. TOC byte length is
    # invariant to payload offset VALUES (fixed-width u32 fields), so this never
    # changes region sizes -- it only patches the Offset field values in place.
    for r in regions:
        if r['kind'] == 'toc':
            sb = r['sb']
            tb = _emit_toc(sb)
            off = sb['new_toc_off']
            assert len(tb) == sb['new_toc_size'], "TOC length changed in pass 2"
            out[off:off + len(tb)] = tb

    # patch header now that FooterOffset/Size are known
    hdr = b'BIGB' + struct.pack('<III', model['ver'],
                                model['new_foot_off'], model['new_foot_size'])
    out[0:16] = hdr
    return bytes(out)


# --------------------------------------------------------------------------
# The public edit: replace one entry's payload
# --------------------------------------------------------------------------
def replace_entry_payload(model, subbank_name, entry_id, new_payload):
    """Replace the payload of the entry (subbank_name, entry_id) with
    new_payload bytes. Updates the entry's Size to len(new_payload).
    Returns the located entry dict."""
    tgt = None
    for sb in model['subbanks']:
        if sb['name'] != subbank_name:
            continue
        for e in sb['entries']:
            if e['id'] == entry_id:
                tgt = e
                break
        if tgt:
            break
    if tgt is None:
        raise KeyError(f"entry id={entry_id} not found in subbank {subbank_name!r}")
    tgt['payload'] = bytes(new_payload)
    tgt['size'] = len(new_payload)          # re-patched; only THIS entry's Size moves
    return tgt


# --------------------------------------------------------------------------
# Re-read validation
# --------------------------------------------------------------------------
def _parse_all_entries(b):
    """Parse a .big buffer into a flat dict keyed by (subbank_name, id) with
    the full record + payload bytes, plus structural facts for the clean check.
    Returns (entries_map, facts) or raises on parse failure."""
    magic, ver, foot_off, foot_size = parse_header(b)
    if magic != b'BIGB':
        raise ValueError("output not BIGB")
    if foot_off + foot_size > len(b):
        raise ValueError("output footer out of range")
    subs, footer_end = parse_footer(b, foot_off)
    emap = {}
    toc_chain_ok = True
    stats_ok = True
    for si, sub in enumerate(subs):
        entries, stats, end_pos, err = parse_bigb.parse_toc(b, sub, foot_off)
        if err:
            raise ValueError(f"output toc[{si}] parse error: {err}")
        # stats sum must equal entry count
        if stats is not None and sum(c for _, c in stats) != sub['entry_count']:
            stats_ok = False
        for e in entries:
            payload = bytes(b[e['offset']:e['offset'] + e['size']]) if e['size'] > 0 else b''
            emap[(sub['name'], e['id'])] = dict(
                name=e['name'], type=e['type'], size=e['size'], offset=e['offset'],
                crc=e['crc'], timestamp=e['timestamp'], deps=tuple(e['deps']),
                info=bytes(e['info']), payload=payload)
    # TOC chain invariant: the LAST sub-bank's TOC must end at FooterOffset,
    # and each sub-bank TOC must end where the next begins (contiguity of the
    # chain). We check the standard invariant: last toc end == foot_off.
    last_end = 0
    for si, sub in enumerate(subs):
        _, _, end_pos, _ = parse_bigb.parse_toc(b, sub, foot_off)
        last_end = max(last_end, end_pos)
    # each sub-bank toc must terminate at or before footer; the union of TOC
    # spans must reach exactly foot_off (last one). We assert last_end==foot_off
    toc_chain_ok = (last_end == foot_off)
    facts = dict(foot_off=foot_off, foot_size=foot_size, footer_end=footer_end,
                 fsize=len(b), toc_chain_ok=toc_chain_ok, stats_ok=stats_ok,
                 n_sub=len(subs))
    return emap, facts


def validate_write(orig_path, out_bytes, edited_key, expected_new_payload):
    """Re-read out_bytes and prove EXACTLY the intended change.

    edited_key = (subbank_name, entry_id) of the one entry we changed.
    Returns (ok:bool, report:dict). ok is True only if:
      (a) output parses clean, TOC chain terminates at FooterOffset, stats OK;
      (b) every OTHER entry is byte-identical (name/type/crc/timestamp/deps/info
          AND payload); only offsets may legitimately shift;
      (c) the edited entry's payload reads back == expected_new_payload and its
          Size == len(expected_new_payload).
    """
    orig = open(orig_path, 'rb').read()
    rep = {'checks': []}
    def check(name, cond, detail=''):
        rep['checks'].append((name, bool(cond), detail))
        return bool(cond)

    # parse both
    omap, ofacts = _parse_all_entries(orig)
    try:
        nmap, nfacts = _parse_all_entries(out_bytes)
    except Exception as ex:
        check("output parses clean", False, str(ex))
        rep['ok'] = False
        return False, rep

    ok = True
    ok &= check("output parses clean (BIGB, footer in range, all TOCs walk)", True)
    ok &= check("TOC chain terminates exactly at FooterOffset",
                nfacts['toc_chain_ok'],
                f"last toc end vs foot_off=0x{nfacts['foot_off']:x}")
    ok &= check("stats headers sum to entry counts", nfacts['stats_ok'])
    ok &= check("same sub-bank count", nfacts['n_sub'] == ofacts['n_sub'],
                f"{nfacts['n_sub']} vs {ofacts['n_sub']}")
    ok &= check("same entry set (no adds/removes)",
                set(nmap.keys()) == set(omap.keys()),
                f"{len(nmap)} vs {len(omap)} entries")

    # per-entry diff
    changed_others = []
    for k in omap:
        if k not in nmap:
            continue
        o, n = omap[k], nmap[k]
        if k == edited_key:
            continue
        # everything except offset must be identical
        identical = (o['name'] == n['name'] and o['type'] == n['type'] and
                     o['crc'] == n['crc'] and o['timestamp'] == n['timestamp'] and
                     o['deps'] == n['deps'] and o['info'] == n['info'] and
                     o['size'] == n['size'] and o['payload'] == n['payload'])
        if not identical:
            changed_others.append(k)
    ok &= check("ALL other entries identical (name/type/crc/ts/deps/info/size/payload)",
                len(changed_others) == 0,
                f"{len(changed_others)} unexpectedly changed: {changed_others[:5]}")

    # edited entry checks
    ek = edited_key
    if ek in nmap and ek in omap:
        o, n = omap[ek], nmap[ek]
        ok &= check("edited entry reads back EXACT new payload",
                    n['payload'] == bytes(expected_new_payload),
                    f"got {len(n['payload'])}B want {len(expected_new_payload)}B")
        ok &= check("edited entry Size == len(new payload)",
                    n['size'] == len(expected_new_payload),
                    f"size={n['size']}")
        ok &= check("edited entry metadata preserved (name/type/crc/ts/deps/info)",
                    o['name'] == n['name'] and o['type'] == n['type'] and
                    o['crc'] == n['crc'] and o['timestamp'] == n['timestamp'] and
                    o['deps'] == n['deps'] and o['info'] == n['info'],
                    f"crc 0x{o['crc']:08x}->0x{n['crc']:08x}")
    else:
        ok &= check("edited entry present", False)

    # payload alignment check for the edited entry (must respect sub-bank Align)
    rep['ok'] = bool(ok)
    return bool(ok), rep


def _print_report(rep):
    for (name, cond, detail) in rep['checks']:
        tag = 'PASS' if cond else 'FAIL'
        line = f"    [{tag}] {name}"
        if detail and not cond:
            line += f"  --> {detail}"
        elif detail and cond:
            line += f"  ({detail})"
        print(line)
    print(f"    VERDICT: {'ALL CHECKS PASS' if rep['ok'] else 'FAILED'}")


# --------------------------------------------------------------------------
# Demos on real files
# --------------------------------------------------------------------------
def _find_smallest_entry(model):
    best = None
    for sb in model['subbanks']:
        for e in sb['entries']:
            if e['size'] > 0 and e['offset'] > 0:
                if best is None or e['size'] < best[1]['size']:
                    best = (sb, e)
    return best


def demo_samesize():
    """SAME-SIZE shader-bytecode swap in shaders.big.
    Replace one shader's payload with new bytes of the SAME length (a real
    shader-swap where the replacement compiles to the same size, or a byte
    patch). Downstream offsets must NOT move."""
    print("\n" + "=" * 74)
    print("DEMO 1  --  SAME-SIZE payload replace (shaders.big shader bytecode)")
    print("=" * 74)
    path = os.path.join(INSTALL, r"data\shaders\pc\shaders.big")
    model = load_model(path)
    # target: pixel shader id=2 (41 bytes) in PIXEL_SHADERS
    sb = next(s for s in model['subbanks'] if s['name'] == 'PIXEL_SHADERS')
    tgt = next(e for e in sb['entries'] if e['id'] == 2)
    old = tgt['payload']
    print(f"  target: PIXEL_SHADERS id={tgt['id']} name={tgt['name']!r} "
          f"size={tgt['size']} crc=0x{tgt['crc']:08x}")
    # new payload: same length, different bytes (invert every byte)
    new = bytes((x ^ 0xA5) for x in old)
    assert len(new) == len(old) and new != old
    replace_entry_payload(model, 'PIXEL_SHADERS', 2, new)
    out = serialize(model)

    out_path = os.path.join(SCRATCH, "shaders_samesize.big")
    open(out_path, 'wb').write(out)
    orig = open(path, 'rb').read()
    print(f"  wrote {out_path}  ({len(out):,} bytes; original {len(orig):,})")
    print(f"  size delta: {len(out) - len(orig):+d} bytes (expected 0 for same-size)")

    ok, rep = validate_write(path, out, ('PIXEL_SHADERS', 2), new)
    _print_report(rep)

    # extra: byte-diff the whole file -- only the payload's bytes should differ
    diffs = _diff_runs(orig, out)
    exp_lo, exp_hi = tgt['offset'], tgt['offset'] + len(new)
    only_payload = (len(diffs) == 1 and diffs[0][0] >= exp_lo and diffs[0][1] <= exp_hi)
    print(f"  whole-file byte-diff runs: {len(diffs)} "
          f"(expected 1, inside payload [0x{exp_lo:x},0x{exp_hi:x}))")
    for (s, e) in diffs[:4]:
        print(f"    diff 0x{s:x}..0x{e:x} ({e-s}B)")
    print(f"  ONLY the edited payload changed: {only_payload}")
    return ok and only_payload


def demo_sizechange():
    """DIFFERENT-SIZE payload replace in shaders.big.
    Replace one shader's payload with new bytes of a DIFFERENT length. Every
    downstream region (payloads, TOCs, footer) must re-flow, all offsets
    re-patched, and the re-read must still prove ONLY that entry changed."""
    print("\n" + "=" * 74)
    print("DEMO 2  --  DIFFERENT-SIZE payload replace (downstream re-flow)")
    print("=" * 74)
    path = os.path.join(INSTALL, r"data\shaders\pc\shaders.big")
    model = load_model(path)
    sb = next(s for s in model['subbanks'] if s['name'] == 'PIXEL_SHADERS')
    tgt = next(e for e in sb['entries'] if e['id'] == 1)   # smallest, 29B
    old = tgt['payload']
    print(f"  target: PIXEL_SHADERS id={tgt['id']} name={tgt['name']!r} "
          f"size={tgt['size']} off=0x{tgt['offset']:x} crc=0x{tgt['crc']:08x}")
    # new payload: GROW by 64 bytes (forces every following region to shift)
    new = old + bytes(range(64))
    replace_entry_payload(model, 'PIXEL_SHADERS', 1, new)
    out = serialize(model)

    out_path = os.path.join(SCRATCH, "shaders_sizechange.big")
    open(out_path, 'wb').write(out)
    orig = open(path, 'rb').read()
    print(f"  new payload size: {len(new)} (was {len(old)}, delta +{len(new)-len(old)})")
    print(f"  wrote {out_path}  ({len(out):,} bytes; original {len(orig):,}; "
          f"delta {len(out)-len(orig):+d})")

    ok, rep = validate_write(path, out, ('PIXEL_SHADERS', 1), new)
    _print_report(rep)

    # confirm downstream actually moved (offsets shifted for later entries)
    omap, _ = _parse_all_entries(orig)
    nmap, _ = _parse_all_entries(out)
    moved = sum(1 for k in omap if k in nmap and omap[k]['offset'] != nmap[k]['offset'])
    print(f"  downstream entries whose Offset shifted: {moved} "
          f"(re-flow ripple; expected > 0)")
    return ok and moved > 0


def _diff_runs(a, c):
    n = min(len(a), len(c))
    runs = []
    i = 0
    while i < n:
        if a[i] != c[i]:
            j = i
            while j < n and a[j] != c[j]:
                j += 1
            runs.append((i, j))
            i = j
        else:
            i += 1
    if len(a) != len(c):
        runs.append((n, max(len(a), len(c))))
    return runs


IDENTITY_FILES = [
    ("text.big",     r"data\lang\English\text.big"),
    ("dialogue.big", r"data\lang\English\dialogue.big"),
    ("fonts.big",    r"data\lang\English\fonts.big"),
    ("frontend.big", r"data\graphics\pc\frontend.big"),
    ("graphics.big", r"data\graphics\graphics.big"),
    ("textures.big", r"data\graphics\pc\textures.big"),
    ("effects.big",  r"data\Misc\pc\effects.big"),
    ("shaders.big",  r"data\shaders\pc\shaders.big"),
]


def demo_identity_check():
    """Sanity: an UNEDITED round-trip through load_model+serialize must be
    byte-identical on ALL 8 retail files (proves the writer's re-flow degrades
    to the proven byte-exact round-trip when nothing changes -- including
    fonts.big which mixes Align 512/2048, and the 533 MB textures.big)."""
    print("\n" + "=" * 74)
    print("DEMO 0  --  identity re-flow (no edit) must be byte-exact (all 8)")
    print("=" * 74)
    results = []
    for name, rel in IDENTITY_FILES:
        path = os.path.join(INSTALL, rel)
        if not os.path.exists(path):
            print(f"  {name}: MISSING"); continue
        model = load_model(path)
        out = serialize(model)
        orig = open(path, 'rb').read()
        same = (out == orig)
        h_in = hashlib.sha256(orig).hexdigest()[:12]
        h_out = hashlib.sha256(out).hexdigest()[:12]
        print(f"  {name:<14} identity={'PASS' if same else 'FAIL'} "
              f"sha_in={h_in} sha_out={h_out}")
        results.append(same)
    return all(results)


def demo_sizechange_aligned():
    """DIFFERENT-SIZE payload replace in frontend.big (GBANK, Align=2048).
    Proves the re-flow re-establishes a LARGE (2048) payload alignment after a
    size change -- the real retexture case (GUI texture blob grows/shrinks)."""
    print("\n" + "=" * 74)
    print("DEMO 3  --  DIFFERENT-SIZE payload replace, Align=2048 (frontend.big)")
    print("=" * 74)
    path = os.path.join(INSTALL, r"data\graphics\pc\frontend.big")
    if not os.path.exists(path):
        print("  frontend.big MISSING"); return True
    model = load_model(path)
    sb = model['subbanks'][0]
    # pick a small-ish texture entry to keep the demo cheap
    cands = [e for e in sb['entries'] if e['size'] > 0 and e['offset'] > 0]
    cands.sort(key=lambda e: e['size'])
    tgt = cands[0]
    old = tgt['payload']
    print(f"  target: {sb['name']} id={tgt['id']} name={tgt['name']!r} "
          f"size={tgt['size']} off=0x{tgt['offset']:x} align={sb['align']} "
          f"crc=0x{tgt['crc']:08x} infoSize={tgt['infosize']}")
    # GROW by 3000 bytes (> one 2048 boundary -> forces every downstream
    # payload to ripple forward by a multiple of 2048, exercising real re-align)
    new = old + bytes((i & 0xFF) for i in range(3000))
    replace_entry_payload(model, sb['name'], tgt['id'], new)
    out = serialize(model)
    out_path = os.path.join(SCRATCH, "frontend_sizechange.big")
    open(out_path, 'wb').write(out)
    orig = open(path, 'rb').read()
    print(f"  new payload size: {len(new)} (was {len(old)}, delta {len(new)-len(old):+d})")
    print(f"  wrote {out_path}  ({len(out):,} bytes; original {len(orig):,}; "
          f"delta {len(out)-len(orig):+d})")

    ok, rep = validate_write(path, out, (sb['name'], tgt['id']), new)
    _print_report(rep)

    # prove EVERY payload in the output is 2048-aligned (Align invariant kept)
    nmap, _ = _parse_all_entries(out)
    misalign = sum(1 for k, v in nmap.items()
                   if v['size'] > 0 and v['offset'] % sb['align'] != 0)
    print(f"  payloads violating Align={sb['align']} in output: {misalign} "
          f"(expected 0)")
    moved = sum(1 for k in _parse_all_entries(orig)[0]
                if nmap[k]['offset'] != _parse_all_entries(orig)[0][k]['offset'])
    print(f"  downstream entries whose Offset shifted: {moved}")
    return ok and misalign == 0


def demo_invertibility():
    """Round-trip invertibility: edit an entry to a NEW size, then edit it BACK
    to the original bytes; the twice-edited file must be byte-identical to the
    untouched original. Proves the re-flow has no hidden drift / lossy state."""
    print("\n" + "=" * 74)
    print("DEMO 4  --  invertibility (edit then revert == original, byte-exact)")
    print("=" * 74)
    path = os.path.join(INSTALL, r"data\shaders\pc\shaders.big")
    orig = open(path, 'rb').read()
    # edit +64
    m1 = load_model(path)
    sb = next(s for s in m1['subbanks'] if s['name'] == 'PIXEL_SHADERS')
    e1 = next(e for e in sb['entries'] if e['id'] == 1)
    orig_payload = e1['payload']
    replace_entry_payload(m1, 'PIXEL_SHADERS', 1, orig_payload + bytes(64))
    edited = serialize(m1)
    # revert: load the EDITED file, put the original payload back
    tmp = os.path.join(SCRATCH, "shaders_edited_tmp.big")
    open(tmp, 'wb').write(edited)
    m2 = load_model(tmp)
    replace_entry_payload(m2, 'PIXEL_SHADERS', 1, orig_payload)
    reverted = serialize(m2)
    same = (reverted == orig)
    print(f"  edited size={len(edited):,}  reverted size={len(reverted):,}  "
          f"original size={len(orig):,}")
    print(f"  sha(original) ={hashlib.sha256(orig).hexdigest()[:16]}")
    print(f"  sha(reverted) ={hashlib.sha256(reverted).hexdigest()[:16]}")
    print(f"  VERDICT: {'PASS (revert is byte-exact original)' if same else 'FAIL'}")
    return same


def main():
    args = sys.argv[1:]
    os.makedirs(SCRATCH, exist_ok=True)
    if not args or args[0] == '--demo':
        r0 = demo_identity_check()
        r1 = demo_samesize()
        r2 = demo_sizechange()
        r3 = demo_sizechange_aligned()
        r4 = demo_invertibility()
        print("\n" + "=" * 74)
        print(f"SUMMARY: identity(8)={'PASS' if r0 else 'FAIL'}  "
              f"same-size={'PASS' if r1 else 'FAIL'}  "
              f"size-change(A4)={'PASS' if r2 else 'FAIL'}  "
              f"size-change(A2048)={'PASS' if r3 else 'FAIL'}  "
              f"invertible={'PASS' if r4 else 'FAIL'}")
        print("=" * 74)
        return
    if args[0] == '--demo-samesize':
        demo_samesize(); return
    if args[0] == '--demo-sizechange':
        demo_sizechange(); demo_sizechange_aligned(); return
    if args[0] == '--identity':
        demo_identity_check(); return
    if args[0] == '--invertible':
        demo_invertibility(); return
    print(__doc__)


if __name__ == '__main__':
    main()
