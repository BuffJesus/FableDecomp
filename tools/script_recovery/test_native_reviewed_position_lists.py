from pathlib import Path
import json,copy,sys
import pytest
from lupa.lua54 import LuaRuntime
from unittest.mock import patch
from tools.script_recovery import native_reviewed_position_lists as adapter
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.build_readable_unit import readable_file

@pytest.fixture(scope='module', autouse=True)
def capture_registered_input(tmp_path_factory):
    global meta, row, source, generated_source
    from tools.script_recovery.convert_quest_unit import UnitConverter
    root=next(p for p in Path(__file__).resolve().parents if (p/'refs/script_recovery/chicken_kicking').is_dir())
    evidence=root/'refs/script_recovery/chicken_kicking'
    unit=json.loads((evidence/'units/V_ChickenKicking.json').read_text())
    entity=unit['entities']['KickedChicken'];entity['functions']={'Main':entity['functions']['Main']}
    unit['entities']={'KickedChicken':entity};unit['quest']['functions']={};unit['quest']['helpers']={}
    original=adapter.restore;captured=[]
    def capture(s,t,c,fn,r,rd,root=None):
        if int(fn['address'],16)==0xe64210:
            captured.append((s,dict(todo=list(t),calls=list(c),fn=fn),copy.deepcopy(r)))
        return original(s,t,c,fn,r,rd,root)
    converter=UnitConverter(evidence/'translation_unit_typed.json')
    with patch.object(adapter,'restore',capture),patch.object(converter,'constructor_defaults',return_value={}):
        out=tmp_path_factory.mktemp('marker_list')
        converter.convert(unit,out)
        generated_source=(out/'FSE/V_ChickenKicking/Entities/KickedChicken.lua').read_text()
    assert len(captured)==1
    source,meta,row=captured[0]
    assert row is not None

def restore(**kw):
 args=dict(source=source,todo=meta['todo'],calls=meta['calls'],function=meta['fn'],row=row,rdata=RData());args.update(kw);return adapter.restore(**args)
def test_expected_recovery():
 result,todo,calls=restore();assert __import__('hashlib').sha256(result.encode()).hexdigest()==row['recoveredSha256']
 assert len(todo)==len(meta['todo'])-sum(row['resolvedDiagnostics'].values())
 assert 'Vector_PushBack_ScriptThing' not in result
 assert 'piVar8 = xStack_3c + 3' not in result and 'pfVar7 = nil --[[unresolved native value]]' not in result
 assert 'markerPositions:GetPosPair(index - 1, r1)' in result and 'markerResources:CosTurnsLessThan' in result
 assert result.count('AppendScriptName(')==4 and calls[-len(row['calls']):]==row['calls']
def test_unreviewed_unchanged():assert restore(row=None)==(source,meta['todo'],meta['calls'])
@pytest.mark.parametrize('field',['sourceSha256','nativeSha256','intermediateSha256','recoveredSha256'])
def test_hash_guards(field):
 r=copy.deepcopy(row);r[field]='0'*64
 with pytest.raises(ValueError):restore(row=r)
@pytest.mark.parametrize('group',['anchors','dependencies','sidecar'])
def test_dependency_guards(group):
 r=copy.deepcopy(row);k=next(iter(r[group]))
 if group=='anchors':r[group][k]='cc'
 else:r[group][k]['sha256']='0'*64
 with pytest.raises(ValueError):restore(row=r)
def test_diagnostics_guard():
 with pytest.raises(ValueError):restore(todo=meta['todo'][1:])
@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('distance',[0,1,1.00001,2,2.00001,3,3.00001,4,4.00001,100,float('nan'),4+2**-22,3+2**-23])
@pytest.mark.parametrize('terminating',[False,True])
def test_scope_normal_and_termination(readable,distance,terminating):run_scope(readable,distance,terminating,0)
@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('failure',[1,2,3,4,5,6,7,8,9,10,11])
def test_scope_errors(readable,failure):run_scope(readable,2.5,False,failure)
def run_scope(readable,distance,terminating,failure):
 result=restore()[0];helper=result[:result.index('local function __native_arena_main(')]
 function=helper+'function Exercise(quest)\nif __native_named_marker_scope(quest,nil,nil,0,1,0,1,2,3,4) then return end\nquest.continued=true\nend'
 if readable:function=readable_file(function)[0]
 lua=LuaRuntime(unpack_returned_tuples=True);lua.globals().distance=distance;lua.globals().terminating=terminating;lua.globals().failure=failure
 lua.execute('''
 events={};pairIndices={};sectorWrites={};quest={};local appendCount=0;local sectorChecks=0
 function quest:WithRetailResources(callback)
  local resources={};local closed=false
  function resources:NormalizeVector3(value)if failure==8 then error('normalize failure')end return value end
  function resources:ArcCosTurns(...)if failure==9 then error('acos failure')end return 0 end
  function resources:RotateVector3(value,...)if failure==10 then error('rotate failure')end return value end
  function resources:CosTurnsLessThan(...)if failure==11 then error('cosine failure')end sectorChecks=sectorChecks+1;return sectorChecks~=2 end
  function resources:DistanceFromThingToPosition(...)if failure==5 then error('distance failure')end return distance end
  function resources:NewThingPositionList()
   table.insert(events,'new');local list={}
   function list:Count()if failure==7 then error('count failure')end return appendCount end
   function list:GetPosPair(index,...)if failure==6 then error('pair failure')end pairIndices[#pairIndices+1]=index;return {first={x=1,y=0,z=0},second={x=0,y=0,z=0}}end
   function list:AppendScriptName(name)
    appendCount=appendCount+1;table.insert(events,name)
    if failure==appendCount then error('append failure')end
   end
   return list
  end
  local ok,value=pcall(callback,resources);table.insert(events,'close')
  if not ok then error(value)end;return value
 end
 function quest:SetStateInt(key,value)self[key]=value;if key=='FinalSector' then sectorWrites[#sectorWrites+1]=value end end
 function quest:IsActiveThreadTerminating()return terminating end
 ''')
 lua.execute(function);ok=lua.eval('function()return pcall(Exercise,quest)end')();ok=ok[0] if isinstance(ok,tuple) else ok
 assert ok==(failure==0)
 event=list(lua.globals().events.values());names=['FirstAngleMarker','SecondAngleMarker','ThirdAngleMarker','FourthAngleMarker']
 assert event==['new']+names[:failure if 1<=failure<=4 else 4]+['close']
 if failure==0:
  assert list(lua.globals().pairIndices.values())==[0,1,1,2,2,3]
  assert list(lua.globals().sectorWrites.values())==[0,1,3]
 q=lua.globals().quest
 assert bool(q.continued)==(not terminating and failure==0)
 if failure==0 and not terminating:
  import struct
  stored=struct.unpack('<f',struct.pack('<f',distance))[0]
  assert q.DistanceBand==(4 if distance>4 else 3 if stored>3 else 2 if stored>2 else 1 if stored>1 else 0)
 else:assert q.DistanceBand is None

@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('empty_arena',[False,True])
@pytest.mark.parametrize('failure',[0,1,2,3])
def test_arena_retirement_before_main_returns(readable,empty_arena,failure):
 global generated_readable
 if readable and 'generated_readable' not in globals():generated_readable=readable_file(generated_source)[0]
 text=generated_readable if readable else generated_source
 lua=LuaRuntime(unpack_returned_tuples=True)
 lua.globals().emptyArena=empty_arena;lua.globals().failure=failure
 lua.execute('''
 quest={};me={};arena={retired=false};events={};local tracked=false;local markerLive=false;local markerRetirements=0
 local marker={};function marker:GetPos()return {x=1,y=2,z=3}end
 function quest:GetThingWithScriptName(name)
  if name=='ChickenKickingArena' then if emptyArena then return nil end return arena end
  return marker
 end
 local resources={}
 function resources:DistanceBetweenThings(a,b)
  assert(not arena.retired)
  if failure==1 then error('distance failure')end
  return 1
 end
 function quest:RetailResources()return resources end
 function quest:SetStateInt(...)end
 function quest:NewScriptFrame(...)
  assert(tracked and not arena.retired and not markerLive and markerRetirements==10)
  events[#events+1]='frame'
  if failure==2 then error('frame failure')end
  return false
 end
 function quest:IsActiveThreadTerminating()return true end
 function quest:WithRetailResources(callback)
  local scope={}
  function scope:TrackThingOutput(value)
   if tracked then assert(value==marker and not markerLive);markerLive=true;return 2 end
   if emptyArena then assert(value==nil)else assert(value==arena)end
   tracked=true;events[#events+1]='track'
   if failure==3 then error('tracking failure after claim')end
   return 1
  end
  function scope:RetireThingOutput(token)assert(token==2 and markerLive);markerLive=false;markerRetirements=markerRetirements+1 end
  function scope:GetThingPosition(value)assert(value==marker and markerLive);return marker:GetPos()end

  local ok,result=pcall(callback,scope)
  assert(tracked);arena.retired=not emptyArena;events[#events+1]='close'
  if not ok then error(result)end
  return result
 end
 ''')
 lua.execute("debug.sethook(function()error('bounded Main instruction limit')end,'',100000)")
 lua.execute(text);result=lua.eval('function()return pcall(Main,quest,me)end')()
 ok=result[0] if isinstance(result,tuple) else result
 assert ok==(failure==0),result
 assert lua.globals().arena.retired==(not empty_arena)
 assert list(lua.globals().events.values())==['track']+(['frame'] if failure in (0,2) else [])+['close']

@pytest.mark.parametrize('readable',[False,True])
@pytest.mark.parametrize('missing',['none','line','a','b','both','nan_a','nan_b','nan_hero'])
@pytest.mark.parametrize('position_failure',[0,1,2])
def test_marker_retirement_and_copied_foul_positions(readable,missing,position_failure):
 global generated_readable
 if readable and 'generated_readable' not in globals():generated_readable=readable_file(generated_source)[0]
 text=generated_readable if readable else generated_source
 lua=LuaRuntime(unpack_returned_tuples=True)
 lua.globals().missing=missing;lua.globals().positionFailure=position_failure
 lua.execute('''
 quest={state={}};me={};retiredNames={};frames=0;positionQueries=0
 local handles={};local markerLive=nil;local fallback={x=5,y=-2,z=3}
 local a=(missing=='a' or missing=='both')and fallback or {x=1,y=0,z=0}
 local b=(missing=='b' or missing=='both')and fallback or {x=0,y=1,z=0}
 if missing=='nan_a'then a.x=0/0 end
 if missing=='nan_b'then b.y=0/0 end
 local heroPosition={x=(a.x+b.x)/2-(b.y-a.y),y=(a.y+b.y)/2+(b.x-a.x),z=0}
 if missing=='nan_hero'then heroPosition.x=0/0 end
 local hero={};function hero:GetPos()return heroPosition end
 local arena={name='ChickenKickingArena'}
 function quest:GetThingWithScriptName(name)
  assert(not markerLive)
  if name=='ChickenKickingArena'then return arena end
  if name=='FirstLineMarker'and missing=='line'then return nil end
  if name=='FoulLineMarkerA'and(missing=='a'or missing=='both')then return nil end
  if name=='FoulLineMarkerB'and(missing=='b'or missing=='both')then return nil end
  return {name=name,position=name=='FoulLineMarkerA'and{x=a.x,y=a.y,z=a.z}or{x=b.x,y=b.y,z=b.z}}
 end
 local resources={}
 function resources:DistanceBetweenThings(origin,marker)
  assert(origin==arena and not origin.retired and (marker==nil or markerLive==marker));return 1
 end
 function resources:MemberResource(...)return 1 end
 function resources:PrepareResource(...)assert(not arena.retired and not markerLive)end
 function resources:TryAcquire(...)return true end
 function quest:RetailResources()return resources end
 function quest:GetHero()return hero end
 function quest:EntityGetShotStrikePos(...)return nil end
 function me:MsgIsKicked()return false end
 function quest:SetStateInt(key,value)self.state[key]=value end
 function quest:GetStateInt(key)return self.state[key]end
 function quest:SetStateBool(key,value)self.state[key]=value end
 function quest:NewScriptFrame(...)
  assert(not arena.retired and not markerLive);frames=frames+1;assert(frames<=2);return frames<2
 end
 function quest:IsActiveThreadTerminating()return frames>=2 end
 function quest:WithRetailResources(callback)
  local scope={}
  function scope:TrackThingOutput(value)
   if value==nil then return 0 end
   assert(not value.retired)
   handles[#handles+1]=value
   if value~=arena then assert(not markerLive);markerLive=value end
   return #handles
  end
  function scope:RetireThingOutput(token)
   if token==0 then return end
   local value=handles[token];assert(value and not value.retired)
   value.retired=true;retiredNames[#retiredNames+1]=value.name
   if value~=arena then assert(markerLive==value);markerLive=nil end
   if value.position then value.position.x=99;value.position.y=99;value.position.z=99 end
  end
  function scope:GetThingPosition(value)
   if value==hero then return heroPosition end
   positionQueries=positionQueries+1
   if positionQueries==positionFailure then error('position failure')end
   assert(value==nil or markerLive==value)
   return value and value.position or fallback
  end
  local ok,result=pcall(callback,scope)
  for token=#handles,1,-1 do if not handles[token].retired then scope:RetireThingOutput(token)end end
  if not ok then error(result)end
  return result
 end
 ''')
 lua.execute("debug.sethook(function()error('bounded marker instruction limit')end,'',100000)")
 lua.execute(text);result=lua.eval('function()return pcall(Main,quest,me)end')()
 ok=result[0] if isinstance(result,tuple) else result
 assert ok==(position_failure==0),result
 expected=['FirstLineMarker','SecondLineMarker','ThirdLineMarker','FourthLineMarker','RearRight','MidRight','MidRight2','FrontRight','FoulLineMarkerA','FoulLineMarkerB']
 if position_failure==1:expected=expected[:9]
 absent={'line':{'FirstLineMarker'},'a':{'FoulLineMarkerA'},'b':{'FoulLineMarkerB'},'both':{'FoulLineMarkerA','FoulLineMarkerB'}}.get(missing,set())
 assert list(lua.globals().retiredNames.values())==[name for name in expected if name not in absent]+['ChickenKickingArena']
 if position_failure==0:
  assert lua.globals().quest.state.DistanceBand==(0 if missing in ('both','nan_a','nan_b','nan_hero') else 0xffffffff)
  assert lua.globals().frames==(2 if missing in ('both','nan_a','nan_b','nan_hero') else 1)
