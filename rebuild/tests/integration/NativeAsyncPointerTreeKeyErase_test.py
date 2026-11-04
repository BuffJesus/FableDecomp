"""Fresh seven-source/ten-body native tree acceptance, no retained objects."""
import os,argparse,hashlib,importlib.util,json,re,struct,subprocess,sys,tempfile
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import pefile
R=next(p for p in Path(__file__).resolve().parents if (p/'rebuild').is_dir())
W=Path(__file__).resolve().parent;sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare
KEY='?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEIABQAVCBankFileAsyncData@@@Z'
EQUAL='?equal_range@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@U12@@2@ABQAVCBankFileAsyncData@@@Z'
DIST='??$__distance@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@0@0ABUinput_iterator_tag@0@@Z'
RANGE='?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@0@Z'
CLEAR='?_M_erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXPAU?$_Rb_tree_node@PAVCBankFileAsyncData@@@2@@Z'
REBAL='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
MIN='?_S_minimum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z';MAX='?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z'
LEFT='?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z';RIGHT='?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z'
SPECS=[(KEY,0,0x9d7f40,83),(EQUAL,1,0x9d7c80,83),(DIST,2,0x9d7550,78),(RANGE,3,0x9d7760,189),(CLEAR,4,0x9d7430,51),(REBAL,5,0x42a34e,572),(MIN,6,0x41c7e0,26),(MAX,6,0x41c800,26),(LEFT,5,0x42951b,64),(RIGHT,5,0x42955b,64)]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 assert not os.environ.get('CL') and not os.environ.get('_CL_')
 parser=argparse.ArgumentParser();parser.add_argument('--include',type=Path,default=R/'rebuild/include');parser.add_argument('--key-source',type=Path,default=R/'rebuild/src/compiled/00/9d/STLAsyncPointerTree_KeyErase_009d7f40.cpp');parser.add_argument('--equal-source',type=Path,default=R/'rebuild/src/compiled/00/9d/STLAsyncPointerTree_EqualRange_009d7c80.cpp');args=parser.parse_args()
 args.include=args.include.resolve();args.key_source=args.key_source.resolve();args.equal_source=args.equal_source.resolve()
 sources=[args.key_source,args.equal_source,*[R/'rebuild/src/compiled'/p for p in ('00/9d/STLAsyncPointerTree_Distance_009d7550.cpp','00/9d/STLAsyncPointerTree_RangeErase_009d7760.cpp','00/9d/STLAsyncBankTree_erase_009d7430.cpp','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp')]]
 required=W/'NativeAsyncPointerTreeKeyErase_required.cpp';model=W/'NativeAsyncPointerTreeKeyErase_model.py'
 inputs={Path(__file__).resolve(),model,required,R/'rebuild/tests/integration/NativeAsyncPointerTreeRangeErase_model.py',EXE};seen=set()
 def headers(p):
  p=p.resolve()
  if p in seen:return
  seen.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',p.read_text(),re.M):
   q=next((q for q in (p.parent/name,args.include/name) if q.is_file()),None);assert q,(p,name);headers(q)
 for p in [*sources,required]:headers(p)
 for name in ('check_one.py','verify_and_land.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 inputs.add(R/'tools/artifact_layout.py')
 for p in W.glob('gate_native_async_pointer_tree*.py'):inputs.add(p)
 for name in ('gate_native_async_pointer_tree_key_erase.py','gate_native_async_pointer_tree_equal_range.py'):
  p=R/'tools/decomp_pipeline'/name
  if p.is_file():inputs.add(p)
 toolchain={V.VC/'bin'/n for n in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')};inputs.update(toolchain)
 for directory in (V.VC/'include',V.VC/'lib'):
  inputs.update(p for p in directory.rglob('*') if p.is_file())
 hashes=lambda:{str(p):sha(p) for p in sorted(inputs)}
 before=hashes();raw=EXE.read_bytes();pe=pefile.PE(data=raw);imports={i.address:(e.dll.decode(),i.name.decode() if i.name else None) for e in pe.DIRECTORY_ENTRY_IMPORT for i in e.imports}
 assert read_virtual_data(raw,0xbfea14,6)==bytes.fromhex('ff2554014401') and imports[0x1440154][0].lower()=='msvcr71.dll' and imports[0x1440154][1]=='free'
 build=Path(tempfile.mkdtemp(prefix='native-async-key-erase-'));flags=['/nologo','/c','/O2','/MT','/I'+str(args.include)];objects=[];coffs=[];object_before={}
 (build/'before.json').write_text(json.dumps(before,indent=2)+'\n')
 for i,p in enumerate(sources):
  obj=build/('provider%d.obj'%i);command=[*flags,'/Fo'+str(obj),str(p)];r=V.cl(command,V.env(),build)
  (build/('provider%d.compile.json'%i)).write_text(json.dumps(dict(command=command,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2)+'\n')
  assert r.returncode==0,r.stdout+r.stderr;objects.append(obj);object_before[str(obj)]=sha(obj);coffs.append(Coff(obj))
 targets={name:address for name,index,address,size in SPECS};targets['_free']=0xbfea14;original={};compiled={};audit=[];md=Cs(CS_ARCH_X86,CS_MODE_32)
 for symbol,index,address,size in SPECS:
  c=coffs[index];section,offset=c.symbols[symbol];assert offset==0;body,rels=c.sections[section];assert len(body)==size,(symbol,len(body),size)
  patched=bytearray(body);fixups=[]
  for site,(name,kind) in rels.items():
   assert kind==20 and name in targets and struct.unpack_from('<I',body,site)[0]==0
   struct.pack_into('<I',patched,site,(targets[name]-address-site-4)&0xffffffff);fixups.append(dict(site=site,symbol=name,kind=kind,target=hex(targets[name])))
  retail=read_virtual_data(raw,address,size);exact=bytes(patched)==retail;original[address]=retail;compiled[address]=bytes(patched)
  fixed_retail_sha256={0x9d7f40:'c875dfffd39f3ffb076e91363c9fca415df6876e1b5507629461ba2b73de0712',0x9d7c80:'24c1c0cfeb9e0f3d764a9a6b47a03d7566de960c9e6eb7f4905ef993387e9450',0x9d7550:'578d974e61be9db592105b3cce5aa69314cfaa9a42ec44a6e2d2c81330ea6877'}
  if address in fixed_retail_sha256:assert hashlib.sha256(retail).hexdigest()==fixed_retail_sha256[address]
  instructions=list(md.disasm(body,address));assert sum(i.size for i in instructions)==size and instructions[-1].mnemonic=='ret'
  if symbol==KEY:
   assert rels=={20:(EQUAL,20),40:(DIST,20),69:(RANGE,20)},rels
   score=asdict(compare(bytes(patched),retail));assert score['near_match'] and not exact and score['retail_instructions']==score['built_instructions']==33
   grade='NEAR_MATCH';assert instructions[-1].op_str=='4'
  else:assert exact,(symbol,'complete retail mismatch');score=None;grade='MATCH'
  if symbol==EQUAL:assert not rels and len(instructions)==38 and instructions[-1].op_str=='8'
  audit.append(dict(symbol=symbol,source=str(sources[index]),address=hex(address),bytes=size,instructions=len(instructions),grade=grade,exact=exact,similarity=score,fixups=fixups,object_sha256=sha(objects[index]),body_sha256=hashlib.sha256(patched).hexdigest()))
 ro=build/'required.obj';command=[*flags,'/Fo'+str(ro),str(required)];r=V.cl(command,V.env(),build)
 (build/'required.compile.json').write_text(json.dumps(dict(command=command,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2)+'\n')
 assert r.returncode==0,r.stdout+r.stderr;object_before[str(ro)]=sha(ro);caller=Coff(ro)
 caller_receipts=[]
 for target in (KEY,EQUAL):
  calls=[(s,site,b) for s,(b,rr) in caller.sections.items() for site,(name,kind) in rr.items() if name==target and kind==20 and struct.unpack_from('<I',b,site)[0]==0];assert len(calls)==1
  section,site,body=calls[0];ins=list(md.disasm(body,0));call=next(i for i,x in enumerate(ins) if x.mnemonic=='call' and x.address+1==site)
  expected_key=[('mov','eax, dword ptr [esp + 8]'),('mov','ecx, dword ptr [esp + 4]'),('push','eax')]
  expected_equal=[('mov','eax, dword ptr [esp + 0xc]'),('mov','ecx, dword ptr [esp + 8]'),('push','esi'),('mov','esi, dword ptr [esp + 8]'),('push','eax'),('push','esi')]
  assert [(x.mnemonic,x.op_str) for x in ins[:call]]==(expected_key if target==KEY else expected_equal)
  assert [(x.mnemonic,x.op_str) for x in ins[call+1:]]==([('ret','')] if target==KEY else [('mov','eax, esi'),('pop','esi'),('ret','')])
  caller_receipts.append(dict(symbol=target,site=site,disassembly=[dict(mnemonic=i.mnemonic,operands=i.op_str) for i in ins],cdecl_caller_thiscall_provider=True,this_register='ECX',argument_words=1 if target==KEY else 2,callee_cleanup=4 if target==KEY else 8,hidden_pair_output=(target==EQUAL)))
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)];omissions=[]
 for index,symbol in enumerate((KEY,EQUAL,DIST,RANGE,CLEAR,REBAL,MIN)):
  r=subprocess.run(command+[str(p) for i,p in enumerate(objects) if i!=index],env=V.env(),capture_output=True,text=True);diagnostic=r.stdout+r.stderr
  assert r.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines()),(index,diagnostic)
  omissions.append(dict(source=str(sources[index]),symbol=symbol,returncode=r.returncode,diagnostic=diagnostic))
 control=subprocess.run(command+[str(p) for p in objects],env=V.env(),capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
 assert object_before=={str(p):sha(p) for p in [*objects,ro]}
 spec=importlib.util.spec_from_file_location('native_key_model',model);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
 domain=m.run_domain(original,compiled);assert (domain['cases_per_mode'],domain['snapshots_per_mode'],domain['matched'],domain['absent'])==(6000,15000,3000,3000)
 assert domain['mutation_partitions']=={0:2000,1:2000,3:2000}
 equal_domain=m.run_equal_domain(original,compiled)
 assert hashes()==before and object_before=={str(p):sha(p) for p in [*objects,ro]}
 report=dict(status='NATIVE_ASYNC_POINTER_TREE_KEY_ERASE_PASS',providers=audit,key_erase_grade='NEAR_MATCH',equal_range_grade='MATCH',domain=domain,equal_range_domain=equal_domain,source_omissions=omissions,link_control_passed=True,link_executable_executed=False,caller_abi=caller_receipts,input_sha256_before=before,input_sha256_after=hashes(),object_sha256=object_before,flags=flags,free_import=dict(thunk='00BFEA14',iat='01440154',dll='MSVCR71.dll',symbol='free'),boundary='Seven freshly compiled source objects/ten complete bodies. Key near grade retains exact mismatch; all nine children exact. Independent RB/physical deletion and full32KiB snapshots, standalone equal-range out/key/tree/header aliases; genuinecdeclfree only. No physicalheap/async lifetime/concurrency/fullengine startup claim.')
 receipt=build/'result.json';receipt.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],6000,15000,equal_domain['cases_per_mode']);print('RECEIPT '+str(receipt),flush=True)
if __name__=='__main__':main()
