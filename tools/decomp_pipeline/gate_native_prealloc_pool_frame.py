import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-pool-frame-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CPreallocTexturePool_Frame_00a696b0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/prealloc_pool_frame_candidate1_20261009.cpp'
base=ROOT/'rebuild/src/compiled/00/9f/CResourceList_FrameEnd_009fc150.cpp';objects={};proofs=[];body=bytearray(64)
specs=[(source,'?FrameEnd@CPreallocTexturePool@@QAEXXZ',0xa696b0,5,0,1),(source,'?GetTextureCount@CPreallocTexturePool@@QAEKXZ',0xa696c0,4,16,0),(base,'?FrameEnd@CResourceList@@QAEXXZ',0x9fc150,11,32,0)]
for p,symbol,address,length,offset,relocs in specs:
 if p not in objects:
  obj=w/(p.stem+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects[p]=obj
 proofs.append(check(objects[p],symbol,address,length,relocs,{'?FrameEnd@CResourceList@@QAEXXZ':0x9fc150}));body[offset:offset+length]=read_virtual_data(EXE.read_bytes(),address,length)
(w/'pool_frame_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'pool_frame_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in [('@FrameRoot@8',specs[0][1]),('@CountRoot@8',specs[1][1]),('?FrameEnd@CResourceLoadingQueue@@QAEXXZ','@UnusedQueue@8')])+'\n')
fixture=ROOT/'rebuild/tests/integration/NativePreallocPoolFrame_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];objs=list(objects.values())
for omitted in range(2):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objs) if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objs)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=2048 snapshots=16384' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CPreallocTexturePool.h','fable_resource_list.h']]
report=dict(cases=2048,snapshots=16384,providers=proofs,omission_checks=2,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in list(objects)+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Pool frame tail-wrapper and count getter compose actual11B resource-list frame provider. Unsigned count/frame/unload endpoints,two placements-fills,three successive frame calls including wrap,full16KiB comparisons,all2source omissionsfail. Unrelated bank method in existing base TU references unused queue abort alias; queue/bank not executed or recovered anew. Only two new pool roots count; existing resource-list helper not recounted.')
(ROOT/'rebuild/compile-gate/native-prealloc-pool-frame.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_PREALLOC_POOL_FRAME_GATE_PASS')
