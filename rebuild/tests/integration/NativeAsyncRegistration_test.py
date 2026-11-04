"""Proposed fresh canonical gate. No scratch-object/hash receipts consumed."""
import hashlib,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=R/'rebuild/tests/integration';source=R/'rebuild/src/compiled/00/9d/STLAsyncPointerTree_InsertUnique_009d7cf0.cpp';required=w/'NativeAsyncRegistration_required.cpp';model=w/'NativeAsyncRegistration_model.py';standalone=w/'NativeAsyncRegistration_decrement_model.py'
ROOT='?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@_N@2@ABQAVCBankFileAsyncData@@@Z'
INSERT='?_M_insert@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@PAU_Rb_tree_node_base@2@0ABQAVCBankFileAsyncData@@0@Z'
DECREMENT='?_M_decrement@_Rb_tree_base_iterator@_STL@@QAEXXZ'
inputs={source,required,model,standalone,Path(__file__),R/'tools/decomp_pipeline/gate_native_async_registration.py'}
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/name)
inputs.add(R/'tools/artifact_layout.py');visited=set()
def headers(p):
 if p in visited:return
 visited.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q.resolve() for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q;headers(q)
headers(source);headers(required)
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
toolchain=[V.VC/'bin'/name for name in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')]
def tools():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in toolchain}
before=hashes();beforetools=tools();build=Path(tempfile.mkdtemp(prefix='native-registration-'));obj=build/'root.obj';ro=build/'required.obj';flags=['/nologo','/c','/O2','/MT']
for src,dst in ((source,obj),(required,ro)):
 result=V.cl(flags+['/I'+str(R/'rebuild/include'),'/Fo'+str(dst),str(src)],V.env(),build);assert result.returncode==0,result.stdout+result.stderr
c=Coff(obj);s,o=c.symbols[ROOT];built,rels=c.sections[s];assert o==0 and len(built)==180 and rels=={84:(INSERT,20),112:(DECREMENT,20),140:(INSERT,20)}
md=Cs(CS_ARCH_X86,CS_MODE_32);ins=list(md.disasm(built,0));assert len(ins)==78 and sum(x.size for x in ins)==180
original=read_virtual_data(EXE.read_bytes(),0x9d7cf0,180);grade=compare(built,original,tuple(rels),retail_relocations('009d7cf0',180)).to_dict();assert grade['near_match'] and grade['opcode_similarity']==1 and grade['instruction_delta']==0
ds,do=c.symbols[DECREMENT];decrement,dr=c.sections[ds];assert do==0 and len(decrement)==78 and not dr
patched=bytearray(built);audit=[]
for site,(name,kind) in rels.items():
 assert struct.unpack_from('<I',built,site)[0]==0;target=0x6000000 if name==DECREMENT else 0x9d7990;struct.pack_into('<I',patched,site,(target-0x9d7cf0-site-4)&0xffffffff);audit.append(dict(offset=site,symbol=name,kind=kind,addend=0,target=hex(target),synthetic=name==DECREMENT))
command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)]
missing=subprocess.run(command,env=V.env(),capture_output=True,text=True);assert missing.returncode and any(ROOT in line and ('LNK2001' in line or 'LNK2019' in line) for line in (missing.stdout+missing.stderr).splitlines())
control=subprocess.run(command+[str(obj)],env=V.env(),capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
def execute(path,text,extra):exec(compile(text,str(path),'exec'),dict(__file__=str(path),__name__='__main__',RUN_AUTHORIZED=True,**extra))
text=model.read_text();execute(model,text,{})
original_result=json.loads((w/'NativeAsyncRegistration_model_result.json').read_text());assert (original_result['cases'],original_result['snapshots'])==(480,1136)
def replace(old,new):
 global text
 assert text.count(old)==1,(old,text.count(old));text=text.replace(old,new)
replace('u.mem_map(A,N);','u.mem_map(0x6000000,4096);u.mem_write(0x6000000,ACTUAL_DECREMENT)\n u.mem_map(A,N);')
replace('u.mem_write(ENTRY,root)','u.mem_write(ENTRY,COMPILED_ROOT)')
replace("status='ORIGINAL_REGISTRATION_MODEL_PASS'","status='COMPILED_REGISTRATION_ACTUAL_DECREMENT_MODEL_PASS'")
execute(model,text,dict(COMPILED_ROOT=bytes(patched),ACTUAL_DECREMENT=decrement))
compiled_result=json.loads((w/'NativeAsyncRegistration_model_result.json').read_text());assert (compiled_result['cases'],compiled_result['snapshots'])==(480,1136)
compiled_result['boundary']='Actual fresh180B root plus emitted78B iterator at synthetic6000000 versus independent model; _M_insert functional RET20 interface. No original-helper byte parity or extra credit.'
execute(standalone,standalone.read_text(),dict(FRESH_OBJECT=obj))
standalone_result=json.loads((w/'NativeAsyncRegistration_decrement_result.json').read_text());assert (standalone_result['cases'],standalone_result['fullarena_final_comparisons'])==(20,40)
assert before==hashes() and beforetools==tools()
report=dict(status='NATIVE_ASYNC_REGISTRATION_PASS',root_symbol=ROOT,grade='NEAR_MATCH',compiled_grade=grade,relocation_audit=audit,source_omission=dict(symbol=ROOT,missing_failed=True,provider_link_passed=True),original_model=original_result,compiled_model=compiled_result,standalone_model=standalone_result,input_sha256=before,toolchain_sha256=beforetools,compiler_flags=flags,object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),emitted_decrement_sha256=hashlib.sha256(decrement).hexdigest(),boundary='Fresh canonical180B/78ins near root only. Actual emitted78B decrement synthetic, standalone20cases not helper recovery credit. _M_insert remains disclosed functional boundary; optional real179B composition belongs separate future canonical child gate. No balancing/lifetime/concurrency closure.')
out=R/'rebuild/compile-gate/native-async-registration.json';out.write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_ASYNC_REGISTRATION_PASS',480,1136,20)
