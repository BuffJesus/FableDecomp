"""PlayWave's class subscripts are proven byte stores into ArenaSpawnNeeded."""
import json

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.quest_unit_evidence import ROOT, load_layouts, array_descriptors
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.build_readable_unit import readable_file


def spec():
    unit = json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    result = LoweringSpec(unit, 'Q_Arena', entity=False, thing_slots={})
    result.call_labels = {}
    result.native_address = 0xf1eed0
    return result


@pytest.mark.parametrize('cast', ['', '(int)'])
@pytest.mark.parametrize('bucket', [0, 7, 15])
@pytest.mark.parametrize('readable', [False, True])
def test_generated_store_sets_only_selected_spawn_flag(cast, bucket, readable):
    source = '{\nthis[' + cast + 'bucket + 0xe2] = (CQ_ArenaScript)0x1;\n}'
    source, _ = lower(source, spec())
    lifter = make()
    lifter.accessor_kinds = True
    body = finish_lua('\n'.join(lifter.lift('Spawn', source, parameters={'bucket': 'number'})))
    assert not lifter.todo
    source = 'function Spawn(quest,bucket)\n' + body + '\nend'
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    flags = {f'ArenaSpawnNeeded_{i}': False for i in range(16)}
    writes = []
    def set_flag(_q, key, value):
        writes.append((key, value))
        flags[key] = value
    lua.globals().Spawn(lua.table_from({'SetStateBool': set_flag}), bucket)
    assert writes == [(f'ArenaSpawnNeeded_{bucket}', True)]
    assert [k for k, value in flags.items() if value] == [f'ArenaSpawnNeeded_{bucket}']


@pytest.mark.parametrize('change', ['address', 'class', 'offset', 'value', 'count'])
def test_unreviewed_store_is_not_recovered(change):
    source = '{\nthis[bucket + 0xe2] = (CQ_ArenaScript)0x1;\n}'
    config = spec()
    if change == 'address':
        config.native_address = 0xf1eed1
    elif change == 'count':
        next(a for a in config.self_arrays if a['name'] == 'ArenaSpawnNeeded')['count'] = 15
    else:
        source = source.replace({'class': 'CQ_ArenaScript', 'offset': '0xe2', 'value': '0x1'}[change],
                                {'class': 'OtherClass', 'offset': '0xe3', 'value': '0x100'}[change])
    out, _ = lower(source, config)
    assert 'STATE_SetBool' not in out


def test_pdb_extent_and_retail_byte_stores_agree():
    a = next(a for a in array_descriptors(load_layouts(), 'CQ_ArenaScript', delta=-0x14)
             if a['name'] == 'ArenaSpawnNeeded')
    assert (a['base'], a['stride'], a['count'], a['element']) == ('0xe2', 1, 16, 'bool')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    code = {i.address: (i.mnemonic, i.op_str) for i in
            Cs(CS_ARCH_X86, CS_MODE_32).disasm(RData().bytes_at(0xf1eed0, 0x1800), 0xf1eed0)}
    for address in (0xf1fa9f, 0xf1fe2d):
        assert code[address] == ('mov', 'byte ptr [eax + esi + 0xe2], 1')
