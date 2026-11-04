import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-buffered-triangles-'));env=V.env()
specs=[('00/a0/CRenderManagerCore_DrawCurrentBuffersAsTris_00a0ae10.cpp','?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXKK@Z',0xa0ae10,39,1,0),('00/a0/CRenderManagerCore_DrawCurrentBuffersAsIndexedTris_00a0ac60.cpp','?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXKKKK@Z',0xa0ac60,51,1,64),('00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp','?RealiseRenderState@CRenderStateManager@@QAEXXZ',0xa058c0,167,0,256)]
sources=[ROOT/'rebuild/src/compiled'/s[0] for s in specs];fixture=ROOT/'rebuild/tests/integration/NativeBufferedTriangles_test.cpp'
raw=EXE.read_bytes();body=bytearray(423);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32)
for _,symbol,address,size,count,offset in specs:
    span=read_virtual_data(raw,address,size);body[offset:offset+size]=span
    for ins in md.disasm(span,address):
        if ins.mnemonic=='call' and ins.op_str=='0xa058c0':fixups.append(offset+ins.address-address+1)
assert len(fixups)==2
(w/'buffered_triangles_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned realiseFixups[]={'+','.join(map(str,fixups))+'};\n')
(w/'buffered_triangles_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+s[1]+'")' for a,s in zip(('@DrawTris@16','@DrawIndexed@24'),specs))+'\n')
objects=[];proofs=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
    if i<3:proofs.append(check(obj,*specs[i][1:5],{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
    r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=45);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=2304' in r.stdout
report=dict(cases=2304,snapshots=int(r.stdout.split('snapshots=')[1].strip()),providers=proofs,omission_checks=3,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual bound-buffer indexed/nonindexed triangle draws plus complete native realization.32KiB callback/final images and arguments compared native/compiled/independent model. Zero..8primitive counts,eight argument patterns,two objects,clean/dirty/equal states,COMfailures and callback device/desired-state mutation. DeviceCOM remains boundary.')
(ROOT/'rebuild/compile-gate/native-buffered-triangles.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_BUFFERED_TRIANGLES_GATE_PASS')
