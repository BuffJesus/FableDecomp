"""CShaderRenderManager::UpdateCombinedProjectionTransform (retail 0x00988A50) retyped onto the real class:
bit-identical against the transplanted retail bytes for generated matrices, GEnableSIMD off/on and x87
precision 24/53/64. The engine's SSE multiply (Ego_r CMatrix4x4::AlignedSetMultiplyTransposed) is inline
asm in retail, so the genuine intrinsic body is functional, not a byte-near match."""
from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
b=args.output.resolve() if args.output else R/'rebuild/build/native-shader-combined-projection'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from crawl import purity
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
sources=[R/'rebuild/src/compiled/00/98/CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp',R/'rebuild/src/compiled/01/3d/Global_GEnableSIMD_013d2880.cpp',R/'rebuild/tests/integration/NativeShaderCombinedProjection_test.cpp']
assert purity.is_genuine(sources[0].read_text()) and purity.is_genuine((R/'rebuild/include/fable_matrix4x4_simd.h').read_text())
inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(R/'rebuild/include'):
  q=inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
inputs.update(inc.rglob('*.h'))
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
def compile_unit(source,obj,cwd):
 x=V.cl(['/nologo','/c','/O2','/Oy','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(source)],env,cwd);assert not x.returncode,(source.name,x.stdout+x.stderr)
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');compile_unit(snapshot,obj,b);objects.append(obj)
def link(exe,items,cwd):
 return subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/include:?GEnableSIMD@@3_NA','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,items)],env=env,cwd=cwd,capture_output=True,text=True)
exe=b/'probe.exe';x=link(exe,objects,b);assert not x.returncode,x.stdout+x.stderr
def read_map(path):
 mapped={}
 for line in path.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapped[m[1]]=int(m[2],16)
 return mapped
symbol='?UpdateCombinedProjectionTransform@CShaderRenderManager@@QAEXXZ'
data=EXE.read_bytes();# The manifest row says 975 bytes; retail ends three bytes later with mov esp,ebp / pop ebp / ret.
original=read_virtual_data(data,0x988a50,978);assert original[-4:]==b'\x8b\xe5\x5d\xc3' and read_virtual_data(data,0x988a50+978,1)==b'\xcc';(b/'original.bin').write_bytes(original)
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;sites=[]
for i in md.disasm(original,0x988a50):
 assert not (i.mnemonic=='call' and i.operands[0].type==2),i
 if i.mnemonic.startswith('j'):continue
 for o in i.operands:
  v=(o.imm&0xffffffff) if o.type==2 else (o.mem.disp&0xffffffff) if o.type==3 and o.mem.base==0 else None
  if v is not None and 0x400000<=v<0x1600000:
   assert v==0x13d2880,(hex(v),i);raw=bytes(i.bytes);sites.append(i.address-0x988a50+raw.index(struct.pack('<I',v)))
assert len(sites)==2,sites  # one GEnableSIMD load per inlined multiply
def patch_map(path,mapping):
 m=read_map(mapping);path.write_bytes(struct.pack('<I',len(sites))+b''.join(struct.pack('<III',s,m['?GEnableSIMD@@3_NA'],0) for s in sites))
patch_map(b/'patches.bin',b/'probe.map')
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=300);standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr);assert not standalone.returncode,standalone_record
x=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=300)
positive=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);print(positive,flush=True);assert not x.returncode and 'COMBINED_PROJECTION_PASS cases=6144 paths=12288 approximate=' in x.stdout,positive
c=Coff(objects[0]);section,offset=c.symbols[symbol];raw,relocations=c.sections[section];assert offset==0
grade=compare(raw,original,tuple(relocations),retail_relocations("00988a50",978))
out=b/'controls';out.mkdir();records=[]
def reject(name,items,missing=False):
 fault=out/('fault'+str(len(records))+'.exe');x=link(fault,items,out)
 if missing:
  assert x.returncode,(name,x.stdout);records.append(dict(name=name,link_rejected=True,stdout=x.stdout));print(name,'rejected',flush=True);return
 assert not x.returncode,x.stdout+x.stderr;patches=fault.with_suffix('.patches.bin');patch_map(patches,fault.with_suffix('.map'))
 x=subprocess.run([str(fault),str(b/'original.bin'),str(patches)],cwd=out,capture_output=True,text=True,timeout=300)
 record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr[-2000:]);assert x.returncode==80 and 'COMBINED_PROJECTION_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(name,'rejected',flush=True)
header=(R/'rebuild/include/fable_matrix4x4_simd.h').read_text()
mutants=[
 ('source','order-swapped','worldView.AlignedSetMultiplyTransposed(WorldMatrix,ViewMatrix);','worldView.AlignedSetMultiplyTransposed(ViewMatrix,WorldMatrix);'),
 ('source','projection-skipped','CombinedProjectionMatrix.AlignedSetMultiplyTransposed(worldView,ProjectionMatrix);','CombinedProjectionMatrix=worldView;'),
 ('source','flag-kept','updateFlags3D8 &= ~0x80u;',';'),
 ('source','vector-count','&CombinedProjectionMatrix,4);','&CombinedProjectionMatrix,3);'),
 ('source','register-range','CurrentVSConstantLayout->CombinedProjectionMatrix.Offset','CurrentVSConstantLayout->WorldMatrix.Offset'),
 ('header','simd-add-order','_mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w))','_mm_add_ps(_mm_add_ps(x, z), _mm_add_ps(y, w))'),
 ('header','simd-shuffle','y = _mm_shuffle_ps(y, y, _MM_SHUFFLE(1, 1, 1, 1));','y = _mm_shuffle_ps(y, y, _MM_SHUFFLE(2, 2, 2, 2));'),
 ('header','scalar-transposed','m1(i, 0) * m2(0, j)','m1(0, i) * m2(0, j)'),
 ('header','scalar-term-dropped',' + m1(i, 3) * m2(3, j);',';'),
 ('header','operands-unswapped','AlignedSetMultiply(b, a);','AlignedSetMultiply(a, b);'),
 ('header','flag-ignored','if (GEnableSIMD)','if (true)'),
]
for where,name,old,new in mutants:
 overlay=out/name;overlay.mkdir();text=(header if where=='header' else sources[0].read_text());assert text.count(old)==1,(name,text.count(old))
 if where=='header':(overlay/'fable_matrix4x4_simd.h').write_text(text.replace(old,new));src=overlay/'unit0.cpp';src.write_text(sources[0].read_text())
 else:src=overlay/'unit0.cpp';src.write_text(text.replace(old,new))
 obj=overlay/'unit0.obj';x=V.cl(['/nologo','/c','/O2','/Oy','/MT','/Gy','/I'+str(overlay),'/I'+str(inc),'/Fo'+str(obj),str(src)],env,overlay);assert not x.returncode,(name,x.stdout+x.stderr)
 items=list(objects);items[0]=obj;reject(name,items)
reject('missing-candidate',objects[1:],True);reject('missing-GEnableSIMD',[objects[0],objects[2]],True)
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=300);assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(p)==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=6144,original_paths=12288,repeat=True,grade=grade.to_dict(),built_bytes=len(raw),retail_bytes=978,controls=records,run=positive,standalone=standalone_record,build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Bit-identical to retail 0x00988A50 for 1024 generated world/view/projection sets (ordinary, any-normal, denormal/zero, signed zero, inf/NaN mixes), GEnableSIMD off/on, x87 PC24/53/64: combined matrix, uploaded constants, device/register/count/data arguments, update flags and untouched manager/guard bytes. Retail SSE path is inline asm (operands spilled and reloaded into eax/edx/edi, fixed xmm0-7); genuine intrinsics reproduce its results, not its bytes.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None:(R/'rebuild/compile-gate/native-shader-combined-projection.json').write_text(json.dumps(report,indent=2))
print('NATIVE_SHADER_COMBINED_PROJECTION_GATE_PASS cases=6144 original_paths=12288 controls='+str(len(records)),b,flush=True)
