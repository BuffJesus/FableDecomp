#!/usr/bin/env python3
r"""BIGB bank-archive WRITER — pairs with parse_bigb.py (Route B, no EgoCore exe).

`rebuild(buf, edits)` reconstructs a .big container, optionally replacing entry
payloads. Layout is `[16B header][payloads][per-subbank TOCs][footer]`; entries
are located by an absolute `offset` in each TOC record, so a resized payload just
means re-laying-out payloads and patching every record's offset/size.

FAITHFULNESS: each TOC record is COPIED VERBATIM from the source and only its
`size` (@+12) and `offset` (@+16) u32 fields are patched — so entry names, deps,
and Info blobs are byte-preserved (no length/NUL guesswork). The stats header is
copied raw too. Result: rebuild with no edits round-trips every entry's content;
with an edit, only that entry's payload + the recomputed offsets change.
"""
import os
import sys
import struct

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import parse_bigb


def _align(p, a):
    a = a or 1
    return (p + a - 1) // a * a if a > 1 else p


def _lpstr(s):
    b = s.encode('latin1')
    return struct.pack('<I', len(b)) + b


def _new_record(add):
    """Serialize a brand-new TOC record (same field order parse_bigb.parse_toc
    reads; mirrors EgoCore BigBankCompiler's writer): magic=42, id, type,
    size(@+12)/offset(@+16) left 0 for the rebuild loop to patch, crc,
    len-prefixed name (no trailing NUL -- retail stores exact length),
    timestamp, deps, infoSize + info blob."""
    rec = bytearray()
    rec += struct.pack('<IIiII', 42, add['id'], add['type'], 0, 0)
    rec += struct.pack('<I', add.get('crc', 0))
    rec += _lpstr(add['name'])
    rec += struct.pack('<I', add.get('timestamp', 0))
    deps = add.get('deps') or []
    rec += struct.pack('<I', len(deps))
    for d in deps:
        rec += _lpstr(d)
    info = add.get('info', b'')
    rec += struct.pack('<I', len(info))
    rec += info
    return bytes(rec)


def _stats_header(stats):
    out = struct.pack('<I', len(stats))
    for t, c in stats:
        out += struct.pack('<II', t, c)
    return out


def rebuild(buf, edits=None, adds=None, toc_align=4):
    """edits: dict {(subbank_name, entry_name): new_payload_bytes}.
    adds:  list of NEW entries to append, each a dict:
             sub     -- subbank name (e.g. 'MBANK_ALLMESHES')
             name    -- entry name (must be unique within the subbank)
             payload -- payload bytes
             type    -- entry type (e.g. 1 = static compiled mesh)
             info    -- subheader/Info blob bytes (optional, b'')
             id      -- entry id (optional; default max id in subbank + 1 --
                        retail TOCs are id-sorted, so appending keeps order)
             deps    -- list of dependency strings (optional)
             crc/timestamp -- optional u32s (default 0, like EgoCore's new
                        entries; retail CRC is not a name/dep CRC-32)
    New records get a synthesized TOC record, the subbank's stats header
    (type histogram) is incremented, and the footer entry_count is bumped.
    toc_align: alignment of each sub-bank TOC start (4 matches graphics.big;
    text.big packs its TOC unaligned -- pass 1 for byte-exact rebuilds there).
    With edits/adds empty the rebuild remains byte-exact. Returns bytes."""
    edits = edits or {}
    adds = adds or []
    magic, ver, foot_off, _fs = parse_bigb.parse_header(buf)
    if magic != b'BIGB':
        raise ValueError('not a BIGB container')
    subs, _ = parse_bigb.parse_footer(buf, foot_off)

    # each sub's TOC ends where the next TOC (by ascending offset) starts, else at footer
    bounds = sorted([s['offset'] for s in subs] + [foot_off])
    def toc_end(off):
        return next(o for o in bounds if o > off)

    parsed = []
    for sub in subs:
        entries, stats, _end, err = parse_bigb.parse_toc(buf, sub, foot_off)
        if err:
            raise ValueError('parse error in %s: %s' % (sub['name'], err))
        # _end = byte just past the LAST original record (parse_toc walks fields
        # verbatim). toc_end() by contrast points at the next subbank's TOC start,
        # which sits past inter-subbank alignment slack -- copying up to it would
        # fold stray bytes into the last record and misalign appended records.
        sub['_toc_walk_end'] = _end

        sub_adds = [a for a in adds if a['sub'] == sub['name']]
        sub['_new_stats'] = None
        if sub_adds:
            names = {e['name'] for e in entries}
            max_id = max((e['id'] for e in entries), default=0)
            hist = dict(stats or [])
            for a in sub_adds:
                if a['name'] in names:
                    raise ValueError('entry %r already exists in %s'
                                     % (a['name'], sub['name']))
                names.add(a['name'])
                a = dict(a)
                if a.get('id') is None:
                    max_id += 1
                    a['id'] = max_id
                else:
                    max_id = max(max_id, a['id'])
                hist[a['type']] = hist.get(a['type'], 0) + 1
                entries.append(dict(
                    idx=len(entries), id=a['id'], type=a['type'], name=a['name'],
                    size=len(a['payload']), offset=0,
                    _newrec=_new_record(a), _newpayload=bytes(a['payload'])))
            if stats is not None:
                sub['_new_stats'] = _stats_header(sorted(hist.items()))
            sub['_added'] = len(sub_adds)
        else:
            sub['_added'] = 0
        parsed.append((sub, entries))

    unknown = [a['sub'] for a in adds
               if a['sub'] not in {s['name'] for s in subs}]
    if unknown:
        raise ValueError('unknown subbank(s) for adds: %s' % unknown)

    out = bytearray(16)  # header filled in last

    # --- payload region ---
    for sub, entries in parsed:
        a = sub['align'] or 1
        for e in entries:
            payload = e.get('_newpayload')
            if payload is None:
                payload = edits.get((sub['name'], e['name']))
            if payload is None:
                payload = bytes(buf[e['offset']:e['offset'] + e['size']]) if e['size'] else b''
            if not payload:
                e['_off'] = e['offset'] if e['size'] == 0 else 0
                e['_size'] = 0
                continue
            pos = _align(len(out), a)
            out.extend(b'\x00' * (pos - len(out)))
            e['_off'] = pos
            e['_size'] = len(payload)
            out.extend(payload)

    # --- TOC region (raw records, size/offset patched; new records appended) ---
    for sub, entries in parsed:
        pos = _align(len(out), toc_align)
        out.extend(b'\x00' * (pos - len(out)))
        sub['_off'] = pos
        te = toc_end(sub['offset'])
        # true end of the last original record (excludes inter-subbank slack)
        last_orig_end = sub['_toc_walk_end']
        orig = [e for e in entries if '_newrec' not in e]
        first_rec = orig[0]['rec_off'] if orig else te
        if sub['_new_stats'] is not None:
            out.extend(sub['_new_stats'])                         # patched histogram
        else:
            out.extend(buf[sub['offset']:first_rec])              # stats header, verbatim
        for i, e in enumerate(entries):
            if '_newrec' in e:
                rec = bytearray(e['_newrec'])
            else:
                rend = orig[i + 1]['rec_off'] if i + 1 < len(orig) else last_orig_end
                rec = bytearray(buf[e['rec_off']:rend])
            struct.pack_into('<I', rec, 12, e['_size'])           # size @ +12
            struct.pack_into('<I', rec, 16, e['_off'])            # offset @ +16
            out.extend(rec)
        sub['_toc_size'] = len(out) - pos

    # --- footer ---
    fpos = len(out)
    out.extend(struct.pack('<I', len(subs)))
    for sub in subs:
        out.extend(sub['name'].encode('latin1')); out.append(0)
        out.extend(struct.pack('<IIIII', sub['version'],
                               sub['entry_count'] + sub['_added'],
                               sub['_off'], sub['_toc_size'], sub['align']))
    struct.pack_into('<III', out, 4, ver, fpos, len(out) - fpos)
    out[0:4] = b'BIGB'
    return bytes(out)
