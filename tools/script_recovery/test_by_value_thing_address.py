"""Address-source by-value Thing copies retain actor and following scalar operands."""
import json
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
HERE=Path(__file__).resolve().parent
if HERE.parent.name=='work':
 import sys
 sys.path.insert(0,str(HERE))
 from candidate_bootstrap import install
 install()

def source(actor='&localThing',cast='',target='stack0xffffffc4',klass='CScriptThing'):
 return f'''void Main() {{
  CScriptThing_bv copied;
  float range;
  range = 10.0;
  {klass}::CScriptThing((CScriptThing *)&{target},{cast}{actor},staleRegister);
  copied._4_4_ = oldData;
  copied._0_4_ = oldVtable;
  copied._8_4_ = oldInfo;
  GSI->SetCombatNearbyBreakOffRange(copied,range);
}}
'''

@pytest.mark.parametrize('actor',['localThing','&localThing'])
@pytest.mark.parametrize('cast',['','(CScriptThing *)'])
@pytest.mark.parametrize('slot',['stack0xffffffc4','xStack_30'])
def test_copy_source_and_following_argument(actor,cast,slot):
 from tools.script_recovery.native_evidence_lowering import fold_by_value_things
 fixed=fold_by_value_things(source(actor,cast,slot))
 assert f'GSI->SetCombatNearbyBreakOffRange({actor},range);' in fixed
 assert 'range = 10.0;' in fixed and 'copied._' not in fixed and 'CScriptThing::CScriptThing(' not in fixed
 if actor.startswith('&'):assert 'CScriptThing_bv &' not in fixed and 'CScriptThing_bv copied;' not in fixed

@pytest.mark.parametrize('actor,slot,klass',[('&localThing + 4','stack0xffffffc4','CScriptThing'),('&localThing[index]','stack0xffffffc4','CScriptThing'),('&localThing','ordinaryObject','CScriptThing'),('&localThing','stack0xffffffc4','OtherClass')])
def test_unsupported_copy_forms_remain_explicit(actor,slot,klass):
 from tools.script_recovery.native_evidence_lowering import fold_by_value_things
 text=source(actor,target=slot,klass=klass);assert fold_by_value_things(text)==text

@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 unit=json.loads((ROOT/'refs/script_recovery/trader_conflict/units/Q_TraderConflictEvil.json').read_text());unit['entities']={'TC_GuardSpawnPoint':unit['entities']['TC_GuardSpawnPoint']};unit['quest']['functions']={};unit['quest']['helpers']={}
 c=UnitConverter(ROOT/'refs/script_recovery/trader_conflict/translation_unit_typed.json');out=tmp_path_factory.mktemp('guard_range')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 draft=(out/'FSE/TraderConflictEvil/Entities/TC_GuardSpawnPoint.lua').read_text(encoding='utf-8')
 assert '-- TODO(native):' not in draft
 return dict(draft=draft,readable=readable_file(draft,rel='FSE/TraderConflictEvil/Entities/TC_GuardSpawnPoint.lua')[0])

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('initial',[0,13,24])
@pytest.mark.parametrize('sheriff',[False,True])
@pytest.mark.parametrize('wait',[0,2])
def test_complete_guard_spawn_main(generated,style,initial,sheriff,wait):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(generated[style])
 q,me,done=lua.execute(HARNESS,lua.table_from(dict(initial=initial,sheriff=sheriff,wait=wait,visible=False,limit=32)))
 lua.globals().Main(q,me);spawned,frames=done();assert spawned==25-initial

HARNESS="\nlocal s=...;local q,me,hero={},{},{};local frames,spawned,created=0,0,0;local actor;local calls={};local position={x=2,y=3,z=4}\nfunction q:IsActiveThreadTerminating()return frames>=s.limit end\nfunction q:NewScriptFrame(a)assert(a==me);frames=frames+1;assert(frames<40);return not self:IsActiveThreadTerminating()end\nfunction q:GetStateBool(key)assert(key=='QuestStartScreened');return frames>=s.wait end\nfunction q:GetStateInt(key)if key=='InitialNumberInRegion' then return s.initial elseif key=='NumberSpawned' then return spawned else assert(key=='NextTimeToSpawnGuards');return 999 end end\nfunction q:GetStateListCount(key)assert(key=='AllCreatures');return 6 end\nfunction q:SetStateInt(key,value)if key=='NumberSpawned' then assert(value==spawned+1 and calls.range and calls.faction and calls.marker and calls.target);spawned=value else assert(key=='NextTimeToSpawnGuards')end end\nfunction me:GetPos()return position end\nfunction q:IsCameraPosOnScreen(p)assert(p==position);return s.visible end\nfunction q:ReadGlobalGameData(offset)if offset==3976 then return 14 elseif offset==3980 then return 11 else assert(offset==3984);return 5 end end\nfunction q:RetailRandModulo(modulus)assert(modulus==32768);return s.sheriff and 0 or 1 end\nfunction q:CreateCreature(def,p,name,flag)\n local remaining=25-s.initial-spawned;local expected=s.sheriff and 'CREATURE_BS_SHERIFF' or remaining>14 and 'CREATURE_BS_GUARD_BLUE' or remaining>11 and 'CREATURE_BS_GUARD_BLUE_CROSSBOW' or remaining>5 and 'CREATURE_BS_GUARD_RED' or 'CREATURE_BS_GUARD_BLACK'\n assert(def==expected and p==position and name=='IsAGuard' and not flag);created=created+1;actor={};calls={};return actor\nend\nfunction q:SetCombatNearbyBreakOffRange(a,range)assert(a==actor);assert(range==10,'WRONG_BREAKOFF_RANGE:'..tostring(range));calls.range=true end\nfunction q:EntitySetInFaction(a,name)assert(a==actor and name=='FACTION_MONSTERS');calls.faction=true end\nfunction q:MiniMapAddMarker(a,name)assert(a==actor and name=='HUD_ORB_RED_SMALL');calls.marker=true end\nfunction q:GetHero()return hero end\nfunction q:GiveThingBestEnemyTarget(a,target)assert(a==actor and target==hero);calls.target=true end\nreturn q,me,function()assert(created==spawned and spawned<=25-s.initial);return spawned,frames end\n"
