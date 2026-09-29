"""Retail-backed resource receivers; ambiguous native values stay unresolved."""
import json
import os
from pathlib import Path
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import unwrap_statements, _text_order_sites
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import resource_call_evidence, recover_member_resource_calls
from tools.script_recovery.test_entity_persistence import generated, witch_vm

ROOT = Path(__file__).resolve().parents[2]


def synthetic(code):
    class Image:
        def bytes_at(self, start, count):
            assert start == 0x1000
            return code[:count]
    fn = dict(address='0x1000', bodyEndExclusive=hex(0x1000 + len(code)))
    spec = SimpleNamespace(entity=True, parent_off=0x14, self_resources={}, parent_resources={0x58: 'Speaker'})
    return Image(), fn, spec


GOOD = bytes.fromhex('8bf1 8b4e14 83c158 8b01 8d542410 52 ff5030 c3')


def test_receiver_and_vtable_must_name_the_same_member():
    image, fn, spec = synthetic(GOOD)
    proof = resource_call_evidence(image, fn, spec)
    assert len(proof) == 1
    assert next(iter(proof.values()))['offset'] == 0x58


@pytest.mark.parametrize('code', [
    GOOD.replace(bytes.fromhex('83c158'), bytes.fromhex('83c15c')),
    GOOD.replace(bytes.fromhex('8b01'), bytes.fromhex('8b4104')),
    GOOD.replace(bytes.fromhex('ff5030'), bytes.fromhex('e800000000 ff5030')),
    GOOD.replace(bytes.fromhex('ff5030'), bytes.fromhex('8bf3 ff5030')),
    GOOD.replace(bytes.fromhex('ff5030'), bytes.fromhex('ffe7 ff5030')),
    GOOD.replace(bytes.fromhex('ff5030'), bytes.fromhex('b100 ff5030')),
    GOOD.replace(bytes.fromhex('ff5030'), bytes.fromhex('b000 ff5030')),
    GOOD.replace(bytes.fromhex('8b4e14'), bytes.fromhex('5e 8b4e14')),
    GOOD.replace(bytes.fromhex('ff5030'), bytes.fromhex('8907 ff5030')),
    bytes.fromhex('83c104') + GOOD,
    # Incoming branch to ADD bypasses the load of the parent pointer.
    bytes.fromhex('8bf1 eb03 8b4e14 83c158 8b01 8d542410 52 ff5030 c3'),
])
def test_unknown_or_bypassed_receiver_is_rejected(code):
    assert not resource_call_evidence(*synthetic(code))


@pytest.fixture(scope='module')
def witch():
    base = ROOT / 'refs/script_recovery/sick_child'
    unit = json.loads((base / 'units/V_SickChild.json').read_text())
    fn = next(f for f in json.loads((base / 'translation_unit_typed.json').read_text())['functions']
              if f['address'].lower() == '0x00ece7f0')
    spec = LoweringSpec(unit, 'Witch', entity=True, thing_slots={})
    text = unwrap_statements(fn['decompile'])
    return text, fn, _text_order_sites(text, fn), spec, RData()


def test_witch_native_receivers_and_exact_literal_speech_pushes(witch):
    text, fn, entries, spec, image = witch
    proof = resource_call_evidence(image, fn, spec)
    health = {0xecec8a, 0xeced4c, 0xecee0f, 0xecf082, 0xecf26c, 0xecf688}
    assert {a for a, p in proof.items() if p['slot'] == 0x30} == health
    for site, literal in [(0xecece3, 'TEXT_QST_B10_WITCH_NEED_MORE_CURE_10'),
                          (0xeceda5, 'TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10'),
                          (0xecee64, 'TEXT_QST_B10_WITCH_WAITING_FOR_CURE')]:
        values = proof[site]['pushes']
        assert proof[site]['family'] == 'parent' and proof[site]['offset'] == 0x58
        assert values[0][0] == 'hero'
        assert image.string_at(values[1][1]) == literal
        assert values[2:] == [('imm', 0), ('imm', 0), ('imm', 1), ('imm', 0)]
    out = recover_member_resource_calls(*witch)
    assert out.count('RESOURCE_ScriptThing(RESOURCE_MemberResource("seh_Witch"))') == 6
    assert out.count('::_Speak_CScriptGameResourceObjectScriptedThingBase(') == 3
    assert '(CScriptThing_bv *)\n                          RESOURCE_ScriptThing' not in out


def test_missing_export_pairing_or_layout_does_not_guess(witch):
    text, fn, entries, spec, image = witch
    assert recover_member_resource_calls(text, fn, [], spec, image) == text
    empty = SimpleNamespace(entity=True, parent_off=0x14, self_resources={}, parent_resources={})
    assert recover_member_resource_calls(text, fn, entries, empty, image) == text


def test_teacher_entity_resource_retains_its_owner():
    base = ROOT / 'refs/script_recovery/book_collecting'
    unit = json.loads((base / 'units/V_BookCollecting.json').read_text())
    fn = next(f for f in json.loads((base / 'translation_unit_typed.json').read_text())['functions']
              if f['address'].lower() == '0x00e57530')
    spec = LoweringSpec(unit, 'BS_Teacher', entity=True, thing_slots={})
    text = unwrap_statements(fn['decompile'])
    image = RData()
    proof = resource_call_evidence(image, fn, spec)
    assert set(proof) == {0xe576ab, 0xe57769, 0xe5787a, 0xe57956}
    assert all(p['family']=='self' and p['offset']==0x34 and p['slot']==0x30 for p in proof.values())
    out = recover_member_resource_calls(text, fn, _text_order_sites(text, fn), spec, image)
    assert out.count('RESOURCE_ScriptThing(RESOURCE_MemberResource("seh_me", (CScriptThing *)(this + 8)))') == 4


def test_mother_resource_calls_and_literal_speech():
    base = ROOT / 'refs/script_recovery/sick_child'
    unit = json.loads((base / 'units/V_SickChild.json').read_text())
    fn = next(f for f in json.loads((base / 'translation_unit_typed.json').read_text())['functions']
              if f['address'].lower() == '0x00ecd9e0')
    spec = LoweringSpec(unit, 'SickChildsMother', entity=True, thing_slots={})
    image = RData()
    proof = resource_call_evidence(image, fn, spec)
    assert {a for a,p in proof.items() if p['slot']==0x30} == {0xecdef5, 0xecdfd6, 0xece1d5}
    for site,key,selection in [(0xecdf4e,'TEXT_QST_B10_MOTHER_ASK_CURE',2),
                               (0xece22a,'TEXT_QST_B10_MOTHER_HIT',0)]:
        p=proof[site]
        assert p['family']=='parent' and p['offset']==0x48
        assert p['pushes'][0][0]=='hero' and image.string_at(p['pushes'][1][1])==key
        assert p['pushes'][2:]==[('imm',selection),('imm',0),('imm',1),('imm',0)]


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('health', [10, 0, -1])
def test_teacher_generated_health_uses_entity_owned_resource(stage, health):
    base=Path(os.environ.get('MEMBER_RESOURCE_BOOK_TEST_ROOT', ROOT/'refs/script_recovery/lifted/BookCollecting'))
    lua=LuaRuntime()
    lua.execute("package.preload['V_BookCollecting.native_quest_helpers']=function() return {} end")
    lua.execute((base/stage/'FSE/V_BookCollecting/Entities/BS_Teacher.lua').read_text())
    lua.globals().health=health
    lua.execute('''
        me,hero,quest = {},{},{}
        local member,resources,checked,spoken={},{},0,0
        function quest:ReadGlobalGameData() return 20 end
        function quest:ReadGlobalGameDataFloat() return 0 end
        function quest:GetStateInt(key) assert(key=='BooksInGame'); return 0 end
        function quest:RetailResources() return resources end
        function resources:MemberResource(key,owner) assert(key=='seh_me' and owner==me); return member end
        function resources:ScriptThing(v) assert(v==member); return me end
        function quest:IsActiveThreadTerminating() return false end
        function quest:GetHealth(v) assert(v==me); checked=checked+1; return health end
        function quest:GetHero() return hero end
        function me:IsPerformingScriptTask() return false end
        function me:Speak(h,key,selection,a,b,c)
            assert(h==hero and key=='TEXT_QST_B16_BOOK_NOT_FOUND' and selection==2)
            assert(a==false and b==true and c==false); spoken=spoken+1; return true
        end
        Init(quest,me);
        (LookForBook or helper_E57530)(quest,me)
        assert(checked==1 and spoken==(health>0 and 1 or 0))
    ''')


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('health', [10, 0, -1])
def test_mother_declined_cure_checks_mother_health(generated, stage, health):
    lua=LuaRuntime()
    lua.execute("package.preload['V_SickChild.native_quest_helpers']=function() return {} end")
    lua.execute((generated/stage/'FSE/V_SickChild/Entities/SickChildsMother.lua').read_text())
    lua.globals().health=health
    lua.execute('''
        local boundary, member, movie = {}, {}, {}
        local checked, spoken, created, destroyed = 0, 0, 0, 0
        me,hero,quest = {},{},{}
        local resources = {}
        function quest:RetailResources() return resources end
        function resources:MemberResource(key) assert(key=='seh_Mother'); return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,m,p) assert(v==member and m==me and p==4); return true end
        function resources:ScriptThing(v) assert(v==member); return me end
        function resources:StartMovie() created=created+1; assert(created==1); return movie end
        function resources:DestroyMovie(v)
            assert(v==movie and not paused); destroyed=destroyed+1; error(boundary)
        end
        function quest:GetStateBool(key) assert(key=='MotherIntroDone'); return true end
        function quest:GetHero() return hero end
        function quest:NewScriptFrame(v) assert(v==me); return true end
        function quest:IsActiveThreadTerminating() return false end
        function quest:ReadGlobalGameDataString(off) assert(off==0x744); return 'POTION' end
        function quest:IsObjectInThingsPossession(key,h) assert(key=='POTION' and h==hero); return true end
        function quest:SetThingHasInformation(v) assert(v==me) end
        function quest:GiveHeroYesNoQuestion() end
        function quest:MsgIsQuestionAnsweredYesOrNo() return 0 end
        function quest:PauseAllNonScriptedEntities(v) paused=v end
        function quest:GetHealth(v) assert(v==me); checked=checked+1; return health end
        function me:IsTalkedToByHero() return true end
        function me:IsPerformingScriptTask() return false end
        function me:Speak(h,key,selection,a,b,c)
            assert(h==hero and key=='TEXT_QST_B10_MOTHER_ASK_CURE' and selection==2)
            assert(a==false and b==true and c==false); spoken=spoken+1; return true
        end
        local ok,err=pcall(Main,quest,me)
        assert(not ok and err==boundary, tostring(err))
        assert(checked==1 and spoken==(health>0 and 1 or 0) and created==1 and destroyed==1)
    ''')


def test_complete_speech_and_conflicting_getter_result_are_left_alone(witch):
    text, fn, entries, spec, image = witch
    site = next(e for e in entries if e[3].get('site') == '0x00ECECE3')
    a, b = site[:2]
    for replacement in ['pCVar11,key,0,0,1,0', 'unknown,key,0,0']:
        altered = text[:a] + replacement + text[b:]
        out = recover_member_resource_calls(altered, fn, _text_order_sites(altered, fn), spec, image)
        assert '(**(code **)(iVar10 + 0x34))(' + replacement + ')' in out


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('branch', ['accepted', 'declined', 'empty'])
@pytest.mark.parametrize('health,terminate', [(10, False), (10, True), (0, False), (-1, False)])
def test_witch_speech_health_and_movie_boundary(generated, stage, branch, health, terminate):
    """Exercise Main through its first movie cleanup, before unresolved on-hit flow."""
    lua = witch_vm((generated / stage / 'FSE/V_SickChild/Entities/Witch.lua').read_text())
    lua.globals().branch = branch
    lua.globals().health = health
    lua.globals().terminateSpeech = terminate
    lua.execute('''
        snapshot={IntroDone=true, ThingsForPotionGot=1}; loading=true
        OnPersist(quest,me,context); loading=false
        local inventory = branch=='empty' and 0 or 1
        local member, movie, boundary = {}, {}, {}
        local frames, healthCalls, spoken, destroyed, created, waited = 0, 0, 0, 0, 0, 0
        local resources = {}
        function resources:MemberResource(key) assert(key=='seh_Witch'); return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,m,p) assert(v==member and m==me and p==4); return true end
        function resources:ScriptThing(v) assert(v==member); return me end
        function resources:StartMovie() created=created+1; assert(created==1); return movie end
        function resources:DestroyMovie(v)
            assert(v==movie and not paused); destroyed=destroyed+1; assert(destroyed==1)
            if not terminateSpeech then error(boundary) end
        end
        function quest:RetailResources() return resources end
        function quest:GetHero() return hero end
        function quest:NewScriptFrame(v)
            assert(v==me); frames=frames+1; assert(frames<12)
            if spoken>0 then waited=waited+1; stop=terminateSpeech end
            return not stop
        end
        function quest:IsActiveThreadTerminating() return stop end
        function quest:ReadGlobalGameDataString(off) assert(off==0x738); return 'MUSHROOM' end
        function quest:IsObjectInThingsPossession(key,h)
            assert(key=='MUSHROOM' and h==hero); return inventory>0
        end
        function quest:TakeObjectFromHero(key) assert(key=='MUSHROOM' and inventory==1); inventory=0 end
        function quest:MsgIsQuestionAnsweredYesOrNo() return branch=='accepted' and 1 or 0 end
        function quest:PauseAllNonScriptedEntities(v) paused=v end
        function quest:GetHealth(v) assert(v==me); healthCalls=healthCalls+1; return health end
        function me:IsTalkedToByHero() return true end
        function me:IsPerformingScriptTask() return terminateSpeech end
        function me:Speak(h,key,selection,a,b,c)
            assert(h==hero and selection==0 and a==false and b==true and c==false)
            local keys={accepted='TEXT_QST_B10_WITCH_NEED_MORE_CURE_10',
                declined='TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10',
                empty='TEXT_QST_B10_WITCH_WAITING_FOR_CURE'}
            assert(key==keys[branch]); spoken=spoken+1; return true
        end
        local ok,err=pcall(Main,quest,me)
        if terminateSpeech then assert(ok,err); assert(waited==1)
        else assert(not ok and err==boundary, tostring(err)) end
        assert(healthCalls==1 and spoken==(health>0 and 1 or 0))
        assert(created==1 and destroyed==1 and not paused and #macros==0)
        assert(inventory==(branch=='declined' and 1 or 0))
        saved={}; OnPersist(quest,me,context)
        assert(saved.IntroDone==true and saved.ThingsForPotionGot==(branch=='accepted' and 2 or 1))
    ''')
