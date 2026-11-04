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



H=r'''
return function(route,health,speak,tasks,limit,mode)
 local q,me,hero,r={},{},{},{};local frame,active,starts,ends,sequences=0,nil,0,0,0;local paused,pauseStarts=false,0
 local expressionSent,talkSent=false,false;local polls=0;local registered=false
 Init(q,me)
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
 return starts,ends,sequences
end
'''
CASES=list(itertools.product(('EXPRESSION_SHIT','EXPRESSION_PELVIC_THRUST','EXPRESSION_COSSACK','EXPRESSION_GIGGLE','talk'),(False,True),(False,True),(1,3),(1,2,5),('handle','sequence','all')))
@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('route,health,speak,tasks,limit,mode',CASES)
def test_movie_lifecycle(generated,style,route,health,speak,tasks,limit,mode):
 source=generated[style]
 lua=LuaRuntime();lua.execute(source);lua.execute(H)(route,health,speak,tasks,limit,mode)

@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_interface_calls as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedInterfaceCallHeads']['0x00ecf7f0'];captured={};original=adapter.restore
 unit['entities']={'WomanToAttract':unit['entities']['WomanToAttract']};unit['quest']['functions']={};unit['quest']['helpers']={}
 def capture(source,fn,review,rd,entries):
  if review is not None:captured.update(source=source,fn=fn,rd=rd,entries=entries)
  return original(source,fn,review,rd,entries)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,tmp_path_factory.mktemp('pause_review'))
 return captured,row

@pytest.mark.parametrize('fault',('native','source','instruction','anchors','table','method','target','callee','purge','purge_bool','heads_absent','duplicate_site','wrong_site','missing','duplicate','arguments','slot_metadata','callee_memory','table_memory'))
@pytest.mark.parametrize('index',range(3))
def test_stale_interface_review(review_input,index,fault):
 import copy
 from tools.script_recovery.native_reviewed_interface_calls import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=copy.deepcopy(captured['fn']);text=captured['source'];rd=captured['rd'];entries=copy.deepcopy(captured['entries']);head=row['heads'][index]
 if fault=='native':row['nativeSha256']='0'*64
 elif fault=='source':fn['decompile']+='\n'
 elif fault=='instruction':row['nativeInstructions'][hex(int(head['sites'][0],16))]='90'
 elif fault=='anchors':row['nativeInstructions']={}
 elif fault=='table':row['table']='0x01238c8c'
 elif fault=='method':row['method']='GetHero'
 elif fault=='target':row['target']='0x00890ab4'
 elif fault=='callee':row['calleeBytes']='00'*96
 elif fault=='purge':row['purgeWords']=2
 elif fault=='purge_bool':row['purgeWords']=True
 elif fault=='heads_absent':row['heads']=[]
 elif fault=='duplicate_site':head['sites'].append(head['sites'][0])
 elif fault=='wrong_site':head['sites'][0]='0x00ed0120'
 elif fault=='missing':text=text.replace(head['old'],'MISSING_HEAD',1)
 elif fault=='duplicate':text+='\n'+head['old']+'(0);'
 elif fault=='arguments':row['arguments'][hex(int(head['sites'][0],16))]=['99']
 elif fault=='slot_metadata':
  entry=next(e for e in entries if int(e[3]['site'],16)==int(head['sites'][0],16));entry[3]['slot']='0x5c8'
 elif fault=='callee_memory':rd=ChangedBytes(rd,{0x890ab0:b'\0'*96})
 elif fault=='table_memory':rd=ChangedBytes(rd,{0x1260f0c+0x5ec:b'\0'*4})
 with pytest.raises(ValueError,match='reviewed interface calls'):restore(text,fn,row,rd,entries)

@pytest.mark.parametrize('fault',('flag','push_register','slot'))
def test_native_operands_rejected_even_with_refreshed_hashes(review_input,fault):
 import copy,hashlib
 from tools.script_recovery.native_reviewed_interface_calls import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=captured['fn']
 if fault=='flag':patches={0xed033b:bytes.fromhex('6a00')};row['nativeInstructions']['0xed033b']='6a00'
 elif fault=='push_register':patches={0xed033b:bytes.fromhex('5690')};row['nativeInstructions']['0xed033b']='56'
 else:patches={0xed0343:bytes.fromhex('ff92c8050000')};row['nativeInstructions']['0xed0343']='ff92c8050000'
 rd=ChangedBytes(captured['rd'],patches);start=int(fn['address'],16);size=int(fn['bodyEndExclusive'],16)-start
 row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,size)).hexdigest()
 expected={'flag':'native immediate arguments changed','push_register':'native argument setup changed','slot':'native call slot changed'}[fault]
 with pytest.raises(ValueError,match=expected):
  restore(captured['source'],fn,row,rd,captured['entries'])

def test_no_review_preserves_unknown_calls(review_input):
 from tools.script_recovery.native_reviewed_interface_calls import restore
 captured,row=review_input;assert restore(captured['source'],captured['fn'],None,captured['rd'],captured['entries'])==captured['source']

def test_original_native_pause_layouts():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();target=int.from_bytes(rd.bytes_at(0x1260f0c+0x5ec,4),'little');assert target==0x890ab0 and callee_stack_words(target)==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xecf000,0x2000),(0x300000,0x10000),(0x500000,0x5000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xecf7f0,rd.bytes_at(0xecf7f0,4885));cpu.mem_write(0x200100,b'\xc3')
 starts={0xed0120:0xed0117,0xed0343:0xed0336,0xed0755:0xed074c,0xed01de:0xed01d8,0xed0681:0xed067b,0xed0995:0xed098f,0xed09f6:0xed09f0,0xed0a19:0xed0a13,0xed0a3c:0xed0a36,0xed0a4c:0xed0a44,0xed0a5c:0xed0a54,0xed0a7f:0xed0a79,0xed0a99:0xed0a93}
 state={};rows=[]
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 def hook(machine,a,size,user):
  if a==0x200100:
   stack=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi']
   value=word(stack+4);assert value in (0,1);state['values'].append(value)
   ret=word(stack);machine.reg_write(x.UC_X86_REG_EIP,ret);machine.reg_write(x.UC_X86_REG_ESP,stack+8)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for site,gsi,owner,sp in itertools.product(starts,(0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800)):
  state={'gsi':gsi,'values':[]};table=gsi+0x800;put(gsi,table);put(table+0x5ec,0x200100);put(owner+4,gsi)
  cpu.reg_write(x.UC_X86_REG_EBP,owner);cpu.reg_write(x.UC_X86_REG_ESI,gsi);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  if site in (0xed0a4c,0xed0a5c):
   # Execute the original funny-path spill first. The cleanup reload must work
   # even when the old register no longer contains the interface.
   cpu.emu_start(0xed0336,0xed0349,count=100);assert word(sp+0x2c)==gsi and state['values']==[1]
   cpu.reg_write(x.UC_X86_REG_ESI,4)
  cpu.emu_start(starts[site],site+6,count=100)
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp and state['values'][-1]==int(site in (0xed0120,0xed0343,0xed0755))
  rows.append({'site':hex(site),'gsi':gsi,'owner':owner,'stack':sp,'flags':list(state['values'])})
