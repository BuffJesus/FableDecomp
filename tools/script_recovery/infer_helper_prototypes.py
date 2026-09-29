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


def purge(image, address, limit=0x600, *, _seen=None):
    """(purge bytes, uses ECX, uses EDX, float result) from a linear scan to the first ret.
    ECX/EDX read before being written mark __thiscall/__fastcall register parameters; an FPU
    instruction as the last computation before `ret` marks a floating-point result in ST0."""
    seen = set() if _seen is None else _seen
    if address in seen:
        return None, False, False, False
    seen.add(address)
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
        if ins.mnemonic == 'jmp' and ins.address == address:
            if ins.operands and ins.operands[0].type == X86_OP_IMM:
                return purge(image, ins.operands[0].imm, limit, _seen=seen)
            # An import thunk has no local return. Scanning past it attributes the next
            # unrelated routine's ret N to the imported function (e.g. _stricmp).
            return None, uses['ecx'], uses['edx'], last_fpu
        if ins.mnemonic not in ('pop', 'add', 'mov', 'lea'):
            last_fpu = ins.mnemonic.startswith('f')
    return None, uses['ecx'], uses['edx'], last_fpu


# Prototypes proven from the retail disassembly; they replace the purge-only inference (which types every stack
# word `int`, so an immediate float operand printed as a stale register).
REVIEWED = {
    # ECE7D4/ECE7DD take entry ESP+4/+8 and call CCharString_Destroy; ECE7E9
    # returns with ret 8. Callers construct both strings in outgoing stack slots.
    # ECE6FF passes the first to RunCutsceneMacro; ECE544 assigns the second to $ARG1.
    0x00ECE460: {'name': 'helper_ECE460', 'cc': '__thiscall', 'ret': 'void',
                 'params': [{'name': 'macroName', 'type': 'CCharString', 'ctype': 'CCharString'},
                            {'name': 'argument', 'type': 'CCharString', 'ctype': 'CCharString'}],
                 'source': 'reviewed by-value parameter destructors and ret 8 at 0x00ECE460 (2026-09-28)',
                 'bsimLabel': 'Script_InitializeWitchCharacter (untrusted autoname)'},
    # Retail import thunk ff 25 ec 00 44 01; helper ECE460 pushes two string-data
    # pointers at ECE4FF/ECE500, calls this address, then adds esp,8 at ECE506.
    0x00BFEBA8: {'name': '_stricmp', 'cc': '__cdecl', 'ret': 'int',
                 'params': [{'name': 'left', 'type': 'char *', 'ctype': 'const char *'},
                            {'name': 'right', 'type': 'char *', 'ctype': 'const char *'}],
                 'source': 'reviewed CRT import and caller cleanup at 0x00ECE501 (2026-09-28)',
                 'bsimLabel': 'MSVCR71.DLL::_stricmp'},
    # mov eax,ecx; mov ecx,[esp+8]; mov [eax],ecx; ret 8.
    # The inherited __EH_epilog3 label must not inject exception-stack teardown.
    0x004502EB: {'name': 'FUN_004502eb', 'cc': '__thiscall', 'ret': 'int',
                 'params': [{'name': 'p0', 'type': 'int', 'ctype': 'inferred'},
                            {'name': 'p1', 'type': 'int', 'ctype': 'inferred'}],
                 'clearCallFixup': True,
                 'source': 'reviewed disassembly 0x004502EB (2026-09-25): stores second stack word, ret 8',
                 'bsimLabel': '__EH_epilog3'},
    # IsDistanceFromThingToPositionOver: ecx = the thing (IsAlive through its vtable +0x12c, GetPos +0x18),
    # edx = the C3DVector (fld [esi] / [esi+4] / [esi+8]), one float on the stack (`fld [esp+0xc]; fmul`), AL result,
    # `ret 4`. As `int` the pushed 2.0 printed as `iVar20` and every Trader Escort waypoint test stayed a TODO.
    0x00CBE45C: {'name': 'IsDistanceFromThingToPositionOver', 'cc': '__fastcall', 'ret': 'bool',
                 'params': [{'name': 'thing', 'type': 'CScriptThing *', 'ctype': 'ecx'},
                            {'name': 'position', 'type': 'C3DVector *', 'ctype': 'edx'},
                            {'name': 'distance', 'type': 'float', 'ctype': 'float'}],
                 'source': 'reviewed disassembly 0x00CBE45C (2026-09-24)', 'bsimLabel': 'IsDistanceFromThingToPositionOver'},
}


def discard_unproven_inferred(helpers, image):
    """Revalidate earlier purge guesses, including those that crossed import thunks.

    Reviewed signatures and API typedefs retain authority even without a local return.
    """
    removed = []
    for address, row in list(helpers.items()):
        if row.get('source', '').startswith('inferred ret ') and purge(image, int(address, 16))[0] is None:
            del helpers[address]
            removed.append(address)
    return removed


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
    lo, hi = spec_u['lo'], spec_u['hi']
    image = Image(RETAIL_EXE)
    removed = discard_unproven_inferred(spec['helpers'], image)
    typed = {int(k, 16) for k in spec['helpers']}
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
    called = {int(c['target'], 16) for f in tu['functions'] for c in f['calls']}
    for t, row in REVIEWED.items():
        if t in called and spec['helpers'].get(hex(t)) != row:
            spec['helpers'][hex(t)] = row
            added[t] = row
    spec_path.write_text(json.dumps(spec, indent=1) + '\n', encoding='utf-8')
    print(json.dumps({'inferred': len(added), 'removedUnproven': removed,
                      'total helpers': len(spec['helpers'])}, indent=2))
    for t, row in sorted(added.items()):
        print(hex(t), row['cc'], len(row['params']), row['bsimLabel'][:70])


if __name__ == '__main__':
    main()
