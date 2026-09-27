"""Arena scalar counters retain independent buckets and cursor resets."""
import json
import re

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, finish_lua
from tools.script_recovery.native_integer_arrays import lower_integer_array
from tools.script_recovery.quest_unit_evidence import ROOT, load_layouts, array_descriptors
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.convert_quest_unit import UnitConverter

ARRAY = dict(name='TotalCreatures', base=0xd0, count=3, stride=4)
SOURCE = '''{
  CQ_ArenaScript *cursor;
  int total;
  int i;
  cursor = this + 0xd0;
  i = 0;
  while (i < 3) {
    *(int *)cursor = i + 2;
    cursor = cursor + 4;
    i = i + 1;
  }
  cursor = this + 0xd0;
  total = 0;
  i = 0;
  while (i < 3) {
    total = total + *(int *)cursor;
    *(int *)cursor = *(int *)cursor + 1;
    cursor = cursor + 4;
    i = i + 1;
  }
  return total;
}'''


@pytest.mark.parametrize('readable', [False, True])
def test_generated_loops_write_sum_and_increment_three_independent_buckets(readable):
    unit = json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'Q_Arena', entity=False, thing_slots={})
    spec.call_labels = {}
    lowered, _ = lower(SOURCE, spec)
    lifter = make()
    lifter.accessor_kinds = True
    body = finish_lua('\n'.join(lifter.lift('Counters', lowered)))
    assert not lifter.todo
    source = 'function Counters(quest)\n' + body + '\nend'
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    lua.execute('debug.sethook(function() error("instruction budget exceeded") end, "", 10000)')
    state = {}
    writes = []
    def put(_q, key, value):
        writes.append((key, value))
        state[key] = value
    quest = lua.table_from({'SetStateInt': put, 'GetStateInt': lambda _q, key: state[key]})
    assert lua.globals().Counters(quest) == 9
    assert state == {f'TotalCreatures_{i}': i+3 for i in range(3)}
    assert writes == [(f'TotalCreatures_{i}', i+step) for step in (2, 3) for i in range(3)]


@pytest.mark.parametrize('extra', ['Escape(cursor);', 'Escape(&*(int *)cursor);',
    'other = cursor;', 'cursor = other;', 'cursor = cursor + 1;',
    '*(short *)cursor = 0;', 'value = cursor[1];'])
def test_unproven_cursor_uses_are_not_rewritten(extra):
    source = SOURCE.replace('  return total;', '  '+extra+'\n  return total;')
    assert lower_integer_array(source, ARRAY, 'this', 'QUEST') == source


def test_unknown_pointer_stride_and_use_before_reset_are_preserved():
    for source in (SOURCE.replace('CQ_ArenaScript *cursor', 'int *cursor'),
                   SOURCE.replace('  cursor = this + 0xd0;', '  total = *(int *)cursor;\n  cursor = this + 0xd0;', 1)):
        assert lower_integer_array(source, ARRAY, 'this', 'QUEST') == source


def test_pdb_layout_and_native_cursor_instructions_agree():
    a = next(a for a in array_descriptors(load_layouts(), 'CQ_ArenaScript', delta=-0x14)
             if a['name'] == 'TotalCreatures')
    assert (a['base'], a['stride'], a['count'], a['element']) == ('0xd0', 4, 3, 'long')
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    code = {i.address: (i.mnemonic, i.op_str) for i in
            Cs(CS_ARCH_X86, CS_MODE_32).disasm(RData().bytes_at(0xf1eed0, 0x1800), 0xf1eed0)}
    for address, register in ((0xf1ff4c, 'ebx'), (0xf2039c, 'edi'),
                              (0xf20579, 'ebp'), (0xf206ae, 'ebp')):
        assert code[address] == ('lea', register + ', [esi + 0xd0]')
    for address, register in ((0xf1ffff, 'ebx'), (0xf203b4, 'edi'),
                              (0xf205cb, 'ebp'), (0xf206cb, 'ebp')):
        assert code[address] == ('add', register + ', 4')


def test_all_actual_playwave_cursor_uses_are_recovered():
    tu = json.loads((ROOT/'refs/script_recovery/arena/translation_unit_typed.json').read_text())
    source = next(f['decompile'] for f in tu['functions'] if int(f['address'], 16) == 0xf1eed0)
    out = lower_integer_array(source.replace('\r', ''), ARRAY, 'this', 'QUEST')
    assert not re.search(r'\b(?:pCVar15|pCStack_bc)\b', out)
    assert out.count('totalCreaturesIndex2 = 0;') == 4
    assert 'QUESTSTATE_SetInt(__key("TotalCreatures_" .. totalCreaturesIndex), 0);' in out


@pytest.mark.parametrize('index', ['bucket', 'QUESTSTATE_GetInt("Bucket")'])
@pytest.mark.parametrize('parent', [False, True])
def test_dynamic_index_and_static_access_refer_to_the_same_state(index, parent):
    unit = json.loads((ROOT/'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'SUMMONED_CREATURE' if parent else 'Q_Arena', entity=parent, thing_slots={})
    spec.call_labels = {}
    receiver = '*(int *)(this + 0x14)' if parent else 'this'
    source = ('{\n*(int *)(' + index + ' * 4 + 0xd0 + ' + receiver + ') = 7;\n'
              'return *(int *)(' + receiver + ' + 0xd4);\n}')
    out, _ = lower(source, spec)
    assert f'QUESTSTATE_SetInt(__key("TotalCreatures_" .. {index}), 7);' in out
    assert '__element' not in out
    lifter = make(entity=parent, state={} if parent else {'0xd4': ('TotalCreatures_1', 'Int')})
    lifter.parent_state = {'0xd4': ('TotalCreatures_1', 'Int')}
    lifter.receiver = lifter.state_receiver = 'quest'
    lifter.accessor_kinds = True
    # Supply dynamic state separately so both indexes point at bucket one.
    body = finish_lua('\n'.join(lifter.lift('Read', out, parameters={'bucket': 'number'})))
    assert not lifter.todo
    lua = LuaRuntime()
    state = {'Bucket': 1}
    quest = lua.table_from({'SetStateInt': lambda _q, key, value: state.update({key: value}),
                           'GetStateInt': lambda _q, key: state[key]})
    assert lua.execute('return function(quest,bucket)\n'+body+'\nend')(quest, 1) == 7
    assert state == {'Bucket': 1, 'TotalCreatures_1': 7}


@pytest.fixture(scope='module')
def summoned_source(tmp_path_factory):
    evidence = ROOT/'refs/script_recovery/arena'
    out = tmp_path_factory.mktemp('arena_counters')
    unit = json.loads((evidence/'units/Q_Arena.json').read_text())
    UnitConverter(evidence/'translation_unit_typed.json').convert(unit, out)
    return (out/'FSE/Arena/Entities/SUMMONED_CREATURE.lua').read_text()


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('hero_kill', [False, True])
def test_actual_summoned_creature_death_decrements_counter_and_awards_points(summoned_source, readable, hero_kill):
    source = readable_file(summoned_source)[0] if readable else summoned_source
    lua = LuaRuntime()
    lua.execute(source)
    state = {'TotalCreatures_0': 5, 'TotalCreatures_1': 7,
             'TotalCreatures_2': 9, 'NewCrowdPoints': 12}
    writes = []
    def put(_q, key, value):
        writes.append((key, value))
        state[key] = value
    quest = lua.table_from({'SetStateInt': put, 'GetStateInt': lambda _q, key: state[key]})
    killed_by = []
    def killed(_me, name):
        killed_by.append(name)
        return hero_kill
    lua.globals().OnPredicateFail(quest, lua.table_from({'MsgIsKilledBy': killed}))
    assert writes == [('TotalCreatures_0', 4)] + ([('NewCrowdPoints', 20)] if hero_kill else [])
    assert (state['TotalCreatures_1'], state['TotalCreatures_2']) == (7, 9)
    assert killed_by == ['SCRIPT_NAME_HERO']


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('terminate_before_count', [False, True])
def test_actual_spawn_counts_once_unless_already_terminating(summoned_source, readable, terminate_before_count):
    source = readable_file(summoned_source)[0] if readable else summoned_source
    lua = LuaRuntime()
    lua.execute(source)
    state = {'TotalCreatures_0': 5, 'ExtraCreatures': 0}
    writes = []
    frames = []
    terminated = terminate_before_count
    def frame(*_):
        nonlocal terminated
        frames.append('frame')
        assert len(frames) <= 2
        terminated = terminated or len(frames) == 2
        return not terminated
    def put(_q, key, value):
        writes.append((key, value))
        state[key] = value
    quest = lua.table_from({'SetStateInt': put, 'GetStateInt': lambda _q, key: state.get(key, 0),
                           'GetHero': lambda *_: lua.table(),
                           'NewScriptFrame': frame, 'IsActiveThreadTerminating': lambda _: terminated})
    me = lua.table_from({name: lambda *_: False for name in ('MsgIsHitByHero',
        'MsgIsHitByHeroWithFlourish', 'MsgIsHitByHeroWithDecapitate', 'MsgIsHitByAnySpecialAbilityFromHero')})
    lua.globals().Main(quest, me)
    assert writes == ([] if terminate_before_count else [('TotalCreatures_0', 6), ('ExtraCreatures', 1)])
