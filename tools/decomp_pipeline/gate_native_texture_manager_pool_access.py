import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-manager-pool-access-'));env=V.env();source=ROOT/'rebuild/src/compiled/00/a6/CTextureManager_PoolAccess_00a696f0.cpp'
if not source.exists():source=ROOT/'work/parity_addtext/texture_manager_pool_access_candidate1_20261009.cpp'
obj=w/'access.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr
specs=[('?GetPoolCount@CTextureManager@@QAEKXZ',0xa696f0,4,0),('?GetPoolTextureSize@CTextureManager@@QAEKK@Z',0xa69700,11,16),('?GetPoolTextureCount@CTextureManager@@QAEKK@Z',0xa69710,17,32)];proofs=[];body=bytearray(64)
for symbol,address,length,offset in specs:
 proofs.append(check(obj,symbol,address,length,0,{}));body[offset:offset+length]=read_virtual_data(EXE.read_bytes(),address,length)
(w/'manager_pool_access_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\n')
(w/'manager_pool_access_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in zip(['@CountRoot@8','@SizeRoot@12','@TextureCountRoot@12'],[x[0] for x in specs]))+'\n')
fixture=ROOT/'rebuild/tests/integration/NativeTextureManagerPoolAccess_test.cpp';fo=w/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(fo),str(fixture)],env,w);assert not r.returncode,r.stdout+r.stderr
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib',str(fo)];r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=512 snapshots=3072' in r.stdout,r.stdout
headers=[ROOT/'rebuild/include'/p for p in ['engine/CTextureManager.h','engine/CPreallocTexturePool.h']]
report=dict(cases=512,snapshots=3072,providers=proofs,omission_checks=1,result=r.stdout.strip(),source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in [source]+headers},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Three actual canonical manager pool access bodies; original/model/compiled unsigned endpoint values,all16 indices,two placements-fills, full16KiB read-only comparisons,one source omission fails. PoolCount does not bound indexed getters. Original size getter reads+0C; unrelated B244F0 facade reads+120 and remains separate mangled provider, no correction/recount of that historical item.')
(ROOT/'rebuild/compile-gate/native-texture-manager-pool-access.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURE_MANAGER_POOL_ACCESS_GATE_PASS')
