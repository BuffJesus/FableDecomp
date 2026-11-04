import itertools,json,struct,sys
from pathlib import Path
from unittest.mock import patch
import pytest
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
from tools.script_recovery.convert_quest_unit import UnitConverter
@pytest.fixture(scope='module')
def review_input(tmp_path_factory):
 from tools.script_recovery import native_reviewed_interface_calls as adapter
 c=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
 unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
 row=unit['reviewedInterfaceCallHeads']['0x00e64fb0'];captured={};original=adapter.restore
 def capture(source,fn,review,rd,entries):
  if review is not None:captured.update(source=source,fn=fn,rd=rd,entries=entries)
  return original(source,fn,review,rd,entries)
 out=tmp_path_factory.mktemp('pause_review')
 with patch.object(c,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):c.convert(unit,out)
 captured['generated']=(out/'FSE/V_ChickenKicking/Entities/ChickenMaster.lua').read_text()
 from tools.script_recovery.build_readable_unit import readable_file
 captured['readable']=readable_file(captured['generated'])[0]
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
 elif fault=='wrong_site':head['sites'][0]='0x00e66d65'
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
 elif fault=='receiver_cast':head['printedReceiver']='(int *)xStack_374'
 elif fault=='receiver_name':head['printedReceiver']='(void *)OTHER'
 elif fault=='offset_absent':head.pop('receiverStackOffset')
 elif fault=='offset_changed':head['receiverStackOffset']='0x44'
 else:head['receiverStackOffset']=24
 with pytest.raises(ValueError,match='reviewed interface calls'):restore(captured['source'],captured['fn'],row,captured['rd'],captured['entries'])

@pytest.mark.parametrize('fault',('receiver_offset','receiver_register','table_register','flag','push_register','slot'))
def test_semantic_operands_reject_refreshed_bytes(review_input,fault):
 import copy,hashlib
 from tools.script_recovery.native_reviewed_interface_calls import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=captured['fn']
 mutations={'receiver_offset':(0xe67619,'8b4c2444'),'receiver_register':(0xe67619,'8b542440'),'table_register':(0xe6761d,'8b10'),'flag':(0xe6761f,'6a01'),'push_register':(0xe6761f,'5790'),'slot':(0xe67621,'ff92c8050000')}
 site,encoded=mutations[fault];rd=ChangedBytes(captured['rd'],{site:bytes.fromhex(encoded)});row['nativeInstructions'][hex(site)]=encoded
 if fault=='push_register':row['nativeInstructions'][hex(site)]='57'
 start=int(fn['address'],16);size=int(fn['bodyEndExclusive'],16)-start;row['nativeSha256']=hashlib.sha256(rd.bytes_at(start,size)).hexdigest()
 with pytest.raises(ValueError,match='reviewed interface calls'):restore(captured['source'],fn,row,rd,captured['entries'])

def test_no_review_retains_unresolved_heads(review_input):
 from tools.script_recovery.native_reviewed_interface_calls import restore
 captured,row=review_input;assert restore(captured['source'],captured['fn'],None,captured['rd'],captured['entries'])==captured['source']

def test_native_saved_receiver_layout():
 from tools.script_recovery.lift_native_lua import RData
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xe64000,0x5000),(0x300000,0x10000),(0x500000,0x6000),(0x200000,0x1000)):cpu.mem_map(a,n)
 fn=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json').native('0x00e64fb0')
 cpu.mem_write(0xe64fb0,rd.bytes_at(0xe64fb0,int(fn['bodyEndExclusive'],16)-0xe64fb0));cpu.mem_write(0x200100,b'\xc3')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={}
 def hook(machine,a,size,user):
  if a==0x200100:
   sp=machine.reg_read(x.UC_X86_REG_ESP)
   assert machine.reg_read(x.UC_X86_REG_ECX)==state['gsi']
   state['flags'].append(word(sp+4))
   machine.reg_write(x.UC_X86_REG_EIP,word(sp));machine.reg_write(x.UC_X86_REG_ESP,sp+8)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for gsi,owner,sp in itertools.product((0x500000,0x501000,0x502000),(0x503000,0x504000),(0x300800,0x301800)):
  put(gsi,gsi+0x800);put(gsi+0x800+0x5ec,0x200100);put(owner+4,gsi)
  for begin,end in ((0xe67619,0xe67627),(0xe68a73,0xe68a81),(0xe68a8f,0xe68a9d)):
   state={'gsi':gsi,'flags':[]};cpu.reg_write(x.UC_X86_REG_ESP,sp);cpu.reg_write(x.UC_X86_REG_ESI,owner)
   cpu.emu_start(0xe66d58,0xe66d6b,count=100)
   assert word(sp+0x40)==gsi and state['flags']==[1]
   cpu.reg_write(x.UC_X86_REG_EDI,0xdeadc0de);put(owner+4,0xbadc0de)
   cpu.emu_start(begin,end,count=100)
   assert state['flags']==[1,0] and cpu.reg_read(x.UC_X86_REG_ESP)==sp
   put(owner+4,gsi)

def test_only_reviewed_heads_are_replaced(review_input):
 from tools.script_recovery.native_reviewed_interface_calls import restore
 captured,row=review_input;text=captured['source'];expected=text
 for head in row['heads']:
  for flag in ('true','false'):
   expected=expected.replace(head['old']+'('+head['printedReceiver']+','+flag+')','GSI->PauseAllNonScriptedEntities('+flag+')')
 actual=restore(text,captured['fn'],row,captured['rd'],captured['entries'])
 assert actual==expected and all(head['old'] not in actual for head in row['heads'])


def test_deferred_review_preserves_lowering_input(review_input):
 from tools.script_recovery.native_reviewed_interface_calls import prepare,apply_deferred
 from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
 captured,row=review_input;text=captured['source']
 unchanged,edits=prepare(text,captured['fn'],row,captured['rd'],captured['entries'])
 assert unchanged==text and len(edits)==3 and sum(e[2] for e in edits)==6
 normalised=normalise_typed_decompile(unchanged)
 assert 'xStack_374 = ctr_CVar19;' in normalised
 actual=apply_deferred(normalised,edits)
 assert 'xStack_374 = ctr_CVar19;' in actual
 for before,after,count in edits:
  assert before not in actual
  for bad_count in (0,count+1):
   with pytest.raises(ValueError,match='inventory'):apply_deferred(before*bad_count,((before,after,count),))

@pytest.mark.parametrize('value',(False,0,1,'true',None))
def test_deferred_phase_rejects_invalid_value(review_input,value):
 import copy
 from tools.script_recovery.native_reviewed_interface_calls import prepare
 captured,row=review_input;row=copy.deepcopy(row);row['deferUntilLowered']=value
 with pytest.raises(ValueError,match='deferred phase'):prepare(captured['source'],captured['fn'],row,captured['rd'],captured['entries'])

def test_generated_unpause_cleanup_fragments(review_input):
 import re
 from lupa.lua54 import LuaRuntime
 from tools.script_recovery.build_readable_unit import readable_file
 captured,row=review_input;draft=captured['generated']
 for source in (draft,captured['readable']):
  for label in ('LAB_00e67619','LAB_00e68a73','LAB_00e68a8f'):
   match=re.search('::'+label+r'::\s*([^\n]+)\n\s*([^\n]+)',source)
   assert match and match[1].strip()=='quest:PauseAllNonScriptedEntities(false)'
   following=match[2].strip()
   if label=='LAB_00e67619':
    assert re.fullmatch(r'this_00 = \w+',following)
    actor=following.split(' = ')[1]
    block=match[1]+'\n'+following+'\nresources:DestroyMovie(this_00)'
   else:
    actor=re.fullmatch(r'resources:DestroyMovie\((\w+)\)',following)[1]
    block=match[1]+'\n'+following
   lua=LuaRuntime()
   # Exercise the actual emitted cleanup prefix. Full Main contains other
   # unresolved operations and is not certified by these bounded fragments.
   probe='local paused=true;local events={};local quest,resources={},{};local '+actor+'=37;local this_00\n'
   probe+='function quest:PauseAllNonScriptedEntities(flag)assert(flag==false and paused);paused=false;events[#events+1]="resume" end\n'
   probe+='function resources:DestroyMovie(movie)assert(movie==37 and not paused);events[#events+1]="destroy" end\n'
   assert lua.execute(probe+block+'\nreturn table.concat(events,",")')=='resume,destroy'

@pytest.mark.parametrize('fault',('missing','unknown','stack_and_register','wrong_register','bad_phase','stack_phase'))
def test_register_receiver_metadata(review_input,fault):
 import copy
 from tools.script_recovery.native_reviewed_interface_calls import prepare
 captured,row=review_input;row=copy.deepcopy(row);head=row['heads'][1]
 if fault=='missing':head.pop('receiverRegister')
 elif fault=='unknown':head['receiverRegister']='esp'
 elif fault=='stack_and_register':head['receiverStackOffset']='0x40'
 elif fault=='wrong_register':head['receiverRegister']='ebx'
 elif fault=='bad_phase':head['counterRenamedReceiver']=1
 else:row['heads'][0]['counterRenamedReceiver']=True
 with pytest.raises(ValueError,match='reviewed interface calls'):prepare(captured['source'],captured['fn'],row,captured['rd'],captured['entries'])

@pytest.mark.parametrize('fault',('receiver_move','table_register','table_offset','spill_source','spill_base','flag','call_table'))
def test_register_receiver_rejects_refreshed_native_bytes(review_input,fault):
 import copy,hashlib
 from tools.script_recovery.native_reviewed_interface_calls import restore
 from tools.script_recovery.test_native_inline_self_argument import ChangedBytes
 captured,row=review_input;row=copy.deepcopy(row);fn=captured['fn']
 mutations={'receiver_move':(0xe66d5f,'8bcb'),'table_register':(0xe66d5b,'8b13'),
 'table_offset':(0xe66d5b,'8b57'),'spill_source':(0xe66d61,'895c2444'),
 'spill_base':(0xe66d61,'897c2344'),'flag':(0xe66d5d,'6a00'),
 'call_table':(0xe66d65,'ff90ec050000')}
 site,encoded=mutations[fault];rd=ChangedBytes(captured['rd'],{site:bytes.fromhex(encoded)})
 # Recompute all instruction anchors so the semantic receiver checks, rather
 # than only old hashes, must reject the edited machine code.
 start=int(fn['address'],16);size=int(fn['bodyEndExclusive'],16)-start
 raw=rd.bytes_at(start,size);row['nativeSha256']=hashlib.sha256(raw).hexdigest()
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(raw,start))
 row['nativeInstructions']={hex(i.address):i.bytes.hex() for i in ins if 0xe66d58<=i.address<0xe66d6b}
 with pytest.raises(ValueError,match='reviewed interface calls'):restore(captured['source'],fn,row,rd,captured['entries'])

def test_generated_register_pause_calls(review_input):
 import re
 from lupa.lua54 import LuaRuntime
 from tools.script_recovery.build_readable_unit import readable_file
 captured,row=review_input;draft=captured['generated']
 for source in (draft,captured['readable']):
  for label in ('LAB_00e68a3c','LAB_00e68a59'):
   match=re.search('::'+label+r'::\s*([^\n]+)\n\s*([^\n]+)',source)
   assert match and match[1].strip()=='quest:PauseAllNonScriptedEntities(false)'
   following=match[2].strip();actor=re.fullmatch(r'resources:DestroyMovie\((\w+)\)',following)[1]
   probe='local paused=true;local quest,resources={},{};local '+actor+'=37\n'
   probe+='function quest:PauseAllNonScriptedEntities(flag)assert(flag==false and paused);paused=false end\n'
   probe+='function resources:DestroyMovie(movie)assert(movie==37 and not paused)end\n'
   assert LuaRuntime().execute(probe+match[1]+'\n'+following+'\nreturn paused') is False
  # The start call remains immediately after the unresolved interface load
  # (and its harmless pointer-lifetime staging), before camera setup.
  assert not re.search(r'TODO\(native\): .*\*CVar19 \+ 0x5ec',source)
  starts=list(re.finditer(r'(\w+) = resources:StartMovie\(""\)\n\s*quest:PauseAllNonScriptedEntities\(true\)\n\s*(?:__native_entity_state:SetStateBool\("HaveTalked", true\)|haveTalked_ = true)',source))
  assert len(starts)==1
  start=starts[0]
  probe='local live,paused,haveTalked_=false,false,false;local '+start[1]+';local quest,resources,__native_entity_state={},{},{}\n'
  probe+='function resources:StartMovie(key)assert(key=="" and not live);live=true;return 37 end\n'
  probe+='function quest:PauseAllNonScriptedEntities(flag)assert(flag==true and live and not paused);paused=true end\n'
  probe+='function __native_entity_state:SetStateBool(key,value)assert(key=="HaveTalked" and value and paused);haveTalked_=value end\n'
  assert LuaRuntime().execute(probe+start[0]+'\nreturn live and paused and haveTalked_ and '+start[1]+'==37') is True

