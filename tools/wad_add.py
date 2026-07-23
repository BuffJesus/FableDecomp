#!/usr/bin/env python3
"""Add NEW entries to a Fable TLC .wad archive (FinalAlbion.wad).

FableForge `forge wad repack` only REPLACES existing entries; this tool APPENDS
new ones (needed for brand-new levels: a new "Data\\Levels\\FinalAlbion\\X.lev"
+ ".tng" pair).

Layout (verified against retail FinalAlbion.wad + forge::wad reader):
  header: "BBBB" u32? u32? u32?, u32 dataAlign(2048)@16?, u32 entryCount@20,
          u32 entryCount2@24, u32 footerOffset@28
  data:   payloads, each aligned to 2048
  footer: u32 statsCount + statsCount*8 bytes stats, then per entry:
          u32 magic=42, u32 id, u32 type, u32 size, u32 offset, u32 crc(=0),
          u32 nameLen + name, u32 timestamp(=0),
          u32 depCount x { u32 len + bytes } (retail: one empty dep),
          u32 infoSize + info (retail: 88-byte build timestamp blob)

New entries clone the dep list + info blob of a template entry.

Usage:
  python tools/wad_add.py <src.wad> <out.wad> <entryName=payloadFile> ...
"""

import struct
import sys

ALIGN = 2048


def parse_footer_entries(data, footer_off, entry_count):
    pos = footer_off
    stats_count = struct.unpack_from('<I', data, pos)[0]
    pos += 4 + stats_count * 8
    entries_start = pos
    entries = []
    for _ in range(entry_count):
        rec_start = pos
        magic, eid, etype, size, off, crc = struct.unpack_from('<6I', data, pos)
        assert magic == 42, f'bad entry magic at {pos}'
        pos += 24
        nl = struct.unpack_from('<I', data, pos)[0]
        pos += 4
        name = data[pos:pos + nl].decode('latin1')
        pos += nl
        ts = struct.unpack_from('<I', data, pos)[0]
        pos += 4
        depc = struct.unpack_from('<I', data, pos)[0]
        pos += 4
        deps_start = pos - 4
        for _ in range(depc):
            dl = struct.unpack_from('<I', data, pos)[0]
            pos += 4 + dl
        il = struct.unpack_from('<I', data, pos)[0]
        deps_blob = data[deps_start:pos]      # depCount + deps
        pos += 4
        info = data[pos:pos + il]
        pos += il
        entries.append(dict(id=eid, type=etype, size=size, offset=off, crc=crc,
                            name=name, ts=ts, deps_blob=deps_blob, info=info,
                            raw=data[rec_start:pos]))
    return entries, data[footer_off:entries_start], pos


def build_entry_record(e):
    out = struct.pack('<6I', 42, e['id'], e['type'], e['size'], e['offset'], e['crc'])
    nb = e['name'].encode('latin1')
    out += struct.pack('<I', len(nb)) + nb
    out += struct.pack('<I', e['ts'])
    out += e['deps_blob']
    out += struct.pack('<I', len(e['info'])) + e['info']
    return out


def add_entries(src_path, out_path, additions):
    data = open(src_path, 'rb').read()
    entry_count = struct.unpack_from('<I', data, 20)[0]
    entry_count2 = struct.unpack_from('<I', data, 24)[0]
    footer_off = struct.unpack_from('<I', data, 28)[0]
    entries, stats_blob, _ = parse_footer_entries(data, footer_off, entry_count)

    template = entries[0]
    max_id = max(e['id'] for e in entries)
    body = bytearray(data[:footer_off])

    new_entries = []
    for i, (name, payload) in enumerate(additions):
        # align current end
        pad = (-len(body)) % ALIGN
        body += b'\0' * pad
        off = len(body)
        body += payload
        max_id += 1
        new_entries.append(dict(id=max_id, type=template['type'],
                                size=len(payload), offset=off, crc=0,
                                name=name, ts=0,
                                deps_blob=template['deps_blob'],
                                info=template['info']))

    new_footer_off = len(body)
    footer = bytearray(stats_blob)
    for e in entries:
        footer += e['raw']
    for e in new_entries:
        footer += build_entry_record(e)

    out = bytearray(body) + footer
    n = entry_count + len(new_entries)
    struct.pack_into('<I', out, 20, n)
    if entry_count2 == entry_count:
        struct.pack_into('<I', out, 24, n)
    struct.pack_into('<I', out, 28, new_footer_off)
    open(out_path, 'wb').write(out)
    print(f'{out_path}: {n} entries (+{len(new_entries)}), '
          f'{len(out)} bytes (src {len(data)})')
    for e in new_entries:
        print(f'  added id={e["id"]} off={e["offset"]} size={e["size"]} {e["name"]}')


def main():
    if len(sys.argv) < 4:
        print(__doc__)
        return 1
    src, out = sys.argv[1], sys.argv[2]
    additions = []
    for spec in sys.argv[3:]:
        name, path = spec.split('=', 1)
        additions.append((name, open(path, 'rb').read()))
    add_entries(src, out, additions)
    return 0


if __name__ == '__main__':
    sys.exit(main())
