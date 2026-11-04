"""Native request submission and exact pending helper, with explicit map boundaries."""
import hashlib,json,re,struct,subprocess
from datetime import datetime,timezone
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from native_exact_roots import import_names
from threaded_manager_add_oracle import compare_add,compare_pending
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_AddObject_0098f0f0.cpp'
FIXTURE=R/'rebuild/tests/integration/NativeThreadedManagerAdd_test.cpp'
SYMBOL='?AddObject@CThreadedFileManager@@QAEKABVCThreadedFileObjectDesc@@AAVCThreadedFile@@@Z'
PENDING='?AddObjectToPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z'
UID_SLOT='??A?$map@KPAVCFileObject@CThreadedFileManager@@U?$less@K@_STL@@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@4@@_STL@@QAEAAPAVCFileObject@CThreadedFileManager@@ABK@Z'
PENDING_SLOT='??A?$map@UCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@_STL@@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@5@@_STL@@QAEAAPAVCFileObject@CThreadedFileManager@@ABUCPendingObjectSortKey@3@@Z'
ADDRESS={'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10,'__imp__GlobalAlloc@8':0x143fdd4,'__imp__SetEvent@4':0x143fde0,'??2@YAPAXI@Z':0xbfea1a,UID_SLOT:0x9913d0,PENDING_SLOT:0x991350}
ROOTS=[dict(name='add_object',source=SOURCE,symbol=SYMBOL,address=0x98f0f0,size=240),dict(name='add_pending',source=SOURCE,symbol=PENDING,address=0x98eca0,size=44)]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 build=R/'rebuild/build/native-manager-add/00/98/0098f0f0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 inputs={EXE,Path(__file__),R/'tools/decomp_pipeline/threaded_manager_add_oracle.py',R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py'}
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 closure(SOURCE);closure(FIXTURE)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'));inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):(build/name).write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins)
 for p in inputs:
  if p.is_relative_to(R):
   out=build/'snapshot'/p.relative_to(R);out.parent.mkdir(parents=True,exist_ok=True);out.write_bytes(p.read_bytes())
 env=V.env()
 def compile(name,source):
  obj=build/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/W3','/I'+str(R/'rebuild/include'),'/I'+str(FIXTURE.parent),'/Fo'+str(obj),str(source)],env,build)
  save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr;return obj
 obj=compile('provider',SOURCE);test=compile('fixture',FIXTURE);coff=Coff(obj);raw=EXE.read_bytes();imports=import_names(raw);audits=[];code={}
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_OP_IMM,CS_OP_MEM
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
 for root in ROOTS:
  entry=root['address'];sec,off=coff.symbols[root['symbol']];body,rels=coff.sections[sec];assert off==0
  original=read_virtual_data(raw,entry,root['size']);patched=bytearray(body);audit=[]
  for site,(name,kind) in sorted(rels.items()):
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0;target=ADDRESS[name]
   if name.startswith('__imp__'):assert imports[target]==name[7:].split('@')[0]
   struct.pack_into('<I',patched,site,(target-(entry+site+4 if kind==20 else 0))&0xffffffff);audit.append(dict(site=site,symbol=name,kind=kind,target=target))
  grade=compare(body,original,tuple(rels),retail_relocations('%08x'%entry,root['size'])).to_dict();assert grade['near_match'],grade
  if root['name']=='add_pending':assert bytes(patched)==original and len(body)==44 and len(rels)==1
  else:
   assert len(body)==244 and len(rels)==7 and grade['opcode_similarity']>=.90
   targets=[]
   for i in md.disasm(original,entry):
    if i.mnemonic=='call':
     op=i.operands[0];targets.append(op.imm&0xffffffff if op.type==CS_OP_IMM else op.mem.disp&0xffffffff)
   assert sorted(targets)==sorted(x['target'] for x in audit)
  code[entry]=bytes(patched);audits.append(dict(name=root['name'],grade='MATCH' if bytes(patched)==original else 'NEAR_MATCH',metrics=grade,bytes=len(body),relocations=audit))
 behavior=compare_add(code[0x98f0f0],read_virtual_data(raw,0x98f0f0,240));assert behavior['cases']==288
 pending_behavior=compare_pending(code[0x98eca0],read_virtual_data(raw,0x98eca0,44));assert pending_behavior['cases']==48
 def link(name,objects):
  exe=build/(name+'.exe');r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objects),'kernel32.lib'],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,r=link('native',[obj,test]);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=build,capture_output=True,text=True,timeout=30);save('native.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and 'NATIVE_MANAGER_ADD_PASS cases=144 frees=144 maps=288' in r.stdout,r.stdout+r.stderr
 _,r=link('missing',[test]);assert r.returncode and SYMBOL in r.stdout+r.stderr
 controls=[];text=SOURCE.read_text(encoding='utf-8')
 for name,source in (('no-event',text.replace('    SetEvent(Event);','')),('wrong-uid',text.replace('    return uid;','    return uid+1;'))):
  p=build/(name+'.cpp');p.write_text(source);control=compile(name,p);broken,r=link(name,[control,test]);assert r.returncode==0,r.stdout+r.stderr
  r=subprocess.run([str(broken)],cwd=build,capture_output=True,text=True,timeout=30);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==81 and 'NATIVE_MANAGER_ADD_FAILURE' in r.stderr;controls.append(name)
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),roots=audits,behavior=behavior,pending_behavior=pending_behavior,native_cases=144,heap_frees=144,map_calls=288,source_omissions=1,runtime_controls=controls,input_sha256=pins,object_sha256={str(p):sha(p) for p in (obj,test)},executable_sha256=sha(exe),boundaries=['Original/rebuilt/model full24KiB state and ABI:288 cases, both devices, six priorities, UID wrap, GlobalAlloc null/success and ignored SetEvent result.','Native CRT allocation, GlobalAlloc/GlobalFree, actual critical section and event:144 cases; UID/pending map slots controlled, no production queue/tree or worker claim.','Whole native provider links without forcing duplicates. Exact44B pending helper and30B object ctor are shared inline providers.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-manager-add.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_MANAGER_ADD_PASS cases=288 native=144 omissions=1 controls=4 pending_cases=48 add=NEAR_MATCH pending=MATCH');print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
