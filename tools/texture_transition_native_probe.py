"""Bounded execution of original retail texture allocation/constructor blocks.

Does not launch or modify Fable. Pool construction runs real x86 instructions;
heap, vector append, pixel-format construction and size calculation are explicit
boundaries. BeginLoading runs to its allocation decision, not file/GPU upload.
"""
from pathlib import Path
import argparse
import hashlib
import json
import struct
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

PIN = '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10'
PATCHES = {0xB3D7F3: 4, 0xB3DA10: 16, 0xB3DAEA: 8, 0xB3DB5B: 8}
STACK, HEAP, STATE = 0x3000000, 0x3100000, 0x3200000
DXT1, DXT3 = 0x31545844, 0x33545844


def size_of(w, h, mips, fmt):
    bits, minimum = (4, 4) if fmt == DXT1 else (8, 4) if fmt == DXT3 else (32, 1)
    pixels = 0
    while w or h:
        pixels += max(w, minimum) * max(h, minimum)
        if not mips:
            break
        w >>= 1
        h >>= 1
    return pixels * bits // 8


class Machine:
    def __init__(self, pe):
        self.u = Uc(UC_ARCH_X86, UC_MODE_32)
        self.u.mem_map(0x400000, 0x1800000)
        for section in pe.sections:
            self.u.mem_write(0x400000 + section.VirtualAddress, section.get_data())
        for base in (STACK, HEAP, STATE):
            self.u.mem_map(base, 0x10000)
        self.sp = STACK + 0x8000
        self.u.reg_write(UC_X86_REG_ESP, self.sp)

    def get(self, addr):
        return struct.unpack('<I', self.u.mem_read(addr, 4))[0]

    def put(self, addr, value):
        self.u.mem_write(addr, struct.pack('<I', value & 0xffffffff))

    def ret(self, args, value=0):
        sp = self.u.reg_read(UC_X86_REG_ESP)
        target = self.get(sp)
        self.u.reg_write(UC_X86_REG_EAX, value)
        self.u.reg_write(UC_X86_REG_ESP, sp + 4 + args)
        self.u.reg_write(UC_X86_REG_EIP, target)


def pools(pe, patched, budget):
    m = Machine(pe)
    u = m.u
    if patched:
        for addr, value in PATCHES.items():
            u.mem_write(addr, bytes([value]))
    u.reg_write(UC_X86_REG_ESI, STATE)
    u.reg_write(UC_X86_REG_EDI, 0)
    u.reg_write(UC_X86_REG_EBX, 1)
    m.put(STATE + 0x958, STATE + 0x2000)
    m.put(STATE + 0x2000 + 0x280, STATE + 0x4000)
    m.put(STATE + 0x2000 + 0x2a0, 22)
    m.put(STATE + 0x2000 + 0x2a4, 21)
    m.put(m.sp + 0xe0, STATE + 0x6000)
    m.put(STATE + 0x6018, budget)
    m.put(0x1436eb0, 0)  # Exercise constructor's explicit config-budget branch.
    calculations = []

    def hook(uc, addr, length, _):
        sp = uc.reg_read(UC_X86_REG_ESP)
        if addr == 0x9e3830:
            m.put(uc.reg_read(UC_X86_REG_ECX), m.get(sp + 4))
            m.ret(4)
        elif addr == 0xa698f0:
            w, h = uc.reg_read(UC_X86_REG_ECX), uc.reg_read(UC_X86_REG_EDX)
            mip, fmt = m.get(sp + 4), m.get(m.get(sp + 8))
            result = size_of(w, h, mip, fmt)
            calculations.append(dict(width=w, height=h, bytes=result))
            m.ret(8, result)
        elif addr == 0xbfea0e:
            assert m.get(sp + 4) == 200
            m.ret(0, HEAP)
        elif addr == 0xb3ea70:
            vector = uc.reg_read(UC_X86_REG_ECX)
            begin, end = m.get(vector), m.get(vector + 4)
            assert begin == HEAP and HEAP <= end < HEAP + 0x1000
            source = m.get(sp + 8)
            uc.mem_write(end, bytes(uc.mem_read(source, 20)))
            m.put(vector + 4, end + 20)
            m.put(vector + 8, HEAP + 0x1000)
            m.ret(20)
        elif addr == 0xa6aaa0:
            uc.emu_stop()
        elif not 0xb3d7d0 <= addr < 0xb3dbcf:
            raise AssertionError(f'unexpected boundary {addr:08x}')

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(0xb3d7d0, 0xb3dbcf, count=10000)
    assert u.reg_read(UC_X86_REG_EIP) == 0xa6aaa0
    vec = m.get(u.reg_read(UC_X86_REG_ESP) + 4)
    begin, end = m.get(vec), m.get(vec + 4)
    assert end - begin == 13 * 20
    rows = []
    for address in range(begin, end, 20):
        w, h, fmt, mip, count = struct.unpack('<5I', u.mem_read(address, 20))
        rows.append(dict(width=w, height=h, format=hex(fmt), mipmaps=bool(mip & 255),
                         count=count, slotBytes=size_of(w, h, mip & 255, fmt)))
    return dict(patched=patched, budgetKiB=budget, denominator=calculations, pools=rows)


def promotion(pe, width, current, requested, fail_above):
    m = Machine(pe)
    u = m.u
    frame, infoframe, info, bank, managed = [STATE + n for n in (0, 0x100, 0x200, 0x1000, 0x2000)]
    m.put(m.sp, 0x400000)
    m.put(m.sp + 4, requested)
    m.put(m.sp + 8, 0)
    m.put(frame + 0x40, infoframe)
    m.put(infoframe + 0xc, info)
    m.put(infoframe + 8, frame)
    u.mem_write(infoframe + 6, bytes([width.bit_length() - 6])) # smallest resident:32.
    u.mem_write(info, struct.pack('<3H', width, width, 0))
    u.mem_write(info + 0x11, bytes([width.bit_length() - 2]))
    m.put(info + 0x20, bank)
    m.put(bank + 0x280, STATE + 0x3000)
    m.put(bank + 0x288, DXT1)
    if current is not None:
        m.put(frame + 0x24, managed)
        u.mem_write(frame + 0x38, bytes([current]))
    u.reg_write(UC_X86_REG_ECX, frame)
    attempts = []

    def hook(uc, addr, length, _):
        sp = uc.reg_read(UC_X86_REG_ESP)
        if addr == 0x99a2d0:  # trivial base constructor, no allocation policy.
            m.ret(0)
        elif addr == 0x9cc210:  # ordinary 2D graphic.
            m.ret(4, 0)
        elif addr == 0xa69b70:
            args = struct.unpack('<6I', uc.mem_read(sp + 4, 24))
            _, w, h, levels, fmt, owner = args
            attempts.append(dict(width=w, height=h, levels=levels, success=w <= fail_above))
            assert m.get(fmt) == DXT1 and owner == frame
            m.ret(24, int(w <= fail_above))
        elif addr == 0x9fe0dd:
            uc.emu_stop()
        elif not 0x9fde80 <= addr < 0x9fe380:
            raise AssertionError(f'unexpected promotion boundary {addr:08x}')

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(0x9fde80, 0x400000, count=10000)
    assert u.reg_read(UC_X86_REG_EIP) == 0x9fe0dd
    assert attempts
    return dict(width=width, currentMip=current, requestedMip=requested,
                allocationLimit=fail_above, attempts=attempts)


def retention(pe, reduce, unload):
    m = Machine(pe)
    u = m.u
    u.reg_write(UC_X86_REG_ECX, STATE)
    stub = HEAP
    # Startup replacement used by TextureRetentionProbe. Replay the actual
    # constructor around it, including its second use of EAX for bank+0x19c.
    if (reduce, unload) != (3, 30):
        assert bytes(u.mem_read(0x9fd512, 6)) == bytes.fromhex('89866c020000')
        assert bytes(u.mem_read(0x9fd4e8, 10)) == bytes.fromhex('c786680200001e000000')
        u.mem_write(stub, bytes.fromhex('c7866c020000') + struct.pack('<I', reduce)
                    + b'\xe9' + struct.pack('<i', 0x9fd518-(stub+15)))
        u.mem_write(0x9fd512, b'\xe9' + struct.pack('<i', stub-0x9fd517) + b'\x90')
        m.put(0x9fd4ee, unload)
    u.emu_start(0x9fd4e0, 0x9fd51e, count=100)
    assert m.get(STATE+0x268) == unload and m.get(STATE+0x26c) == reduce
    assert m.get(STATE+0x19c) == 3 and m.get(STATE+0x270) == 1
    assert m.get(STATE+0x274) == 0x200000 and u.reg_read(UC_X86_REG_EAX) == 3
    state = bytes(u.mem_read(STATE, 0x300))
    state = state[:0x268] + b'\0'*8 + state[0x270:]
    registers = [u.reg_read(r) for r in (UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,
        UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EFLAGS)]
    return dict(reduce=reduce, unload=unload, otherStateSha256=hashlib.sha256(state).hexdigest(), registers=registers)


def request_scale(pe, width, quality):
    m=Machine(pe);u=m.u
    u.reg_write(UC_X86_REG_EBX,STATE+0x268)
    m.put(m.sp+4,width);m.put(0x1436ea0,STATE+0x1000)
    projection=.8626059
    for address,value in ((STATE+0xa4,640.),(STATE+0xac,quality),(STATE+0x1000+0xd4,projection)):
        u.mem_write(address,struct.pack('<f',value))
    # Run real FrameStart arithmetic and the real _ftol2 implementation.
    u.emu_start(0xb327a6,0xb3280b,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==0xb3280b
    mip=struct.unpack('<f',u.mem_read(STATE+0xb0,4))[0]
    lod=struct.unpack('<f',u.mem_read(STATE+0xa8,4))[0]
    assert abs(mip-min(width,640)*projection*quality)<.001
    assert abs(lod-640*projection)<.001
    return dict(screenWidth=width,quality=quality,mipScale=mip,meshProjection=lod)


def frame_end_retention(pe,reduce):
    m=Machine(pe);u=m.u
    u.reg_write(UC_X86_REG_EDI,STATE)
    m.put(STATE+0x954,STATE+0x2000)
    assert bytes(u.mem_read(0xb3b8ae,2))==b'\x6a\x03'
    u.mem_write(0xb3b8af,bytes([reduce]))
    u.emu_start(0xb3b8a8,0xb3b8b5,count=100)
    assert m.get(STATE+0x2000+0x26c)==reduce
    assert m.get(STATE+0x2000+0x19c)==reduce
    assert u.reg_read(UC_X86_REG_ESP)==m.sp
    return dict(reduce=reduce,bankDelay=reduce,resourceDelay=reduce)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--exe', type=Path, default=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe'))
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    data = args.exe.read_bytes()
    assert hashlib.sha256(data).hexdigest() == PIN
    pe = pefile.PE(data=data)
    setup = [pools(pe, patched, budget) for budget in (6144, 10240, 16384) for patched in (False, True)]
    cases = [promotion(pe, width, current, requested, limit)
             for width in (512, 1024, 2048) for current in (None, 4)
             for requested in (0, 1, 2) for limit in (256, width)]
    for case in cases:
        assert case['attempts'][0]['width'] == case['width'] >> case['requestedMip']
        for previous, following in zip(case['attempts'], case['attempts'][1:]):
            assert not previous['success'] and following['width'] == previous['width'] // 2
    for original, patched in zip(setup[::2], setup[1::2]):
        assert original['denominator'][0]['width'] == 512
        assert patched['denominator'][0]['width'] == 1024
        assert patched['pools'][8]['width'] == 4096
        assert patched['pools'][9]['width'] == 4096
        assert patched['pools'][11]['width'] == 2048
        assert patched['pools'][12]['width'] == 2048
    retention_cases = [retention(pe, r, d) for r, d in ((3,30),(30,120),(60,240))]
    for result in retention_cases[1:]:
        assert result['otherStateSha256'] == retention_cases[0]['otherStateSha256']
        assert result['registers'] == retention_cases[0]['registers']
    request_cases=[request_scale(pe,w,q) for w in (320,640,1024,2560) for q in (1.,1.5,2.)]
    end_cases=[frame_end_retention(pe,r) for r in (3,30,60)]
    report = dict(state='PASS', executableSha256=PIN, poolCases=setup, promotionCases=cases,
                  retentionCases=retention_cases,requestScaleCases=request_cases,frameEndRetentionCases=end_cases,
                  boundaries=__doc__, limitations=['No live residency, GPU, file I/O or visual transition claim.',
                  'Pool descriptors precede sorting/initialization and later video-option resizing.'])
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, indent=2) + '\n')
    print(f'PASS {len(setup)} native pool constructions; {len(cases)} native promotion decisions: {args.out}')


if __name__ == '__main__':
    main()
