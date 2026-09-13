"""Execute retail C41A70 directional factor; synthetic inputs, no live-cause claim."""
import argparse
import hashlib
import json
import math
import struct
import sys
from pathlib import Path
sys.path.insert(0, str(Path('work/runtime_re_tools').resolve()))
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    exe = Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe')
    digest = hashlib.sha256(exe.read_bytes()).hexdigest()
    assert digest == '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10'
    image = pefile.PE(str(exe)).get_memory_mapped_image()
    rows = []
    for inner, outer, outside in [(360, 360, 1), (30, 90, 0.001), (0, 180, 0.1)]:
        for angle in [0, 15, 30, 45, 60, 90, 120, 180]:
            u = Uc(UC_ARCH_X86, UC_MODE_32)
            u.mem_map(0x400000, (len(image)+4095)&~4095)
            u.mem_write(0x400000, image)
            u.mem_map(0x3000000, 0x10000)
            actor, stack, result, trampoline = 0x3001000, 0x3008000, 0x3009000, 0x300a000
            # Return trampoline stores native x87 ST(0) without a math double.
            u.mem_write(trampoline, b'\xd9\x1d' + struct.pack('<I', result))
            u.mem_write(stack, struct.pack('<I', trampoline))
            u.mem_write(actor+0x114, struct.pack('<fff', 0, 0, 1))
            a = math.radians(angle)
            u.mem_write(actor+0x120, struct.pack('<fff', math.sin(a), 0, math.cos(a)))
            u.mem_write(actor+0x190, struct.pack('<IIf', inner, outer, outside))
            u.reg_write(UC_X86_REG_ECX, actor)
            u.reg_write(UC_X86_REG_ESP, stack)
            u.emu_start(0xC41A70, trampoline+6, count=1000)
            value = struct.unpack('<f', u.mem_read(result, 4))[0]
            # Determine the actual angle scale from native constants, not an API guess.
            scale = struct.unpack_from('<f', image, 0x12B7B7C-0x400000)[0]
            # This function compares against the opposite of the source vector:
            # C41B14/C41B1A subtract the half-angle result from 180 degrees.
            native_angle = (180-angle)*math.pi/180/2*scale
            expected = 1 if native_angle <= inner else outside if native_angle >= outer else 1+(outside-1)*(native_angle-inner)/(outer-inner)
            assert math.isclose(value, expected, abs_tol=2e-6), (angle, native_angle, value, expected)
            assert u.reg_read(UC_X86_REG_ESP) == stack+4
            rows.append(dict(inner=inner, outer=outer, outside=outside, angle=angle, native_angle=native_angle, factor=value))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open('x') as f:
        json.dump(dict(exe_sha256=digest, function='0xC41A70',
                       code_sha256=hashlib.sha256(image[0xC41A70-0x400000:0xC41BA4-0x400000]).hexdigest(),
                       cases=rows, passed=True,
                       limits='Synthetic fields; no evidence Father uses these cone values or that this factor caused his quiet speech.'), f, indent=2)
    print(f'{len(rows)} native directional-factor cases passed')

if __name__ == '__main__':
    main()
