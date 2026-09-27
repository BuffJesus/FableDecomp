"""PDB scalar widths must agree for constant and dynamically indexed fields."""
import json
import re

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.quest_unit_evidence import (
    ROOT, array_descriptors, class_fields, load_layouts,
)
from tools.script_recovery.convert_quest_unit import lift_persist_evidence
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file


@pytest.mark.parametrize('element,width,kind', [
    ('bool', 1, 'Bool'), ('char', 1, 'Int'), ('unsigned char', 1, 'Int'),
    ('short', 2, 'Int'), ('unsigned short', 2, 'Int'), ('int', 4, 'Int'),
    ('float', 4, 'Float'), ('CCharString', 4, 'String'),
])
def test_scalar_array_and_following_field_do_not_overlap(element, width, kind):
    layouts = {'Example': [[
        {'offset': 0x48, 'type': element + '[3]', 'name': 'Values'},
        {'offset': 0x48 + 3 * width, 'type': 'int', 'name': 'Following'},
    ]]}
    fields, _, _ = class_fields(layouts, 'Example')
    fields.pop('__timers__')
    fields.pop('__resources__')
    assert fields == {
        **{hex(0x48 + i * width): [f'Values_{i}', kind] for i in range(3)},
        hex(0x48 + 3 * width): ['Following', 'Int'],
    }
    array, = array_descriptors(layouts, 'Example')
    assert array['stride'] == width
    assert array['members'] == {'0x0': ['', kind]}


@pytest.mark.parametrize('classname,field,base,count', [
    ('CQ_ArenaScript', 'ArenaSpawnNeeded', 0xe2, 16),
    ('CV_BordelloScript', 'ClientInUse', 0x58, 3),
    ('CV_GuildMasterScript', 'GuildMasterDialogue', 0x48, 5),
])
def test_real_pdb_boolean_arrays_have_byte_stride(classname, field, base, count):
    layouts = load_layouts()
    fields, _, _ = class_fields(layouts, classname, delta=-0x14)
    array = next(a for a in array_descriptors(layouts, classname, delta=-0x14) if a['name'] == field)
    assert (array['base'], array['count'], array['stride']) == (hex(base), count, 1)
    for i in range(count):
        assert fields[hex(base+i)] == [f'{field}_{i}', 'Bool']
    assert sum(isinstance(v, list) and bool(v) and str(v[0]).startswith(field+'_') for v in fields.values()) == count


def test_bordello_native_persist_offsets_save_and_restore_the_same_array():
    layouts = load_layouts()
    fields, _, _ = class_fields(layouts, 'CV_BordelloScript', delta=-0x14)
    fields.pop('__timers__')
    fields.pop('__resources__')
    unit = {'quest': {'fields': fields, 'arrays': array_descriptors(layouts, 'CV_BordelloScript', delta=-0x14)}}
    evidence = json.loads((ROOT/'refs/script_recovery/bordello/translation_unit_typed.json').read_text())
    native = next(f['decompile'] for f in evidence['functions'] if f['address'].lower() == '0x00e3b8f0')
    for i in range(3):
        assert f'"ClientInUse[{i}]",(void *)((int)this + {hex(0x58+i)})' in native
    # The normalized calls retain the native field addresses and serialized keys.
    source = '\n'.join(f'CPersistContext::Transfer<bool>(param_2,"ClientInUse[{i}]",(bool *)(this + {hex(0x58+i)}));' for i in range(3))
    spec = LoweringSpec(unit, 'V_Bordello', entity=False, thing_slots={})
    spec.call_labels = {'CPersistContext::Transfer<bool>': 1}
    spec.resolve_string = None
    source, _ = lower(source, spec)
    lines, _, todo = lift_persist_evidence(source, unit, spec, {1: 'Bool'})
    assert not todo
    lua = LuaRuntime()
    state = {'ClientInUse_0': True, 'ClientInUse_1': False, 'ClientInUse_2': True}
    serialized = {}
    def transfer(_q, _ctx, key, value):
        serialized[key] = value
        return not value
    quest = lua.table_from({
        'GetStateBool': lambda _q, key: state[key],
        'PersistTransferBool': transfer,
        'SetStateBool': lambda _q, key, value: state.update({key: value}),
    })
    lua.execute('return function(quest, context)\n'+'\n'.join(lines)+'\nend')(quest, lua.table())
    assert serialized == {'ClientInUse[0]': True, 'ClientInUse[1]': False, 'ClientInUse[2]': True}
    assert state == {'ClientInUse_0': False, 'ClientInUse_1': True, 'ClientInUse_2': False}


@pytest.mark.parametrize('parent', [True, False])
@pytest.mark.parametrize('initial', [True, False])
def test_dynamic_byte_read_and_clear_touch_only_requested_flag(parent, initial):
    unit = json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'ArenaSpawn', entity=parent, thing_slots={})
    spec.call_labels = {}
    base = '*(int *)(this + 0x14)' if parent else 'this'
    source = '{\ncVar1 = *(char *)('+base+' + 0xe2 + iVar4);\n*(undefined1 *)('+base+' + 0xe2 + iVar4) = 0;\nreturn cVar1;\n}'
    lowered, _ = lower(source, spec)
    lifter = make(entity=parent)
    body = finish_lua('\n'.join(lifter.lift('ReadAndClear', lowered, parameters={'iVar4': 'number'})))
    assert not lifter.todo
    lua = LuaRuntime()
    fn = lua.execute('return function(quest,iVar4)\n'+body+'\nend')
    for index in range(16):
        state = {f'ArenaSpawnNeeded_{i}': initial for i in range(16)}
        events = []
        def clear(_q, key, value):
            events.append((key, value))
            state[key] = value
        quest = lua.table_from({'GetStateBool': lambda _q, key: state[key], 'SetStateBool': clear})
        assert fn(quest,index) is initial
        assert events == [(f'ArenaSpawnNeeded_{index}', False)]
        assert state == {f'ArenaSpawnNeeded_{i}': False if i == index else initial for i in range(16)}


def test_packed_zero_store_clears_only_proven_array_extent():
    unit = json.loads((ROOT/'refs/script_recovery/guild_master_village/units/V_GuildMaster.json').read_text())
    spec = LoweringSpec(unit, 'V_GuildMaster', entity=False, thing_slots={})
    spec.call_labels = {}
    source = '{\n*(undefined4 *)(this + 0x48) = 0;\n*(undefined1 *)(this + 0x4c) = 0;\n}'
    out, _ = lower(source, spec)
    for i in range(5):
        assert f'QUESTSTATE_SetBool("GuildMasterDialogue_{i}", false);' in out
    # A wide store crossing the array boundary cannot be split into flag writes.
    rejected, _ = lower('{\n*(undefined4 *)(this + 0x4b) = 0;\n}', spec)
    assert 'QUESTSTATE_SetBool(' not in rejected


CURSOR_SOURCE = '''{
char *pcVar4;
iVar3 = 0;
pcVar4 = (char *)(*(int *)(this + 0x14) + 0x58);
do {
    if (*pcVar4 == '\\0') {
        *(undefined1 *)(*(int *)(this + 0x14) + 0x58 + iVar3) = 1;
        return iVar3;
    }
    iVar3 = iVar3 + 1;
    pcVar4 = pcVar4 + 1;
} while (iVar3 < 3);
return -1;
}'''


@pytest.mark.parametrize('flags,expected', [
    ((False, False, False), 0), ((True, False, False), 1),
    ((True, True, False), 2), ((True, True, True), -1),
])
def test_byte_cursor_reserves_first_free_client(flags, expected):
    unit = json.loads((ROOT/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    spec = LoweringSpec(unit, 'BordelloClient', entity=True, thing_slots={})
    spec.call_labels = {}
    source, _ = lower(CURSOR_SOURCE, spec)
    lifter = make(entity=True)
    lifter.accessor_kinds = True  # UnitConverter enables typed accessor/local tracking.
    body = finish_lua('\n'.join(lifter.lift('Reserve', source)))
    assert not lifter.todo
    lua = LuaRuntime()
    state = {f'ClientInUse_{i}': v for i, v in enumerate(flags)}
    writes = []
    quest = lua.table_from({'GetStateBool': lambda _q, key: state[key],
                           'SetStateBool': lambda _q, key, value: writes.append((key,value))})
    assert lua.execute('return function(quest)\n'+body+'\nend')(quest) == expected
    assert writes == ([] if expected == -1 else [(f'ClientInUse_{expected}', True)])


@pytest.mark.parametrize('use', ['Escape(pcVar4);', '*pcVar4 = 1;', 'pcVar4 = pcVar4 + 2;',
                                'Escape(&*pcVar4);', 'Escape(pcVar4[1]);'])
def test_byte_cursor_with_unproven_uses_is_preserved(use):
    unit = json.loads((ROOT/'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    spec = LoweringSpec(unit, 'BordelloClient', entity=True, thing_slots={})
    spec.call_labels = {}
    out, _ = lower(CURSOR_SOURCE.replace('return -1;', use+'\nreturn -1;'), spec)
    assert 'clientInUseIndex' not in out


@pytest.fixture(scope='module')
def converted_bordello(tmp_path_factory):
    evidence = ROOT/'refs/script_recovery/bordello'
    unit = json.loads((evidence/'units/V_Bordello.json').read_text())
    out = tmp_path_factory.mktemp('bordello_byte_arrays')
    UnitConverter(evidence/'translation_unit_typed.json').convert(unit, out)
    return out/'FSE/V_Bordello'


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('flags,expected', [
    ((False, False, False), 0), ((True, False, False), 1),
    ((True, True, False), 2), ((True, True, True), None),
])
def test_generated_client_init_reserves_available_slot(converted_bordello, readable, flags, expected):
    source = (converted_bordello/'Entities/BordelloClient.lua').read_text()
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    # Shared quest helpers are not called by this entity's Init path.
    lua.execute("package.preload['V_Bordello.native_quest_helpers'] = function() return {} end")
    state = {f'ClientInUse_{i}': v for i, v in enumerate(flags)}
    writes = []
    def set_bool(_q, key, value):
        writes.append((key, value))
        state[key] = value
    quest = lua.table_from({
        'GetStateInt': lambda _q, key: 2,
        'SetStateInt': lambda _q, key, value: None,
        'GetStateBool': lambda _q, key: state[key],
        'SetStateBool': set_bool,
        'SetThingPersistent': lambda *args: None,
        'EntitySetOpinionReactionMask': lambda *args: None,
    })
    lua.execute(source)
    lua.globals().Init(quest,lua.table())
    assert writes == ([] if expected is None else [(f'ClientInUse_{expected}', True)])


def test_generated_quest_persistence_uses_initialized_boolean_keys(converted_bordello):
    source = (converted_bordello/'V_Bordello.lua').read_text()
    persist = re.search(r'^function OnPersist\(.*?(?=^function |\Z)', source, re.M|re.S).group()
    lua = LuaRuntime()
    state = {'ClientInUse_0': True, 'ClientInUse_1': False, 'ClientInUse_2': True}
    saved = {}
    quest = lua.table_from({
        'GetStateBool': lambda _q, key: state.get(key, False),
        'GetStateInt': lambda _q, key: 0,
        'PersistTransferBool': lambda _q, _ctx, key, value: saved.setdefault(key, value),
        'PersistTransferInt': lambda _q, _ctx, key, value: value,
        'SetStateBool': lambda _q, key, value: None,
        'SetStateInt': lambda _q, key, value: None,
    })
    lua.execute(persist)
    lua.globals().OnPersist(quest,lua.table())
    assert {k:v for k,v in saved.items() if k.startswith('ClientInUse')} == {
        'ClientInUse[0]': True, 'ClientInUse[1]': False, 'ClientInUse[2]': True}
