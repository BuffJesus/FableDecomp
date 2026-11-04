import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
w=Path(tempfile.mkdtemp(prefix='native-textured-buffers-'));env=V.env()
specs=[('00/a0/CRenderManagerCore_DrawCurrentBuffersAsTrisTextures_00a0abe0.cpp', '?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z', 10529760, 127, 1, 0), ('00/a0/CRenderManagerCore_DrawCurrentBuffersAsIndexedTrisTextures_00a0ab50.cpp', '?DrawCurrentBuffersAsIndexedTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKKK@Z', 10529616, 139, 1, 256), ('00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp', '?RealiseRenderState@CRenderStateManager@@QAEXXZ', 10508480, 167, 0, 512)]
sources=[ROOT/'rebuild/src/compiled'/s[0] for s in specs];fixture=ROOT/'rebuild/tests/integration/NativeTexturedBuffers_test.cpp'
raw=EXE.read_bytes();body=bytearray(679);fixups=[];md=Cs(CS_ARCH_X86,CS_MODE_32)
for _,symbol,address,size,count,offset in specs:
    span=read_virtual_data(raw,address,size);body[offset:offset+size]=span
    for ins in md.disasm(span,address):
        if ins.mnemonic=='call' and ins.op_str=='0xa058c0':fixups.append(offset+ins.address-address+1)
assert len(fixups)==2
(w/'textured_buffers_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned realiseFixups[]={'+','.join(map(str,fixups))+'};\n')
(w/'textured_buffers_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+s[1]+'")' for a,s in zip(('@DrawTris@24','@DrawIndexed@32'),specs))+'\n')
objects=[];proofs=[]
for i,path in enumerate(sources+[fixture]):
    obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
    if i<3:proofs.append(check(obj,*specs[i][1:5],{'?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0}))
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(3):
    r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=45);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=2304' in r.stdout
report=dict(cases=2304,snapshots=int(r.stdout.split('snapshots=')[1].strip()),providers=proofs,omission_checks=3,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual textured bound-buffer indexed/nonindexed draws plus complete native realization;zero..8stages,null/live wrappers,cache hits/misses,signed initial stage counts,two objects,dirty/clean/equal states,COM failures and texture-list/device/desired-state mutation. Full32KiB callback/final images and arguments compared native/compiled/independent model;only COM boundary.')
(ROOT/'rebuild/compile-gate/native-textured-buffers.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_TEXTURED_BUFFERS_GATE_PASS')
