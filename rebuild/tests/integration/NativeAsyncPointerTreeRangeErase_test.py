"""Fresh real range/clear/rebalance/extrema composition, no scratch inputs."""
import hashlib,importlib.util,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
import pefile
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
W=Path(__file__).parent;MODEL=W/'NativeAsyncPointerTreeRangeErase_model.py';REQUIRED=W/'NativeAsyncPointerTreeRangeErase_required.cpp'
ROOT='?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@0@Z'
CLEAR='?_M_erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@PAVCBankFileAsyncData@@@2@@Z'
REBAL='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
MIN='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';MAX='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z'
LEFT='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';RIGHT='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
sources=[R/'rebuild/src/compiled'/p for p in ('00/9d/STLAsyncPointerTree_RangeErase_009d7760.cpp','00/9d/STLAsyncBankTree_erase_009d7430.cpp','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp')]
specs=[(ROOT,0,0x9d7760,189),(CLEAR,1,0x9d7430,51),(REBAL,2,0x42a34e,572),(MIN,3,0x41c7e0,26),(MAX,3,0x41c800,26),(LEFT,2,0x42951b,64),(RIGHT,2,0x42955b,64)]
targets={name:address for name,index,address,size in specs};targets['_free']=0xbfea14
inputs={Path(__file__),MODEL,REQUIRED,R/'tools/decomp_pipeline/gate_native_async_pointer_tree_range_erase.py'};seen=set()
def headers(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
  q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q;headers(q)
for p in [*sources,REQUIRED]:headers(p)
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/name)
inputs.add(R/'tools/artifact_layout.py')
def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
def tools():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll'))}
before=hashes();tc=tools();raw=EXE.read_bytes();pehash=hashlib.sha256(raw).hexdigest();pe=pefile.PE(data=raw);imports={i.address:(e.dll.decode(),i.name.decode() if i.name else None) for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports}
thunk=read_virtual_data(raw,0xbfea14,6);assert thunk==bytes.fromhex('ff2554014401') and imports[0x1440154][0].lower()=='msvcr71.dll' and imports[0x1440154][1]=='free'
build=Path(tempfile.mkdtemp(prefix='native-range-erase-'));objects=[];coffs=[];flags=['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include')]
for i,p in enumerate(sources):
 obj=build/('provider%d.obj'%i);r=V.cl(flags+['/Fo'+str(obj),str(p)],V.env(),build);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj);coffs.append(Coff(obj))
bodies={};audit=[];md=Cs(CS_ARCH_X86,CS_MODE_32)
for symbol,index,address,size in specs:
 c=coffs[index];s,o=c.symbols[symbol];assert o==0;b,rels=c.sections[s];assert len(b)==size;patched=bytearray(b);fixups=[]
 for site,(name,kind) in rels.items():
  assert kind==20 and name in targets and struct.unpack_from('<I',b,site)[0]==0;struct.pack_into('<I',patched,site,(targets[name]-address-site-4)&0xffffffff);fixups.append(dict(site=site,symbol=name,kind=kind,addend=0,target=hex(targets[name])))
 assert bytes(patched)==read_virtual_data(raw,address,size);ins=list(md.disasm(b,address));assert sum(i.size for i in ins)==size and ins[-1].mnemonic=='ret'
 if symbol==ROOT:assert len(ins)==78 and ins[-1].op_str=='8' and rels=={42:(CLEAR,20),152:(REBAL,20),165:('_free',20)}
 bodies[address]=bytes(patched);audit.append(dict(symbol=symbol,source=str(sources[index].relative_to(R)),address=hex(address),bytes=size,instructions=len(ins),grade='RELOCATION_MATCH',fixups=fixups,body_sha256=hashlib.sha256(patched).hexdigest(),object_sha256=hashlib.sha256(objects[index].read_bytes()).hexdigest()))
ro=build/'required.obj';r=V.cl(flags+['/Fo'+str(ro),str(REQUIRED)],V.env(),build);assert r.returncode==0,r.stdout+r.stderr;caller=Coff(ro)
calls=[(s,site) for s,(b,rr) in caller.sections.items() for site,(name,kind) in rr.items() if name==ROOT and kind==20 and struct.unpack_from('<I',b,site)[0]==0];assert len(calls)==1
caller_body=caller.sections[calls[0][0]][0];caller_ins=list(md.disasm(caller_body,0));call_index=next(i for i,x in enumerate(caller_ins) if x.address+1==calls[0][1] and x.mnemonic=='call')
expected_prefix=[('mov','ecx, dword ptr [esp + 8]'),('push','esi'),('mov','esi, dword ptr [esp + 8]'),('push','ecx'),('lea','eax, [ecx + esi*4]'),('mov','edx, esp'),('mov','dword ptr [edx], eax'),('push','ecx'),('mov','eax, esp'),('mov','dword ptr [eax], ecx')]
assert [(x.mnemonic,x.op_str) for x in caller_ins[:call_index]]==expected_prefix
assert [(x.mnemonic,x.op_str) for x in caller_ins[call_index+1:]]==[('mov','eax, esi'),('pop','esi'),('ret','')]
caller_abi=dict(symbol=ROOT,relocation_site=calls[0][1],this_register='ECX=argv complete tree',argument_words=2,byvalue_iterators=True,callee_cleanup=8,caller_stack_cleanup_after_call=0,object_sha256=hashlib.sha256(ro.read_bytes()).hexdigest(),disassembly=[dict(address=hex(x.address),mnemonic=x.mnemonic,operands=x.op_str) for x in caller_ins])
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)];omissions=[]
for index,symbol in enumerate((ROOT,CLEAR,REBAL,MIN)):
 r=subprocess.run(cmd+[str(p) for i,p in enumerate(objects) if i!=index],env=V.env(),capture_output=True,text=True);diagnostic=r.stdout+r.stderr;assert r.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines());omissions.append(dict(source=str(sources[index].relative_to(R)),symbol=symbol,diagnostic=diagnostic))
r=subprocess.run(cmd+[str(p) for p in objects],env=V.env(),capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
spec=importlib.util.spec_from_file_location('native_range_model',MODEL);f=importlib.util.module_from_spec(spec);spec.loader.exec_module(f)
phases={}
for phase,run_domain in (('baseline',f.run_baseline),('supplement',f.run_supplement)):
 original=f.Runner(f.bodies);compiled=f.Runner(bodies);a=run_domain(original);b=run_domain(compiled);assert a==b
 assert original.hits==compiled.hits;phases[phase]=dict(model=a,mode_provider_hits=dict(original=original.hits,compiled=compiled.hits))
assert (phases['baseline']['model']['cases'],phases['baseline']['model']['snapshots'])==(4448,16648)
assert (phases['supplement']['model']['supplement_cases'],phases['supplement']['model']['snapshots'])==(80,704)
assert phases['supplement']['model']['partitions']=={name:16 for name in ('count_live_interior','count_live_fullclear','late_header_fullclear','cached_left_fullclear','cached_successor_interior')}
assert all(n>0 for n in phases['baseline']['mode_provider_hits']['compiled'].values())
assert phases['baseline']['mode_provider_hits']['compiled']=={'0x9d7760':4448,'0x9d7430':1312,'0x42a34e':6688,'0x41c7e0':656,'0x41c800':360,'0x42951b':792,'0x42955b':864}
assert hashes()==before and tools()==tc and hashlib.sha256(EXE.read_bytes()).hexdigest()==pehash
report=dict(status='NATIVE_ASYNC_POINTER_TREE_RANGE_ERASE_PASS',providers=audit,phases=phases,source_omissions=omissions,link_control_passed=True,link_executable_executed=False,translation_cache_policy='Separate immutable Unicorn machines',input_sha256_before=before,input_sha256_after=hashes(),toolchain_sha256_before=tc,toolchain_sha256_after=tools(),original_exe_sha256=pehash,free_import=dict(thunk='00BFEA14',iat='01440154',dll='MSVCR71.dll',symbol='free'),boundary='Genuine exact189B range root plus six existing actual child roles from three childTUs; only cdeclfree boundary. Independent validRB deletion/fullclear model and80distinct temporalmutations, full32KiB snapshots. Rawpointer pointees untouched; no physicalheap/lifetime/concurrency/destructorownership closure or extra child/helper credit.')
report['caller_abi']=caller_abi
out=R/'rebuild/compile-gate/native-async-pointer-tree-range-erase.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],4448,16648,80,704,flush=True)
