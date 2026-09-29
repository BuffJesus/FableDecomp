"""String-map cutscene helper: native operands, rejected guesses and Lua lifetime."""
import os
import json
import re
import struct
from pathlib import Path
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import RData, Lifter, load_manifest
from tools.script_recovery.native_evidence_lowering import finish_lua, fold_resource_objects
from tools.script_recovery.native_local_helper_operands import pushed_operands
from tools.script_recovery.native_string_map_lowering import (
    fold_string_buffers, fold_case_compare, fold_string_maps,
)

ROOT = Path(__file__).resolve().parents[2]
BUFFER = '''  if (value == (CCharString)0x0) {
    buffer = (int *)&DAT_0129aaf4;
  }
  else {
    buffer = *(int **)value;
  }
'''
MAP = '''  head = (undefined1 *)0x0;
  head = malloc(0x18);
  size = 0;
  *head = 0;
  *(undefined4 *)(head + 4) = 0;
  *(undefined1 **)(head + 8) = head;
  *(undefined1 **)(head + 0xc) = head;
  node = Index(&head,&key);
  if (size != 0) {
    Destroy(&head,*(int *)(head + 4));
    *(undefined1 **)(head + 8) = head;
    *(undefined4 *)(head + 4) = 0;
    *(undefined1 **)(head + 0xc) = head;
    size = 0;
  }
  if (head != (undefined1 *)0x0) {
    free(head);
  }
'''


@pytest.mark.parametrize('entry,site,expected', [
    (0xECD9E0, 0xECDBDB, ['"CS_SICK_INTRO"', '"NULL"']),
    (0xECD9E0, 0xECE311, ['"CS_SICK_OUTRO"', '"NULL"']),
    (0xECE7F0, 0xECE9B4, ['"CS_SICK_WITCH"', '"NULL"']),
    (0xECE7F0, 0xECF337, ['"CS_SICK_POTION"', 'ENGINE_GlobalGameDataString(0x744)']),
])
def test_native_caller_operands(entry, site, expected):
    assert pushed_operands(RData(), entry, site, 2, parent=True) == expected
    assert pushed_operands(RData(), entry, site, 2) is None


@pytest.mark.parametrize('mutation', ['receiver', 'length', 'branch'])
def test_native_argument_walk_rejects_changed_receiver_or_constructor(mutation):
    raw = bytes.fromhex('51 8bcc 6aff 6800200000')
    raw += b'\xe8' + struct.pack('<i', 0x99EBF0 - (0x1000 + len(raw) + 5))
    raw += bytes.fromhex('8b4e14')
    def image(code):
        return SimpleNamespace(bytes_at=lambda _a, n: code[:n], string_at=lambda _: 'NULL')
    assert pushed_operands(image(raw), 0x1000, 0x1000 + len(raw), 1, parent=True) == ['"NULL"']
    if mutation == 'receiver':
        raw += bytes.fromhex('31c9')
    elif mutation == 'length':
        raw = raw.replace(bytes.fromhex('6aff'), bytes.fromhex('6a00'))
    else:
        raw += bytes.fromhex('eb00')
    assert pushed_operands(image(raw), 0x1000, 0x1000 + len(raw), 1, parent=True) is None


def test_string_buffer_requires_empty_fallback_and_same_destination():
    assert fold_string_buffers(BUFFER, lambda _: 0) == '  buffer = ENGINE_StringValue(value);\n'
    assert fold_string_buffers(BUFFER, lambda _: 65) == BUFFER
    changed = BUFFER.replace('buffer = *(int **)value', 'other = *(int **)value')
    assert fold_string_buffers(changed, lambda _: 0) == changed


@pytest.mark.parametrize('value', [None, '', 'NULL', 'MiXeD'])
def test_string_buffer_preserves_value(value):
    lowered = '{\n' + fold_string_buffers(BUFFER, lambda _: 0) + 'return buffer;\n}'
    lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData())
    body = finish_lua('\n'.join(lifter.lift('Main', lowered, parameters={'value': 'string'})))
    assert LuaRuntime().execute('return function(value)\n' + body + '\nend')(value) == (value or '')


def test_case_compare_requires_proven_callee_and_zero_only_use():
    source = '  result = _stricmp(a,b);\n  if (result != 0) {\n  }\n'
    assert 'ENGINE_StrCaseCmp' in fold_case_compare(source, {'MSVCR71.DLL::_stricmp': 0xBFEBA8})
    assert fold_case_compare(source, {'_stricmp': 1}) == source
    for tail in ['return result;', 'other = result + 1;', 'if (result < 0) {}',
                 'if (result == 0.5) {}', 'if (result != 0 + 1) {}']:
        changed = source + tail
        assert fold_case_compare(changed, {'_stricmp': 0xBFEBA8}) == changed


def test_string_map_needs_matching_type_construction_and_destruction():
    labels = {'Index': 0x9AC700, 'Destroy': 0x9AA830}
    result = fold_string_maps(MAP, labels)
    assert 'head = STRINGMAP_New();' in result and 'STRINGMAP_Destroy(head);' in result
    assert 'malloc' not in result and 'free(' not in result
    for changed in [MAP.replace('0x18', '0x24'), MAP.replace('free(head)', 'free(other)'),
                    MAP.replace('(head + 8) = head', '(head + 8) = other', 1)]:
        assert fold_string_maps(changed, labels) == changed
    assert fold_string_maps(MAP, {'Index': 1, 'Destroy': 0x9AA830}) == MAP
    assert fold_string_maps(MAP, {'Index': 0x9AC700, 'Destroy': 1}) == MAP


@pytest.mark.parametrize('value', ['NULL', '50', '200'])
def test_map_alias_of_literal_keeps_string_value(value):
    source = '''{
  CCharString::CCharString((CCharString *)&slot,"VALUE",-1);
  alias = &slot;
  node = Index(&map,&key);
  CCharString::operator=((CCharString *)node,alias);
}'''.replace('VALUE', value)
    labels = {'Index': 0x9AC700}
    lowered = fold_resource_objects(fold_string_maps(source, labels), labels)
    lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData())
    body = finish_lua('\n'.join(lifter.lift('Main', lowered, parameters={'map': 'number', 'key': 'string'})))
    lua = LuaRuntime()
    lua.globals().expected = value
    lua.execute('resources = {SetString=function(_, m, k, v) assert(m==71 and k=="$KEY" and v==expected) end}')
    lua.execute('return function(map,key)\n' + body + '\nend')(71, '$KEY')


def test_char_literal_pointer_is_not_a_string_object_address():
    source = ('  alias = &DAT_012393e4;\n  node = Index(&map,&key);\n'
              '  CCharString::operator=((CCharString *)node,alias);\n')
    assert fold_string_maps(source, {'Index': 0x9AC700}) == source


def test_scratch_smoke_uses_its_native_parameter_report(tmp_path):
    from tools.script_recovery.smoke_run_unit import recorded_parameter_types
    report = tmp_path / 'report.json'
    row = {'path': 'FSE/V_SickChild/V_SickChild.lua', 'function': 'helper_ECE460',
           'address': '0x00ECE460', 'nativeSignature': {'parameters': [
               {'type': 'CCharString_bv'}, {'type': 'CCharString_bv'}]}}
    report.write_text(json.dumps({'units': [{'functions': [row]}]}))
    types = recorded_parameter_types('sick_child', report)
    assert types[('V_SickChild/V_SickChild.lua', 'helper_ECE460')] == ['string', 'string']


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('shared', [False, True])
@pytest.mark.parametrize('argument', ['NULL', 'null', 'NuLl', '', 'TEXT_POTION', None])
def test_generated_helper_preserves_arguments_and_resource_lifetime(stage, shared, argument):
    base = Path(os.environ.get('SICK_CHILD_TEST_ROOT', ROOT / 'refs/script_recovery/lifted/SickChild'))
    filename = 'native_quest_helpers.lua' if shared else 'V_SickChild.lua'
    lua = LuaRuntime()
    result = lua.execute((base / stage / 'FSE/V_SickChild' / filename).read_text())
    fn = result['helper_ECE460'] if shared else lua.globals().helper_ECE460
    lua.globals().helper = fn
    lua.globals().argument = argument
    lua.globals().shared = shared
    lua.execute('''
        local live, calls = {}, {}
        local function new(kind)
            local v = {kind=kind}; live[v] = true; return v
        end
        local strings, actors, heroResource
        local hero, witch, mother = {}, {}, {}
        local resources = {}
        function resources:NewStringMap() strings = new('strings'); return strings end
        function resources:NewActorMap() actors = new('actors'); return actors end
        function resources:NewResource() heroResource = new('resource'); return heroResource end
        function resources:TryAcquire(r, thing, priority)
            assert(r == heroResource and live[r] and thing == hero and priority == 4)
            return true
        end
        function resources:MemberResource(name)
            assert(name == 'seh_Witch' or name == 'seh_Mother')
            return name == 'seh_Witch' and witch or mother
        end
        function resources:SetActor(map, key, value)
            assert(map == actors and live[map]); map[key] = value
        end
        function resources:SetString(map, key, value)
            assert(map == strings and live[map] and key == '$ARG1')
            assert(value == (argument or '')); map[key] = value
        end
        function resources:RunMacroWithStrings(name, a, s, setup, skip)
            assert(name == 'CS_TEST' and a == actors and s == strings)
            assert(live[a] and live[s] and live[heroResource])
            assert(a.WITCH == witch and a.MUM == mother and a.HERO == heroResource)
            local expected = string.lower(argument or '') ~= 'null'
            assert((s['$ARG1'] ~= nil) == expected)
            assert(setup == false and skip == true)
            table.insert(calls, 'macro')
        end
        local function release(v, expected, label)
            assert(v == expected and live[v]); live[v] = nil; table.insert(calls, label)
        end
        function resources:DestroyActorMap(v) release(v, actors, 'actors') end
        function resources:ReleaseResource(v) release(v, heroResource, 'hero') end
        function resources:DestroyStringMap(v) release(v, strings, 'strings') end
        local quest = {RetailResources=function() return resources end, GetHero=function() return hero end}
        function quest:FixMovieSequenceCamera(fix) table.insert(calls, fix and 'camera:on' or 'camera:off') end
        if shared then helper(quest, {}, 'CS_TEST', argument) else helper(quest, 'CS_TEST', argument) end
        assert(next(live) == nil)
        assert(table.concat(calls, ',') == 'camera:on,macro,camera:off,actors,hero,strings')
    ''')


@pytest.mark.parametrize('stage', ['draft', 'readable'])
def test_generated_callers_pass_macro_and_constructed_argument(stage):
    base = Path(os.environ.get('SICK_CHILD_TEST_ROOT', ROOT / 'refs/script_recovery/lifted/SickChild'))
    expected = {'SickChildsMother': ['CS_SICK_INTRO', 'CS_SICK_OUTRO'],
                'Witch': ['CS_SICK_WITCH', 'CS_SICK_POTION']}
    for name, names in expected.items():
        source = (base / stage / 'FSE/V_SickChild/Entities' / (name + '.lua')).read_text()
        calls = [s.strip() for s in source.splitlines() if '.helper_ECE460(' in s]
        assert len(calls) == 2
        for line, macro in zip(calls, names):
            lua = LuaRuntime()
            lua.globals().expected_macro = macro
            prefix = ''
            if macro == 'CS_SICK_POTION':
                # Existing constructor result, not an extra data-table read.
                value = re.search(r',\s*(\w+)\)$', line)
                if value:
                    assignment = re.findall(r'^[ \t]*(?:local )?' + re.escape(value[1])
                                            + r' = quest:ReadGlobalGameDataString\(0x744\)[ \t]*$',
                                            source[:source.index(line)], re.M)
                    assert assignment
                    prefix = assignment[-1].strip() + '\n'
                lua.globals().expected_argument = 'TEXT_POTION'
            else:
                lua.globals().expected_argument = 'NULL'
            lua.execute('''
                quest, me = {}, {}
                reads = 0
                function quest:ReadGlobalGameDataString(offset)
                    assert(offset == 0x744); reads = reads + 1; return 'TEXT_POTION'
                end
                function require(name)
                    assert(name == 'V_SickChild.native_quest_helpers')
                    return {helper_ECE460=function(q,m,macro,arg)
                        assert(q==quest and m==me and macro==expected_macro and arg==expected_argument)
                    end}
                end
                helpers = require('V_SickChild.native_quest_helpers')
            ''')
            lua.execute(prefix + line)
            assert lua.globals().reads == (1 if macro == 'CS_SICK_POTION' else 0)


@pytest.mark.parametrize('stage', ['draft', 'readable'])
def test_generated_leaving_cutscene_owns_its_string_map(stage):
    base = Path(os.environ.get('SICK_CHILD_TEST_ROOT', ROOT / 'refs/script_recovery/lifted/SickChild'))
    lua = LuaRuntime()
    source = (base / stage / 'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
    lua.execute(source)
    name = re.search(r'function (helper_ED0B10|DoLeavingCutscene)\(', source)[1]
    lua.globals().helper = lua.globals()[name]
    lua.execute('''
        local live, order = {}, {}
        local resource, actors, strings
        local hero, woman, me = {}, {}, {}
        local function new() local value = {}; live[value] = true; return value end
        local resources = {}
        function resources:NewResource() resource = new(); return resource end
        function resources:TryAcquire(r, h, p) assert(r==resource and h==hero and p==4); return true end
        function resources:NewActorMap() actors=new(); return actors end
        function resources:NewStringMap() strings=new(); return strings end
        function resources:MemberResource(n,m) assert(n=='seh_me' and m==me); return woman end
        function resources:SetActor(a,k,v) assert(a==actors and live[a]); a[k]=v end
        function resources:SetString(s,k,v) assert(s==strings and live[s] and k=='$ITEM' and v=='ITEM_KEY'); s[k]=v end
        function resources:RunMacroWithStrings(name,a,s,setup,skip)
            assert(name=='CS_SICK_LAUGHING_WOMAN_LEAVES' and a==actors and s==strings)
            assert(a.HERO==resource and a.WOMAN==woman and s['$ITEM']=='ITEM_KEY')
            assert(live[a] and live[s] and live[resource] and not setup and skip)
            table.insert(order, 'macro')
        end
        local function release(v, expected, label)
            assert(v==expected and live[v]); live[v]=nil; table.insert(order,label)
        end
        function resources:DestroyStringMap(v) release(v,strings,'strings') end
        function resources:DestroyActorMap(v) release(v,actors,'actors') end
        function resources:ReleaseResource(v) release(v,resource,'resource') end
        local quest = {RetailResources=function() return resources end, GetHero=function() return hero end}
        function quest:ReadGlobalGameDataString(offset) assert(offset==0x738); return 'ITEM_KEY' end
        function quest:FixMovieSequenceCamera(v) table.insert(order,v and 'on' or 'off') end
        helper(quest,me)
        assert(next(live)==nil and table.concat(order,',')=='on,macro,off,strings,actors,resource')
    ''')


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('scenario', ['mother_intro', 'mother_outro', 'witch_intro', 'witch_potion'])
def test_generated_caller_branch_releases_its_own_movie(stage, scenario):
    base = Path(os.environ.get('SICK_CHILD_TEST_ROOT', ROOT / 'refs/script_recovery/lifted/SickChild'))
    name = 'SickChildsMother' if scenario.startswith('mother') else 'Witch'
    lua = LuaRuntime()
    lua.globals().scenario = scenario
    lua.execute('''
        stop, macros = false, {}
        function require(name)
            assert(name=='V_SickChild.native_quest_helpers')
            return {helper_ECE460=function(_,_,macro,arg)
                table.insert(macros,{macro,arg}); stop=true
            end}
        end
    ''')
    lua.execute((base / stage / 'FSE/V_SickChild/Entities' / (name + '.lua')).read_text())
    lua.execute('''
        local movies, count, paused, frames = {}, 0, false, 0
        local inventory = scenario=='witch_potion' and 1 or 0
        local hero, me, member = {}, {}, {}
        local state = {MotherIntroDone=scenario~='mother_intro'}
        local resources = {}
        function resources:MemberResource() return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,m,p) assert(v==member and m==me and p==4); return true end
        function resources:StartMovie(name)
            assert(name==''); local v={}; movies[v]=true; count=count+1; return v
        end
        function resources:DestroyMovie(v)
            assert(movies[v], 'destroyed wrong movie'); movies[v]=nil
        end
        local quest = {RetailResources=function() return resources end, GetHero=function() return hero end}
        function quest:NewScriptFrame()
            frames=frames+1; assert(frames<20,'fixture failed to reach requested branch'); return not stop
        end
        function quest:IsActiveThreadTerminating() return stop end
        function quest:GetStateBool(k) return state[k] or false end
        function quest:SetStateBool(k,v) state[k]=v end
        function quest:ReadGlobalGameDataString(off)
            assert(off==0x744 or off==0x738); return off==0x744 and 'POTION' or 'MUSHROOM'
        end
        function quest:IsObjectInThingsPossession(key,h)
            assert(h==hero); return key=='POTION' and scenario=='mother_outro' or key=='MUSHROOM' and inventory>0
        end
        function quest:TakeObjectFromHero(key)
            if key=='MUSHROOM' then assert(inventory>0); inventory=inventory-1 else assert(key=='POTION') end
        end
        function quest:IsDistanceBetweenThingsUnder() return true end
        function quest:PauseAllNonScriptedEntities(v)
            assert(type(v)=='boolean'); paused=v
        end
        function quest:GetActiveQuestName() return 'V_SickChild' end
        function quest:MsgIsQuestionAnsweredYesOrNo() return 1 end
        for _,name in ipairs({'SetIsPushableByHero','SetThingHasInformation','EntitySetAsKillable',
            'EntitySetAsToAddToComboMultiplierWhenHit','GiveHeroQuestCardDirectly','SetQuestCardObjective',
            'ClearThingHasInformation','GiveHeroYesNoQuestion'}) do quest[name]=function() end end
        function me:IsTalkedToByHero() return scenario~='mother_intro' end
        function me:MsgIsHitByHero() return false end
        function me:MsgIsHitByAnySpecialAbilityFromHero() return false end
        Init(quest,me)
        if scenario=='witch_potion' then
            -- Seed the pre-existing progress for this branch fixture. This is
            -- not a claim that the separate persistence callback is recovered.
            for i=1,30 do
                local name,value=debug.getupvalue(Main,i); if not name then break end
                if name=='introDone' then debug.setupvalue(Main,i,true) end
                if name=='thingsForPotionGot_' then debug.setupvalue(Main,i,3) end
                if name=='__native_entity_state' then
                    value:SetStateBool('IntroDone',true); value:SetStateInt('ThingsForPotionGot',3)
                end
            end
        end
        Main(quest,me)
        local expected={mother_intro='CS_SICK_INTRO',mother_outro='CS_SICK_OUTRO',
                        witch_intro='CS_SICK_WITCH',witch_potion='CS_SICK_POTION'}
        assert(#macros==1 and macros[1][1]==expected[scenario])
        assert(macros[1][2]==(scenario=='witch_potion' and 'POTION' or 'NULL'))
        assert(count==1 and next(movies)==nil and not paused)
    ''')
