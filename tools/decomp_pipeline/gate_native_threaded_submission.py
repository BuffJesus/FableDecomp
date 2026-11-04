"""Recreated submission with actual UID/pending trees; predecessor variant stays qualified."""
import hashlib,json,re,struct,subprocess
from datetime import datetime,timezone
from pathlib import Path
import pefile
import check_one as V
import threaded_submission_tree_oracle as T
import gate_native_threaded_manager_add as A
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
R=Path(__file__).resolve().parents[2]
FIXTURE=R/'rebuild/tests/integration/NativeThreadedSubmission_test.cpp'
ROOTS=[
 dict(name='pending_index',source=R/'rebuild/src/compiled/00/99/STLThreadedPendingMap_OperatorIndex_00991350.cpp',symbol='??A?$map@UCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@_STL@@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@5@@_STL@@QAEAAPAVCFileObject@CThreadedFileManager@@ABUCPendingObjectSortKey@3@@Z',address=0x991350,size=124),
 dict(name='uid_index',source=R/'rebuild/src/compiled/00/99/STLThreadedUIDMap_OperatorIndex_009913d0.cpp',symbol='??A?$map@KPAVCFileObject@CThreadedFileManager@@U?$less@K@_STL@@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@4@@_STL@@QAEAAPAVCFileObject@CThreadedFileManager@@ABK@Z',address=0x9913d0,size=115),
 dict(name='pending_hint',source=R/'rebuild/src/compiled/00/99/STLThreadedMaps_InsertHint_00990dc0.cpp',symbol='?insert_unique@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@U32@ABU?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@2@@Z',address=0x990dc0,size=610),
 dict(name='uid_hint',source=R/'rebuild/src/compiled/00/99/STLThreadedMaps_InsertHint_00990dc0.cpp',symbol='?insert_unique@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U32@ABU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@2@@Z',address=0x991030,size=518),
 dict(name='pending_link',source=R/'rebuild/src/compiled/00/99/STLThreadedMaps_InsertNode_00990830.cpp',symbol='?_M_insert@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@2@0@Z',address=0x990830,size=201),
 dict(name='uid_link',source=R/'rebuild/src/compiled/00/99/STLThreadedMaps_InsertNode_00990830.cpp',symbol='?_M_insert@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@2@0@Z',address=0x990a10,size=191),
 dict(name='pending_unique',source=R/'rebuild/src/compiled/00/99/STLThreadedMaps_InsertUnique_00990900.cpp',symbol='?insert_unique@?$_Rb_tree@UCPendingObjectSortKey@CThreadedFileManager@@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@U?$_Select1st@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@U?$less@UCPendingObjectSortKey@CThreadedFileManager@@@4@V?$allocator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@4@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@_STL@@@_STL@@_N@2@ABU?$pair@$$CBUCPendingObjectSortKey@CThreadedFileManager@@PAVCFileObject@2@@2@@Z',address=0x990900,size=263),
 dict(name='uid_unique',source=R/'rebuild/src/compiled/00/99/STLThreadedMaps_InsertUnique_00990900.cpp',symbol='?insert_unique@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@_STL@@_N@2@ABU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@2@@Z',address=0x990ad0,size=180)
]

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 build=R/'rebuild/build/native-threaded-submission/00/98/0098f0f0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 inputs={EXE,Path(__file__),Path(T.__file__),Path(A.__file__)}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py','threaded_manager_add_oracle.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 for p in T.SOURCES+[A.SOURCE,FIXTURE]:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'));inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins)
 for p in inputs:
  if p.is_relative_to(R):
   q=build/'snapshot'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 original,rebuilt,audits,objects=T.build_codes(build)
 assert len(audits)==16 and [a['grade'] for a in audits].count('FUNCTIONAL_VARIANT')==1
 behavior=T.compare_trees(original,rebuilt);assert (behavior['cases'],behavior['checkpoints'],len(behavior['controls']))==(180,4500,17)
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
 r=run('native',exe);assert r.returncode==0 and 'NATIVE_SUBMISSION_PASS cases=60 submissions=336 objects=336 nodes=672 headers=900' in r.stdout,r.stdout+r.stderr
 omissions=[]
 for index in range(7):
  omitted,r=link('omitted%d'%index,[p for i,p in enumerate(objects) if i!=index],index==0)
  if index==0:
   assert r.returncode==0,r.stdout+r.stderr;detected=False
   try:verify(omitted)
   except AssertionError:detected=True
   assert detected;omissions.append(dict(index=index,link_failed=False,selected_body_rejected=True))
  else:assert r.returncode!=0;omissions.append(dict(index=index,link_failed=True,selected_body_rejected=False))
 text=A.SOURCE.read_text(encoding='utf-8');link_text=T.SOURCES[5].read_text(encoding='utf-8')
 controls=[(6,'wrong-uid',text.replace('    return uid;','    return uid+1;')),(5,'wrong-node-count',link_text.replace('++_M_node_count;','_M_node_count += 2;'))]
 for index,name,key in ((1,'missing-pending','CThreadedFileManager::CPendingObjectSortKey'),(2,'missing-uid','unsigned long')):
  controls.append((index,name,'#include "fable_threaded_file_object.h"\nnamespace _STL {template<> CThreadedFileManager::CFileObject*& map<'+key+',CThreadedFileManager::CFileObject*>::operator[](const '+key+'&){static CThreadedFileManager::CFileObject* slot;return slot;}}\n'))
 for index,name,source in controls:
  p=build/(name+'.cpp');p.write_text(source);obj=compile(name,p);bad,r=link(name,[obj if i==index else p for i,p in enumerate(objects)]);assert r.returncode==0,r.stdout+r.stderr
  result=run(name,bad);assert result.returncode==82 and 'NATIVE_SUBMISSION_FAILURE' in result.stderr,(name,result.stdout,result.stderr)
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),roots=audits,behavior=behavior,native_cases=60,submissions=336,objects_released=336,nodes_released=672,headers_released=900,new_roots=8,linked_bodies_verified=17,functional_variants=1,source_omissions=omissions,required_source_omissions=6,native_controls=[name for i,name,s in controls],emulated_controls=17,selection_controls=1,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),boundaries=['Actual AddObject with actual map constructors, lookup/unique/hinted insertion, node allocation/linkage, comparator, iterator and red-black balancing. Real CRT node allocation, request allocation, Windows critical section/event and zeroed overlap lifetime.','Explicit fixture initializes map storage and synchronization, walks invariants after each submission, then frees all nodes/headers and destroys requests. Production manager startup, queue removal/worker handoff are not part of this test.','Reused78B predecessor helper remains FUNCTIONAL_VARIANT:79B retail,91.89% similarity but10.2564% instruction delta. No root credit. Two direct forms deferred. Both identical original79B addresses98FDD0/98FE20 are audited aliases; compiled78B is independently exercised and removal-detected.','Six new root providers required by link; omitting canonical rebalance still links an alternate inline COMDAT but selected-body verification rejects it. Eight new roots:five exact/three near. Actual parent is existing near.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-submission.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_SUBMISSION_PASS sequences=180 checkpoints=4500 native=60 submissions=336 nodes=672 headers=900 roots=8 bodies=17 functional_variants=1 omissions=6 controls=22');print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
