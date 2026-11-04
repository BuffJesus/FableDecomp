"""Recreated submission with actual UID/pending trees; predecessor variant stays qualified."""
import hashlib,json,re,struct,subprocess
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
import threaded_submission_tree_oracle as T
import threaded_pending_removal_oracle as M
import gate_native_threaded_manager_add as A
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
R=Path(__file__).resolve().parents[2]
FIXTURE=R/'rebuild/tests/integration/NativeThreadedRetirement_test.cpp'
ROOTS=[M.roots[-1]]
EXTRA_ROOTS=[dict(r) for r in M.G.ROOTS if r['name'] in ('remove_object','uid_find')]
EXTRA_ROOTS.append(dict(name='uid_iterator_erase',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_IteratorErase_00990430.cpp',symbol=M.G.ERASE,address=0x990430,size=50))

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 build=R/'rebuild/build/native-threaded-retirement/00/98/0098eb20'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 inputs={EXE,Path(__file__),Path(T.__file__),Path(A.__file__),Path(M.__file__),Path(M.G.__file__)}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py','threaded_manager_add_oracle.py','movable_pool_construct_oracle.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 for p in T.SOURCES+[r['source'] for r in M.roots+EXTRA_ROOTS]+[A.SOURCE,FIXTURE]:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'));inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins)
 for p in inputs:
  if p.is_relative_to(R):
   q=build/'snapshot'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 original,rebuilt,audits,objects,addresses=M.build_codes(build)
 extra_build=build/'retirement';extra_build.mkdir()
 addresses.update({r['symbol']:r['address'] for r in EXTRA_ROOTS})
 extra_codes=M.compile_codes(EXTRA_ROOTS,addresses,extra_build)
 for r in EXTRA_ROOTS:
  original[r['address']]=read_virtual_data(EXE.read_bytes(),r['address'],r['size'])
  rebuilt[r['address']]=extra_codes[r['address']];assert rebuilt[r['address']]==original[r['address']],r['name']
 source_objects={};extra_objects=[]
 for root in M.roots+EXTRA_ROOTS:
  source=root['source']
  if source not in source_objects:
   obj=(extra_build/('provider%d.obj'%EXTRA_ROOTS.index(root))) if root in EXTRA_ROOTS else build/('provider%d.obj'%len(source_objects));source_objects[source]=obj;extra_objects.append(obj)
  c=Coff(source_objects[source]);sec,off=c.symbols[root['symbol']];assert off==0;body,rels=c.sections[sec]
  va=root['address'];original_body=read_virtual_data(EXE.read_bytes(),va,root['size'])
  metrics=compare(body,original_body,tuple(rels),retail_relocations('%08x'%va,root['size'])).to_dict()
  assert metrics['near_match'] and metrics['opcode_similarity']>=.90
  md=M.T.Cs(M.T.CS_ARCH_X86,M.T.CS_MODE_32);md.detail=True
  calls=[(i.operands[0].imm if i.operands[0].type==M.T.CS_OP_IMM else i.operands[0].mem.disp)&0xffffffff for i in md.disasm(original_body,va) if i.mnemonic=='call']
  assert sorted(calls)==sorted(addresses[name] for name,kind in rels.values()),root['name']
  entry=dict(symbol=root['symbol'],address=va,size=root['size'],source=str(source),body=body.hex(),bytes=len(body),grade='MATCH' if rebuilt[va]==original[va] else 'NEAR_MATCH',metrics=metrics,relocations=[dict(site=site,symbol=name,target=addresses[name]) for site,(name,kind) in rels.items()])
  previous=next((a for a in audits if a['address']==va),None)
  if previous:assert previous['body']==entry['body'] and previous['relocations']==entry['relocations']
  else:audits.append(entry)
 objects.extend(extra_objects)
 assert len(audits)==29 and [a['grade'] for a in audits].count('FUNCTIONAL_VARIANT')==1
 behavior=M.compare_removal(original,rebuilt);assert (behavior['cases'],behavior['removals'],len(behavior['controls']))==(360,2016,13)
 env=V.env()
 def compile(name,p):
  obj=build/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/W3','/I'+str(R/'rebuild/include'),'/I'+str(FIXTURE.parent),'/Fo'+str(obj),str(p)],env,build);save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr;return obj
 parent=compile('parent',A.SOURCE);fixture=compile('fixture',FIXTURE);objects.extend((parent,fixture))
 c=Coff(parent);sec,off=c.symbols[A.SYMBOL];body,rels=c.sections[sec];assert off==0 and len(body)==244 and len(rels)==7
 parent_grade=compare(body,read_virtual_data(EXE.read_bytes(),0x98f0f0,240),tuple(rels),retail_relocations('0098f0f0',240)).to_dict();assert parent_grade['near_match'] and parent_grade['opcode_similarity']>=.90
 audits.append(dict(symbol=A.SYMBOL,address=0x98f0f0,size=240,index=6,body=body.hex(),bytes=len(body),grade='NEAR_MATCH',metrics=parent_grade,relocations=[dict(site=site,symbol=name,target=A.ADDRESS[name]) for site,(name,kind) in rels.items()]))
 exports=['/export:Root%d=%s'%(i,a['symbol']) for i,a in enumerate(audits)]
 def link(name,objs,export=False):
  exe=build/(name+'.exe');r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib',*(exports if export else [])],env=env,cwd=build,capture_output=True,text=True,timeout=30);save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 def verify(exe):
  with pefile.PE(str(exe)) as pe:
   exports={x.name.decode():x.address for x in pe.DIRECTORY_ENTRY_EXPORT.symbols if x.name}
   for i,a in enumerate(audits):
    selected=bytearray(pe.get_data(exports['Root%d'%i],a['bytes']))
    for rel in a['relocations']:selected[rel['site']:rel['site']+4]=b'\0'*4
    assert selected==bytes.fromhex(a['body']),('linked body changed',a['symbol'])
 def run(name,exe):
  r=subprocess.run([str(exe)],cwd=build,capture_output=True,text=True,timeout=30);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return r
 exe,r=link('native',objects,True);assert r.returncode==0,r.stdout+r.stderr;verify(exe)
 r=run('native',exe);assert r.returncode==0 and 'NATIVE_RETIREMENT_PASS cases=60 submissions=336 removals=336 objects=336 refusals=672 missing=336 headers=900' in r.stdout,r.stdout+r.stderr
 omissions=[];redundant=[]
 for index in range(6,len(objects)-1):
  omitted,r=link('omitted%d'%index,[p for i,p in enumerate(objects) if i!=index],True)
  if r.returncode:
   omissions.append(dict(index=index,link_failed=True))
  else:
   verify(omitted);result=run('omitted%d'%index,omitted);assert result.returncode==0
   redundant.append(dict(index=index,identical_selected_bodies=True,native_pass=True))
 source=EXTRA_ROOTS[0]['source'];text=source.read_text(encoding='utf-8');remove_index=objects.index(source_objects[source])
 controls=[('missing-destruction',text.replace('        delete object;','        (void)object;')),('missing-uid-removal',text.replace('        Objects.erase(it);','        (void)it;')),('false-acceptance',text.replace('            return false;','            return true;')),('false-missing',text.replace('    if (it!=Objects.end()) {','    if (it==Objects.end()) { LeaveCriticalSection(section); return false; }\n    if (it!=Objects.end()) {'))]
 for name,changed in controls:
  assert changed!=text;p=build/(name+'.cpp');p.write_text(changed);obj=compile(name,p)
  bad,r=link(name,[obj if i==remove_index else p for i,p in enumerate(objects)]);assert r.returncode==0,r.stdout+r.stderr
  result=run(name,bad);assert result.returncode==82 and 'NATIVE_SUBMISSION_FAILURE' in result.stderr,(name,result.stdout,result.stderr)
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),roots=audits,behavior=behavior,native_cases=60,submissions=336,pending_removals=336,objects_released=336,uid_nodes_released_by_fixture=0,refusals=672,missing_uids=336,headers_released=900,new_roots=0,linked_bodies_verified=len(audits),functional_variants=1,source_omissions=omissions,native_controls=[name for name,s in controls],emulated_controls=13,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),boundaries=['Actual AddObject insertion followed by actual RemoveObjectFromPendingMap lookup/erase/extrema/rebalancing; real CRT nodes and requests, Windows event/critical section and overlap allocation lifetime.','360 original/rebuilt/model sequences and2016 removals check full guarded storage, exact freed node, tree invariants, cursor and x86 ABI. Native fixture checks all queues, surviving UID entries and request/file storage after336 retirements; protected calls preserve guarded manager/request bytes.','Actual RemoveObject retires336 requests and UID nodes,168 pending and168 finished.672 protected-state refusals and336 missing-UID returns checked. Fixture frees900 empty headers only; production manager startup and worker handoff remain outside this test.','Existing78B predecessor remains FUNCTIONAL_VARIANT; no new helper acceptance. Previously accepted91B pending removal root NEAR_MATCH97.142857%,35 instructions equal to original.'])
 report['redundant_source_omissions']=redundant
 assert len(omissions)==9 and len(redundant)==1
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-retirement.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_RETIREMENT_PASS sequences=360 removals=2016 native=60 submissions=336 refusals=672 missing=336 bodies=%d functional_variants=1 omissions=%d controls=17'%(len(audits),len(omissions)));print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
