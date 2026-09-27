"""Arena definition boundaries: native/PDB evidence, fail-closed lowering, Lua traces."""
import copy
import json
import re
from functools import lru_cache

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.quest_unit_evidence import ROOT, load_layouts
from tools.script_recovery.native_arena_rounds import (
    CONTAINER_HELPERS, CREATURES, ROUNDS, initialise_rounds, recover_round_reads,
    replaced_container_helpers, recover_spawn_thing_destination,
)
from tools.script_recovery.convert_quest_unit import UnitConverter, unwrap_statements
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
from tools.script_recovery.native_arena_counter_arrays import recover_counter_arrays
from tools.script_recovery.build_readable_unit import readable_file

EVIDENCE = ROOT / 'refs/script_recovery/arena'


@lru_cache(maxsize=1)
def readable_arena(source):
    return readable_file(source)[0]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((EVIDENCE/'units/Q_Arena.json').read_text())
    converter = UnitConverter(EVIDENCE/'translation_unit_typed.json')
    return unit, converter


@pytest.fixture(scope='module')
def native(evidence):
    return normalise_typed_decompile(unwrap_statements(evidence[1].native('0x00f1eed0')['decompile']))


@pytest.fixture(scope='module')
def generated(evidence, tmp_path_factory):
    out = tmp_path_factory.mktemp('arena_rounds')
    report = evidence[1].convert(evidence[0], out)
    return (out/'FSE/Arena/Arena.lua').read_text(), report


def test_pdb_names_and_retail_copy_instructions_agree():
    layouts = load_layouts()
    def members(name):
        return {f['name']: (f['offset'], f['type']) for f in layouts[name][0] if f['name'] != '_padding_'}
    assert members('CArenaRoundDef') == {'NumWaves': (0x28, 'long'),
        'Waves': (0x2c, 'vector<CArenaWaveDef,std::allocator<CArenaWaveDef>_>')}
    assert members('CArenaWaveDef') == {'NumWaveCreatures': (0x28, 'long'),
        'Creatures': (0x2c, 'vector<CArenaCreatureDef,std::allocator<CArenaCreatureDef>_>'),
        'ShortWave': (0x3c, 'bool')}
    assert members('CArenaCreatureDef') == {'CreatureType': (0x28, 'CCharString'),
        'NumCreatures': (0x2c, 'long'), 'HUDType': (0x30, 'CCharString'), 'DeathScore': (0x34, 'long')}
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    memory = RData()
    code = {}
    for address, length in [(0xf25840, 0x20), (0xf26b30, 0x60), (0xf25f10, 0x80), (0xf1eed0, 0x2430)]:
        code.update({i.address: (i.mnemonic, i.op_str) for i in
            Cs(CS_ARCH_X86, CS_MODE_32).disasm(memory.bytes_at(address, length), address)})
    expected = {
        0xf25840: ('mov', 'eax, dword ptr [0x143e90c]'),
        0xf25848: ('add', 'eax, 0x1044'), 0xf2584e: ('lea', 'ecx, [esi + 0x98]'),
        0xf25854: ('call', '0xf25980'),
        0xf26b6e: ('mov', 'dword ptr [edi + 0x28], ecx'),
        0xf26b75: ('call', '0xf26920'), 0xf26b7a: ('add', 'edi, 0x38'),
        0xf25f71: ('lea', 'ecx, [edi + 0x2c]'), 0xf25f80: ('mov', 'dl, byte ptr [edi + 0x38]'),
        0xf25f1e: ('lea', 'eax, [edi + 0x28]'), 0xf25f25: ('call', '0x99efb0'),
        0xf25f2a: ('mov', 'ecx, dword ptr [edi + 0x2c]'),
        0xf25f2d: ('lea', 'edx, [edi + 0x30]'), 0xf25f37: ('call', '0x99efb0'),
        0xf25f3c: ('mov', 'eax, dword ptr [edi + 0x34]'),
        0xf1ff23: ('imul', 'ecx, ecx, 0x3c'), 0xf1ff26: ('add', 'ebx, 0x38'),
        0xf1fff9: ('add', 'edi, 0x38'), 0xf20002: ('cmp', 'edi, 0xa8'),
        0xf1f86b: ('push', 'eax'), 0xf1f86c: ('lea', 'ecx, [esp + 0xa4]'),
        0xf1f873: ('call', '0x8ab980'),
        0xf1f914: ('push', 'eax'), 0xf1f915: ('lea', 'ecx, [esp + 0xa4]'),
        0xf1f91c: ('call', '0x8ab980'), 0xf1fa98: ('lea', 'ecx, [esp + 0xa0]'),
        0xf1faba: ('mov', 'eax, dword ptr [esp + 0x9c]'),
    }
    assert {address: code[address] for address in expected} == expected


@pytest.mark.parametrize('unified', [False, True])
def test_all_round_reads_recover_and_other_string_lifetimes_are_unchanged(native, unified):
    if unified:
        native = re.sub(r'\b(?:C|au)Stack_([0-9a-f]+)\b', r'xStack_\1', native)
    native = recover_counter_arrays(native)
    out = recover_round_reads(native)
    assert out != native and ROUNDS not in out
    assert out.count('savedCreatureGroupIndex = creatureGroupIndex;') == 4
    assert out.count('creatureGroupIndex = savedCreatureGroupIndex + 1;') == 2
    assert out[out.index('LAB_00f20ce5:'):] == native[native.index('LAB_00f20ce5:'):]


@pytest.mark.parametrize('old,new', [
    ('0xa8);', '0xe0);'), ('(int)CVar14 + 0x38', '(int)CVar14 + 0x3c'),
    ('CVar14 = CStack_108;', 'CVar14 = other;'),
    ('CStack_108 = (CCharString)0x0;', 'CStack_108 = (CCharString)1;'),
    ('  CCharString CVar14;', '  CCharString CVar14;\n  Escape(CVar14);'),
    ('CVar14 = CStack_108;', 'CVar14 = CStack_108;\n  Escape(&CStack_108);'),
    (f'iVar6 = {CREATURES};', f'iVar6 = {CREATURES};\n  Escape(iVar6);'),
    (f'iVar6 = {CREATURES};', f'iVar6 = {CREATURES};\n  goto UNPROVEN;'),
    ('  CCharString CVar14;', '  CCharString CVar14;\n  int creatureGroupIndex;'),
    (ROUNDS, '*(int *)(this + 0x9c)'),
])
def test_changed_cursor_or_escaped_alias_declines_every_round_rewrite(native, old, new):
    source = native.replace(old, new, 1)
    assert source != native
    assert recover_round_reads(source) == source


def test_initializer_requires_the_reviewed_target_and_operands():
    source = '  Copy(this + 0x98,DAT_0143e90c + 0x1044);'
    assert initialise_rounds(source, {'Copy': 0xf25980}) == '  ENGINE_InitialiseArenaRounds();'
    assert initialise_rounds(source, {'Copy': 0xf26920}) == source
    changed = source.replace('0x98', '0x9c')
    assert initialise_rounds(changed, {'Copy': 0xf25980}) == changed


@pytest.mark.parametrize('unified', [False, True])
def test_spawn_handle_copy_does_not_overwrite_the_numeric_loop_counter(native, unified):
    if unified:
        native = native.replace('CStack_cc', 'xStack_c0')
    slot = 'xStack_c0' if unified else 'CStack_cc'
    old = 'CScriptThing::operator=((CScriptThing *)&iStack_d0,(int)pCVar5);'
    new = f'CScriptThing::operator=((CScriptThing *)&{slot},(int)pCVar5);'
    out = recover_spawn_thing_destination(native, {'CScriptThing::operator=': 0x8ab980})
    assert out == native.replace(old, new)


@pytest.mark.parametrize('old,new', [
    ('LAB_00f1f934:', 'DIFFERENT_JOIN:'), ('LAB_00f1faba:', 'DIFFERENT_LOOP:'),
    ('iStack_d0 = iStack_d0 + 1;', 'iStack_d0 = iStack_d0 + 2;'),
    ('CScriptThing::operator=((CScriptThing *)&CStack_cc,(int)pCVar5);',
     'CScriptThing::operator=((CScriptThing *)&other,(int)pCVar5);'),
])
def test_changed_handle_join_keeps_unproven_destination(native, old, new):
    source = native.replace(old, new)
    assert source != native
    assert recover_spawn_thing_destination(source, {'CScriptThing::operator=': 0x8ab980}) == source
    assert recover_spawn_thing_destination(native, {'CScriptThing::operator=': 0x8ab990}) == native


def test_container_omission_is_limited_to_unreachable_library_closure(evidence):
    unit, converter = evidence
    expected = {f'0x{a:08x}' for a in CONTAINER_HELPERS}
    assert replaced_container_helpers(unit, converter.native) == expected
    changed = copy.deepcopy(converter.native('0x00f0fb70'))
    changed['calls'].append({'target': '0x00f26920', 'currentName': 'SharedLibraryUse'})
    def shared(address):
        return changed if address == '0x00f0fb70' else converter.native(address)
    omitted = replaced_container_helpers(unit, shared)
    assert '0x00f25980' in omitted
    assert '0x00f26920' not in omitted and '0x00f25f10' not in omitted
    changed['calls'].append({'target': '0x00f25980', 'currentName': 'OtherCopy'})
    assert replaced_container_helpers(unit, shared) == set()
    altered_unit = copy.deepcopy(unit)
    altered_unit['quest']['functions']['InitialiseVariables']['address'] = '0x00f25844'
    assert replaced_container_helpers(altered_unit, converter.native) == set()


@pytest.mark.parametrize('readable', [False, True])
def test_complete_arena_file_loads_and_initializer_orders_snapshot_before_twenty_tags(generated, readable):
    source, report = generated
    if readable:
        source = readable_arena(source)
    lua = LuaRuntime()
    lua.execute(source)
    calls = []
    quest = lua.table_from({'InitialiseArenaRounds': lambda _: calls.append('snapshot'),
        'SetStateString': lambda _q, key, value: calls.append((key, value))})
    lua.globals().InitialiseVariables(quest)
    assert calls[0] == 'snapshot' and len(calls) == 21
    assert all('function helper_' + f'{address:X}' not in source for address in CONTAINER_HELPERS)
    assert 'function helper_F14250' in source
    assert len(report['runtimeBoundaries'][0]['omittedUnreachableContainerBodies']) == 19


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('short_wave,round_index,wave_index,expected', [
    (False,0,0,False), (False,0,1,True), (False,2,0,True), (True,0,1,False), (True,2,3,False),
])
def test_actual_short_wave_guard_has_lua_boolean_semantics(generated, readable, short_wave, round_index, wave_index, expected):
    source = generated[0]
    source = source[source.index('function PlayWave('):]
    condition = next(line.strip()[3:-5] for line in source.splitlines() if 'ShortWave' in line and line.strip().startswith('if '))
    source = 'function ShortWaveGuard(quest)\nreturn '+condition+'\nend'
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    calls = []
    def flag(_q, key):
        calls.append(key)
        return short_wave
    quest = lua.table_from({'GetStateBool': flag, 'GetStateInt': lambda _q, key:
        {'ArenaRound': round_index, 'ArenaRoundWave': wave_index}[key]})
    assert lua.globals().ShortWaveGuard(quest) is expected
    assert calls == [f'Rounds_{round_index}_Waves_{wave_index}_ShortWave']


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('round_index,wave_index,counts', [(0,0,[5,0,2]), (2,3,[-1,3,1]), (4,1,[0,0,0])])
def test_actual_hud_loop_uses_each_group_and_preserves_inactive_sentinels(generated, readable, round_index, wave_index, counts):
    source = generated[0]
    start = source.index('    creatureGroupIndex = 0', source.index('creatureGroupIndex = savedCreatureGroupIndex + 1'))
    end = source.index('    until not (creatureGroupIndex < 3)', start) + len('    until not (creatureGroupIndex < 3)')
    source = ('function Counters(quest)\nlocal initialCreatureCounts, creatureCounterIds = {}, {}\n' +
              source[start:end] + '\ndo return initialCreatureCounts, creatureCounterIds end\n'
              '::LAB_00f20ca1::\nreturn nil\nend')
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    lua.execute('debug.sethook(function() error("instruction budget exceeded") end,"",10000)')
    state = {'ArenaRound': round_index, 'ArenaRoundWave': wave_index,
             **{f'TotalCreatures_{i}': count for i,count in enumerate(counts)}}
    adds, updates = [], []
    def add(_q, tag, count, scale):
        adds.append((tag, count, scale))
        return 100 + len(adds)
    quest = lua.table_from({'GetStateInt': lambda _q,key: state[key],
        'GetStateString': lambda _q,key: key+'_text', 'IsActiveThreadTerminating': lambda _: False,
        'AddQuestInfoCounterList': add, 'UpdateQuestInfoCounterList': lambda _q,*args: updates.append(args)})
    initial, ids = lua.globals().Counters(quest)
    active = [i for i,count in enumerate(counts) if count > 0]
    assert adds == [(f'Rounds_{round_index}_Waves_{wave_index}_Creatures_{i}_HUDType_text', counts[i], 1.0) for i in active]
    assert updates == [(101+n, counts[i], -1) for n,i in enumerate(active)]
    assert [initial[i+1] for i in range(3)] == counts
    assert [ids[i+1] for i in range(3)] == [101+active.index(i) if i in active else -1 for i in range(3)]


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('group', [0, 1, 2])
def test_actual_spawn_alias_reads_selected_creature_instead_of_folded_zero(generated, readable, group):
    source = generated[0]
    assignments = [line.strip().removeprefix('creatureType = ') for line in source.splitlines()
                   if line.strip().startswith('creatureType = quest:GetStateString(') and '_CreatureType' in line]
    assert len(assignments) == 5 and len(set(assignments)) == 1
    source = 'function CreatureType(quest,savedCreatureGroupIndex)\nreturn '+assignments[0]+'\nend'
    if readable:
        source = readable_file(source)[0]
    lua = LuaRuntime()
    lua.execute(source)
    keys = []
    def get_string(_q, key):
        keys.append(key)
        return {'Rounds_2_Waves_4_Creatures_0_CreatureType': 'HOBBE',
                'Rounds_2_Waves_4_Creatures_1_CreatureType': 'BANDIT',
                'Rounds_2_Waves_4_Creatures_2_CreatureType': 'BALVERINE'}[key]
    quest = lua.table_from({'GetStateString': get_string,
        'GetStateInt': lambda _q,key: {'ArenaRound': 2, 'ArenaRoundWave': 4}[key]})
    assert lua.globals().CreatureType(quest, group) == ['HOBBE','BANDIT','BALVERINE'][group]
    assert keys == [f'Rounds_2_Waves_4_Creatures_{group}_CreatureType']


@pytest.mark.parametrize('readable', [False, True])
@pytest.mark.parametrize('round_index', [2, 3])
@pytest.mark.parametrize('group', [0, 1, 2])
@pytest.mark.parametrize('short_wave', [False, True])
def test_actual_play_wave_reaches_hud_after_selected_group_spawn(generated, readable, round_index, group, short_wave):
    """One spawn point isolates the recovered group cursor from other known gaps.

    Run the real function through spawning and all HUD entries. Deliberately stop
    at DisplayQuestInfo: subsequent combat/reward flow is outside this oracle.
    This catches a string aliased over the numeric spawn-point loop bound.
    """
    source = generated[0]
    if readable:
        source = readable_arena(source)
    lua = LuaRuntime()
    lua.execute(source)
    lua.execute('''
events, counters, updates, state, dialogue = {}, {}, {}, {}, {}
timerRemaining, frames = 0, 0
actor={IsNull=function()return false end, IsAlive=function()return false end,
       GetDataString=function()return '0' end, GetPos=function()return{x=0,y=0,z=0}end}
quest={}
function quest:RetailResources()return{}end
function quest:GetStateInt(k)assert(state[k]~=nil,k);return state[k]end
function quest:SetStateInt(k,v)state[k]=v end
function quest:GetStateBool(k)assert(k:find('ShortWave'));return shortWave end
function quest:SetStateBool(k,v)end
function quest:GetStateString(k)return k end
function quest:RegisterTimer()return 12 end
function quest:SetTimer(id,seconds)assert(id==12 and seconds==2);timerRemaining=seconds end
function quest:GetTimer(id)assert(id==12);return timerRemaining end
function quest:NewScriptFrame()timerRemaining=timerRemaining-1;frames=frames+1;return true end
function quest:AddNewConversation(who,a,b)assert(who==actor and not a and not b);return 900 end
function quest:AddLineToConversation(id,line,a,b,c)
    assert(id==900 and a==actor and b==actor and not c);table.insert(dialogue,line)
end
function quest:GetThingWithScriptName(k)return actor end
function quest:GetHero()return actor end
function quest:GetAllThingsWithScriptName(k)if k=='ArenaSpawn' then return{actor}else return{}end end
function quest:GetFurthestWithScriptName(...)return actor end
function quest:IsActiveThreadTerminating()return false end
function quest:SetCreatureCreationDelayFrames(v)end
function quest:ResetCreatureCreationDelayFrames()end
function quest:CreateCreatureNearby(kind,pos,scale,script)
    assert(scale==1 and script=='ArenaEnemy');table.insert(events,kind);return actor
end
function quest:CreateCreature(kind,pos,script)
    assert(script=='ArenaEnemy');table.insert(events,kind);return actor
end
function quest:EntitySetCutsceneBehaviour(thing,mode)assert(thing==actor and mode==1)end
function quest:GiveThingBestEnemyTarget(thing,target)assert(thing==actor and target==actor)end
function quest:AddQuestInfoCounterList(tag,count,scale)
    assert(count==1 and scale==1);table.insert(counters,tag);return 101
end
function quest:UpdateQuestInfoCounterList(id,count,maximum)
    assert(id==101 and count==1 and maximum==-1);table.insert(updates,id)
end
function quest:DisplayQuestInfo(v)assert(v==true);error('STOP_AFTER_SPAWNS')end
debug.sethook(function()error('instruction budget exceeded')end,'',10000)
''')
    state = lua.globals().state
    lua.globals().shortWave = short_wave
    state.ArenaRound, state.ArenaRoundWave = round_index, 1
    prefix = f'Rounds_{round_index}_Waves_1'
    state[prefix+'_NumWaveCreatures'] = 3
    for i in range(3):
        state[f'{prefix}_Creatures_{i}_NumCreatures'] = int(i == group)
    with pytest.raises(Exception, match='STOP_AFTER_SPAWNS'):
        lua.globals().PlayWave(lua.globals().quest)
    assert list(lua.globals().events.values()) == [f'{prefix}_Creatures_{group}_CreatureType']
    assert list(lua.globals().counters.values()) == [f'{prefix}_Creatures_{group}_HUDType']
    assert list(lua.globals().updates.values()) == [101]
    assert [state[f'TotalCreatures_{i}'] for i in range(3)] == [int(i == group) for i in range(3)]
    assert list(lua.globals().dialogue.values()) == ([] if short_wave else
        ['TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_'+part for part in ('THREE','TWO','ONE','GO')])
    assert lua.globals().frames == (0 if short_wave else 6)
