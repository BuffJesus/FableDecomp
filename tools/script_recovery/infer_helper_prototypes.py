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
    """(purge bytes, uses ECX before writing it) from a linear scan to the first ret."""
    cs = Cs(CS_ARCH_X86, CS_MODE_32); cs.detail = True
    raw = image.bytes_at(address, limit)
    uses_ecx, wrote_ecx = False, False
    for ins in cs.disasm(raw, address):
        if not wrote_ecx and 'ecx' in ins.op_str:
            regs_read, regs_write = ins.regs_access()
            names_r = {ins.reg_name(r) for r in regs_read}; names_w = {ins.reg_name(r) for r in regs_write}
            if 'ecx' in names_r:
                uses_ecx = True
            if 'ecx' in names_w and 'ecx' not in names_r:
                wrote_ecx = True
        if ins.mnemonic == 'ret':
            n = ins.operands[0].imm if ins.operands and ins.operands[0].type == X86_OP_IMM else 0
            return n, uses_ecx
        if ins.mnemonic == 'jmp' and ins.operands and ins.operands[0].type == X86_OP_IMM and ins.address == address:
            return purge(image, ins.operands[0].imm, limit)   # thunk
    return None, uses_ecx


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
            n, uses_ecx = purge(image, t)
            if n is None:
                continue
            count = n // 4
            cc = '__thiscall' if uses_ecx else '__cdecl'
            if cc == '__cdecl' and n:
                cc = '__stdcall'
            if cc == '__cdecl' or t & 0xF not in (0, 0x8) and n == 0 and not uses_ecx:
                continue   # caller-cleaned: Ghidra's own knowledge is better than a 0-parameter claim
            added[t] = {'name': (call['currentName'] or f'FUN_{t:08X}').replace('::', '__').replace('<', '_').replace('>', '_'),
                        'cc': cc, 'ret': 'int', 'params': [{'name': f'p{i}', 'type': 'int', 'ctype': 'inferred'} for i in range(count)],
                        'source': f'inferred ret {n} ({"ecx used" if uses_ecx else "no ecx"})', 'bsimLabel': call['currentName']}
    for t, row in added.items():
        spec['helpers'][hex(t)] = row
    spec_path.write_text(json.dumps(spec, indent=1) + '\n', encoding='utf-8')
    print(json.dumps({'inferred': len(added), 'total helpers': len(spec['helpers'])}, indent=2))
    for t, row in sorted(added.items()):
        print(hex(t), row['cc'], len(row['params']), row['bsimLabel'][:70])


if __name__ == '__main__':
    main()
