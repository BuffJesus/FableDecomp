"""Native attachment dispatch and actual singleton storage."""
import hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2]
w=Path(tempfile.mkdtemp(prefix='native-camera-copy-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/b2/CEngine_SetAttachedPrimitive_00b23c80.cpp'
data_source=ROOT/'rebuild/src/compiled/01/43/NEngineInternal_GPPrimitiveAttachmentManager_01436e70.cpp'
fixture=ROOT/'rebuild/tests/integration/NativePrimitiveAttachment_test.cpp'
symbol='?SetAttachedPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z'
global_symbol='?GPPrimitiveAttachmentManager@NEngineInternal@@3PAVCEnginePrimitiveAttachmentManager@@A'
method='?SetAttachedPrimitive@CEnginePrimitiveAttachmentManager@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABV2@JK@Z'
raw=read_virtual_data(EXE.read_bytes(),0xb23c80,11)
(w/'primitive_attachment_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,raw))+'};\n#pragma comment(linker,"/alternatename:@Attach@32='+symbol+'")\n#pragma comment(linker,"/alternatename:_AttachmentGlobal='+global_symbol+'")\n#pragma comment(linker,"/alternatename:'+method+'=@SetAttached@32")\n')
objects=[]
for n,p in enumerate((source,data_source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0xb23c80,11,2,{global_symbol:0x1436e70,method:0xb3a180})
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
assert coff_data(Coff(objects[1]),global_symbol,4)==read_virtual_data(EXE.read_bytes(),0x1436e70,4)==bytes(4)
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(2):
 r=subprocess.run(link+[str(o) for n,o in enumerate(objects) if n!=omit],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.returncode,r.stdout,r.stderr)
assert 'NATIVE_PRIMITIVE_ATTACHMENT_PASS cases=384 snapshots=6912' in r.stdout;rpt=dict(cases=384,snapshots=6912,omission_checks=2,providers=[proof],source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in (source,data_source)},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),headers={name:hashlib.sha256((ROOT/'rebuild/include'/name).read_bytes()).hexdigest() for name in ('fable_engine.h','fable_engine_internal.h','engine/CEnginePrimitiveAttachmentManager.h')},boundary='Engine attachment forwarding and actual singleton storage; full256-byte state and global checks on three-call sequences. Signed bone and arbitrary flag bits,aliased argument references,manager replacements and handle mutations. Actual attachment-manager operation remains callback. Existing donor layout is not a recovered retail allocation-size claim.')
(ROOT/'rebuild/compile-gate/native-primitive-attachment.json').write_text(json.dumps(rpt,indent=2)+'\n')
print(r.stdout.strip());print('NATIVE_PRIMITIVE_ATTACHMENT_GATE_PASS 6912 snapshots')
