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



H="return function(state,limit,health,speak,tasks)\n local me,hero={},{}\n local resources,q={},{}\n local calls,frames,starts,ends,releases=0,0,0,0,0\n local held,paused,live=false,false,false\n local queries,events=0,{}\n local function event(key)events[#events+1]=key end\n function resources:NewResource()assert(not held);held=true;event('new');return 21 end\n function resources:PrepareResource(r)assert(r==21 and held)end\n function resources:TryAcquire(r,actor,kind)assert(r==21 and actor==me and kind==4);return true end\n function resources:ReleaseResource(r)assert(r==21 and held and not live and not paused);held=false;releases=releases+1;event('release')end\n function resources:ResourceThingHealthAbove(value,minimum) assert(minimum==0.0);return q:GetHealth(self:ScriptThing(value))>minimum end\n function resources:ScriptThing(r)assert(r==21 and held);return me end\n function resources:StartMovie(key)assert(key=='' and not live and held);live=true;starts=starts+1;event('start');return 31 end\n function resources:DestroyMovie(r)assert(r==31 and live and not paused,'movie destroyed while entities remain paused');live=false;ends=ends+1;event('end')end\n function q:RetailResources()return resources end\n function q:SetThingPersistent(actor,flag)assert(actor==me and flag)end\n function q:GetHero()return hero end\n function q:GetStateInt(key)assert(key=='MansLoverState');return state end\n function q:ReadGlobalGameDataString(offset)assert(offset==0x73c);return 'LETTER' end\n function q:NewScriptFrame()frames=frames+1;assert(frames<30,'frame loop');return not (calls>=limit or ends>0)end\n function q:IsActiveThreadTerminating()calls=calls+1;assert(calls<100,'termination loop');return calls>=limit or ends>0 end\n function q:SetThingHasInformation(actor,a,b,c)assert(actor==me and a and b and not c)end\n function q:PauseAllNonScriptedEntities(flag)assert(live and paused~=flag);paused=flag;event(flag and 'pause' or 'resume')end\n function q:FixMovieSequenceCamera(flag)assert(live and paused and type(flag)=='boolean')end\n function q:EntitySetFacingAngleTowardsThing(actor,target,flag)assert(actor==me and target==hero and type(flag)=='boolean')end\n function q:Pause(amount)assert(amount==1)end\n function q:CameraUseCameraPoint(key,actor,period,mode,duration)assert(live and paused and actor==me and period==-1 and mode==0 and duration==-1 and (key=='CAM_REL_WOMAN' or key=='CAM_REL_WOMANHERO'))end\n function q:IsObjectInThingsPossession(item,actor)assert(item=='LETTER' and actor==hero);return true end\n function q:GetHealth(actor)assert(actor==me);return health and 1 or 0 end\n function q:GiveHeroYesNoQuestion(key,yes,no,empty,flag)assert(live and paused and empty=='' and flag);queries=queries+1 end\n function q:MsgIsQuestionAnsweredYesOrNo()assert(queries>0);return 0 end\n function me:MsgIsPresentedWithItem()return false end\n function me:IsTalkedToByHero()return true end\n local remaining=0\n function me:Speak(target,key,group,a,b,c)assert(target==hero and group==0 and not a and b and not c and key:find('TEXT_QST_B10_MANS_LOVER_',1,true)==1);remaining=tasks;return speak end\n function me:IsPerformingScriptTask()if remaining>0 then remaining=remaining-1;return true end;return false end\n Init(q,me);Main(q,me)\n assert(not live and not paused and not held and starts==ends,'unbalanced resource/movie/pause lifetime')\n assert(releases==(limit>1 and 1 or 0),'resource release missing')\n return calls,frames,starts,ends,releases,table.concat(events,',')\nend"
CASES=[(0, 1, False, False, 0), (0, 1, False, False, 2), (0, 1, False, True, 0), (0, 1, False, True, 2), (0, 1, True, False, 0), (0, 1, True, False, 2), (0, 1, True, True, 0), (0, 1, True, True, 2), (0, 2, False, False, 0), (0, 2, False, False, 2), (0, 2, False, True, 0), (0, 2, False, True, 2), (0, 2, True, False, 0), (0, 2, True, False, 2), (0, 2, True, True, 0), (0, 2, True, True, 2), (0, 3, False, False, 0), (0, 3, False, False, 2), (0, 3, False, True, 0), (0, 3, False, True, 2), (0, 3, True, False, 0), (0, 3, True, False, 2), (0, 3, True, True, 0), (0, 3, True, True, 2), (0, 4, False, False, 0), (0, 4, False, False, 2), (0, 4, False, True, 0), (0, 4, False, True, 2), (0, 4, True, False, 0), (0, 4, True, False, 2), (0, 4, True, True, 0), (0, 4, True, True, 2), (0, 5, False, False, 0), (0, 5, False, False, 2), (0, 5, False, True, 0), (0, 5, False, True, 2), (0, 5, True, False, 0), (0, 5, True, False, 2), (0, 5, True, True, 0), (0, 5, True, True, 2), (0, 6, False, False, 0), (0, 6, False, False, 2), (0, 6, False, True, 0), (0, 6, False, True, 2), (0, 6, True, False, 0), (0, 6, True, False, 2), (0, 6, True, True, 0), (0, 6, True, True, 2), (0, 7, False, False, 0), (0, 7, False, False, 2), (0, 7, False, True, 0), (0, 7, False, True, 2), (0, 7, True, False, 0), (0, 7, True, False, 2), (0, 7, True, True, 0), (0, 7, True, True, 2), (0, 8, False, False, 0), (0, 8, False, False, 2), (0, 8, False, True, 0), (0, 8, False, True, 2), (0, 8, True, False, 0), (0, 8, True, False, 2), (0, 8, True, True, 0), (0, 8, True, True, 2), (0, 9, False, False, 0), (0, 9, False, False, 2), (0, 9, False, True, 0), (0, 9, False, True, 2), (0, 9, True, False, 0), (0, 9, True, False, 2), (0, 9, True, True, 0), (0, 9, True, True, 2), (0, 10, False, False, 0), (0, 10, False, False, 2), (0, 10, False, True, 0), (0, 10, False, True, 2), (0, 10, True, False, 0), (0, 10, True, False, 2), (0, 10, True, True, 0), (0, 10, True, True, 2), (0, 11, False, False, 0), (0, 11, False, False, 2), (0, 11, False, True, 0), (0, 11, False, True, 2), (0, 11, True, False, 0), (0, 11, True, False, 2), (0, 11, True, True, 0), (0, 11, True, True, 2), (0, 12, False, False, 0), (0, 12, False, False, 2), (0, 12, False, True, 0), (0, 12, False, True, 2), (0, 12, True, False, 0), (0, 12, True, False, 2), (0, 12, True, True, 0), (0, 12, True, True, 2), (0, 13, False, False, 0), (0, 13, False, False, 2), (0, 13, False, True, 0), (0, 13, False, True, 2), (0, 13, True, False, 0), (0, 13, True, False, 2), (0, 13, True, True, 0), (0, 13, True, True, 2), (0, 200, False, False, 0), (0, 200, False, False, 2), (0, 200, False, True, 0), (0, 200, False, True, 2), (0, 200, True, False, 0), (0, 200, True, False, 2), (0, 200, True, True, 0), (0, 200, True, True, 2), (1, 1, False, False, 0), (1, 1, False, False, 2), (1, 1, False, True, 0), (1, 1, False, True, 2), (1, 1, True, False, 0), (1, 1, True, False, 2), (1, 1, True, True, 0), (1, 1, True, True, 2), (1, 2, False, False, 0), (1, 2, False, False, 2), (1, 2, False, True, 0), (1, 2, False, True, 2), (1, 2, True, False, 0), (1, 2, True, False, 2), (1, 2, True, True, 0), (1, 2, True, True, 2), (1, 3, False, False, 0), (1, 3, False, False, 2), (1, 3, False, True, 0), (1, 3, False, True, 2), (1, 3, True, False, 0), (1, 3, True, False, 2), (1, 3, True, True, 0), (1, 3, True, True, 2), (1, 4, False, False, 0), (1, 4, False, False, 2), (1, 4, False, True, 0), (1, 4, False, True, 2), (1, 4, True, False, 0), (1, 4, True, False, 2), (1, 4, True, True, 0), (1, 4, True, True, 2), (1, 5, False, False, 0), (1, 5, False, False, 2), (1, 5, False, True, 0), (1, 5, False, True, 2), (1, 5, True, False, 0), (1, 5, True, False, 2), (1, 5, True, True, 0), (1, 5, True, True, 2), (1, 6, False, False, 0), (1, 6, False, False, 2), (1, 6, False, True, 0), (1, 6, False, True, 2), (1, 6, True, False, 0), (1, 6, True, False, 2), (1, 6, True, True, 0), (1, 6, True, True, 2), (1, 7, False, False, 0), (1, 7, False, False, 2), (1, 7, False, True, 0), (1, 7, False, True, 2), (1, 7, True, False, 0), (1, 7, True, False, 2), (1, 7, True, True, 0), (1, 7, True, True, 2), (1, 8, False, False, 0), (1, 8, False, False, 2), (1, 8, False, True, 0), (1, 8, False, True, 2), (1, 8, True, False, 0), (1, 8, True, False, 2), (1, 8, True, True, 0), (1, 8, True, True, 2), (1, 9, False, False, 0), (1, 9, False, False, 2), (1, 9, False, True, 0), (1, 9, False, True, 2), (1, 9, True, False, 0), (1, 9, True, False, 2), (1, 9, True, True, 0), (1, 9, True, True, 2), (1, 10, False, False, 0), (1, 10, False, False, 2), (1, 10, False, True, 0), (1, 10, False, True, 2), (1, 10, True, False, 0), (1, 10, True, False, 2), (1, 10, True, True, 0), (1, 10, True, True, 2), (1, 11, False, False, 0), (1, 11, False, False, 2), (1, 11, False, True, 0), (1, 11, False, True, 2), (1, 11, True, False, 0), (1, 11, True, False, 2), (1, 11, True, True, 0), (1, 11, True, True, 2), (1, 12, False, False, 0), (1, 12, False, False, 2), (1, 12, False, True, 0), (1, 12, False, True, 2), (1, 12, True, False, 0), (1, 12, True, False, 2), (1, 12, True, True, 0), (1, 12, True, True, 2), (1, 13, False, False, 0), (1, 13, False, False, 2), (1, 13, False, True, 0), (1, 13, False, True, 2), (1, 13, True, False, 0), (1, 13, True, False, 2), (1, 13, True, True, 0), (1, 13, True, True, 2), (1, 200, False, False, 0), (1, 200, False, False, 2), (1, 200, False, True, 0), (1, 200, False, True, 2), (1, 200, True, False, 0), (1, 200, True, False, 2), (1, 200, True, True, 0), (1, 200, True, True, 2), (2, 1, False, False, 0), (2, 1, False, False, 2), (2, 1, False, True, 0), (2, 1, False, True, 2), (2, 1, True, False, 0), (2, 1, True, False, 2), (2, 1, True, True, 0), (2, 1, True, True, 2), (2, 2, False, False, 0), (2, 2, False, False, 2), (2, 2, False, True, 0), (2, 2, False, True, 2), (2, 2, True, False, 0), (2, 2, True, False, 2), (2, 2, True, True, 0), (2, 2, True, True, 2), (2, 3, False, False, 0), (2, 3, False, False, 2), (2, 3, False, True, 0), (2, 3, False, True, 2), (2, 3, True, False, 0), (2, 3, True, False, 2), (2, 3, True, True, 0), (2, 3, True, True, 2), (2, 4, False, False, 0), (2, 4, False, False, 2), (2, 4, False, True, 0), (2, 4, False, True, 2), (2, 4, True, False, 0), (2, 4, True, False, 2), (2, 4, True, True, 0), (2, 4, True, True, 2), (2, 5, False, False, 0), (2, 5, False, False, 2), (2, 5, False, True, 0), (2, 5, False, True, 2), (2, 5, True, False, 0), (2, 5, True, False, 2), (2, 5, True, True, 0), (2, 5, True, True, 2), (2, 6, False, False, 0), (2, 6, False, False, 2), (2, 6, False, True, 0), (2, 6, False, True, 2), (2, 6, True, False, 0), (2, 6, True, False, 2), (2, 6, True, True, 0), (2, 6, True, True, 2), (2, 7, False, False, 0), (2, 7, False, False, 2), (2, 7, False, True, 0), (2, 7, False, True, 2), (2, 7, True, False, 0), (2, 7, True, False, 2), (2, 7, True, True, 0), (2, 7, True, True, 2), (2, 8, False, False, 0), (2, 8, False, False, 2), (2, 8, False, True, 0), (2, 8, False, True, 2), (2, 8, True, False, 0), (2, 8, True, False, 2), (2, 8, True, True, 0), (2, 8, True, True, 2), (2, 9, False, False, 0), (2, 9, False, False, 2), (2, 9, False, True, 0), (2, 9, False, True, 2), (2, 9, True, False, 0), (2, 9, True, False, 2), (2, 9, True, True, 0), (2, 9, True, True, 2), (2, 10, False, False, 0), (2, 10, False, False, 2), (2, 10, False, True, 0), (2, 10, False, True, 2), (2, 10, True, False, 0), (2, 10, True, False, 2), (2, 10, True, True, 0), (2, 10, True, True, 2), (2, 11, False, False, 0), (2, 11, False, False, 2), (2, 11, False, True, 0), (2, 11, False, True, 2), (2, 11, True, False, 0), (2, 11, True, False, 2), (2, 11, True, True, 0), (2, 11, True, True, 2), (2, 12, False, False, 0), (2, 12, False, False, 2), (2, 12, False, True, 0), (2, 12, False, True, 2), (2, 12, True, False, 0), (2, 12, True, False, 2), (2, 12, True, True, 0), (2, 12, True, True, 2), (2, 13, False, False, 0), (2, 13, False, False, 2), (2, 13, False, True, 0), (2, 13, False, True, 2), (2, 13, True, False, 0), (2, 13, True, False, 2), (2, 13, True, True, 0), (2, 13, True, True, 2), (2, 200, False, False, 0), (2, 200, False, False, 2), (2, 200, False, True, 0), (2, 200, False, True, 2), (2, 200, True, False, 0), (2, 200, True, False, 2), (2, 200, True, True, 0), (2, 200, True, True, 2)]

@pytest.mark.parametrize('style',('draft','readable'))
@pytest.mark.parametrize('state,limit,health,speak,tasks',CASES)
def test_generated_pause_lifetime(generated,style,state,limit,health,speak,tasks):
 lua=LuaRuntime();lua.execute(generated[style]);lua.execute(H)(state,limit,health,speak,tasks)
@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_interface_calls as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedInterfaceCallHeads']['0x00ec9d30'];captured={};original=adapter.restore
 unit['entities']={'MansLover':unit['entities']['MansLover']};unit['quest']['functions']={};unit['quest']['helpers']={}
 def capture(source,fn,review,rd,entries):
  if review is not None:captured.update(source=source,fn=fn,rd=rd,entries=entries)
  return original(source,fn,review,rd,entries)
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,tmp_path_factory.mktemp('pause_review'))
 return captured,row

@pytest.mark.parametrize('fault',('native','source','instruction','anchors','table','method','target','callee','purge','purge_bool','heads_absent','duplicate_site','wrong_site','missing','duplicate','arguments','slot_metadata','callee_memory','table_memory'))
@pytest.mark.parametrize('index',range(1))
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


@pytest.mark.parametrize('fault',('receiver_absent','receiver_type','receiver_cast','receiver_name','offset_absent','offset_changed','offset_type'))
def test_explicit_receiver_metadata_is_required(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_interface_calls import restore
 captured,row=review_input;row=copy.deepcopy(row);head=row['heads'][0]
 if fault=='receiver_absent':head.pop('printedReceiver')
 elif fault=='receiver_type':head['printedReceiver']=False
 elif fault=='receiver_cast':head['printedReceiver']='(int *)xStack_124'
 elif fault=='receiver_name':head['printedReceiver']='(void *)OTHER'
 elif fault=='offset_absent':head.pop('receiverStackOffset')
 elif fault=='offset_changed':head['receiverStackOffset']='0x1c'
 else:head['receiverStackOffset']=24
 with pytest.raises(ValueError,match='reviewed interface calls'):restore(captured['source'],captured['fn'],row,captured['rd'],captured['entries'])

@pytest.mark.parametrize('fault',('receiver_offset','receiver_register','table_register','flag','push_register','slot'))
def test_semantic_operands_reject_refreshed_bytes(review_input,fault):
 import copy,hashlib
 from tools.script_recovery.native_reviewed_interface_calls import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=captured['fn']
 mutations={'receiver_offset':(0xeca325,'8b4c241c'),'receiver_register':(0xeca325,'8b542418'),'table_register':(0xeca329,'8b10'),'flag':(0xeca32b,'6a01'),'push_register':(0xeca32b,'5790'),'slot':(0xeca32d,'ff92c8050000')}
 site,encoded=mutations[fault];rd=ChangedBytes(captured['rd'],{site:bytes.fromhex(encoded)});row['nativeInstructions'][hex(site)]=encoded
 if fault=='push_register':row['nativeInstructions'][hex(site)]='57'
 start=int(fn['address'],16);size=int(fn['bodyEndExclusive'],16)-start;row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,size)).hexdigest()
 with pytest.raises(ValueError,match='reviewed interface calls'):restore(captured['source'],fn,row,rd,captured['entries'])

def test_no_review_retains_unresolved_heads(review_input):
 from tools.script_recovery.native_reviewed_interface_calls import restore
 captured,row=review_input;assert restore(captured['source'],captured['fn'],None,captured['rd'],captured['entries'])==captured['source']

def test_original_native_pause_layouts():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();assert int.from_bytes(rd.bytes_at(0x1260f0c+0x5ec,4),'little')==0x890ab0 and callee_stack_words(0x890ab0)==1
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x300000,0x10000),(0x500000,0x6000),(0x200000,0x1000)):cpu.mem_map(a,n)
 fn=UnitConverter(ROOT/'refs/script_recovery/sick_child/translation_unit_typed.json').native('0x00ec9d30');start=int(fn['address'],16);size=int(fn['bodyEndExclusive'],16)-start;cpu.mem_write(start,rd.bytes_at(start,size));cpu.mem_write(0x200100,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};rows=[]
 def hook(machine,a,size,user):
  if a==0x200100:
   sp=machine.reg_read(x.UC_X86_REG_ESP);assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi'];state['arguments'].append(word(sp+4))
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+8)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 segments=((0xec9f95,0xec9fa8,1),(0xeca325,0xeca333,0),(0xecae21,0xecae2f,0),(0xecaf1d,0xecaf28,0),(0xecaf33,0xecaf3e,0),(0xecaf49,0xecaf57,0))
 for gsi,owner,sp in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800)):
  table=gsi+0x800;put(gsi,table);put(table+0x5ec,0x200100);put(owner+4,gsi)
  for begin,end,flag in segments:
   state={'gsi':gsi,'arguments':[]};cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EDI,0)
   cpu.reg_write(x.UC_X86_REG_EBX,gsi if begin in (0xecaf1d,0xecaf33) else 0xdeadc0de)
   cpu.emu_start(begin,end,count=100);assert state['arguments']==[flag] and cpu.reg_read(x.UC_X86_REG_ESP)==sp
   assert word(sp+0x18)==gsi,'saved receiver was not retained'
   rows.append(dict(start=hex(begin),end=hex(end),gsi=gsi,owner=owner,stack=sp,flag=flag))
 assert len(rows)==72
