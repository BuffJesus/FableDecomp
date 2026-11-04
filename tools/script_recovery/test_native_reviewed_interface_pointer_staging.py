from pathlib import Path
from copy import deepcopy
import hashlib,json,struct,sys
import pytest
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
R=Path(__file__).resolve().parents[2];W=Path(__file__).with_name('fixtures')/'interface_pointer/book_main'
from tools.script_recovery.native_reviewed_interface_pointer_staging import classify
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import resource_call_evidence
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.readable_lua import tokens
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
digest=lambda data:hashlib.sha256(data).hexdigest()
@pytest.fixture(scope='module')
def fixture():
 unit=read(W/'unit_baseline.json');spec=LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={})
 return ((W/'phase.lua').read_text(encoding='utf-8'),read(W/'phase_todo.json'),read(W/'native_function.json'),read(W/'candidate_row.json'),spec,RData())
def run(values,facts=resource_call_evidence):return classify(*values,facts,prove_unused_staging)
def test_only_initial_pointer_diagnostic_and_annotations_change(fixture):
 source,todo,fn,row,spec,image=fixture;before=list(todo)
 out,remaining,proof=run(fixture)
 assert todo==before and remaining==[value for value in todo if value!=row['diagnostic']]
 assert len(todo)-len(remaining)==1 and len(proof)==1
 assert proof[0]['liveness']['visitedNodes']==381
 assert set(proof[0]['liveness']['copiedAliases'])=={'CVar12','xStack_7c'}
 assert proof[0]['cachePrefixEspOffset']==20 and proof[0]['executableLuaUnchanged']
 sig=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
 assert sig(out)==sig(source) and out==(W/'candidate_phase.lua').read_text(encoding='utf-8')
 assert out.count('xStack_7c = CVar12')==source.count('xStack_7c = CVar12')
def test_optional_hook_does_not_access_unavailable_evidence():
 todo=['diagnostic'];out,remaining,proof=classify('source',todo,{},None,None,None,None,None)
 assert (out,remaining,proof)==('source',todo,[]) and remaining is not todo
def test_contiguous_prefix_agrees_with_an_available_argument_fact(fixture):
 source,todo,fn,row,spec,image=fixture
 facts=resource_call_evidence(image,fn,spec,include_pause=True)
 facts[int(row['dispatch'],16)]['pushes']=[('imm',1)]
 assert run(fixture,lambda *args,**kwargs:facts)==run(fixture)

@pytest.mark.parametrize('fault',[
 'imageSha256','nativeSha256','sourceSha256','luaPhaseSha256','scope','evidence','slot','method','target','callee_prefix','callee_short','purge',
 'wrong_spec','site','dispatch','cache_coordinate','argument_value','argument_type','alias','copy_alias','diagnostic','fragment','ambiguous_fragment','region',
 'missing_todo','duplicate_todo','duplicate_region','observable_alias','observable_copy','fact_missing','fact_family','fact_offset','fact_slot','fact_argument',
 'native_field','native_pointer_register','native_table_register','native_receiver','native_cache_disp','native_cache_value','native_argument','native_slot','native_owner','branch_entry',
])
def test_stale_or_ambiguous_evidence_is_rejected(fixture,fault):
 source,todo,fn,row,spec,image=fixture;todo=list(todo);row=deepcopy(row);spec=deepcopy(spec)
 base_image=image;patches={};fact_fn=resource_call_evidence
 class Overlay:
    ok=True;data=base_image.data
    def bytes_at(self,address,size):return patches.get((address,size),base_image.bytes_at(address,size))
    def string_at(self,address):return base_image.string_at(address)
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='scope':row['scopeLimit']='unconditional'
 elif fault=='evidence':row['acceptedEvidenceSha256']=''
 elif fault=='slot':row['slot']='0x5e8'
 elif fault=='method':row['method']='Other'
 elif fault=='target':row['target']='0x890ab1'
 elif fault=='callee_prefix':row['calleeWitnessPrefix']='00'*100
 elif fault=='callee_short':row['calleeWitnessPrefix']='c3'
 elif fault=='purge':row['purgeWords']=0
 elif fault=='wrong_spec':spec.entity=False
 elif fault=='site':row['nativeRead']='0xe551c7'
 elif fault=='dispatch':row['dispatch']='0xe551d4'
 elif fault=='cache_coordinate':row['cachePrefixEspOffset']=24
 elif fault=='argument_value':row['argument']=False
 elif fault=='argument_type':row['argument']=1
 elif fault=='alias':row['alias']='other'
 elif fault=='copy_alias':row['copyAlias']='other'
 elif fault=='diagnostic':row['diagnostic']='unlifted: actual_effect()'
 elif fault=='fragment':row['nativeSourceFragment']='missing source'
 elif fault=='ambiguous_fragment':row['nativeSourceFragment']='CVar12 = '
 elif fault=='region':row['luaRegion']+='unrelated()\n'
 elif fault=='missing_todo':todo.remove(row['diagnostic'])
 elif fault=='duplicate_todo':todo.append(row['diagnostic'])
 elif fault=='duplicate_region':
    source+='\n--[=[\n'+row['luaRegion']+']=]\n';row['luaPhaseSha256']=digest(source.encode())
 elif fault in ('observable_alias','observable_copy'):
    region=row['luaRegion'];alias=row['alias'] if fault=='observable_alias' else row['copyAlias']
    source=source.replace(region,region+'                quest:Log('+alias+')\n',1);row['luaPhaseSha256']=digest(source.encode())
 elif fault.startswith('fact_'):
    facts=deepcopy(resource_call_evidence(image,fn,spec,include_pause=True));key=int(row['dispatch'],16)
    if fault=='fact_missing':facts.pop(key)
    elif fault=='fact_family':facts[key]['family']='self'
    elif fault=='fact_offset':facts[key]['offset']=4
    elif fault=='fact_slot':facts[key]['slot']=0x5e8
    elif fault=='fact_argument':facts[key]['pushes']=[('imm',0)]
    fact_fn=lambda *args,**kwargs:facts
 else:
    raw=bytearray((W/'native_function.bin').read_bytes());base=int(fn['address'],16);site=int(row['nativeRead'],16)
    changes={'native_field':(site+2,b'\x08'),'native_pointer_register':(site+1,b'\x46'),
      'native_table_register':(site+4,b'\x1b'),'native_receiver':(site+8,b'\xce'),
      'native_cache_disp':(site+12,b'\x14'),'native_cache_value':(site+10,b'\x44'),
      'native_argument':(site+6,b'\x00'),'native_slot':(site+15,b'\xe8'),'native_owner':(0xe54e9b,b'\xf0')}
    if fault=='branch_entry':
       decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
       jump=next(i for i in decoder.disasm(bytes(raw),base) if i.group(x86.X86_GRP_JUMP) and i.size==6)
       address=jump.address+2;data=struct.pack('<i',site+3-(jump.address+jump.size))
    else:address,data=changes[fault]
    raw[address-base:address-base+len(data)]=data;patches[(base,len(raw))]=bytes(raw)
    facts=resource_call_evidence(image,fn,spec,include_pause=True);image=Overlay();row['nativeSha256']=digest(bytes(raw))
    fact_fn=lambda *args,**kwargs:facts
 with pytest.raises(ValueError):run((source,todo,fn,row,spec,image),fact_fn)
