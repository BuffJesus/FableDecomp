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
 unit['entities']={'MansLover':unit['entities']['MansLover']};unit['quest']['functions']={};unit['quest']['helpers']={}
 out=tmp_path_factory.mktemp('mans_pause')
 with patch.object(c,'constructor_defaults',return_value={}):c.convert(unit,out)
 source=(out/'FSE/V_SickChild/Entities/MansLover.lua').read_text()
 return {'draft':source,'readable':readable_file(source)[0]}



H="return function(state,limit,health,speak,tasks,message,matching,talked,firstYes,handle)\n local me,hero={},{}\n local resources,q={},{}\n local calls,frames,starts,ends,releases=0,0,0,0,0\n local held,paused,live=false,false,false\n local queries,events=0,{}\n local function event(key)events[#events+1]=key end\n function resources:NewResource()assert(not held);held=true;event('new');return 21 end\n function resources:PrepareResource(r)assert(r==21 and held)end\n function resources:TryAcquire(r,actor,kind)assert(r==21 and actor==me and kind==4);return true end\n function resources:ReleaseResource(r)assert(r==21 and held and not live and not paused);held=false;releases=releases+1;event('release')end\n function resources:ResourceThingHealthAbove(value,minimum) assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end\n function resources:ScriptThing(r)assert(r==21 and held);return me end\n function resources:StartMovie(key)local pending;for i=1,100 do local n,v=debug.getlocal(2,i);if not n then break end;if n=='c_stk_10d' or n=='scratchValue3' or n=='letterPresentationPending' then pending=v end end;assert((pending==true or pending==1)==(message and matching),'wrong latch at movie start');assert(key=='' and not live and held);live=true;starts=starts+1;event('start');return handle end\n function resources:DestroyMovie(r)assert(r==handle and live and not paused,'movie destroyed while entities remain paused');live=false;ends=ends+1;event('end')end\n function q:RetailResources()return resources end\n function q:SetThingPersistent(actor,flag)assert(actor==me and flag)end\n function q:GetHero()return hero end\n function q:GetStateInt(key)assert(key=='MansLoverState');return state end\n function q:ReadGlobalGameDataString(offset)assert(offset==0x73c or offset==0x740);return offset==0x73c and 'LETTER' or 'REWARD' end\n function q:NewScriptFrame()frames=frames+1;assert(frames<100,'frame loop');return not (calls>=limit or ends>0)end\n function q:IsActiveThreadTerminating()calls=calls+1;assert(calls<100,'termination loop');return calls>=limit or ends>0 end\n function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end\n function q:PauseAllNonScriptedEntities(flag)assert(live and paused~=flag);paused=flag;event(flag and 'pause' or 'resume')end\n function q:FixMovieSequenceCamera(flag)assert(live and paused and type(flag)=='boolean')end\n function q:EntitySetFacingAngleTowardsThing(actor,target,flag)assert(actor==me and target==hero and type(flag)=='boolean')end\n function q:Pause(amount)assert(amount==1)end\n function q:CameraUseCameraPoint(key,actor,period,mode,duration)assert(live and paused and actor==me and period==-1 and mode==0 and duration==-1 and (key=='CAM_REL_WOMAN' or key=='CAM_REL_WOMANHERO'))end\n function q:IsObjectInThingsPossession(item,actor)assert(item=='LETTER' and actor==hero);return true end\n function q:GetHealth(actor)assert(actor==me);return health and 1 or 0 end\n function q:GiveHeroYesNoQuestion(key,yes,no,empty,flag)assert(live and paused and empty=='' and flag);queries=queries+1 end\n function q:MsgIsQuestionAnsweredYesOrNo()assert(queries>0);return not (message and matching) and queries==1 and firstYes and 1 or 0 end\n function me:MsgIsPresentedWithItem()g_PresentedItemName=matching and 'LETTER' or 'OTHER';return message end\n function me:IsTalkedToByHero()return talked end\n local remaining=0\n function me:Speak(target,key,group,a,b,c)assert(target==hero and group==0 and not a and b and not c and key:find('TEXT_QST_B10_MANS_LOVER_',1,true)==1);remaining=tasks;return speak end\n function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end\n local taken,morality=0,0\n function q:TakeObjectFromHero(item)local pending;for i=1,100 do local n,v=debug.getlocal(2,i);if not n then break end;if n=='c_stk_10d' or n=='scratchValue3' or n=='letterPresentationPending' then pending=v end end;assert(pending==false or pending==0,'latch not reset before take');assert(item=='LETTER' and live and paused);taken=taken+1 end\n function q:GiveHeroObject(item,count,flag)assert(item=='REWARD' and count==-1 and not flag and taken==1 and live and paused)end\n function q:ReadGlobalGameDataFloat(offset)assert(offset==0x758 or offset==0x75c);return 1.25 end\n function q:GiveHeroMorality(amount)assert(amount==1.25 and taken==1 and live and paused);morality=morality+1 end\n function q:SetStateInt(key,value)assert(key=='MansLoverState' and (value==1 or value==2) and morality==1)end\n function q:ClearThingHasInformation(actor)assert(actor==me and taken==1 and morality==1)end\n Init(q,me);Main(q,me)\n assert(taken<=1 and morality<=taken,'repeated gift processing')\n assert(not live and not paused and not held and starts==ends,'unbalanced resource/movie/pause lifetime')\n assert(releases==(limit>1 and 1 or 0),'resource release missing');if message and matching and limit==200 and speak then assert(taken==1,'matching presentation not consumed')end;if not talked and not (message and matching) then assert(starts==0 and taken==0,'idle entered movie')end\n return calls,frames,starts,ends,releases,table.concat(events,',')\nend"
CASES=[]
for state,limit,health,speak,tasks,message,matching,talked,firstYes,handle in itertools.product((0,1,2),(1,2,3,5,9,15,30,50,200),(False,True),(False,True),(0,2),(False,True),(False,True),(False,True),(False,True),(0,31,713)):
 if limit==200 and not talked and not (message and matching):continue
 CASES.append((state,limit,health,speak,tasks,message,matching,talked,firstYes,handle))
@pytest.mark.parametrize('case',CASES)
def test_real_letter_latch_behavior(generated,case):
 traces=[]
 for source in generated.values():
  lua=LuaRuntime();lua.execute(source);traces.append(lua.execute(H)(*case))
 assert len(set(traces))==1

def test_native_latch_init_set_and_gated_reset():
 import unicorn as u
 from unicorn import x86_const as x
 from tools.script_recovery.lift_native_lua import RData
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0xf35000,0x1000),(0x300000,0x10000),(0x500000,0x3000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000));cpu.mem_write(0xf35b30,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 def hook(m,a,size,user):
  if a==0xecaf49:state['abort']=True;m.emu_stop();return
  if a==0xf35b30:
   assert m.reg_read(x.UC_X86_REG_ECX)==state['owner'];sp=m.reg_read(x.UC_X86_REG_ESP)
   m.reg_write(x.UC_X86_REG_EAX,state['term']);m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for sp,owner,initial,term in itertools.product((0x300800,0x301800),(0x500000,0x501000),(0,1,255),(0,1,255)):
  state=dict(owner=owner,term=term,abort=False);cpu.mem_write(sp+0x2e,bytes([0x55,initial,0x66]))
  # Init includes a single pending hidden conversion-output word.
  cpu.reg_write(x.UC_X86_REG_ESP,sp-4);cpu.emu_start(0xec9e52,0xec9e57,count=10)
  assert cpu.mem_read(sp+0x2e,3)==b'\x55\x00\x66'
  cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.emu_start(0xec9ed4,0xec9ed9,count=10)
  assert cpu.mem_read(sp+0x2e,3)==b'\x55\x01\x66'
  cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_ECX,owner);cpu.emu_start(0xeca306,0xeca317,count=100)
  assert cpu.mem_read(sp+0x2e,3)==bytes([0x55,int(bool(term)),0x66]) and state['abort']==bool(term) and cpu.reg_read(x.UC_X86_REG_ESP)==sp
  layouts+=1
 assert layouts==36




@pytest.fixture(scope='module')
def latch_review_input(tmp_path_factory):
 import copy
 from tools.script_recovery import native_reviewed_stack_aliases as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json');unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedStackScalarAliases']['0x00ec9d30'];unit['entities']={'MansLover':unit['entities']['MansLover']};unit['quest']['functions']={};unit['quest']['helpers']={};captured={};original=adapter.restore
 def capture(text,fn,review,rd):
  if fn['address'].lower()=='0x00ec9d30':captured.update(text=text,fn=fn,rd=rd,row=review)
  return original(text,fn,review,rd)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,tmp_path_factory.mktemp('mans_latch_review'))
 return captured,captured['row']

@pytest.mark.parametrize('fault',('init','set','reset','frame','source','count'))
def test_stale_latch_review_rejected(latch_review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=latch_review_input;row=copy.deepcopy(row);fn=captured['fn'];source=captured['text'];rd=captured['rd']
 if fault=='init':rd=ChangedBytes(rd,{0xec9e52:bytes.fromhex('c644243301')})
 elif fault=='set':rd=ChangedBytes(rd,{0xec9ed4:bytes.fromhex('c644242f00')})
 elif fault=='reset':rd=ChangedBytes(rd,{0xeca313:bytes.fromhex('8844242e')})
 elif fault=='frame':rd=ChangedBytes(rd,{0xec9e2e:bytes.fromhex('90')})
 elif fault=='source':source=source.replace('cStack_10d =','otherLatch =',1)
 else:row['editCounts'][-1]=7
 with pytest.raises(ValueError,match='reviewed stack alias'):restore(source,fn,row,rd)

def test_unreviewed_latch_remains_explicit(latch_review_input):
 from tools.script_recovery.native_reviewed_stack_aliases import restore
 captured,row=latch_review_input;assert restore(captured['text'],captured['fn'],None,captured['rd'])==captured['text']
