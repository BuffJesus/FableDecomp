"""Pinned native speaker translation and gate/channel-selection slices.

Only the COM GetSpeakerConfig call is doubled. No game launch or patch.
"""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path
sys.path.insert(0, str(Path('work/runtime_re_tools').resolve()))
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--output', type=Path, required=True)
    args = ap.parse_args()
    exe = Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe')
    digest = hashlib.sha256(exe.read_bytes()).hexdigest()
    assert digest == '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10'
    im = pefile.PE(str(exe)).get_memory_mapped_image()
    expected_modes = {1: 1, 2: 2, 3: 8, 4: 4, 6: 64, 7: 128}
    rows = []
    for config in [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 255, 0x140004]:
        u = Uc(UC_ARCH_X86, UC_MODE_32)
        u.mem_map(0x400000, (len(im)+4095)&~4095)
        u.mem_write(0x400000, im)
        u.mem_map(0x3000000, 0x10000)
        obj, com, vt, stub, stack, stop = (0x3001000, 0x3002000, 0x3003000,
                                          0x3004000, 0x3008000, 0x3009000)
        def put(a, *values):
            u.mem_write(a, struct.pack('<'+'I'*len(values), *values))
        def get(a):
            return struct.unpack('<I', u.mem_read(a, 4))[0]
        put(obj+8, com)
        put(com, vt)
        put(vt+0x20, stub)
        # stdcall HRESULT GetSpeakerConfig(this, out), returns S_OK.
        u.mem_write(stub, b'\x31\xc0\xc2\x08\x00')
        calls = []
        def query(machine, address, size, data):
            if address == stub:
                sp = machine.reg_read(UC_X86_REG_ESP)
                assert get(sp+4) == com
                put(get(sp+8), config)
                calls.append(config)
        u.hook_add(UC_HOOK_CODE, query)
        put(stack, stop)
        u.reg_write(UC_X86_REG_ESP, stack)
        u.reg_write(UC_X86_REG_ECX, obj)
        u.emu_start(0xC53340, stop, count=500)
        mode = get(obj+4)
        assert mode == expected_modes.get(config & 255, 0x80000000)
        assert calls == [config]
        assert u.reg_read(UC_X86_REG_ESP) == stack+4
        # Actual constructor gate instructions, stop before unrelated init.
        put(stack+8, mode)
        u.reg_write(UC_X86_REG_ESP, stack)
        u.reg_write(UC_X86_REG_ESI, obj)
        u.mem_write(obj+0x19, b'\x00')
        u.emu_start(0xC1C738, 0xC1C74F, count=100)
        gate = u.mem_read(obj+0x19, 1)[0]
        assert gate == int(mode in (1, 2, 4))
        # Actual selector up to first allocation/configuration side effect.
        put(stack, stop, mode)
        u.reg_write(UC_X86_REG_ESP, stack)
        u.reg_write(UC_X86_REG_ECX, obj)
        u.emu_start(0xC3F110, 0xC3F16E, count=100)
        channels = u.reg_read(UC_X86_REG_EDI)
        selected = get(obj+0x1B8)
        assert (selected, channels) == {1: (1, 2), 4: (4, 2), 8: (8, 4)}.get(mode, (64, 6))
        rows.append(dict(config=config, translated_mode=mode,
                         position_update_gate=gate, initial_selected_mode=selected,
                         initial_channel_count=channels))
    result = dict(exe_sha256=digest, passed=True, cases=rows,
                  limits='Synthetic COM results. Native selector may retry after device failure; no live game mode or final audible output proved.',
                  functions=['C53340 full successful-query path', 'C1C738..C1C74F gate slice', 'C3F110..C3F16E channel selection slice'])
    with args.output.open('x') as f:
        json.dump(result, f, indent=2)
    print(f'{len(rows)} native speaker-mode/gate/channel cases passed')


if __name__ == '__main__':
    main()
