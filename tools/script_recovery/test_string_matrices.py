"""PDB string matrices preserve row/column identity and Arena crowd reactions."""
import json
import re

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.quest_unit_evidence import ROOT, load_layouts, class_fields, array_descriptors
from tools.script_recovery.convert_quest_unit import UnitConverter, state_map
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.build_readable_unit import readable_file


def arena_spec(entity=False):
    unit = json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'SUMMONED_CREATURE' if entity else 'Q_Arena', entity=entity, thing_slots={})
    spec.call_labels = {}
    return unit, spec


@pytest.mark.parametrize('element', ['CCharString', 'CWideString'])
@pytest.mark.parametrize('rows,cols', [(2, 3), (4, 5)])
def test_matrix_layout_is_row_major_and_stops_before_following_field(element, rows, cols):
    layout = {'Example': [[{'offset': 0x48, 'type': f'{element}[{rows}][{cols}]', 'name': 'Tags'},
                          {'offset': 0x48+rows*cols*4, 'type': 'int', 'name': 'Following'}]]}
    fields, skipped, _ = class_fields(layout, 'Example')
    assert not skipped
    fields.pop('__timers__')
    fields.pop('__resources__')
    assert fields == {**{hex(0x48+(r*cols+c)*4): [f'Tags_{r}_{c}', 'String']
                        for r in range(rows) for c in range(cols)},
                      hex(0x48+rows*cols*4): ['Following', 'Int']}
    descriptor, = array_descriptors(layout, 'Example')
    assert (descriptor['count'], descriptor['stride'], descriptor['dimensions']) == (rows, cols*4, [rows, cols])


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('row,col', [(r,c) for r in range(4) for c in range(5)])
def test_dynamic_rows_and_columns_select_exact_entry(readable, row, col):
    _, spec = arena_spec()
    source, _ = lower('{\nreturn (CCharString *)(this + column * 4 + row * 0x14 + 0x48);\n}', spec)
    lifter = make()
    lifter.accessor_kinds = True
    body = finish_lua('\n'.join(lifter.lift('Reaction', source, parameters={'row': 'number', 'column': 'number'})))
    assert not lifter.todo
    source = 'function Reaction(quest,row,column)\n'+body+'\nend'
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    requests = []
    def get_string(_q, key):
        requests.append(key)
        return key + '_value'
    quest = lua.table_from({'GetStateString': get_string})
    expected = f'CrowdLoopTags_{row}_{col}'
    assert lua.globals().Reaction(quest, row, col) == expected+'_value'
    assert requests == [expected]


@pytest.mark.parametrize('row', range(4))
def test_parent_field_selects_cheer_column(row):
    unit, spec = arena_spec(entity=True)
    source, _ = lower('{\nreturn (CCharString *)(*(int *)(this + 0x14) + 0x54 + *(int *)(*(int *)(this + 0x14) + 0xfc) * 0x14);\n}', spec)
    lifter = make(entity=True)
    lifter.parent_state = state_map(unit['quest']['fields'])
    body = finish_lua('\n'.join(lifter.lift('Cheer', source)))
    assert not lifter.todo
    lua = LuaRuntime()
    keys = []
    def get_string(_q, key):
        keys.append(key)
        return 'cheer'
    quest = lua.table_from({'GetStateInt': lambda _q, key: row, 'GetStateString': get_string})
    assert lua.execute('return function(quest)\n'+body+'\nend')(quest) == 'cheer'
    assert keys == [f'CrowdLoopTags_{row}_3']


@pytest.mark.parametrize('bad', ['this + column * 8 + row * 0x14 + 0x48',
                               'this + column * 4 + row * 0x18 + 0x48',
                               'this + column * 4 + row * 0x14 + 0x4c'])
def test_unproven_address_scaling_remains_unresolved(bad):
    _, spec = arena_spec()
    source, _ = lower('{\nreturn (CCharString *)('+bad+');\n}', spec)
    assert 'STATE_GetString(__key(' not in source


@pytest.mark.parametrize('use', ['return result;', 'Use(result);', 'before'])
def test_used_assignment_reference_is_not_discarded(use):
    _, spec = arena_spec()
    assignment = 'result = CCharString::operator=((CCharString *)(this + 0x94),"ARENA_AWWW");'
    source = '{\nCCharString *result;\n' + ('Use(result);\n' if use == 'before' else '') + assignment + '\n' + (use if use != 'before' else '') + '\n}'
    out, _ = lower(source, spec)
    assert 'result = CCharString::operator=' in out
    assert 'STATE_SetString("CrowdLoopTags_3_4"' not in out


@pytest.fixture(scope='module')
def retail_tags():
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    rdata = RData()
    code = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(rdata.bytes_at(0xf25840, 0x130), 0xf25840))
    tags = {}
    for n, insn in enumerate(code):
        if insn.mnemonic == 'call' and insn.op_str == '0x99efe0':
            push, dest = code[n-2:n]
            assert push.mnemonic == 'push' and dest.mnemonic == 'lea'
            offset = int(re.fullmatch(r'ecx, \[esi \+ (0x[0-9a-f]+)\]', dest.op_str)[1], 16)
            tags[offset] = rdata.string_at(int(push.op_str, 16))
    assert sorted(tags) == list(range(0x48, 0x98, 4))
    fields, _, _ = class_fields(load_layouts(), 'CQ_ArenaScript', delta=-0x14)
    assert all(fields[hex(offset)] == [f'CrowdLoopTags_{(offset-0x48)//20}_{((offset-0x48)//4)%5}', 'String'] for offset in tags)
    return {f'CrowdLoopTags_{(offset-0x48)//20}_{((offset-0x48)//4)%5}': tag for offset,tag in tags.items()}


@pytest.fixture(scope='module')
def generated(tmp_path_factory):
    evidence = ROOT/'refs/script_recovery/arena'
    out = tmp_path_factory.mktemp('arena_crowd_tags')
    UnitConverter(evidence/'translation_unit_typed.json').convert(json.loads((evidence/'units/Q_Arena.json').read_text()), out)
    return out/'FSE/Arena'


@pytest.mark.parametrize('readable', [False, True])
def test_generated_initializer_stores_all_twenty_retail_tags(generated, retail_tags, readable):
    source = (generated/'Arena.lua').read_text()
    start = source.index('function InitialiseVariables(')
    end = source.index('\nfunction ', start+1)
    source = source[start:end]
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    # Snapshot construction is tested separately; this case checks the twenty
    # native string assignments after that explicit runtime boundary.
    lua.execute(source)
    writes = []
    quest = lua.table_from({'SetStateString': lambda _q,key,value: writes.append((key,value)),
                           'InitialiseArenaRounds': lambda _q: None})
    lua.globals().InitialiseVariables(quest)
    assert len(writes) == 20
    assert dict(writes) == retail_tags


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('row', range(4))
def test_actual_summoned_flourish_plays_named_cheer(generated, retail_tags, readable, row):
    source = (generated/'Entities/SUMMONED_CREATURE.lua').read_text()
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    state = {'NewCrowdBaseLevel': row, 'GlobalCrowdTimer': 42, 'NewCrowdPoints': 0,
             'TotalCreatures_0': 0, 'ExtraCreatures': 0}
    frames, sounds = [], []
    def frame(*_):
        frames.append(1)
        assert len(frames) <= 2
        return len(frames) < 2
    quest = lua.table_from({'NewScriptFrame': frame, 'IsActiveThreadTerminating': lambda _: len(frames)>=2,
        'GetStateInt': lambda _q,key: state[key], 'SetStateInt': lambda _q,key,value: state.update({key:value}),
        'GetStateString': lambda _q,key: retail_tags[key], 'GetHero': lambda _: 'hero',
        'GetNearestWithScriptName': lambda *_: 'crowd', 'GetTimer': lambda *_: 0,
        'SetTimer': lambda *_: None, 'PlayCriteriaSoundOnThing': lambda _q,actor,tag: sounds.append((actor,tag))})
    lua.globals().Main(quest, lua.table_from({'MsgIsHitByHeroWithFlourish': lambda _: True}))
    assert sounds == [('crowd', retail_tags[f'CrowdLoopTags_{row}_3'])]
    assert state['NewCrowdPoints'] == 5
