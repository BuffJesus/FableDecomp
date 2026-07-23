#!/usr/bin/env python3
"""BIGB bank-archive parser for the Fable: TLC .big family.

Ports the reader spec from docs/TEXTBIG_FORMAT.md. Pure byte-level, no Ghidra.
Usage: python parse_bigb.py <file.big> [--dump N] [--type-payload]
"""
import struct, sys, os

class Reader:
    def __init__(self, buf):
        self.b = buf; self.p = 0
    def u32(self):
        v = struct.unpack_from('<I', self.b, self.p)[0]; self.p += 4; return v
    def i32(self):
        v = struct.unpack_from('<i', self.b, self.p)[0]; self.p += 4; return v
    def lpstr(self):
        """u32 length-prefixed 8-bit string (strip trailing NUL)."""
        n = self.u32()
        s = self.b[self.p:self.p+n]; self.p += n
        return s.rstrip(b'\x00')
    def cstr(self):
        """NUL-terminated ASCII string."""
        e = self.b.index(b'\x00', self.p)
        s = self.b[self.p:e]; self.p = e + 1
        return s

def parse_header(b):
    magic = b[0:4]
    ver, foot_off, foot_size = struct.unpack_from('<III', b, 4)
    return magic, ver, foot_off, foot_size

def parse_footer(b, foot_off):
    r = Reader(b); r.p = foot_off
    n = r.u32()
    subs = []
    for _ in range(n):
        name = r.cstr()
        version, entry_count, offset, size, align = struct.unpack_from('<IIIII', b, r.p)
        r.p += 20
        subs.append(dict(name=name.decode('latin1'), version=version,
                         entry_count=entry_count, offset=offset, size=size, align=align))
    return subs, r.p

def parse_toc(b, sub, foot_off):
    """Walk the sub-bank TOC. Returns (entries, stats, end_pos, error)."""
    r = Reader(b); r.p = sub['offset']
    stats = None
    # stats header: u32 statsCount then statsCount*2 u32 pairs when statsCount<1000
    save = r.p
    sc = r.u32()
    if sc < 1000:
        stats = []
        for _ in range(sc):
            t = r.u32(); c = r.u32()
            stats.append((t, c))
    else:
        r.p = save  # rewind, no stats header
    entries = []
    err = None
    for i in range(sub['entry_count']):
        try:
            estart = r.p
            magic = r.u32()
            eid = r.u32()
            etype = r.i32()
            size = r.u32()
            offset = r.u32()
            crc = r.u32()
            name = r.lpstr()
            timestamp = r.u32()
            depcount = r.u32()
            deps = [r.lpstr() for _ in range(depcount)]
            infosize = r.u32()
            info = b[r.p:r.p+infosize]; r.p += infosize
            entries.append(dict(idx=i, magic=magic, id=eid, type=etype, size=size,
                                offset=offset, crc=crc, name=name.decode('latin1'),
                                timestamp=timestamp, depcount=depcount,
                                deps=[d.decode('latin1') for d in deps],
                                infosize=infosize, info=info, rec_off=estart))
            if magic != 42 and not (size > 0 and eid > 0):
                err = f"entry {i}: bad magic {magic} at 0x{estart:x}"
                break
        except Exception as ex:
            err = f"entry {i}: exception {ex} at pos 0x{r.p:x}"
            break
    return entries, stats, r.p, err

def decode_lipsync(b, entry):
    """Decode a dialogue.big LIPSYNC entry payload. Returns dict or None if empty.
    Grammar (byte-exact validated on all 20,505 dialogue.big entries):
      u32 visemeCount; visemeCount x {u8 index, char[2] mnemonic, u8 NUL};
      u32 blockMarker(=0x2B); u32 kfCount;
      kfCount x { u8 pairCount; pairCount x {u8 visemeIdx, u8 weight} }
    """
    if entry['size'] == 0:
        return None
    off = entry['offset']; endp = off + entry['size']
    r = Reader(b); r.p = off
    n = r.u32()
    visemes = []
    for _ in range(n):
        idx = b[r.p]; mnem = b[r.p+1:r.p+3].decode('latin1'); r.p += 4
        visemes.append((idx, mnem))
    marker = r.u32(); kfcount = r.u32()
    kfs = []
    for _ in range(kfcount):
        c = b[r.p]; r.p += 1
        pairs = [(b[r.p+j*2], b[r.p+j*2+1]) for j in range(c)]; r.p += c*2
        kfs.append(pairs)
    duration = struct.unpack('<f', entry['info'])[0] if entry['infosize'] == 4 else None
    speaker = entry['deps'][1] if len(entry['deps']) > 1 else None
    return dict(visemes=visemes, marker=marker, kfcount=kfcount, keyframes=kfs,
                duration=duration, speaker=speaker, byte_exact=(r.p == endp))


def main():
    path = sys.argv[1]
    dump_n = 0
    show_payload = '--type-payload' in sys.argv
    if '--dump' in sys.argv:
        dump_n = int(sys.argv[sys.argv.index('--dump')+1])
    with open(path, 'rb') as f:
        b = f.read()
    fsize = len(b)
    magic, ver, foot_off, foot_size = parse_header(b)
    print(f"=== {os.path.basename(path)} ({fsize:,} bytes) ===")
    print(f"  magic={magic!r} version={ver} footerOffset=0x{foot_off:x} footerSize=0x{foot_size:x}")
    if magic != b'BIGB':
        print("  !! NOT BIGB — deviates"); return
    if foot_off + foot_size > fsize:
        print(f"  !! footer out of range (fsize=0x{fsize:x})"); return
    subs, footer_end = parse_footer(b, foot_off)
    print(f"  subBankCount={len(subs)}  footerEnd=0x{footer_end:x} (declared size end=0x{foot_off+foot_size:x})")
    all_clean = True
    for si, sub in enumerate(subs):
        print(f"  -- subbank[{si}] name={sub['name']!r} version={sub['version']} "
              f"entries={sub['entry_count']:,} tocOff=0x{sub['offset']:x} tocSize=0x{sub['size']:x} align={sub['align']}")
        entries, stats, end_pos, err = parse_toc(b, sub, foot_off)
        # type histogram
        hist = {}
        for e in entries:
            hist[e['type']] = hist.get(e['type'], 0) + 1
        print(f"     stats-header={stats}  parsed={len(entries):,}/{sub['entry_count']:,}  typeHist={dict(sorted(hist.items()))}")
        # invariant: TOC walk ends exactly at footerOffset
        toc_ok = (end_pos == foot_off)
        print(f"     TOC end=0x{end_pos:x}  footerOffset=0x{foot_off:x}  "
              f"{'CLEAN (ends at footer)' if toc_ok else 'DEVIATES'}")
        if err:
            print(f"     ERROR: {err}"); all_clean = False
        if not toc_ok and not err:
            all_clean = False
        # payload offset sanity: min/max offsets
        offs = [e['offset'] for e in entries if e['offset'] > 0]
        if offs:
            print(f"     payload offsets: min=0x{min(offs):x} max=0x{max(offs):x} "
                  f"(payload region 0x10..0x{foot_off:x})")
        # infosize histogram (non-zero)
        infos = {}
        for e in entries:
            if e['infosize'] > 0:
                infos[e['infosize']] = infos.get(e['infosize'], 0) + 1
        if infos:
            print(f"     infoSize histogram (nonzero): {dict(sorted(infos.items())[:10])}")
        # dump sample entries
        for e in entries[:dump_n]:
            print(f"     [{e['idx']}] id={e['id']} type={e['type']} size={e['size']} "
                  f"off=0x{e['offset']:x} crc=0x{e['crc']:08x} ts={e['timestamp']} "
                  f"deps={e['depcount']} infoSize={e['infosize']} name={e['name']!r}")
    print(f"  RESULT: {'CLEAN' if all_clean else 'DEVIATES'}")
    return subs

if __name__ == '__main__':
    main()
