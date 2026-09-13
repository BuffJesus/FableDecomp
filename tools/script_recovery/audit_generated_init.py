"""Compare complete generated Init effects with the native scalar-write prefix.

Speech construction is checked separately by native_speech_vectors. This audit
does not establish mutable-vector or repeated-Init append equivalence.
"""
import json

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_REG, X86_OP_IMM
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import (ROOT, RData, Lifter, load_manifest,
    load_slots, load_thing_tables, annotate)
from tools.script_recovery.native_function_parameters import function_parameters, rename_parameters
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_speech_vectors import recover_vectors, vector_prelude


def audit(timer_id=57):
    fields = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/persistence_manifest.json').read_text())['fields']
    kinds = {'bool': 'Bool', 'int': 'Int', 'float': 'Float'}
    state = {f['retailOffset']: (f['name'], kinds[f['type']]) for f in fields if f['type'] in kinds}
    arrays = {'0xfc': ('WhichBadDeedsPerformed', 'Bool')}
    byte_fields = {int(offset, 16): (name, kind, 1 if kind == 'Bool' else 4)
                   for offset, (name, kind) in state.items()}
    for index in range(5):
        byte_fields[0xFC + index] = ('WhichBadDeedsPerformed_' + str(index), 'Bool', 1)
    data, native = RData(), []
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    prefix = list(decoder.disasm(data.bytes_at(0xDAADD0, 0xA4), 0xDAADD0))
    if prefix[-1].address + prefix[-1].size != 0xDAAE74:
        raise ValueError('native Init prefix boundary changed')
    zero = set()
    aliases = {'bl': 'ebx', 'al': 'eax'}
    for ins in prefix:
        args = ins.operands
        if (ins.mnemonic == 'xor' and args[0].type == args[1].type == X86_OP_REG
                and args[0].size == 4 and args[0].reg == args[1].reg):
            zero.add(ins.reg_name(args[0].reg))
            continue
        if ins.mnemonic == 'call':
            if ins.address != 0xDAAE2A:
                raise ValueError('unexpected native Init prefix call')
            setup = read_call_window(data, 0xDAADD0, 3306, ins.address, argument_count=2)
            expected = (('memory', ('address', ('register', 'ecx'), 0x104)), ('constant', 0))
            if setup is None or setup.stack_arguments != expected:
                raise ValueError('native timer operands changed')
            native.append(['timer', timer_id, 0])
            zero.difference_update(('eax', 'ecx', 'edx'))
            continue
        if ins.mnemonic == 'mov' and args[0].type == X86_OP_MEM:
            mem = args[0].mem
            if ins.reg_name(mem.base) != 'esi' or mem.index or mem.segment:
                raise ValueError('unreviewed native Init store')
            if args[1].type == X86_OP_IMM:
                value = args[1].imm & ((1 << (8 * args[0].size)) - 1)
            elif args[1].type == X86_OP_REG and aliases.get(ins.reg_name(args[1].reg), ins.reg_name(args[1].reg)) in zero:
                value = 0
            else:
                raise ValueError('unknown native Init store value')
            at, remaining = mem.disp, args[0].size
            while remaining:
                name, kind, size = byte_fields[at]
                if size > remaining:
                    raise ValueError('partial typed state write')
                part = value & ((1 << (8 * size)) - 1)
                converted = bool(part) if kind == 'Bool' else part - (1 << 32) if part & 0x80000000 else part
                native.append(['state', kind, name, converted])
                at, remaining, value = at + size, remaining - size, value >> (8 * size)
        elif ins.mnemonic not in ('mov', 'lea', 'push', 'sub'):
            raise ValueError('unreviewed native Init instruction')
        _, written = ins.regs_access()
        zero.difference_update(aliases.get(ins.reg_name(reg), ins.reg_name(reg)) for reg in written)

    unit = json.loads((ROOT / 'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    fn = next(f for f in unit['functions'] if f['address'] == '0x00DAADD0')
    manifest, slots = load_manifest(), load_slots()
    things, returning = load_thing_tables(manifest, slots)
    source = annotate(rename_parameters(fn['decompile'], function_parameters(fn['decompile'], member=True)),
                      slots, things, returning, entity=False)
    vectors = recover_vectors(data.bytes_at)
    lifter = Lifter(manifest, state, 'quest', False, '', data, state_arrays=arrays, static_vectors=vectors)
    body = '\n'.join(lifter.lift('Init', source, native_function=fn))
    lua, actual = LuaRuntime(), []
    values = {name: True if kind == 'Bool' else 123 for name, kind, _ in byte_fields.values()}
    values['TalkIntermittentTimer'] = timer_id
    def setter(kind):
        def write(q, name, value):
            actual.append(['state', kind, name, value])
            values[name] = value
        return write
    quest = lua.table_from({'SetStateBool': setter('Bool'), 'SetStateInt': setter('Int'),
        'GetStateInt': lambda q, name: values[name],
        'SetTimer': lambda q, timer, value: actual.append(['timer', timer, value])})
    lua.execute(vector_prelude(vectors) + '\nreturn function(quest)\n' + body + '\nend')(quest)
    return {'ok': native == actual and not lifter.todo, 'nativeEffects': native,
            'luaEffects': actual, 'todo': lifter.todo, 'timerIdAfter': values['TalkIntermittentTimer'],
            'speechKeys': sum(map(len, vectors.values())),
            'scope': 'Init scalar writes and timer reset; immutable speech data checked separately'}


if __name__ == '__main__':
    print(json.dumps(audit(), indent=2))
