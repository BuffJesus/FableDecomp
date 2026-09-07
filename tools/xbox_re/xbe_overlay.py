"""Overlay-aware XBE scanner for Fable's default.xbe.

The XBE has 135 sections: a resident 6.3 MB `.text` plus 66 `S_Q*` per-quest and
43 `S_V*` per-subsystem demand-paged CODE overlays. `xbe_xref.py` only searches
the resident `.text`, so it CANNOT see per-quest / per-script code. This scanner
searches every section.

Usage (default XBE path is D:\\tmp\\fable_xbox\\extracted\\default.xbe):
  python xbe_overlay.py sections                       section table (name / VA / size / file offset)
  python xbe_overlay.py str <text> [...]               locate ASCIIZ strings + every xref, any section
  python xbe_overlay.py imm <int> [...]                push imm32 / mov r32,imm32 anywhere in code
                                                       (Xbox references localized text by NUMERIC id)
  python xbe_overlay.py dis <SECT> [start_va] [end_va]  annotated capstone dump
  python xbe_overlay.py dis 0x<va> [size]               ditto, raw VA window
  python xbe_overlay.py funcs <SECT>                   candidate function starts (xrefs + vtable ptrs)
  python xbe_overlay.py script <SECT>                  engine script-name <-> overlay <-> factory binding

Addressing: file_offset = section.raw + (VA - section.va); see the `sections` output.
"""
import sys
import struct

DEFAULT_XBE = r'D:\tmp\fable_xbox\extracted\default.xbe'
REGS = ['eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi']


class Xbe(object):
    def __init__(self, path=DEFAULT_XBE):
        self.path = path
        self.d = open(path, 'rb').read()
        d = self.d
        assert d[:4] == b'XBEH', 'not an XBE'
        self.base = struct.unpack_from('<I', d, 0x104)[0]
        nsec = struct.unpack_from('<I', d, 0x11C)[0]
        sec_addr = struct.unpack_from('<I', d, 0x120)[0]
        so = sec_addr - self.base
        self.secs = []
        for i in range(nsec):
            o = so + i * 0x38
            flags, va, vsz, raw, rawsz = struct.unpack_from('<IIIII', d, o)
            na = struct.unpack_from('<I', d, o + 0x14)[0]
            nm = d[na - self.base:na - self.base + 24].split(b'\x00')[0].decode('latin-1')
            self.secs.append(dict(name=nm, flags=flags, va=va, vsz=vsz, raw=raw, rawsz=rawsz))

    def sec_by_name(self, nm):
        for s in self.secs:
            if s['name'] == nm:
                return s
        raise KeyError(nm)

    def sec_of_va(self, va):
        for s in self.secs:
            if s['va'] <= va < s['va'] + max(s['vsz'], s['rawsz']):
                return s

    def sec_of_off(self, off):
        for s in self.secs:
            if s['raw'] <= off < s['raw'] + s['rawsz']:
                return s

    def va2off(self, va):
        s = self.sec_of_va(va)
        if not s:
            return None
        o = s['raw'] + (va - s['va'])
        return o if o < s['raw'] + s['rawsz'] else None

    def off2va(self, off):
        s = self.sec_of_off(off)
        return None if not s else s['va'] + (off - s['raw'])

    def bytes_of(self, s):
        return self.d[s['raw']:s['raw'] + s['rawsz']]

    def read(self, va, n):
        o = self.va2off(va)
        return None if o is None else self.d[o:o + n]

    def is_code(self, s):
        return s['name'] == '.text' or s['name'].startswith('S_') or s['name'] == 'CS_OVR'

    def strat(self, va, maxlen=100):
        """ASCIIZ string at VA, or None."""
        o = self.va2off(va)
        if o is None:
            return None
        b = self.d[o:o + maxlen]
        e = b.find(b'\x00')
        if e < 3:
            return None
        s = b[:e]
        if not all(32 <= c < 127 for c in s):
            return None
        return s.decode('ascii')

    def find_strings(self, text):
        """VAs of every NUL-delimited ASCIIZ string exactly equal to `text`."""
        pat = b'\x00' + text.encode() + b'\x00'
        out = []
        i = self.d.find(pat)
        while i != -1:
            va = self.off2va(i + 1)
            if va is not None:
                out.append(va)
            i = self.d.find(pat, i + 1)
        return out

    def find_dword_refs(self, value, only=None):
        """Every little-endian dword `value` in every section (name, VA, file offset)."""
        pat = struct.pack('<I', value)
        out = []
        for s in self.secs:
            if only and s['name'] not in only:
                continue
            b = self.bytes_of(s)
            i = b.find(pat)
            while i != -1:
                out.append((s['name'], s['va'] + i, s['raw'] + i))
                i = b.find(pat, i + 1)
        return out

    def find_imm(self, value):
        """`push imm32` and `mov r32, imm32` occurrences across all code sections."""
        enc = [(b'\x68' + struct.pack('<I', value), 'push')]
        for r in range(0xB8, 0xC0):
            enc.append((bytes([r]) + struct.pack('<I', value), 'mov ' + REGS[r - 0xB8]))
        out = []
        for s in self.secs:
            if not self.is_code(s):
                continue
            b = self.bytes_of(s)
            for pat, kind in enc:
                i = b.find(pat)
                while i != -1:
                    out.append((s['name'], s['va'] + i, kind))
                    i = b.find(pat, i + 1)
        return out

    def code_refs_into(self, lo, hi):
        """rel32 call/jmp targets, push-imm32, and aligned .rdata/.data pointers
        that land inside [lo, hi). Recovers function starts in an overlay."""
        found = {}
        for s in self.secs:
            b = self.bytes_of(s)
            va = s['va']
            if s['name'] in ('.rdata', '.data'):
                for k in range(0, len(b) - 4, 4):
                    v = struct.unpack_from('<I', b, k)[0]
                    if lo <= v < hi:
                        found.setdefault(v, []).append('%s@0x%08X' % (s['name'], va + k))
            if not self.is_code(s):
                continue
            for k in range(len(b) - 5):
                op = b[k]
                if op in (0xE8, 0xE9):
                    t = (va + k + 5 + struct.unpack_from('<i', b, k + 1)[0]) & 0xFFFFFFFF
                    if lo <= t < hi:
                        kind = 'call' if op == 0xE8 else 'jmp'
                        found.setdefault(t, []).append('%s:%s@0x%08X' % (s['name'], kind, va + k))
                elif op == 0x68:
                    t = struct.unpack_from('<I', b, k + 1)[0]
                    if lo <= t < hi:
                        found.setdefault(t, []).append('%s:push@0x%08X' % (s['name'], va + k))
        return found


def disasm(x, va, size, out=sys.stdout):
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    b = x.read(va, size)
    for ins in md.disasm(b, va):
        ann = ''
        for op in ins.operands:
            if op.type == X86_OP_IMM:
                t = x.strat(op.imm)
                if t:
                    ann += '   ; "%s"' % t
                else:
                    s = x.sec_of_va(op.imm)
                    if s and ins.mnemonic in ('call', 'jmp'):
                        ann += '   ; -> %s' % s['name']
            elif op.type == X86_OP_MEM and op.mem.base == 0 and op.mem.index == 0:
                t = x.strat(op.mem.disp)
                if t:
                    ann += '   ; ["%s"]' % t
        print('0x%08X  %-20s %-7s %s%s'
              % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str, ann), file=out)


def cmd_script(x, names):
    """Decode the engine's script-class registry entries that bind a script name
    to an overlay section name and a factory function inside that overlay."""
    for nm in names:
        print('== %s' % nm)
        for sva in x.find_strings(nm):
            refs = x.find_dword_refs(sva)
            if not refs:
                print('   section-name string @0x%08X has no xref' % sva)
            for _n, ref, _o in refs:
                b = x.read(ref - 0x60, 0xC0)
                if not b:
                    continue
                for k in range(len(b) - 5):
                    if b[k] != 0x68:
                        continue
                    t = struct.unpack_from('<I', b, k + 1)[0]
                    st = x.strat(t)
                    sec = x.sec_of_va(t)
                    if st:
                        print('   0x%08X push "%s"' % (ref - 0x60 + k, st))
                    elif sec and sec['name'] == nm:
                        print('   0x%08X push FACTORY 0x%08X [%s]' % (ref - 0x60 + k, t, sec['name']))
                break


def main(argv):
    xbe = DEFAULT_XBE
    if '--xbe' in argv:
        i = argv.index('--xbe')
        xbe = argv[i + 1]
        del argv[i:i + 2]
    cmd = argv[1] if len(argv) > 1 else 'sections'
    x = Xbe(xbe)
    if cmd == 'sections':
        print('BaseAddr=0x%08X  sections=%d  file=%s' % (x.base, len(x.secs), x.path))
        for s in x.secs:
            print('  %-10s VA=0x%08X vsz=0x%06X fileoff=0x%08X rawsz=0x%06X flags=0x%X'
                  % (s['name'], s['va'], s['vsz'], s['raw'], s['rawsz'], s['flags']))
    elif cmd == 'str':
        for t in argv[2:]:
            vas = x.find_strings(t)
            if not vas:
                print('%-30s ABSENT' % t)
                continue
            for va in vas:
                r = x.find_dword_refs(va)
                txt = ', '.join('%s@0x%08X' % (n, v) for n, v, _o in r) or 'NONE'
                print('%-30s str@0x%08X (%s)  refs: %s' % (t, va, x.sec_of_va(va)['name'], txt))
    elif cmd == 'imm':
        for a in argv[2:]:
            v = int(a, 0)
            h = x.find_imm(v)
            txt = ', '.join('%s@0x%08X(%s)' % (n, va, k) for n, va, k in h)
            print('%-10s (0x%X): %s' % (a, v, txt or 'NO CODE IMMEDIATE ANYWHERE'))
    elif cmd == 'dis':
        tgt = argv[2]
        if tgt.startswith('0x'):
            va = int(tgt, 0)
            size = int(argv[3], 0) if len(argv) > 3 else 0x80
        else:
            s = x.sec_by_name(tgt)
            va = int(argv[3], 0) if len(argv) > 3 else s['va']
            end = int(argv[4], 0) if len(argv) > 4 else s['va'] + s['rawsz']
            size = end - va
        disasm(x, va, size)
    elif cmd == 'funcs':
        s = x.sec_by_name(argv[2])
        lo = s['va']
        hi = s['va'] + s['rawsz']
        found = x.code_refs_into(lo, hi)
        print('== %s VA 0x%08X..0x%08X fileoff 0x%08X' % (s['name'], lo, hi, s['raw']))
        for k in sorted(found):
            print('  0x%08X (fileoff 0x%08X)  <- %s'
                  % (k, x.va2off(k), ', '.join(sorted(set(found[k])))))
    elif cmd == 'script':
        cmd_script(x, argv[2:])
    else:
        print(__doc__)


if __name__ == '__main__':
    main(sys.argv)
