"""Compare the genuine virtual display bounds provider with retail x87 behavior."""
import itertools
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one as V
from gate_game_component_abi import DEFAULT_RETAIL, retail_bytes
from variable_allocator_parity import check
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EAX, UC_X86_REG_FPCW

ROOT = Path(__file__).resolve().parents[2]
w = Path(tempfile.mkdtemp(prefix='native-visible-area-'))
env = V.env()
sources = ['rebuild/src/compiled/00/9b/CDisplayManager_GetVVisibleTelevisionScreenArea_009bece0.cpp',
           'rebuild/tests/integration/NativeVisibleArea_test.cpp']
objects = []
for i, source in enumerate(sources):
    obj = w / ('%d.obj' % i)
    r = V.cl(['/nologo', '/c', '/O2', '/MT', '/Fo'+str(obj), str(ROOT/source)], env, w)
    assert r.returncode == 0, r.stdout+r.stderr
    objects.append(obj)
parity = check(objects[0], '?GetVVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ',
               0x9bece0, 38, 2, {'?g_FableVirtualWidth_013961E8@@3NA': 0x13961e8,
                                '?g_FableVirtualHeight_013961F0@@3NA': 0x13961f0})
exe = w/'fixture.exe'
cmd = [str(V.VC/'bin/link.exe'), '/nologo', '/out:'+str(exe), 'kernel32.lib']
r = subprocess.run(cmd+[str(objects[1])], env=env, capture_output=True, text=True)
assert r.returncode != 0, 'Missing production provider must fail to link'
r = subprocess.run(cmd+list(map(str, objects)), env=env, capture_output=True, text=True)
assert r.returncode == 0, r.stdout+r.stderr

uc = Uc(UC_ARCH_X86, UC_MODE_32)
uc.mem_map(0x9be000, 0x1000)
uc.mem_write(0x9bece0, retail_bytes(DEFAULT_RETAIL.read_bytes(), 0x9bece0, 38))
uc.mem_map(0x1396000, 0x1000)
uc.mem_map(0x24000000, 0x10000)
stack, result, stop = 0x24008000, 0x24001000, 0x2400f000
values = [0.0, -0.0, 1.0, -1.0, 640.0, 480.0, 1920.0, 1080.0,
          1.0+2**-24, -1.0-2**-24, 1.0+3*2**-25, 2**-140,
          -2**-140, 2**100+2**76, -(2**100+2**76), 12345.6789]
inputs, expected = bytearray(), bytearray()
cases = 0
for mode, width, height in itertools.product(range(4), values, values):
    inputs.extend(struct.pack('<Idd', mode, width, height))
    uc.mem_write(0x13961e8, struct.pack('<dd', width, height))
    uc.mem_write(result-16, b'\xa5'*48)
    uc.mem_write(stack, struct.pack('<II', stop, result))
    uc.reg_write(UC_X86_REG_ESP, stack)
    uc.reg_write(UC_X86_REG_ECX, 0)  # Retail never dereferences the receiver.
    uc.reg_write(UC_X86_REG_FPCW, 0x37f | (mode << 10))
    uc.emu_start(0x9bece0, stop, count=32)
    assert uc.reg_read(UC_X86_REG_EIP) == stop
    assert uc.reg_read(UC_X86_REG_ESP) == stack+8
    assert uc.reg_read(UC_X86_REG_EAX) == result
    assert uc.mem_read(result-16, 16) == b'\xa5'*16 and uc.mem_read(result+16, 16) == b'\xa5'*16
    expected.extend(uc.mem_read(result, 16))
    cases += 1
(w/'input.bin').write_bytes(inputs)
r = subprocess.run([str(exe), str(w/'input.bin'), str(w/'actual.bin')], env=env, capture_output=True, text=True, timeout=30)
assert r.returncode == 0, (r.returncode, r.stdout, r.stderr, w)
assert (w/'actual.bin').read_bytes() == expected, w
report = dict(provider=parity, cases=cases, omission_checks=1,
              boundary='Full virtual-canvas output, four x87 rounding modes, unchanged receiver. No rendering or visual checkpoint claimed.')
(ROOT/'rebuild/compile-gate/native-visible-area.json').write_text(json.dumps(report, indent=2)+'\n')
print(r.stdout.strip())
