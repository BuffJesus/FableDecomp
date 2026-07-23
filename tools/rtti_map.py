"""Map vtable slots between FableWin (PDB-named donor) and retail Fable.exe
via MSVC RTTI: TypeDescriptor -> CompleteObjectLocator -> vtable."""
import struct, sys

class PE:
    def __init__(self, path):
        self.data = open(path, "rb").read()
        d = self.data
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        nsec = struct.unpack_from("<H", d, pe + 6)[0]
        opt_size = struct.unpack_from("<H", d, pe + 20)[0]
        self.base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
        sec0 = pe + 24 + opt_size
        self.sections = []
        for i in range(nsec):
            o = sec0 + i * 40
            name = d[o:o+8].rstrip(b"\0").decode("latin1")
            vsize, va, rsize, raw = struct.unpack_from("<IIII", d, o + 8)
            self.sections.append((name, va, vsize, raw, rsize))

    def va_to_off(self, va):
        rva = va - self.base
        for name, sva, vsize, raw, rsize in self.sections:
            if sva <= rva < sva + max(vsize, rsize):
                return raw + (rva - sva)
        return None

    def off_to_va(self, off):
        for name, sva, vsize, raw, rsize in self.sections:
            if raw <= off < raw + rsize:
                return self.base + sva + (off - raw)
        return None

    def u32_at_va(self, va):
        off = self.va_to_off(va)
        return None if off is None else struct.unpack_from("<I", self.data, off)[0]

    def in_text(self, va):
        rva = va - self.base
        for name, sva, vsize, raw, rsize in self.sections:
            if name == ".text" and sva <= rva < sva + vsize:
                return True
        return False

    def find_all(self, needle):
        offs, start = [], 0
        while True:
            i = self.data.find(needle, start)
            if i < 0:
                return offs
            offs.append(i)
            start = i + 1

def vtables_for_class(pe, mangled):
    """Returns list of (col_va, vtable_va, [slot VAs])."""
    results = []
    for name_off in pe.find_all(mangled + b"\0"):
        td_va = pe.off_to_va(name_off) - 8  # {pVFTable, spare, name[]}
        # COLs reference the TD at +12
        for ptr_off in pe.find_all(struct.pack("<I", td_va)):
            col_off = ptr_off - 12
            if col_off < 0:
                continue
            sig, offset, cd = struct.unpack_from("<III", pe.data, col_off)
            if sig != 0 or offset > 0x100000 or cd > 0x100000:
                continue
            col_va = pe.off_to_va(col_off)
            if col_va is None:
                continue
            # vtable is preceded by a pointer to the COL
            for meta_off in pe.find_all(struct.pack("<I", col_va)):
                vt_off = meta_off + 4
                vt_va = pe.off_to_va(vt_off)
                if vt_va is None:
                    continue
                slots = []
                o = vt_off
                while o + 4 <= len(pe.data):
                    fn = struct.unpack_from("<I", pe.data, o)[0]
                    if not pe.in_text(fn):
                        break
                    slots.append(fn)
                    o += 4
                if slots:
                    results.append((col_va, offset, vt_va, slots))
    return results

def resolve_thunk(pe, va):
    """Follow one 'jmp rel32' (incremental-link thunk) if present."""
    off = pe.va_to_off(va)
    if off is not None and pe.data[off] == 0xE9:
        rel = struct.unpack_from("<i", pe.data, off + 1)[0]
        return va + 5 + rel
    return va

def main():
    donor = PE(r"D:\Documents\FableTLC\symbols\Fable_Anniversary-2013-02-25\Fable\FableWin.exe")
    retail = PE(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
    print(f"donor base {hex(donor.base)}, retail base {hex(retail.base)}")

    names = {}
    for line in open(r"D:\Documents\FableTLC\ghidra_out\fablewin_pdb_names.tsv", encoding="utf-8", errors="replace"):
        parts = line.rstrip("\n").split("\t")
        if len(parts) >= 2:
            names[int(parts[0], 16)] = parts[1]

    for cls in sys.argv[1:]:
        mangled = (".?AV" + cls + "@@").encode()
        print(f"\n===== {cls} =====")
        dvt = vtables_for_class(donor, mangled)
        rvt = vtables_for_class(retail, mangled)
        print(f"donor vtables: {len(dvt)}, retail vtables: {len(rvt)}")
        for col_va, offset, vt_va, slots in dvt:
            print(f"donor vtable @ {hex(vt_va)} (COL offset {offset}), {len(slots)} slots")
            for i, fn in enumerate(slots):
                target = resolve_thunk(donor, fn)
                print(f"  [{i:2d}] {hex(target)}  {names.get(target, '?')}")
        for col_va, offset, vt_va, slots in rvt:
            print(f"retail vtable @ {hex(vt_va)} (COL offset {offset}), {len(slots)} slots")
            for i, fn in enumerate(slots):
                print(f"  [{i:2d}] {hex(fn)}")

if __name__ == "__main__":
    main()
