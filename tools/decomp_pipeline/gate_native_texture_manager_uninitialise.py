import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-manager-uninitialise-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CTextureManager_Uninitialise_00a696d0.cpp'
if 'CTextureManagerRetail' in source.read_text():source=ROOT/'work/parity_addtext/texture_manager_uninitialise_canonical_candidate_20261009.cpp'
obj=w/'access.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
specs=[('?Uninitialise@CTextureManager@@QAEXXZ',0xa696d0,22,0)];proofs=[];body=bytearray(22)
for symbol,address,length,offset in specs:
 proofs.append(check(obj,symbol,address,length,0,{}));body[offset:offset+length]=read_virtual_data(EXE.read_bytes(),address,length)
(w/'manager_uninitialise_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'manager_uninitialise_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in zip(['@ShutdownRoot@8'],[x[0] for x in specs]))+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeTextureManagerUninitialise_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=1024 snapshots=4096' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CTextureManager.h','engine/CPreallocTexturePool.h']]
report=dict(cases=1024,snapshots=4096,providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Previously accepted22B shutdown moved from temporaryRetail facade to canonical1492B class without any body/relocation change. One actual source,1024cases/4096full16KiB original/model/compiled comparisons,raw initialised flags,rawNULL-invalid buffer pointer words,arbitraryPoolCount,endpoints,two placements-fills,two successive calls;onlyInitialised andTextureBuffer cleared,no pointer dereference/free inferred. Source omission fails. Identity correction only,zero additionalcoverage.')
(ROOT/'rebuild/compile-gate/native-texture-manager-uninitialise.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_MANAGER_UNINITIALISE_GATE_PASS')
