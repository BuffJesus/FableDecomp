#!/usr/bin/env python3
"""Minimal Fable II BNK v3 repacker: read all entries, allow replacing/adding a few,
rebuild the container. Only handles v3 (continuous-stream header, compress_file_data=1),
which is what gamescripts_r.bnk uses. Preserves untouched entries' raw compressed bytes;
recompresses only the entries we change (as a single zlib stream = one chunk)."""
import struct, zlib, sys, io

def be32(v): return struct.pack(">I", v)

def read_bnk(path):
    d = open(path, "rb").read()
    base_offset = struct.unpack(">I", d[0:4])[0]
    ver = struct.unpack(">I", d[4:8])[0]
    assert ver == 3, f"only v3 supported, got {ver}"
    flag = d[8]
    assert flag == 1, "expected compress_file_data=1"
    # file-table chunks: (comp_size, decomp_size, comp_bytes)* terminated by comp_size==0
    pos = 9
    tbl = io.BytesIO()
    dco = zlib.decompressobj(15)
    while True:
        comp_size = struct.unpack(">I", d[pos:pos+4])[0]; pos += 4
        decomp_size = struct.unpack(">I", d[pos:pos+4])[0]; pos += 4
        if comp_size == 0:
            break
        comp = d[pos:pos+comp_size]; pos += comp_size
        tbl.write(dco.decompress(comp))
    tbl.write(dco.flush())
    blob = tbl.getvalue()

    # parse table blob
    b = io.BytesIO(blob)
    def ru32(): return struct.unpack(">I", b.read(4))[0]
    n = ru32()
    entries = []
    for _ in range(n):
        nl = ru32(); name = b.read(nl)
        if nl and name[-1] == 0: name = name[:-1]
        rel = ru32(); dsz = ru32(); csz = ru32(); cc = ru32()
        chunks = [ru32() for _ in range(cc)]
        raw = d[base_offset+rel: base_offset+rel+csz]   # raw compressed file bytes
        entries.append(dict(name=name.decode("utf-8","replace"), dsz=dsz, csz=csz,
                            chunks=chunks, raw=raw))
    return base_offset, entries

def zlib1(data):
    """Compress as one zlib chunk; return (comp_bytes, [decomp_size])."""
    c = zlib.compressobj(9, zlib.DEFLATED, 15)
    out = c.compress(data) + c.flush()
    return out, [len(data)]

def write_bnk(path, base_offset, entries):
    # build file-data section (contiguous), assign rel offsets
    data = io.BytesIO(); rels = []
    for e in entries:
        rels.append(data.tell())
        data.write(e["raw"])
    data_bytes = data.getvalue()

    # build table blob
    tb = io.BytesIO()
    tb.write(be32(len(entries)))
    for e, rel in zip(entries, rels):
        nm = e["name"].encode("utf-8") + b"\x00"
        tb.write(be32(len(nm))); tb.write(nm)
        tb.write(be32(rel)); tb.write(be32(e["dsz"])); tb.write(be32(e["csz"]))
        tb.write(be32(len(e["chunks"])))
        for cs in e["chunks"]:
            tb.write(be32(cs))
    blob = tb.getvalue()

    # compress table as one chunk
    tc = zlib.compressobj(9, zlib.DEFLATED, 15)
    tcomp = tc.compress(blob) + tc.flush()

    header = io.BytesIO()
    header.write(be32(base_offset))     # keep base_offset (pad table to it)
    header.write(be32(3))               # version
    header.write(b"\x01")               # compress_file_data
    header.write(be32(len(tcomp))); header.write(be32(len(blob))); header.write(tcomp)
    header.write(be32(0)); header.write(be32(0))   # terminator (comp=0, decomp=0)
    hb = header.getvalue()
    assert len(hb) <= base_offset, f"table too big for base_offset: {len(hb)} > {base_offset}"
    hb = hb + b"\x00" * (base_offset - len(hb))     # pad to base_offset

    open(path, "wb").write(hb + data_bytes)

def replace_entry(entries, name_sub, data):
    for e in entries:
        if name_sub.lower() in e["name"].lower():
            raw, chunks = zlib1(data)
            e["raw"] = raw; e["csz"] = len(raw); e["dsz"] = len(data); e["chunks"] = chunks
            return e
    raise KeyError(name_sub)

def add_entry(entries, name, data):
    raw, chunks = zlib1(data)
    entries.append(dict(name=name, dsz=len(data), csz=len(raw), chunks=chunks, raw=raw))

if __name__ == "__main__":
    src = sys.argv[1]
    base, ents = read_bnk(src)
    print(f"read {len(ents)} entries, base_offset={hex(base)}")
    if len(sys.argv) > 2 and sys.argv[2] == "--roundtrip":
        out = sys.argv[3]
        write_bnk(out, base, ents)
        print("wrote roundtrip", out)
