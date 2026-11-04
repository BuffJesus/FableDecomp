"""Preserve counted scan iteration when a forward jump enters past its header."""
from pathlib import Path
import sys,importlib.util
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
HERE=Path(__file__).resolve().parent
from tools.script_recovery import native_loop_reentry,lift_native_lua as lift
from tools.script_recovery.test_lift_native_lua import make
candidate=HERE/'candidate_native_loop_reentry.py'
if candidate.exists():
 spec=importlib.util.spec_from_file_location('candidate_counted_entry',candidate);mod=importlib.util.module_from_spec(spec);spec.loader.exec_module(mod)
else:mod=native_loop_reentry
SOURCE='{\niVar1 = 0;\nif (limit > 0) goto LAB_00123456;\ngoto LAB_00123457;\nreturn;\nwhile (iVar1 = iVar1 + 1, iVar1 != limit) {\nLAB_00123456:\nGSI->SetTimer(10,iVar1);\nif (iVar1 == selected) {\nGSI->SetTimer(11,iVar1);\nbreak;\n}\n}\nLAB_00123457:\nGSI->SetTimer(12,iVar1);\nreturn;\n}'
@pytest.mark.parametrize('limit',[0,1,2,5])
@pytest.mark.parametrize('selected',[-1,0,1,4,9])
def test_scan_visits_remaining_elements_and_break_exits_its_loop(monkeypatch,limit,selected):
 monkeypatch.setattr(native_loop_reentry,'flatten',mod.flatten)
 lifter=make();lifter.native_gotos=True;lifter.accessor_kinds=True
 body='\n'.join(lifter.lift('Main',SOURCE,parameters={'limit':'int','selected':'int'}))
 lua=LuaRuntime();lua.execute("debug.sethook(function()error('budget')end,'',10000)");events=[]
 q=lua.table_from({'SetTimer':lambda _,key,value:events.append((key,value))})
 lua.execute('return function(Quest,limit,selected)\n'+body+'\nend')(q,limit,selected)
 expected=[];end=limit
 for i in range(limit):
  expected.append((10,i))
  if i==selected:expected.append((11,i));end=i;break
 expected.append((12,end));assert events==expected,(body,events,expected)
 assert not lifter.todo,lifter.todo

@pytest.mark.parametrize('source',[
 SOURCE.replace('return;\nwhile','GSI->SetTimer(13,0);\nwhile',1),
 SOURCE.replace('iVar1 + 1','iVar1 + Step()'),
 SOURCE.replace('iVar1 != limit','GetCondition()'),
 SOURCE.replace('LAB_00123456:\n','GSI->SetTimer(13,0);\nLAB_00123456:\n'),
 SOURCE.replace('GSI->SetTimer(10,iVar1);','if (skip) continue;'),
 SOURCE.replace('while (iVar1','for (iVar1'),
 SOURCE.replace('if (limit > 0) goto LAB_00123456;',''),
 SOURCE+'}',
])
def test_unsupported_entry_shapes_unchanged(source):
 lines=lift.strip_declarations(source);assert mod._rotate_forward_counted_entry(lines)==lines

def test_result_is_idempotent_and_exposes_entry_label():
 lines=lift.strip_declarations(SOURCE);result=mod.flatten(lines)
 assert result!=lines and mod.flatten(result)==result
 stripped=[s.strip() for s in result]
 assert stripped.index('LAB_00123456:')<stripped.index('while (true) {')

@pytest.fixture(scope='module')
def generated_trader(tmp_path_factory):
 import json
 from unittest.mock import patch
 from tools.script_recovery.convert_quest_unit import UnitConverter
 from tools.script_recovery.build_readable_unit import readable_file
 c=UnitConverter(ROOT/'refs/script_recovery/trader_conflict/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/trader_conflict/units/Q_TraderConflictGood.json').read_text(encoding='utf-8'))
 out=tmp_path_factory.mktemp('trader_scan')
 with patch.object(native_loop_reentry,'flatten',mod.flatten),patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 text=(out/'FSE/TraderConflictGood/TraderConflictGood.lua').read_text(encoding='utf-8')
 return dict(draft=text,readable=readable_file(text,rel='FSE/TraderConflictGood/TraderConflictGood.lua')[0])

@pytest.mark.parametrize('style',['draft','readable'])
@pytest.mark.parametrize('count,selected',[(n,i) for n in [1,2,5] for i in range(1,n+1)])
def test_generated_trader_removes_selected_element_after_full_scan(generated_trader,style,count,selected):
 lua=LuaRuntime(unpack_returned_tuples=True);lua.execute(generated_trader[style])
 lua.execute("function UpdateLiveEnemies(q)end;debug.sethook(function()error('budget')end,'',20000)")
 q,done=lua.execute(HARNESS,lua.table_from(dict(count=count,selected=selected)))
 lua.execute("""return function(q)
 function q:WithRetailResources(callback)
  -- This fixture stops at selection/scan; ownership is tested separately.
  return callback({TrackThingOutput=function(_,t)return t and 1 or 0 end,
   RetireThingOutput=function()end,
   AssignThingOutput=function()error('following target must not be cleared')end})
 end
end""")(q)
 with pytest.raises(Exception,match='SCAN_FINISHED'):lua.globals().AttackPeople(q)
 remaining,compared=done()
 assert remaining==','.join(str(i) for i in range(1,count+1) if i!=selected)
 assert compared==','.join(str(i) for i in range(1,selected+1))

HARNESS="\nlocal s=...;local q,hero,trader={},{},{};local creatures={};local frames,selected,marker=0,0,0;local compared={}\nfor i=1,s.count do\n local actor={id=i};function actor:IsEqualTo(other)compared[#compared+1]=other.id;return self==other end\n creatures[i]=actor\nend\nlocal original={};for i,a in ipairs(creatures) do original[i]=a end\nfunction q:GetHero()return hero end\nfunction q:GetStateListCopy(name)assert(name=='AllCreatures');return creatures end\nfunction q:RegisterTimer()return 19 end\nfunction q:GetStateBool(name)assert(name=='EnteredNewRegion');return false end\nfunction q:GetStateInt(name)if name=='TradersReachedTeleporter' then return 0 end;assert(name=='TradersFollowing');return 1 end\nfunction q:NewScriptFrame()frames=frames+1;if frames==2 then error('SCAN_FINISHED')end;return true end\nfunction q:IsActiveThreadTerminating()return false end\nfunction q:GetNearestWithScriptName(actor,name)assert(name=='TraderToRescue');return trader end\nfunction q:IsEntityFollowingHero(actor)assert(actor==trader);return true end\nfunction q:IsDistanceBetweenThingsUnder(actor,other,distance)assert(distance==15.0);return actor.id==s.selected end\nfunction q:IsThingAwareOfOtherThingInAnyWay(actor,other)return false end\nfunction q:GiveThingBestEnemyTarget(actor,other)assert(actor.id==s.selected and other==trader);selected=selected+1 end\nfunction q:GetTimer(id)assert(id==19);return 1 end\nfunction q:MiniMapAddMarker(actor,key)assert(actor.id==s.selected and key=='HUD_ORB_RED_SMALL');marker=marker+1 end\nreturn q,function()\n assert(selected==1 and marker==1,'WRONG_TARGET_COUNT')\n local ids={};for _,a in ipairs(creatures)do ids[#ids+1]=a.id end\n return table.concat(ids,','),table.concat(compared,',')\nend\n"
