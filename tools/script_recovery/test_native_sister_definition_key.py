import json,os,sys,copy
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
@pytest.fixture(scope='module')
def generated(tmp_path_factory):
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text());unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('sister_key')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/SickChildsSister.lua').read_text();return source,readable_file(source)[0]
H=r'''
return function(count,rng,limit,taskTicks)
 local frame,draws,reads,waves=0,0,0,0;local timers,resources={},{};local q,me,hero,r={},{},{},{}
 local taskEnd=0
 local function term()return frame>=limit end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;for id,v in pairs(timers)do timers[id]=math.max(0,v-1)end;return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:RetailResources()return r end
 function q:GetStateBool(name)assert(name=='MotherIntroDone');return false end
 function q:GetHero()return hero end
 function me:FollowThing(actor,speed,flag)assert(actor==hero and speed==1 and flag)end
 function me:StopFollowingThing(actor)assert(actor==hero)end
 function me:IsTalkedToByHero()return false end
 function me:MsgIsHitByHero()return false end
 function me:MsgIsHitByAnySpecialAbilityFromHero()return false end
 function me:IsPerformingScriptTask()return frame<taskEnd end
 function q:RegisterTimer()assert(next(timers)==nil);timers[713]=0;return 713 end
 function q:GetTimer(id)assert(timers[id]~=nil);return timers[id]end
 function q:SetTimer(id,value)assert(timers[id]~=nil and value==10);timers[id]=value end
 function q:DeregisterTimer(id)assert(timers[id]~=nil);timers[id]=nil end
 function q:AddNewConversation(actor,a,b)assert(actor==me and not a and not b);return 719 end
 function q:AddPersonToConversation(id,actor)assert(id==719 and actor==hero)end
 function q:AddLineToConversation(id,key,actor,target,flag)assert(id==719 and key=='TEXT_QST_B10_SISTER_CALLS_OVER_10' and actor==me and target==hero and not flag)end
 function q:ReadGlobalGameData(off)assert(off==0x74c or off==0x750);return off==0x74c and 1000 or 1000+count*4 end
 function q:RetailRandModulo(n)assert(n==32768);draws=draws+1;return (rng+draws-1)%32768 end
 function q:ReadGlobalGameDataDefStringAt(off,index)
  assert(off==0x74c and index==((rng+draws-1)%32768)%count);reads=reads+1;return 'ANIMATION_TOKEN_'..index
 end
 function r:NewResource()resources[811]=true;return 811 end
 function r:PrepareResource(id)assert(resources[id])end
 function r:TryAcquire(id,actor,kind)assert(resources[id] and actor==me and kind==4);return true end
 function r:ReleaseResource(id)assert(resources[id]);resources[id]=nil end
 function r:PlayAnimationWithNativeArgument5(id,key,a,b,c,d,f,g)
  assert(resources[id] and key=='ANIMATION_TOKEN_'..(((rng+draws-1)%32768)%count),'missing or wrong native animation key')
  assert(not a and not b and not c and d and not f and not g);waves=waves+1;taskEnd=frame+taskTicks
 end
 debug.sethook(function()error('instruction budget')end,'',50000);Main(q,me);debug.sethook()
 assert(next(resources)==nil and next(timers)==nil,'resource/timer leak');assert(draws==reads and reads==waves,'draw/conversion count changed')
 assert(limit<3 or waves>0,'animation branch missed');return waves
end
'''
@pytest.mark.parametrize('style',[0,1])
@pytest.mark.parametrize('count',[1,3,7])
@pytest.mark.parametrize('rng',[0,1,32767])
@pytest.mark.parametrize('limit',[1,3,15])
@pytest.mark.parametrize('taskTicks',[0,2])
def test_actual_key(generated,style,count,rng,limit,taskTicks):
 lua=LuaRuntime();lua.execute(generated[style]);lua.execute(H)(count,rng,limit,taskTicks)

@pytest.fixture(scope='module')
def native_review(tmp_path_factory):
 from tools.script_recovery.lift_native_lua import Lifter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedDefinitionVectorConversions']['0x00ec68a0']
 unit.pop('reviewedDefinitionVectorConversions')
 # Capture the pre-key source without a downstream review pinned to the keyed source.
 unit.pop('reviewedCleanupReturns',None)
 unit['entities']={'SickChildsSister':unit['entities']['SickChildsSister']}
 unit['quest']['functions']={};unit['quest']['helpers']={}
 captured={};original=Lifter.lift
 def capture(self,name,source,*args,**kwargs):
  fn=kwargs.get('native_function',{})
  if str(fn.get('address','')).lower()=='0x00ec68a0':captured.update(fn=fn,source=source)
  return original(self,name,source,*args,**kwargs)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(Lifter,'lift',capture):
  c.convert(unit,tmp_path_factory.mktemp('sister_native_review'))
 assert captured
 return captured['fn'],captured['source'],row,c.rdata

@pytest.mark.parametrize('fault',['no_review','native','export','helper','missing','duplicate','offset','output','empty','range'])
def test_review_guards(fault,native_review):
 from tools.script_recovery.native_reviewed_definition_vectors import restore
 from tools.script_recovery.lift_native_lua import RData
 fn,source,row,rd=native_review;fn=copy.deepcopy(fn);row=copy.deepcopy(row)
 if fault=='no_review':assert restore(source,fn,None,rd)==source;return
 if fault in ('native','helper'):
  original=rd
  class Changed:
   def bytes_at(self,address,size):return b'\x90'*size if address==(0x415d70 if fault=='helper' else 0xec68a0) else original.bytes_at(address,size)
  rd=Changed()
 elif fault=='export':fn['decompile']+='\n'
 elif fault=='missing':source=source.replace(row['sites'][0]['sourceAnchor'],'')
 elif fault=='duplicate':source+='\n'+row['sites'][0]['sourceAnchor']
 elif fault=='offset':row['sites'][0]['offset']+=4
 elif fault=='output':row['sites'][0]['output']='wrong'
 elif fault=='empty':row['sites']=[]
 elif fault=='range':row['sites'][0]['start']='0x00000000'
 with pytest.raises(ValueError,match='reviewed definition vector'):restore(source,fn,row,rd)
