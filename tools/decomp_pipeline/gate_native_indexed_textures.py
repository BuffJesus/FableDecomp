import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-indexed-textures-'));env=V.env()
symbol='?DrawAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KPBXJJPAXJ@Z'
sources=[ROOT/'rebuild/src/compiled'/p for p in ['00/a0/CRenderManagerCore_DrawAsIndexedTrisTextures_00a0aea0.cpp','00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp']]
fixture=ROOT/'rebuild/tests/integration/NativeIndexedTextures_test.cpp'
raw=EXE.read_bytes();body=bytearray(423);body[:146]=read_virtual_data(raw,0xa0aea0,146);body[256:]=read_virtual_data(raw,0xa058c0,167)
md=Cs(CS_ARCH_X86,CS_MODE_32);fixup=next(i.address-0xa0aea0+1 for i in md.disasm(body[:146],0xa0aea0) if i.mnemonic=='call' and i.op_str=='0xa058c0')
(w/'indexed_textures_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned realiseFixup='+str(fixup)+';\n')
(w/'indexed_textures_aliases.h').write_text('#pragma comment(linker,"/alternatename:@DrawTextures@36='+symbol+'")\n')
objects=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proofs=[check(objects[0],symbol,0xa0aea0,146,1,{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}),check(objects[1],'?RealiseRenderState@CRenderStateManager@@QAEXXZ',0xa058c0,167,0,{})]
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
    r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=45);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=1152' in r.stdout
report=dict(cases=1152,snapshots=int(r.stdout.split('snapshots=')[1].strip()),providers=proofs,omission_checks=2,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual texture-list indexed draw plus actual realization.0..8stages,null/live wrappers and cache hits/misses,initial signed stage count,two locations,dirty/clean states,COMfailures,texture-list/device/desired-state callback mutations. Full32KiB at each COM call and final return,original native/compiled/independent model snapshots and arguments. Only device COM callbacks.')
(ROOT/'rebuild/compile-gate/native-indexed-textures.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_INDEXED_TEXTURES_GATE_PASS')
