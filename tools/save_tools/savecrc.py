#!/usr/bin/env python3
"""Fable TLC save-stream GetCRC + HEADER inflater/walker.
GetCRC = reflected CRC-32, poly 0xEDB88320, seed 0xFFFFFFFF, NO final inversion.
"""
import sys, os, zlib, struct

# Build the reflected CRC-32 table (same as the game's table at donor 0x0449BE30)
def build_table():
    tbl = []
    for i in range(256):
        c = i
        for _ in range(8):
            c = (c >> 1) ^ 0xEDB88320 if (c & 1) else (c >> 1)
        tbl.append(c & 0xFFFFFFFF)
    return tbl
TABLE = build_table()

def getcrc(name, seed=0xFFFFFFFF):
    """CCharString::GetCRC() = Calc(0xFFFFFFFF, chars, len) NO final xor."""
    crc = seed
    for b in name.encode('ascii', 'replace') if isinstance(name, str) else name:
        crc = (crc >> 8) ^ TABLE[(crc ^ b) & 0xFF]
    return crc & 0xFFFFFFFF

def getcrc_le(name, seed=0xFFFFFFFF):
    return struct.pack('<I', getcrc(name, seed))

# ---- save inflater ----
def read_file(path):
    with open(path, 'rb') as f:
        return f.read()

def parse_header(data):
    assert data[:10] == b'FableSave!', "bad magic: %r" % data[:10]
    sig      = struct.unpack_from('<I', data, 0x0C)[0]
    c0_ulen  = struct.unpack_from('<I', data, 0x10)[0]
    c1_ulen  = struct.unpack_from('<I', data, 0x14)[0]
    c0_clen  = struct.unpack_from('<I', data, 0x18)[0]
    off = 0x1C
    chunk0_comp = data[off:off+c0_clen]
    off += c0_clen
    c1_clen  = struct.unpack_from('<I', data, off)[0]
    off += 4
    chunk1_comp = data[off:off+c1_clen]
    hdr = zlib.decompress(chunk0_comp)
    body = zlib.decompress(chunk1_comp)
    return dict(sig=sig, c0_ulen=c0_ulen, c1_ulen=c1_ulen, c0_clen=c0_clen,
                c1_clen=c1_clen, header=hdr, body=body)

if __name__ == '__main__':
    cmd = sys.argv[1] if len(sys.argv) > 1 else 'demo'
    if cmd == 'crc':
        # SAVE-STREAM field tag == Calc(seed=0, name) (same as game.bin). See docs/SAVE_HEADER_MAP.md.
        for nm in sys.argv[2:]:
            t0 = getcrc(nm, 0)
            print("%-30s -> save/game.bin tag(seed0)=%08X (LE %s)   [seedFFFF=%08X]" %
                  (nm, t0, struct.pack('<I', t0).hex(), getcrc(nm, 0xFFFFFFFF)))
    elif cmd == 'inflate':
        p = parse_header(read_file(sys.argv[2]))
        print("sig=%08X c0_ulen=%d c1_ulen=%d c0_clen=%d c1_clen=%d hdrlen=%d bodylen=%d" %
              (p['sig'], p['c0_ulen'], p['c1_ulen'], p['c0_clen'], p['c1_clen'],
               len(p['header']), len(p['body'])))
        outdir = sys.argv[3] if len(sys.argv) > 3 else '.'
        base = os.path.splitext(os.path.basename(sys.argv[2].rstrip('/')))[0]
        with open(os.path.join(outdir, base+'.header.bin'),'wb') as f: f.write(p['header'])
        with open(os.path.join(outdir, base+'.body.bin'),'wb') as f: f.write(p['body'])
        print("wrote", base+'.header.bin', base+'.body.bin')
