#!/usr/bin/env python3
"""Compare recovered graphics-bank ownership with actual retail x86; no GUI."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


SOURCES = [
    'rebuild/src/compiled/00/41/CCountedPointer_ReleaseBankReference_00419108.cpp',
    'rebuild/src/compiled/00/41/CCountedPointer_ShareBankReference_00419134.cpp',
    'rebuild/src/compiled/00/42/CManager_SetGraphicsBank_0042a9b7.cpp',
]
# Reviewed VC7.1 /O2 residues: stack/zero-store encodings and explicit unused
# EDX arguments. A new instruction stream needs review even if fixtures pass.
EXPECTED_CODE = {
    0x419108: '2e6274c55345496fc2947e377c4696d6363e9aa6868c4ba2e9acd5a81afe713b',
    0x419134: 'f6fbff0dc463e295445625fac9adf3fa74fde935e0329ac18b24507a7fecbcc8',
    0x42A9B7: 'c2226d3af9f3c6ecd67bf02a20bff1e222621c660c84e14e38ee15de5405bba4',
}


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':
        raise RuntimeError('Retail oracle changed')
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    uc.mem_map(0x400000, 0x1100000)
    uc.mem_map(0x20000000, 0x20000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        uc.mem_write(0x400000+va, image[raw:raw+size])
    stop, stack, manager, infos, callback = 0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000
    owned = manager+0x10
    def put(p, v): uc.mem_write(p, struct.pack('<I', v & 0xFFFFFFFF))
    def word(p): return struct.unpack('<I', uc.mem_read(p, 4))[0]
    def ident(p): return 0 if not p else (p-infos)//12+1
    def ptr(i): return infos+(i-1)*12 if i else 0
    def snapshot():
        return f':{word(owned):08x}:{ident(word(owned+4))}'+''.join(':'+str(word(infos+i*12)) for i in range(3))
    uc.mem_write(callback, b'\xc3')
    uc.mem_write(0xBFE9BC, b'\xc3')
    events = []
    case = None
    def hook(uc, address, size, data):
        if address == callback:
            events.append('D'+str(uc.reg_read(UC_X86_REG_ECX))+snapshot())
            if case[7]: put(owned+4, infos+24)
        elif address == 0xBFE9BC:
            events.append('F'+str(ident(word(uc.reg_read(UC_X86_REG_ESP)+4)))+snapshot())
    uc.hook_add(UC_HOOK_CODE, hook)
    cases = [(*c, 0) for c in itertools.product(range(3), range(3), range(3), (1, 2, 17), (1, 3), range(3), (1, 3))]
    cases += [(0, 1, 0, 1, 1, 0, 1, 1), (1, 1, 2, 1, 3, 0, 1, 1)]
    directory = ROOT/'work/ui_bank_ownership_check'
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory/'cases.txt'
    inputs.write_text('\n'.join(' '.join(map(str, c)) for c in cases)+'\n')
    env = parity.env()
    objects = []
    for i, source in enumerate(SOURCES+['rebuild/tests/integration/UiBankOwnership_test.cpp']):
        obj = directory/f'part{i}.obj'
        run([parity.CL_EXE, '/nologo', '/c', '/W3', '/MT', '/GS', '/O2', '/Oy', '/I'+str(ROOT/'rebuild/include'), '/Fo'+str(obj), ROOT/source], env)
        objects.append(obj)
    exe = directory/'behavior.exe'
    run([parity.VC/'bin/link.exe', '/nologo', '/subsystem:console', '/out:'+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError('Incomplete ownership traces')
    errors = []
    def execute(address, receiver, args=()):
        put(stack, stop)
        for i, arg in enumerate(args): put(stack+4+i*4, arg)
        uc.reg_write(UC_X86_REG_ESP, stack)
        uc.reg_write(UC_X86_REG_ECX, receiver)
        uc.emu_start(address, stop, count=10000)
        if uc.reg_read(UC_X86_REG_EIP) != stop or uc.reg_read(UC_X86_REG_ESP) != stack+4+len(args)*4:
            raise RuntimeError('Ownership oracle return/stack contract failed')
    for index, (case, actual) in enumerate(zip(cases, lines)):
        mode, old_id, new_id, old_count, new_count, same_data, repeat, mutation = case
        events.clear()
        uc.mem_write(manager, b'\xA5'*0xD0)
        for i in range(3):
            put(infos+i*12, 7); put(infos+i*12+4, callback); put(infos+i*12+8, i+1)
        if new_id: put(ptr(new_id), new_count)
        if old_id: put(ptr(old_id), old_count)
        put(owned, 0x11111111); put(owned+4, ptr(old_id))
        incoming = (0 if same_data == 2 else 0x11111111 if same_data else 0x22222222, ptr(new_id))
        for _ in range(repeat):
            if mode == 2 and new_id: put(ptr(new_id), word(ptr(new_id))+1)
            execute((0x419108, 0x419134, 0x42A9B7)[mode], manager if mode == 2 else owned, incoming if mode else ())
            events.append('S'+snapshot())
        execute(0x419108, owned)
        events.append('Z'+snapshot())
        raw = bytes(uc.mem_read(manager, 0xD0))
        if raw[:0x10]+raw[0x18:] != b'\xA5'*(0xD0-8): raise RuntimeError('Unexpected retail manager write')
        expected = 'TRACE'+''.join(' '+e for e in events)+' END'
        if actual != expected: errors.append(dict(case=index, input=case, actual=actual, retail=expected))
    comparisons = []
    for obj, address, size, symbol in zip(objects, (0x419108, 0x419134, 0x42A9B7), (44, 38, 28),
                                          ('?FableUiReleaseBankReference', '?FableUiShareBankReference', '?FableUiSetGraphicsBank')):
        built, section, _ = parity.obj_text(obj, symbol)
        relocs = parity.obj_relocs(obj, section)
        offset = pe_oracle.va_to_off(pe_oracle.pe_sections(image), address)
        retail = image[offset:offset+size]
        matched = len(built) == size and parity.mask(built, relocs) == parity.mask(retail, relocs)
        comparisons.append(dict(address=f'{address:08x}', compiled_bytes=len(built), retail_bytes=size,
                                grade='RELOCATION_MATCH' if matched else 'DIFFER',
                                masked_sha256=hashlib.sha256(parity.mask(built, relocs)).hexdigest()))
        if comparisons[-1]['masked_sha256'] != EXPECTED_CODE[address]:
            errors.append(dict(address=f'{address:08x}', reason='Unreviewed compiler instruction drift'))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP, '-dr', obj], env))
    report = dict(accepted=not errors, cases=len(cases), errors=errors, comparisons=comparisons,
                  scope='actual retail release/share/setter; callback/free observed; repeated calls, aliasing, null control records, callback replacement; no asset factory or renderer')
    (directory/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(f"UI_BANK_OWNERSHIP {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    print(json.dumps(comparisons))
    return int(bool(errors))


if __name__ == '__main__': raise SystemExit(main())
