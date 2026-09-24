#!/usr/bin/env python3
"""Check CManager::GetUIScale and effective coordinate dimensions without doubles."""
import hashlib
import itertools
import json
import math
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run
from check_ui_transform import bits, number
from check_frontend_startup import relocation_symbols


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle executable changed')
    machine = Uc(UC_ARCH_X86, UC_MODE_32); machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, output, getter = 0x20000000, 0x20008000, 0x20010000, 0x20011000
    def put(address, value): machine.mem_write(address, struct.pack('<I', value))
    # Tiny caller stores each returned x87 float; the actual getters are unchanged.
    for i, address in enumerate((0x41CC14, 0x41CC2B)):
        code = b'\xe8'+struct.pack('<i', address-(getter+i*16+5))+b'\xd9\x1d'+struct.pack('<I', output+8+i*4)+b'\xc3'
        machine.mem_write(getter+i*16, code)
    values = list(map(bits, (0, -1, 640, 767.5, 768, 1023.5, 1024, 1280, 1920)))+[0x7FC00000, 0x7F800000]
    cases = [[context, enabled, x, y, bits(1280), bits(720)]
             for context, enabled, x, y in itertools.product(range(2), (0, 1, 255), values, values)]
    cases += [[context, enabled, bits(1920), bits(1080), x, y]
              for context, enabled, x, y in itertools.product(range(2), (0, 1), values, values)]
    directory = ROOT/'work/ui_scale_check'; directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str, row)) for row in cases)+'\n')
    objects, env = [], parity.env()
    sources = ['rebuild/src/compiled/00/41/'+name+'.cpp' for name in
               ('CManager_GetUIScale_0041cf47', 'global_GetCoordinateWidth_0041cc14', 'global_GetCoordinateHeight_0041cc2b')]
    sources.append('rebuild/tests/integration/UiScale_test.cpp')
    for i, source in enumerate(sources):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy', '/I'+str(ROOT/'rebuild/include'),
             '/Fo'+str(obj), ROOT/source], env); objects.append(obj)
    exe = directory/'behavior.exe'; run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete scale results')
    errors = []
    for index, (case, line) in enumerate(zip(cases, lines)):
        context, enabled, x, y, target_x, target_y = case
        put(0x13B86A0, 0x1234 if context else 0); machine.mem_write(0x13B8768, bytes((enabled,)))
        machine.mem_write(0x1375CD4, struct.pack('<II', x, y)); machine.mem_write(0x13B876C, struct.pack('<II', target_x, target_y))
        for address in (0x41CF47, getter, getter+16):
            put(stack, stop); put(stack+4, output)
            machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, 0)
            machine.reg_write(UC_X86_REG_FPCW, 0x37F); machine.emu_start(address, stop, count=1000)
            if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError('Native scale did not return')
            if address == 0x41CF47 and machine.reg_read(UC_X86_REG_EAX) != output: raise RuntimeError('Native scale returned wrong pointer')
        expected = list(struct.unpack('<IIII', machine.mem_read(output, 16)))
        actual = list(map(int, line.split()))
        if len(actual) != 4 or not all(a == b or (math.isnan(number(a)) and math.isnan(number(b))) for a,b in zip(actual, expected)):
            errors.append({'case': index, 'actual': actual, 'retail': expected})
    disassemblies = [run([parity.OBJDUMP, '-dr', obj], env) for obj in objects[:3]]
    for i, assembly in enumerate(disassemblies): (directory/f'part{i}.asm').write_text(assembly)
    globals_by_symbol = {'?FableUiCoordinateConversionEnabled@@3EA': 0x13B8768,
                         '?FableUiCoordinateDestinationExtent@@3UFableUiStateVector2@@A': 0x13B876C,
                         '?FableUiCoordinateSourceExtent@@3UFableUiStateVector2@@A': 0x1375CD4}
    getter_parity = []
    for i, (symbol_name, address) in enumerate((('FableUiGetCoordinateWidth', 0x41CC14), ('FableUiGetCoordinateHeight', 0x41CC2B)), 1):
        body, section, symbol = parity.obj_text(objects[i], '?'+symbol_name+'@@')
        linked = bytearray(body)
        relocations = relocation_symbols(disassemblies[i], symbol)
        if len(relocations) != 3: raise RuntimeError('Unexpected dimension getter relocations')
        for offset, kind, target in relocations:
            if kind != 'dir32' or target not in globals_by_symbol: raise RuntimeError('Unreviewed dimension getter dependency')
            addend = struct.unpack_from('<I', linked, offset)[0]
            struct.pack_into('<I', linked, offset, globals_by_symbol[target]+addend)
        offset = pe_oracle.va_to_off(pe_oracle.pe_sections(image), address)
        getter_parity.append('RELOCATION_MATCH' if linked == image[offset:offset+23] else 'DIFFER')
    accepted = not errors and all(status == 'RELOCATION_MATCH' for status in getter_parity)
    report = {'accepted': accepted, 'cases': len(cases), 'errors': errors, 'getter_parity': getter_parity,
              'scope': 'complete native manager scale and both dimension getters; no dependency doubles',
              'comparison': 'exact float bits except NaN payloads', 'manager_parity': 'DIFFER; functional acceptance'}
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"UI_SCALE {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    print('UI_DIMENSION_GETTERS '+','.join(getter_parity))
    return int(not accepted)


if __name__ == '__main__': raise SystemExit(main())
