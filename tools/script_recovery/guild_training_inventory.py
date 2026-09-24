"""Anchor Guild entity ownership to retail instructions, without trusting symbols.

Requires the read-only ExportScriptTranslationUnit export covering D3B390..D68F00.
Writes addresses, hashes and decompilation references, never retail instruction bytes.
"""
from __future__ import annotations

import hashlib
import json
import re
import struct
from pathlib import Path

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_ESP, X86_REG_EBP, X86_REG_EAX, X86_REG_ECX, X86_REG_EDX

from tools.script_recovery.export_native_threads import Image, RETAIL_EXE
from tools.script_recovery.script_units import unit as script_unit

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'refs/script_recovery/guild_training'
ROLES = ('destructor', 'Main', 'Init', 'GetParentScript', 'OnPersist', 'OnPredicateFail', 'OnInterrupted')


def spawned_thread_name(code, index, cstring):
    """Read literal names, including the native ParentClass. + member form."""
    recent = code[max(0, index-32):index]
    parent_prefix = False
    for pos in range(len(recent)-1, -1, -1):
        call = recent[pos]
        if not (call.mnemonic == 'call' and call.operands[0].type == X86_OP_IMM
                and call.operands[0].imm == 0x99EBF0):
            continue
        pushes = [p for p in recent[:pos] if p.mnemonic == 'push'
                  and p.operands[0].type == X86_OP_IMM]
        value = cstring(pushes[-1].operands[0].imm) if pushes else None
        if value and re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', value):
            return value
        # Entity methods start quest threads through ParentClass.<name>.
        # Require the actual string concatenation call, not arbitrary nearby text.
        if value == 'ParentClass.' and not parent_prefix and any(
                i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM
                and i.operands[0].imm == 0x99F570 for i in recent[pos+1:]):
            parent_prefix = True
            continue
        return None
    return None


def recover(exe=RETAIL_EXE, evidence=EVIDENCE, unit_name='guild_training'):
    spec = script_unit(unit_name)
    evidence = spec['evidence'] if evidence is EVIDENCE else evidence
    LO, HI, IR_GLOB = spec['lo'], spec['hi'], spec['ir_glob']
    image = Image(exe)
    source = evidence / 'translation_unit.json'
    translation = json.loads(source.read_text(encoding='utf-8-sig'))
    functions = {int(f['address'], 16): f for f in translation['functions']}
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True

    def instructions(address):
        function = functions[address]
        result = []
        for span in function['bodyRanges']:
            start, end = int(span['start'], 16), int(span['endExclusive'], 16)
            result.extend(decoder.disasm(image.bytes_at(start, end-start), start))
        return result

    def stores(address):
        # Immediate stores, plus register stores whose register was last loaded with an
        # immediate earlier in the same function (the compiler hoists shared factory
        # pointers, e.g. two bindings of one entity class share `mov ebp, factory`).
        last_imm = {}
        for ins in instructions(address):
            if ins.mnemonic == 'mov' and len(ins.operands) == 2 and ins.operands[0].type == X86_OP_REG:
                if ins.operands[1].type == X86_OP_IMM:
                    last_imm[ins.operands[0].reg] = ins.operands[1].imm
                else:
                    last_imm.pop(ins.operands[0].reg, None)
            elif ins.mnemonic in ('xor', 'lea', 'pop', 'add', 'sub', 'and', 'or') and ins.operands                     and ins.operands[0].type == X86_OP_REG:
                last_imm.pop(ins.operands[0].reg, None)
            elif ins.mnemonic == 'call':
                for reg in (X86_REG_EAX, X86_REG_ECX, X86_REG_EDX):
                    last_imm.pop(reg, None)
            if (ins.mnemonic == 'mov' and len(ins.operands) == 2
                    and ins.operands[0].type == X86_OP_MEM):
                if ins.operands[1].type == X86_OP_IMM:
                    yield ins, ins.operands[0].mem, ins.operands[1].imm
                elif ins.operands[1].type == X86_OP_REG and ins.operands[1].reg in last_imm:
                    yield ins, ins.operands[0].mem, last_imm[ins.operands[1].reg]

    def cstring(address):
        try:
            return image.bytes_at(address, 256).split(b'\0', 1)[0].decode('ascii')
        except UnicodeDecodeError:
            return None

    quests, errors = [], []
    threads = []
    for address in functions:
        code = instructions(address)
        for index, ins in enumerate(code):
            if not (ins.mnemonic == 'mov' and len(ins.operands) == 2
                    and ins.operands[0].type == X86_OP_MEM and ins.operands[0].mem.disp == 0x34
                    and ins.operands[1].type == X86_OP_IMM
                    and LO <= ins.operands[1].imm < HI):
                continue
            # Keep the source site even when a name cannot be established. A
            # stored function pointer is evidence; a propagated symbol is not.
            name = spawned_thread_name(code, index, cstring)
            target = ins.operands[1].imm
            threads.append({'registrationFunction': f'0x{address:08X}', 'store': f'0x{ins.address:08X}',
                            'body': f'0x{target:08X}', 'name': name,
                            'bodyExported': target in functions and bool(functions[target].get('decompile'))})
    for path in sorted((ROOT / 'refs/script_recovery/native_operation_ir').glob(IR_GLOB)):
        ir = json.loads(path.read_text(encoding='utf-8-sig'))
        quest = {'script': ir['script'], 'allocator': ir['allocatorAddress'],
                 'vtable': ir['vtableAddress'], 'entities': []}
        # The cluster's shared constructor attribution can be wrong. A direct
        # vtable store is an independent ownership anchor in the full export.
        vtable = int(ir['vtableAddress'], 16)
        quest['vtableWriters'] = [f'0x{a:08X}' for a in functions
                                 if any(mem.disp == 0 and value == vtable
                                        for _, mem, value in stores(a))]
        for lifecycle in ir['lifecycle']:
            expected = lifecycle['entityBindings']
            if not expected:
                continue
            address = int(lifecycle['address'], 16)
            code = instructions(address)
            previous = -1
            for binding in expected:
                name = binding['entityName']
                name_pushes = [i for i in code if i.address > previous and i.mnemonic == 'push'
                               and i.operands[0].type == X86_OP_IMM
                               and cstring(i.operands[0].imm) == name]
                if not name_pushes:
                    raise ValueError(f'{ir["script"]}.{name}: missing name push')
                start = name_pushes[0].address
                finish = next(i.address for i in code if i.address > start and i.mnemonic == 'call'
                              and i.operands[0].type == X86_OP_IMM and i.operands[0].imm == 0xCB8230)
                window = [(i, mem, value) for i, mem, value in stores(address) if start < i.address < finish]
                binding_vtable = int(re.search(r'([0-9a-fA-F]{8})$', binding['bindingVtable'])[1], 16)
                owners = [(i, mem) for i, mem, value in window if mem.disp == 0 and value == binding_vtable]
                if len(owners) != 1:
                    raise ValueError(f'{name}: missing unique binding vtable store')
                register = owners[0][1].base
                callbacks = [(i, value) for i, mem, value in window if mem.base == register and mem.disp == 0x10]
                if len(callbacks) != 1:
                    raise ValueError(f'{name}: missing unique allocator store')
                store, factory = callbacks[0]
                if factory not in functions:
                    quest['entities'].append({'name': name, 'registrationFunction': lifecycle['address'],
                        'namePush': f'0x{start:08X}', 'addBindingCall': f'0x{finish:08X}',
                        'factoryStore': f'0x{store.address:08X}', 'factory': f'0x{factory:08X}',
                        'functions': {}, 'missingExports': [f'0x{factory:08X}']})
                    previous = finish
                    continue
                # Constructors may first install a shared base table. Keep only
                # tables whose entity Main belongs to the exported Guild range.
                tables = []
                # Classes with constructed members get a real constructor that the
                # factory calls; the vtable store then lives one call level down.
                constructors = [factory] + [int(c['target'], 16) for c in functions[factory].get('calls', [])
                                            if LO <= int(c['target'], 16) < HI and int(c['target'], 16) in functions]
                for owner in constructors:
                    for i, mem, value in stores(owner):
                        if mem.disp or mem.base in (0, X86_REG_ESP, X86_REG_EBP) or mem.index:
                            continue
                        raw = image.bytes_at(value, 4 * len(ROLES))
                        if len(raw) != 4 * len(ROLES):
                            continue
                        slots = struct.unpack('<7I', raw)
                        if LO <= slots[1] < HI:
                            tables.append((value, slots))
                    if tables:
                        break
                tables = list(dict.fromkeys(tables))
                if len(tables) != 1:
                    raise ValueError(f'{ir["script"]}.{name}: ambiguous entity vtable {tables}')
                table, slots = tables[0]
                row = {'name': name, 'registrationFunction': lifecycle['address'],
                       'namePush': f'0x{start:08X}', 'addBindingCall': f'0x{finish:08X}',
                       'factoryStore': f'0x{store.address:08X}', 'factory': f'0x{factory:08X}',
                       'vtable': f'0x{table:08X}',
                       'functions': {role: f'0x{value:08X}' for role, value in zip(ROLES, slots)},
                       'missingExports': [f'0x{value:08X}' for value in slots
                                          if value not in functions or not functions[value].get('decompile')]}
                quest['entities'].append(row)
                previous = finish
        quests.append(quest)
    return {'schema': spec['schema'],
            'retailSha256': hashlib.sha256(image.data).hexdigest(),
            'translationSha256': hashlib.sha256(source.read_bytes()).hexdigest(),
            'exportedFunctions': len(functions), 'quests': quests, 'threads': threads, 'errors': errors,
            'status': 'ownership inventory; Lua behavior and lifecycle parity are not verified'}


def main():
    import argparse
    a = argparse.ArgumentParser(description=__doc__)
    a.add_argument('--unit', default='guild_training')
    args = a.parse_args()
    result = recover(unit_name=args.unit)
    (script_unit(args.unit)['evidence'] / 'inventory.json').write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'quests': len(result['quests']),
                      'entities': sum(len(q['entities']) for q in result['quests']),
                      'exportedFunctions': result['exportedFunctions']}, indent=2))


if __name__ == '__main__':
    main()
