"""Infer minimal prototypes (calling convention + stack purge) for engine helpers a unit calls that
FSE never typed: disassemble the retail callee and read its `ret N`. `__thiscall` with N/4 int
parameters keeps Ghidra's stack tracking exact through the call (the parameter *types* are not
claimed; only the purge and the ECX receiver). Merged into the unit's typing_spec.json as
`helpers[address] = {..., 'source': 'inferred ret N'}`.
"""
import argparse, json, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32  # noqa: E402
from capstone.x86 import X86_OP_IMM  # noqa: E402
from tools.script_recovery.export_native_threads import Image, RETAIL_EXE  # noqa: E402
from tools.script_recovery.script_units import unit as script_unit  # noqa: E402


def purge(image, address, limit=0x600):
    """(purge bytes, uses ECX, uses EDX, float result) from a linear scan to the first ret.
    ECX/EDX read before being written mark __thiscall/__fastcall register parameters; an FPU
    instruction as the last computation before `ret` marks a floating-point result in ST0."""
    cs = Cs(CS_ARCH_X86, CS_MODE_32); cs.detail = True
    raw = image.bytes_at(address, limit)
    uses = {'ecx': False, 'edx': False}
    wrote = {'ecx': False, 'edx': False}
    last_fpu = False
    for index, ins in enumerate(cs.disasm(raw, address)):
        regs_read, regs_write = ins.regs_access()
        names_r = {ins.reg_name(r) for r in regs_read}; names_w = {ins.reg_name(r) for r in regs_write}
        for reg in ('ecx', 'edx'):
            if not wrote[reg] and reg in names_r and (reg == 'ecx' or index < 8):
                uses[reg] = True
            if reg in names_w and reg not in names_r:
                wrote[reg] = True
        if ins.mnemonic == 'ret':
            n = ins.operands[0].imm if ins.operands and ins.operands[0].type == X86_OP_IMM else 0
            return n, uses['ecx'], uses['edx'], last_fpu
        if ins.mnemonic == 'jmp' and ins.operands and ins.operands[0].type == X86_OP_IMM and ins.address == address:
            return purge(image, ins.operands[0].imm, limit)   # thunk
        if ins.mnemonic not in ('pop', 'add', 'mov', 'lea'):
            last_fpu = ins.mnemonic.startswith('f')
    return None, uses['ecx'], uses['edx'], last_fpu


def main():
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--unit', required=True)
    args = a.parse_args()
    spec_u = script_unit(args.unit)
    evidence = spec_u['evidence']
    tu = json.loads((evidence / 'translation_unit_typed.json').read_text(encoding='utf-8-sig')) \
        if (evidence / 'translation_unit_typed.json').is_file() else \
        json.loads((evidence / 'translation_unit.json').read_text(encoding='utf-8-sig'))
    spec_path = evidence / 'typing_spec.json'
    spec = json.loads(spec_path.read_text(encoding='utf-8'))
    typed = {int(k, 16) for k in spec['helpers']}
    lo, hi = spec_u['lo'], spec_u['hi']
    image = Image(RETAIL_EXE)
    added = {}
    for f in tu['functions']:
        for call in f['calls']:
            t = int(call['target'], 16)
            if lo <= t < hi or t in typed or t in added:
                continue
            n, uses_ecx, uses_edx, fpu = purge(image, t)
            if n is None:
                continue
            count = n // 4
            if uses_ecx and uses_edx:
                cc, params = '__fastcall', [{'name': 'p0', 'type': 'void *', 'ctype': 'ecx'}, {'name': 'p1', 'type': 'void *', 'ctype': 'edx'}]
            elif uses_ecx:
                cc, params = '__thiscall', []
            elif n:
                cc, params = '__stdcall', []
            else:
                continue   # caller-cleaned cdecl: Ghidra's own knowledge is better than a 0-parameter claim
            params += [{'name': f'p{len(params) + i}', 'type': 'int', 'ctype': 'inferred'} for i in range(count)]
            added[t] = {'name': (call['currentName'] or f'FUN_{t:08X}').replace('::', '__').replace('<', '_').replace('>', '_'),
                        'cc': cc, 'ret': 'float' if fpu else 'int', 'params': params,
                        'source': f'inferred ret {n} (ecx={uses_ecx}, edx={uses_edx}, fpu={fpu})', 'bsimLabel': call['currentName']}
    for t, row in added.items():
        spec['helpers'][hex(t)] = row
    spec_path.write_text(json.dumps(spec, indent=1) + '\n', encoding='utf-8')
    print(json.dumps({'inferred': len(added), 'total helpers': len(spec['helpers'])}, indent=2))
    for t, row in sorted(added.items()):
        print(hex(t), row['cc'], len(row['params']), row['bsimLabel'][:70])


if __name__ == '__main__':
    main()
