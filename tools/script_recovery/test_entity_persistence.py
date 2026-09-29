"""Entity callbacks must persist the same state used by Init and Main."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import lift_persist_evidence, persist_kinds_from_spec
from tools.script_recovery.native_evidence_lowering import LoweringSpec

ROOT = Path(__file__).resolve().parents[2]


@pytest.mark.parametrize('typed', [False, True])
def test_entity_member_wins_over_colliding_quest_offset(typed):
    unit = {'quest': {'fields': {'0x20': ['WrongQuestField', 'Bool']}},
            'entities': {'Actor': {'fields': {'0x20': ['Progress', 'Int']}}}}
    spec = LoweringSpec(unit, 'Actor', entity=True, thing_slots={})
    spec.call_labels = {'CPersistContext::Transfer<signed_char>': 0x410be0}
    source = 'CPersistContext::Transfer<signed_char>(ctx,"SaveKey",(void *)(this + 0x20));'
    lines, _, todo = lift_persist_evidence(source, unit, spec, {0x410be0: 'Int'} if typed else {})
    assert not todo
    assert '__native_entity_state:GetStateInt("Progress")' in '\n'.join(lines)
    assert '__native_entity_state:SetStateInt("Progress"' in '\n'.join(lines)
    assert 'quest:PersistTransferInt(context, "SaveKey"' in '\n'.join(lines)
    assert 'WrongQuestField' not in '\n'.join(lines)


def test_ambiguous_field_address_stays_diagnostic():
    unit = {'quest': {'fields': {}}, 'entities': {'Actor': {'fields': {'0x20': ['Progress', 'Int']}}}}
    spec = LoweringSpec(unit, 'Actor', entity=True, thing_slots={})
    spec.call_labels = {}
    source = 'CPersistContext::Transfer<int>__at410be0(ctx,"Progress",unknown);'
    lines, _, todo = lift_persist_evidence(source, unit, spec, {0x410be0: 'Int'})
    assert any('not a member' in item for item in todo)
    assert '__native_entity_state' not in '\n'.join(lines)


def test_entity_offset_does_not_pick_a_quest_container():
    unit = {'quest': {'fields': {}, 'unmappedFields': [dict(offset='0x20', name='WrongVector', type='vector<CCharString>')]},
            'entities': {'Actor': {'fields': {'0x20': ['Progress', 'Int']}}}}
    spec = LoweringSpec(unit, 'Actor', entity=True, thing_slots={})
    spec.call_labels = {}
    source = 'CPersistContext::Transfer<int>__at410be0(ctx,"Progress",(void *)(this + 0x20));'
    lines, _, todo = lift_persist_evidence(source, unit, spec, {0x410be0: 'Int'})
    assert not todo
    assert 'StringList' not in '\n'.join(lines)


def test_witch_retail_uses_bool_and_integer_transfers_on_entity_offsets():
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    image = RData()
    ops = [(i.address, i.mnemonic, i.op_str) for i in
           Cs(CS_ARCH_X86, CS_MODE_32).disasm(image.bytes_at(0xecd890, 70), 0xecd890)]
    assert [op for op in ops if op[1] == 'call'] == [(0xecd8ae, 'call', '0x4045c0'), (0xecd8cb, 'call', '0x410be0')]
    assert (0xecd89e, 'lea', 'ecx, [esi + 0x1c]') in ops
    assert (0xecd8b8, 'add', 'esi, 0x20') in ops
    assert (0xecd897, 'mov', 'esi, ecx') in ops
    assert (0xecd8c3, 'mov', 'dword ptr [esp + 0x1c], 0') in ops
    assert image.string_at(0x12c6978) == 'IntroDone'
    assert image.string_at(0x12f24d4) == 'ThingsForPotionGot'
    kinds = persist_kinds_from_spec(ROOT / 'refs/script_recovery/sick_child/translation_unit_typed.json')
    assert kinds[0x4045c0] == 'Bool' and kinds[0x410be0] == 'Int'


@pytest.fixture(scope='module')
def generated():
    explicit = os.environ.get('ENTITY_PERSIST_TEST_ARTIFACT')
    if explicit:
        yield Path(explicit).resolve()
        return
    with tempfile.TemporaryDirectory(prefix='entity_persist_', dir=ROOT / 'work') as directory:
        p = Path(directory)
        subprocess.run([sys.executable, '-m', 'tools.script_recovery.convert_quest_unit',
                        '--unit', 'sick_child', '--out', str(p / 'draft')], cwd=ROOT, check=True, capture_output=True)
        code = 'from pathlib import Path; import sys; from tools.script_recovery.build_readable_unit import build; p=Path(sys.argv[1]); build("sick_child",draft=p/"draft",out=p/"readable")'
        subprocess.run([sys.executable, '-c', code, str(p)], cwd=ROOT, check=True, capture_output=True)
        yield p


def witch_vm(source):
    lua = LuaRuntime()
    lua.execute('''
        stop, macros = false, {}
        package.preload['V_SickChild.native_quest_helpers'] = function()
            return {helper_ECE460=function(_,_,macro,arg) macros[#macros+1]={macro,arg}; stop=true end}
        end
    ''')
    lua.execute(source)
    lua.execute('''
        me, hero, context = {}, {}, {}
        saved, loading, snapshot = {}, false, {}
        quest = {}
        function quest:GetStateBool(key) assert(key=='MotherIntroDone'); return true end
        function quest:PersistTransferBool(ctx,key,value)
            assert(ctx==context and key=='IntroDone' and type(value)=='boolean')
            if loading then return snapshot[key] end
            saved[key]=value; return value
        end
        function quest:PersistTransferInt(ctx,key,value)
            assert(ctx==context and key=='ThingsForPotionGot' and math.type(value)=='integer')
            if loading then return snapshot[key] end
            saved[key]=value; return value
        end
        for _,name in ipairs({'SetIsPushableByHero','SetThingHasInformation','EntitySetAsKillable',
            'EntitySetAsToAddToComboMultiplierWhenHit','SetQuestCardObjective','ClearThingHasInformation',
            'GiveHeroYesNoQuestion'}) do quest[name]=function() end end
        Init(quest,me)
        OnPersist(quest,me,context)
        assert(saved.IntroDone==false and saved.ThingsForPotionGot==0)
    ''')
    return lua


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('progress', [-1, 0, 1, 2, 3, 4, 256])
@pytest.mark.parametrize('intro', [False, True])
def test_witch_round_trip_keeps_integer_progress_and_entity_isolation(generated, stage, progress, intro):
    source = (generated / stage / 'FSE/V_SickChild/Entities/Witch.lua').read_text()
    first, second = witch_vm(source), witch_vm(source)
    first.globals().snapshot = first.table_from({'IntroDone': intro, 'ThingsForPotionGot': progress})
    first.execute('loading=true; OnPersist(quest,me,context); loading=false; saved={}; OnPersist(quest,me,context)')
    assert dict(first.globals().saved) == {'IntroDone': intro, 'ThingsForPotionGot': progress}
    second.execute('saved={}; OnPersist(quest,me,context)')
    assert dict(second.globals().saved) == {'IntroDone': False, 'ThingsForPotionGot': 0}


@pytest.mark.parametrize('stage', ['draft', 'readable'])
def test_loaded_three_mushrooms_then_one_more_reaches_potion_and_saves_four(generated, stage):
    lua = witch_vm((generated / stage / 'FSE/V_SickChild/Entities/Witch.lua').read_text())
    lua.execute('''
        snapshot={IntroDone=true, ThingsForPotionGot=3}; loading=true
        OnPersist(quest,me,context); loading=false
        local inventory, frames, movieCount = 1, 0, 0
        local member, movies = {}, {}
        local resources = {}
        function resources:MemberResource() return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,m,p) assert(v==member and m==me and p==4); return true end
        function resources:StartMovie() local m={}; movies[m]=true; movieCount=movieCount+1; return m end
        function resources:DestroyMovie(m) assert(movies[m]); movies[m]=nil end
        function quest:RetailResources() return resources end
        function quest:GetHero() return hero end
        function quest:NewScriptFrame() frames=frames+1; assert(frames<20); return not stop end
        function quest:IsActiveThreadTerminating() return stop end
        function quest:ReadGlobalGameDataString(off)
            assert(off==0x738 or off==0x744); return off==0x738 and 'MUSHROOM' or 'POTION'
        end
        function quest:IsObjectInThingsPossession(key,h) assert(h==hero); return key=='MUSHROOM' and inventory>0 end
        function quest:TakeObjectFromHero(key) assert(key=='MUSHROOM' and inventory==1); inventory=0 end
        function quest:GetActiveQuestName() return 'V_SickChild' end
        function quest:MsgIsQuestionAnsweredYesOrNo() return 1 end
        function quest:PauseAllNonScriptedEntities(v) paused=v end
        function me:IsTalkedToByHero() return true end
        function me:MsgIsHitByHero() return false end
        function me:MsgIsHitByAnySpecialAbilityFromHero() return false end
        Main(quest,me)
        assert(#macros==1 and macros[1][1]=='CS_SICK_POTION' and macros[1][2]=='POTION')
        assert(inventory==0 and movieCount==1 and next(movies)==nil and not paused)
        saved={}; OnPersist(quest,me,context)
        assert(saved.IntroDone==true and saved.ThingsForPotionGot==4)
    ''')
