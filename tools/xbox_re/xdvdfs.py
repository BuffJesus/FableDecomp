import sys, os, struct, datetime

SECTOR = 2048
MAGIC = b"MICROSOFT*XBOX*MEDIA"

def find_partition_base(f, size):
    # descriptor sits at partition_base + 32*SECTOR ; magic at start and +0x7EC
    # scan common bases plus brute search on sector 32 boundaries
    candidates = [0, 0x18300000, 0xFD90000, 0x1FB20000, 0x81E80000]
    for base in candidates:
        off = base + 32*SECTOR
        if off+20 <= size:
            f.seek(off);
            if f.read(20) == MAGIC:
                return base
    # brute: scan whole file for magic, back off 32 sectors
    f.seek(0)
    CH = 1<<20
    pos = 0
    while pos < size:
        f.seek(pos); buf = f.read(CH+len(MAGIC))
        i = buf.find(MAGIC)
        if i != -1:
            desc = pos+i
            base = desc - 32*SECTOR
            if base >= 0:
                return base
        pos += CH
    return None

def read_dir(f, base, sector, dsize, path, out):
    # directory table: entries in a contiguous region starting at `sector`
    data_off = base + sector*SECTOR
    f.seek(data_off); table = f.read(dsize)
    # binary tree of entries indexed by 16-bit left/right offsets (in 4-byte units)
    def walk(pos):
        if pos*4 + 14 > len(table): return
        left, right = struct.unpack_from('<HH', table, pos*4)
        start_sec, fsize, attr, nlen = struct.unpack_from('<IIBB', table, pos*4+4)
        name = table[pos*4+14:pos*4+14+nlen].decode('latin-1', 'replace')
        if left: walk(left)
        is_dir = bool(attr & 0x10)
        full = path + '/' + name
        out.append((full, is_dir, fsize, start_sec))
        if is_dir and fsize and start_sec:
            read_dir(f, base, start_sec, fsize, full, out)
        if right: walk(right)
    if dsize: walk(0)

def main():
    path = sys.argv[1]
    size = os.path.getsize(path)
    with open(path,'rb') as f:
        base = find_partition_base(f, size)
        if base is None:
            print("No XDVDFS partition found"); return
        print(f"Partition base: 0x{base:X}")
        f.seek(base+32*SECTOR+20)
        root_sec, root_size = struct.unpack('<II', f.read(8))
        print(f"Root dir sector {root_sec} size {root_size}")
        out=[]
        read_dir(f, base, root_sec, root_size, '', out)
        out.sort()
        for full, is_dir, fsize, sec in out:
            print(f"{'D' if is_dir else 'F'}\t{fsize}\t{full}")

if __name__=='__main__':
    main()
