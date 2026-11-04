from pathlib import Path
from copy import deepcopy
import hashlib,json,struct,sys
import pytest
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86
R=Path(__file__).resolve().parents[2];W=Path(__file__).with_name('fixtures')/'register_dispatch/book_main'
from tools.script_recovery.native_reviewed_register_dispatch_staging import classify
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import resource_call_evidence
from tools.script_recovery.lua_staging_liveness import prove_unused_staging
from tools.script_recovery.readable_lua import tokens
read=lambda p:json.loads(p.read_text(encoding='utf-8'))
sha=lambda data:hashlib.sha256(data).hexdigest()
@pytest.fixture(scope='module')
def fixture():
 unit=read(W/'unit_baseline.json');image=RData();fn=read(W/'native_function.json')
 return ((W/'phase.lua').read_text(encoding='utf-8'),read(W/'phase_todo.json'),fn,read(W/'candidate_row.json'),
    LoweringSpec(unit,'BS_Teacher',entity=True,thing_slots={}),image)
def run(values,facts=resource_call_evidence):return classify(*values,facts,prove_unused_staging)
def test_only_two_diagnostics_and_annotations_change(fixture):
 source,todo,fn,row,spec,image=fixture;before=list(todo)
 out,remaining,reviews=run(fixture)
 assert todo==before and remaining.count('unlifted: CVar12 = *pCVar11')==0
 assert len(todo)-len(remaining)==2 and len(reviews)==2
 significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ('space','comment','longcomment')]
 assert significant(out)==significant(source)
 assert all(review['executableLuaUnchanged'] and review['liveness']['visitedNodes']==19 for review in reviews)
 assert out.count('CVar12 = nil')==source.count('CVar12 = nil')
 assert out.count('--[[reviewed native register-dispatch staging]]')==2
 assert out==(W/'candidate_phase.lua').read_text(encoding='utf-8')
def test_optional_hook_has_no_effect_and_does_not_alias_todo(fixture):
 source,todo,fn,row,spec,image=fixture
 out,remaining,reviews=run((source,todo,fn,None,spec,image))
 assert (out,remaining,reviews)==(source,todo,[]) and remaining is not todo

@pytest.mark.parametrize('fault',[
 'imageSha256','nativeSha256','sourceSha256','luaPhaseSha256','not_entity','resource_name',
 'abi_register','abi_frame','abi_exceptions','abi_reentry','abi_heap','scope','evidence',
 'getter_slot','getter_method','getter_target','getter_bytes','getter_writes_table','getter_far_ret',
 'missing_read','duplicate_read','wrong_native_read','wrong_dispatch','wrong_hero','wrong_register',
 'wrong_literal','wrong_group','wrong_flag','wrong_alias','wrong_diagnostic','wrong_fragment','ambiguous_fragment',
 'wrong_region','wrong_hero_alias','wrong_result_alias','missing_todo','duplicate_region','observable_read',
 'fact_table_missing','fact_call_missing','fact_family','fact_table_register','fact_slot','fact_offset','fact_pushes',
 'native_table_register','native_getter_slot','native_gsi_offset','native_gsi_table','native_owner_origin','branch_entry',
])
def test_stale_or_ambiguous_evidence_is_rejected(fixture,fault):
 source,todo,fn,row,spec,image=fixture
 todo=list(todo);row=deepcopy(row);spec=deepcopy(spec);read_row=row['reads'][0]
 facts=resource_call_evidence(image,fn,spec,include_resource_table_loads=True);fact_fn=resource_call_evidence
 patches={};base_image=image
 class Overlay:
    ok=True;data=base_image.data
    def bytes_at(self,address,size):return patches.get((address,size),base_image.bytes_at(address,size))
    def string_at(self,address):return base_image.string_at(address)
 if fault.endswith('Sha256'):row[fault]='0'*64
 elif fault=='not_entity':spec.entity=False
 elif fault=='resource_name':spec.self_resources[52]='seh_someone_else'
 elif fault.startswith('abi_'):
    key={'abi_register':'ordinaryMsvcCalleeSaved','abi_frame':'callerFramePreserved','abi_exceptions':'exceptionsVerified',
       'abi_reentry':'engineReentryVerified','abi_heap':'heapVerified'}[fault]
    row['abiContract'][key]=not row['abiContract'][key]
 elif fault=='scope':row['scopeLimit']='unconditional proof'
 elif fault=='evidence':row['acceptedEvidenceSha256']=''
 elif fault=='getter_slot':row['getter']['slot']='0x114'
 elif fault=='getter_method':row['getter']['method']='Other'
 elif fault=='getter_target':row['getter']['target']='0x00891ca1'
 elif fault=='getter_bytes':row['getter']['bytes']='00'*173
 elif fault in ('getter_writes_table','getter_far_ret'):
    body=bytearray.fromhex(row['getter']['bytes'])
    if fault=='getter_writes_table':body[0x891d46-0x891ca0:0x891d48-0x891ca0]=bytes.fromhex('31db')
    else:body[-1]=0xcb
    row['getter']['bytes']=body.hex();patches[(0x891ca0,len(body))]=bytes(body);image=Overlay()
 elif fault=='missing_read':row['reads'].pop()
 elif fault=='duplicate_read':row['reads'][1]['nativeRead']=read_row['nativeRead']
 elif fault=='wrong_native_read':read_row['nativeRead']='0x00e55684'
 elif fault=='wrong_dispatch':read_row['dispatch']='0x00e5569c'
 elif fault=='wrong_hero':read_row['heroCall']='0x00e55693'
 elif fault=='wrong_register':read_row['tableRegister']='eax'
 elif fault=='wrong_literal':read_row['literal']='TEXT_QST_OTHER'
 elif fault=='wrong_group':read_row['group']=2
 elif fault=='wrong_flag':read_row['flags'][0]=1
 elif fault=='wrong_alias':read_row['alias']='other'
 elif fault=='wrong_diagnostic':read_row['diagnostic']='unlifted: actual_effect()'
 elif fault=='wrong_fragment':read_row['nativeSourceFragment']='missing native source'
 elif fault=='ambiguous_fragment':read_row['nativeSourceFragment']='CVar12 = *pCVar11;'
 elif fault=='wrong_region':read_row['luaRegion']+='unrelated()\n'
 elif fault=='wrong_hero_alias':read_row['heroAlias']='other'
 elif fault=='wrong_result_alias':read_row['resultAlias']='other'
 elif fault=='missing_todo':todo.remove(read_row['diagnostic'])
 elif fault=='duplicate_region':
    source=source+'\n--[=[\n'+read_row['luaRegion']+']=]\n';row['luaPhaseSha256']=sha(source.encode())
 elif fault=='observable_read':
    old=read_row['luaRegion'];lines=old.splitlines(True);lines.insert(4,'                                quest:Log(CVar12)\n')
    new=''.join(lines);source=source.replace(old,new,1);read_row['luaRegion']=new;row['luaPhaseSha256']=sha(source.encode())
 elif fault.startswith('fact_'):
    facts=deepcopy(facts);site=int(read_row['nativeRead'],16);call=int(read_row['dispatch'],16)
    if fault=='fact_table_missing':facts.pop(site)
    elif fault=='fact_call_missing':facts.pop(call)
    elif fault=='fact_family':facts[site]['family']='parent'
    elif fault=='fact_table_register':facts[site]['tableRegister']='ebp'
    elif fault=='fact_slot':facts[call]['slot']=0x30
    elif fault=='fact_offset':facts[call]['offset']=56
    elif fault=='fact_pushes':facts[call]['pushes']=facts[call]['pushes'][:-1]
    fact_fn=lambda *args,**kwargs:facts
 else:
    raw=bytearray((W/'native_function.bin').read_bytes());base=int(fn['address'],16);site=int(read_row['nativeRead'],16)
    changes={'native_table_register':(site,bytes.fromhex('8b0f')),'native_getter_slot':(int(read_row['heroCall'],16)+2,b'\x14'),
      'native_gsi_offset':(site-3,b'\x08'),'native_gsi_table':(site-1,b'\x10'),'native_owner_origin':(0xe54e9b,b'\xf0')}
    if fault=='branch_entry':
       decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
       jump=next(i for i in decoder.disasm(bytes(raw),base) if i.group(x86.X86_GRP_JUMP) and i.size==6)
       address=jump.address+2;data=struct.pack('<i',site+2-(jump.address+jump.size))
    else:address,data=changes[fault]
    raw[address-base:address-base+len(data)]=data
    patches[(base,len(raw))]=bytes(raw);image=Overlay();row['nativeSha256']=sha(bytes(raw))
    fact_fn=lambda *args,**kwargs:facts
 with pytest.raises(ValueError):run((source,todo,fn,row,spec,image),fact_fn)
