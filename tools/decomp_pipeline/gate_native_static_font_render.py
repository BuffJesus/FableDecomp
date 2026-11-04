from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse,pefile
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args()
w=args.staging.resolve() if args.staging else None
b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-render'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from crawl import purity
helpers=[
 '00/99/CBaseClass_CBaseClass_0099a2d0.cpp',
 '00/99/CCharString_DefaultConstructor_0099e4b0.cpp',
 '00/99/CCharString_Destructor_0099eae0.cpp',
 '00/99/CCharString_UnassignString_0099e9b0.cpp',
 '00/9a/CFontBank_Preload_009a0b20.cpp',
 '00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp',
 '00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp',
 '00/99/CWideString_Constructor_0099aed0.cpp',
 '00/99/CWideString_Destructor_0099b510.cpp',
 '00/99/CCharString_operatorcharconstptr_0099af00.cpp',
 '00/99/CCharString_operatorcharconstptr2_0099af10.cpp',
 '00/9e/CFontManager_GetFreeVertexBuffer_009e2ce0.cpp',
 '00/a6/CVertexBuffer_Dispatch_00a63240.cpp',
 '00/a0/CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp',
]
sources=[w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/ab/CStaticFontBank_Render_00ab7c20.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeStaticFontRender_test.cpp']
inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
metric_path=R/'rebuild/compile-gate/native-static-font-extents.json';metric=json.loads(metric_path.read_text());assert metric['status']=='PASS' and metric['cases']==metric['original_cases']==5377 and len(metric['controls'])==12 and metric['grade']['near_match'];inputs.add(metric_path)
for path,digest in metric['input_sha256'].items():
 path=Path(path)
 if path.suffix in ('.h','.cpp','.py'):
  assert hashlib.sha256(path.read_bytes()).hexdigest()==digest,str(path)
  inputs.add(path)
assert purity.is_genuine(sources[0].read_text())

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
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj')
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b)
 (b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)))
 assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True)
(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapped[m[1]]=int(m[2],16)
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9e2ce0:'?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',0x99af00:'??BCWideString@@QBEPBGXZ',0x99af10:'?GetAsCharArray@CWideString@@QBEPBGXZ',0xbfeb8a:'_floor',0xac20a0:'?Lock@CVertexBufferBase@@QAEPAEKKK@Z',0xac20b0:'?Unlock@CVertexBufferBase@@QAEXXZ',0xa0abe0:'?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z'}
constants={0x122f59c:'_FontHalf',0x122ded8:'_FontOne',0x122dcb4:'_FontUnsignedBias'}
data=EXE.read_bytes();original=read_virtual_data(data,0xab7c20,1910);assert original[-3:]==b'\xc2\x18\0';(b/'original.bin').write_bytes(original)
for address,name in constants.items():
 expected={'_FontHalf':.5,'_FontOne':1.,'_FontUnsignedBias':4294967296.}[name]
 assert read_virtual_data(data,address,4)==struct.pack('<f',expected)
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;patches=[]
for i in md.disasm(original,0xab7c20):
 if i.mnemonic=='call' and i.operands[0].type==2:
  address=i.operands[0].imm;patches.append((i.address-0xab7c20+i.imm_offset,mapped[targets[address]],1))
 for address,name in constants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:patches.append((i.address-0xab7c20+bytes(i.bytes).index(value),mapped[name],0))
(b/'patches.bin').write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));(b/'patches.json').write_text(json.dumps(patches,indent=2))
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=60)
standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr)
(b/'standalone.json').write_text(json.dumps(standalone_record,indent=2));assert not standalone.returncode,standalone_record
x=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
result=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(result,indent=2));print(result,flush=True)
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
assert not x.returncode,result
r=dict(build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)})
root_symbol='?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z'
coff=Coff(objects[0]);section,offset=coff.symbols[root_symbol];raw,relocations=coff.sections[section];assert offset==0
root_grade=compare(raw,original,tuple(relocations),retail_relocations('00ab7c20',1910));assert root_grade.near_match,root_grade
metric_index=next(i for i,p in enumerate(sources) if p.name=='CStaticFontBank_GetStringExtents_00ab7b00.cpp')
metric_reference=Path(metric['object']);assert sha(metric_reference)==metric['objects'][str(metric_reference)]
metric_symbol=metric['symbol'];current=Coff(objects[metric_index]);accepted=Coff(metric_reference);cs,co=current.symbols[metric_symbol];rs,ro=accepted.symbols[metric_symbol];assert co==ro==0 and current.sections[cs]==accepted.sections[rs]
pe=pefile.PE(str(exe));image=pe.OPTIONAL_HEADER.ImageBase
selected=[(0,root_symbol),(metric_index,metric_symbol)]
helper_specs=[('CCharString_operatorcharconstptr_0099af00.cpp','??BCWideString@@QBEPBGXZ',0x99af00,15),('CCharString_operatorcharconstptr2_0099af10.cpp','?GetAsCharArray@CWideString@@QBEPBGXZ',0x99af10,15),('CFontManager_GetFreeVertexBuffer_009e2ce0.cpp','?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',0x9e2ce0,31),('CVertexBuffer_Dispatch_00a63240.cpp','?Lock@CVertexBufferBase@@QAEPAEKKK@Z',0xac20a0,5),('CVertexBuffer_Dispatch_00a63240.cpp','?Unlock@CVertexBufferBase@@QAEXXZ',0xac20b0,5)]
helper_proof=[]
for filename,symbol,address,size in helper_specs:
 index=next(i for i,p in enumerate(sources) if p.name==filename);c=Coff(objects[index]);section,offset=c.symbols[symbol];body,rels=c.sections[section];assert offset==0 and len(body)==size
 fixed=bytearray(body)
 for site,(target,kind) in rels.items():
  assert target=='?g_FableEmptyWideString_0129A8E0@@3PAGA' and kind==6
  struct.pack_into('<I',fixed,site,(0x129a8e0+struct.unpack_from('<i',body,site)[0])&0xffffffff)
 assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),address,size),symbol
 helper_proof.append(dict(symbol=symbol,address=hex(address),bytes=size,exact=True));selected.append((index,symbol))
final_bytes=0
for index,symbol in selected:
 c=Coff(objects[index]);section,offset=c.symbols[symbol];body,rels=c.sections[section];assert offset==0;fixed=bytearray(body)
 for site,(target,kind) in rels.items():
  assert kind in (6,20)
  struct.pack_into('<I',fixed,site,(mapped[target]+struct.unpack_from('<i',body,site)[0]-(mapped[symbol]+site+4 if kind==20 else 0))&0xffffffff)
 assert pe.get_data(mapped[symbol]-image,len(body))==bytes(fixed),symbol
 final_bytes+=len(body)
positive=result
out=b/'controls';out.mkdir();records=[]
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9e2ce0:'?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',0x99af00:'??BCWideString@@QBEPBGXZ',0x99af10:'?GetAsCharArray@CWideString@@QBEPBGXZ',0xbfeb8a:'_floor',0xac20a0:'?Lock@CVertexBufferBase@@QAEPAEKKK@Z',0xac20b0:'?Unlock@CVertexBufferBase@@QAEXXZ',0xa0abe0:'?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z'}
constants={0x122f59c:'_FontHalf',0x122ded8:'_FontOne',0x122dcb4:'_FontUnsignedBias'}
original=(b/'original.bin').read_bytes();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;specs=[]
for i in md.disasm(original,0xab7c20):
 if i.mnemonic=='call' and i.operands[0].type==2:specs.append((i.address-0xab7c20+i.imm_offset,targets[i.operands[0].imm],1))
 for address,name in constants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:specs.append((i.address-0xab7c20+bytes(i.bytes).index(value),name,0))

def patch_map(path,mapping,half_to_one=False):
 mapped={}
 for line in mapping.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapped[m[1]]=int(m[2],16)
 patches=[(site,mapped['_FontOne' if half_to_one and name=='_FontHalf' else name],kind) for site,name,kind in specs]
 path.write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches))

def reject(name,items,missing=False):
 exe=out/('fault'+str(len(records))+'.exe');mapping=exe.with_suffix('.map')
 x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(mapping),'/out:'+str(exe),*['/include:'+name for name in targets.values()],*map(str,items)],env=env,capture_output=True,text=True)
 if missing:
  assert x.returncode,(name,x.stdout,x.stderr);record=dict(name=name,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr
  patches=exe.with_suffix('.patches.bin');patch_map(patches,mapping)
  x=subprocess.run([str(exe),str(b/'original.bin'),str(patches)],cwd=out,capture_output=True,text=True,timeout=60)
  record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr)
  assert x.returncode==80 and 'FONT_RENDER_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(name,'rejected',flush=True)

text=Path(r['object_sources'][str(objects[0])]).read_text()
mutants=[
 ('zero-textures','unsigned long textureCount=texture?2:1;','unsigned long textureCount=1;'),
 ('wrong-owned-texture','const CTexture* textures[2]={PTexture,texture};','const CTexture* textures[2]={0,texture};'),
 ('vertical-alignment','if(flags&8)','if(false)'),
 ('right-alignment','if(flags&2)','if(false)'),
 ('center-alignment','if(flags&4)','if(false)'),
 ('first-line-start','long lineStart=1;','long lineStart=0;'),
 ('newline-line-start','lineStart=1;}','lineStart=0;}'),
 ('newline-height','static_cast<float>(FontHeight);lineStart','static_cast<float>(FontHeight+1);lineStart'),
 ('newline-skip','lineStart=1;}','lineStart=1;continue;}'),
 ('glyph-bank','if(!bank.Glyphs)continue;','if(bank.Glyphs)continue;'),
 ('glyph-count','if(index>=bank.NoGlyphs)continue;','if(index>bank.NoGlyphs)continue;'),
 ('glyph-offset','penX+=glyph->Offset;',';'),
 ('glyph-advance','penX+=glyph->Advance;',';'),
 ('unrounded-left','*storedLeft+(glyph->Width+1.0f)*scale','left+(glyph->Width+1.0f)*scale'),
 ('quad-width','glyph->Width+1.0f','glyph->Width+2.0f'),
 ('quad-height','static_cast<float>(FontHeight)+1.0f','static_cast<float>(FontHeight)+2.0f'),
 ('u-left','float uLeft=glyph->Left','float uLeft=glyph->Right'),
 ('v-top','float vTop=glyph->Top','float vTop=glyph->Bottom'),
 ('triangle-corner','C4DVector(left,top,position.Z,1.0f)','C4DVector(right,top,position.Z,1.0f)'),
 ('packed-colour','packedColour,uLeft','0,uLeft'),
 ('flush-too-early','if(triangles*3>294)','if(triangles*3>288)'),
 ('flush-too-late','if(triangles*3>294)','if(triangles*3>300)'),
 ('final-unlock','    buffer->Unlock();\n    if(triangles)','    if(triangles)'),
 ('final-empty-draw','    if(triangles){','    if(true){'),
 ('lock-flags','buffer->Lock(0,0,0)','buffer->Lock(8,0,0)'),
]
for name,old,new in mutants:
 count=text.count(old);assert count>0,(name,count)
 path=out/(name+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj')
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out)
 assert not x.returncode,(name,x.stdout,x.stderr)
 reject(name,[obj,*objects[1:]])
for basename in ('Candidate.cpp','CStaticFontBank_GetStringExtents_00ab7b00.cpp','CCharString_operatorcharconstptr_0099af00.cpp','CCharString_operatorcharconstptr2_0099af10.cpp','CFontManager_GetFreeVertexBuffer_009e2ce0.cpp','CVertexBuffer_Dispatch_00a63240.cpp'):
 index=0 if basename=='Candidate.cpp' else next(i for i,p in enumerate(objects) if Path(r['object_sources'][str(p)]).name==basename)
 reject('missing-'+basename,[p for i,p in enumerate(objects) if i!=index],True)
patches=out/'original-alignment.patches.bin';patch_map(patches,b/'probe.map',True)
x=subprocess.run([str(b/'probe.exe'),str(b/'original.bin'),str(patches)],cwd=out,capture_output=True,text=True,timeout=60)
assert x.returncode==80 and 'FONT_RENDER_FAILURE' in x.stderr
records.append(dict(name='original-half-to-one',code=x.returncode,stdout=x.stdout,stderr=x.stderr))
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in r['input_sha256'].items())
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=241920,original_paths=483840,grade=root_grade.to_dict(),helper_proof=helper_proof,helper_original_bytes_exact=sum(row['bytes'] for row in helper_proof),final_pe_candidate_bytes_exact=final_bytes,metric_qualification=metric['build'],repeat=True,controls=records,run=positive,standalone=standalone_record,**r,boundary='Synthetic CPU glyph vertices and buffer/stream/draw traces at PC53-nearest. Actual qualified string access, metrics, free-buffer selection and base dispatch. Borrowed font glyphs/system/display/render/font-manager storage and controlled Lock/Unlock/SetStreamSource/DrawCurrentBuffersAsTris. No actual Win32 COM buffer path, device state application, graphics conversion/rasterization or gameplay certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-static-font-render.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_RENDER_GATE_PASS cases=241920 original_paths=483840 controls='+str(len(records)),b,flush=True)
