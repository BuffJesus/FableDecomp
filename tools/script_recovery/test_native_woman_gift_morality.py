import itertools,json,re,struct,sys
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
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('woman_timer')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/WomanToAttract.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}



H=r'''return function(route,health,speak,tasks,limit,mode,amount,bits)
 local q,me,hero,r={},{},{},{};local frame,active,starts,ends,sequences=0,nil,0,0,0;local paused,pauseStarts=false,0
 local expressionSent,talkSent=false,false;local polls=0;local registered=false
 Init(q,me)
 local giftCalls,floatReads,intReads,moralityCalls,removals=0,0,0,0,0
 for n=1,40 do
  local name,value=debug.getupvalue(Main,n);if not name then break end
  if type(value)=='table' and value.SetStateInt then value:SetStateInt('TimesMadeLaugh',2)end
  if name=='timesMadeLaugh_' then debug.setupvalue(Main,n,2)end
 end
 assert(type(helper_ED0B10)=='function' or type(DoLeavingCutscene)=='function','missing native helper')
 local function leaving(quest,actor)assert(quest==q and actor==me);giftCalls=giftCalls+1 end
 helper_ED0B10=leaving;DoLeavingCutscene=leaving
 function q:ReadGlobalGameData(offset)assert(offset==0x774);intReads=intReads+1;return bits end
 function q:ReadGlobalGameDataFloat(offset)assert(offset==0x774);floatReads=floatReads+1;return amount end
 function q:GiveHeroMorality(value)assert(value==amount,'integer bits used as morality amount');moralityCalls=moralityCalls+1 end
 function q:RemoveThing(actor,a,b)assert(actor==me and not a and b);removals=removals+1 end
 local function term()return frame>=limit end
 function q:GetHero()return hero end
 function q:RetailResources()return r end
 function q:NewScriptFrame(actor)assert(actor==me);frame=frame+1;return not term()end
 function q:IsActiveThreadTerminating()return term()end
 function q:GetStateBool(name)assert(name=='LaughingWomanKilled');return false end
 function q:SetIsPushableByHero(actor,flag)assert(actor==me and not flag)end
 function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and not a and not b and not c)end
 function q:RegisterTimer()assert(not registered);registered=true;return 713 end
 function q:GetTimer(id)assert(registered and id==713);return 1 end
 function q:SetTimer(id,value)assert(registered and id==713 and (value==20 or value==6))end
 function q:DeregisterTimer(id)assert(registered and id==713);registered=false end
 function q:IsDistanceBetweenThingsUnder(a,b,d)assert(a==me and b==hero and d==15);return false end
 function q:StartMovieSequence()sequences=sequences+1;if mode~='handle' then error('duplicate sequence start')end end
 function q:PauseAllNonScriptedEntities(flag)assert(type(flag)=='boolean');if flag then assert(active~=nil and not paused);pauseStarts=pauseStarts+1 end;paused=flag end
 function q:GetHealth(actor)assert(actor==me);return health and 10 or 0 end
 function r:MemberResource(name,actor)assert(name=='seh_me' and actor==me);return 811 end
 function r:PrepareResource(id)assert(id==811)end
 function r:ScriptThing(id)assert(id==811);return me end
 function r:StartMovie(name)assert(name=='');assert(active==nil,'movie already live');starts=starts+1;active=1000+starts;return active end
 function r:DestroyMovie(id)
  if mode~='sequence' then assert(id==active and active~=nil,'wrong movie cleanup handle')end
  assert(not paused and pauseStarts==starts,'movie pause/unpause omitted');active=nil;ends=ends+1
 end
 function me:AcquireControl(kind)assert(kind==4);return true end
 function me:MsgExpressionPerformedTo()
  if expressionSent or route=='talk' then return nil end;expressionSent=true;return route
 end
 function me:IsTalkedToByHero()if route~='talk' or talkSent then return false end;talkSent=true;return true end
 -- Existing speech operand/termination mapping is deliberately outside this resource identity probe.
 function me:Speak(...)return speak end
 function me:IsPerformingScriptTask()polls=polls+1;return polls<=tasks end
 debug.sethook(function()error('instruction budget')end,'',200000)
 Main(q,me);debug.sethook()
 assert(active==nil and not registered and starts==ends and not paused and pauseStarts==starts,'movie/timer/pause remains live')
 assert(starts==(limit>1 and 1 or 0),'movie route not exercised')
 if mode~='handle' then assert(sequences==0,'extra native sequence')end
 assert(giftCalls==(limit>1 and 1 or 0) and moralityCalls==giftCalls and floatReads==giftCalls and intReads==0,'wrong gift/reader count')
 assert(removals==giftCalls,'given-object exit not reached')
 return starts,ends,sequences,giftCalls,moralityCalls
end
'''
VALUES=(0.0,-0.0,1.25,-1.25,10.0,100.0)
CASES=list(itertools.product(('EXPRESSION_PELVIC_THRUST','EXPRESSION_COSSACK','EXPRESSION_GIGGLE'),(False,True),(False,True),(1,2,5),VALUES))

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('route,health,speak,limit,amount',CASES)
def test_gift_morality(generated,style,route,health,speak,limit,amount):
 source=generated[style]
 bits=struct.unpack('<I',struct.pack('<f',amount))[0]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(route,health,speak,1,limit,'all',amount,bits)
@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedStackScalarAliases']['0x00ecf7f0']
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 original=adapter.restore;captured={}
 def capture(source,fn,row,rd):
  if str(fn['address']).lower()=='0x00ecf7f0':captured.update(source=source,fn=fn,rd=rd)
  return original(source,fn,row,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,tmp_path_factory.mktemp('woman_timer_review'))
 return captured,row

@pytest.mark.parametrize('fault',('native','source','instruction','list_type','entry_type','duplicate','offset','read_site','call_site','slot','method','target','fpu_bytes','fpu_site','missing','duplicate_read','table_memory','fpu_memory'))
def test_global_float_argument_guards(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);text=captured['source'];rd=captured['rd'];read=row['globalFloatArguments'][0]
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions']['0xed04f2']='90'
 elif fault=='list_type':row['globalFloatArguments']=False
 elif fault=='entry_type':row['globalFloatArguments']=[None]
 elif fault=='duplicate':row['globalFloatArguments'].append(dict(read))
 elif fault=='offset':read['offset']='0x768'
 elif fault=='read_site':read['readSite']='0x00ed04ed'
 elif fault=='call_site':read['callSite']='0x00ed0343'
 elif fault=='slot':read['slot']='0x274'
 elif fault=='method':read['method']='GetHeroMorality'
 elif fault=='target':read['target']='0x00899f90'
 elif fault=='fpu_bytes':read['calleeFloatBytes']='90'
 elif fault=='fpu_site':read['calleeFloatUse']='0x00899f63'
 elif fault=='missing':text=text.replace('*(undefined4 *)(DAT_0143e90c + 0x774)','MISSING_FLOAT_READ')
 elif fault=='duplicate_read':text+='\n'+ 'copy = *(undefined4 *)(DAT_0143e90c + 0x774);'
 elif fault=='table_memory':rd=ChangedBytes(rd,{0x1260f0c+0x270:b'\0'*4})
 elif fault=='fpu_memory':rd=ChangedBytes(rd,{0x899f62:b'\0'*4})
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(text,fn,row,rd)

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('route',('EXPRESSION_PELVIC_THRUST','EXPRESSION_COSSACK','EXPRESSION_GIGGLE'))
def test_gift_main_calls_generated_leaving_helper(generated,style,route):
 lua=LuaRuntime();lua.execute(generated[style])
 probe=H.replace(' helper_ED0B10=leaving;DoLeavingCutscene=leaving','')
 probe=probe.replace("assert(offset==0x774);floatReads=floatReads+1;return amount", "assert(offset==0x774 and giftCalls==1);floatReads=floatReads+1;return amount")
 methods=r'''
 local live,order={},{};local resource,actors,strings
 local function new()local v={};live[v]=true;return v end
 function r:NewResource()resource=new();return resource end
 function r:TryAcquire(id,actor,kind)assert(id==resource and actor==hero and kind==4);return true end
 function r:NewActorMap()actors=new();return actors end
 function r:SetActor(map,key,value)assert(map==actors);map[key]=value end
 function r:NewStringMap()strings=new();return strings end
 function r:SetString(map,key,value)assert(map==strings and key=='$ITEM' and value=='TEST_ITEM');map[key]=value end
 function q:ReadGlobalGameDataString(offset)assert(offset==0x738);return 'TEST_ITEM' end
 function q:FixMovieSequenceCamera(flag)order[#order+1]=flag and 'on' or 'off' end
 function r:RunMacroWithStrings(name,a,s,setup,skip)
  assert(name=='CS_SICK_LAUGHING_WOMAN_LEAVES' and a==actors and s==strings and not setup and skip)
  assert(a.HERO==resource and a.WOMAN==811 and s['$ITEM']=='TEST_ITEM')
  assert(live[a] and live[s] and live[resource] and paused);order[#order+1]='macro'
 end
 local function destroy(id,expected,name)assert(id==expected and live[id]);live[id]=nil;order[#order+1]=name end
 function r:DestroyStringMap(id)destroy(id,strings,'strings')end
 function r:DestroyActorMap(id)destroy(id,actors,'actors')end
 function r:ReleaseResource(id)
  destroy(id,resource,'resource');assert(next(live)==nil and table.concat(order,',')=='on,macro,off,strings,actors,resource');giftCalls=giftCalls+1
 end
 '''
 needle=' debug.sethook(function()error'
 assert probe.count(needle)==1;probe=probe.replace(needle,methods+'\n'+needle)
 lua.execute(probe)(route,True,True,1,5,'all',1.25,0x3fa00000)
 # Executes the emitted helper/maps/camera cleanup; macro engine work remains mocked.

def test_omitted_float_review_preserves_uint_read(review_input):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 captured,row=review_input;row=copy.deepcopy(row);row.pop('globalFloatArguments')
 out=restore(captured['source'],captured['fn'],row,captured['rd'])
 assert '*(undefined4 *)(DAT_0143e90c + 0x774)' in out
 assert '*(float *)(DAT_0143e90c + 0x774)' not in out

def test_original_native_gift_float_arguments():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();assert int.from_bytes(rd.bytes_at(0x1260f0c+0x270,4),'little')==0x899ef0 and callee_stack_words(0x899ef0)==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x899000,0x2000),(0x57a000,0x1000),(0x143e000,0x1000),(0x300000,0x10000),(0x500000,0x5000),(0x600000,0x2000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885));cpu.mem_write(0x899f53,rd.bytes_at(0x899f53,0x17));cpu.mem_write(0x57a67e,b'\xc3');cpu.mem_write(0x200100,b'\xc3');cpu.mem_write(0x200200,bytes.fromhex('dbe3'))
 state={};rows=[];products=[];values=(0.0,-0.0,1.25,-1.25,10.0,100.0)
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 def hook(machine,a,size,user):
  if a==0x200100:
   sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'];state['argument']=word(sp+4)
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+8)
  elif a==0x57a67e:
   sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==0x500100
   machine.reg_write(x.UC_X86_REG_EAX,state['scale']&0xffffffff);machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+4)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp,value in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800),values):
  state={'gsi':gsi};table=gsi+0x800;put(gsi,table);put(table+0x270,0x200100);put(owner+4,gsi);cpu.mem_write(owner+0x1c,b'\0');put(0x143e90c,0x600000)
  bits=struct.unpack('<I',struct.pack('<f',value))[0];put(0x600774,bits)
  cpu.reg_write(x.UC_X86_REG_EBP,owner);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xed04e6,0xed0501,count=100)
  assert state['argument']==bits and cpu.mem_read(owner+0x1c,1)==b'\1' and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  rows.append(dict(value=value,bits=bits,owner=owner,gsi=gsi,stack=sp))
 for value,scale in itertools.product(values,(1,2,3,-2)):
  state={'scale':scale};sp=0x300800;cpu.emu_start(0x200200,0x200202,count=2)
  cpu.reg_write(x.UC_X86_REG_ESI,0x500100);cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.mem_write(sp+12,struct.pack('<f',value))
  cpu.emu_start(0x899f53,0x899f6a,count=100)
  actual=word(sp+12);expected=struct.unpack('<I',struct.pack('<f',value*scale))[0]
  assert actual==expected and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  products.append(dict(value=value,scale=scale,bits=actual))
