from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse,pefile
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-render-buffer'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from gate_game_component_abi import Coff
from crawl import purity
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
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
 '00/9e/CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp',
 '00/a6/CVertexBufferWin32_Initialise_00a63150.cpp',
 '00/a6/CVertexBufferWin32_Uninitialise_00a63200.cpp',
]
sources=[R/'rebuild/src/compiled/00/ab/CStaticFontBank_Render_00ab7c20.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[R/'rebuild/tests/integration/NativeStaticFontRenderBuffer_test.cpp']
assert purity.is_genuine(sources[0].read_text())
inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
# Dependencies: the accepted Render and vertex-buffer gates must still describe the current sources.
qualifications={}
for name in ('native-static-font-render.json','native-vertex-buffer-dispatch.json'):
 path=R/'rebuild/compile-gate'/name;receipt=json.loads(path.read_text());assert receipt['status']=='PASS' and receipt['repeat'];inputs.add(path)
 for p,h in receipt['input_sha256'].items():
  if Path(p).suffix in ('.h','.cpp','.py'):assert sha(p)==h,p;inputs.add(Path(p))
 qualifications[name]=receipt
render=qualifications['native-static-font-render.json'];vertex=qualifications['native-vertex-buffer-dispatch.json']
assert render['cases']==241920 and len(render['controls'])==32 and vertex['cases']==23328 and len(vertex['controls'])==26
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
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj')
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b)
 (b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)))
 assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True)
(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
def read_map(path):
 mapped={}
 for line in path.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapped[m[1]]=int(m[2],16)
 return mapped
mapped=read_map(b/'probe.map')
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9e2ce0:'?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',0x99af00:'??BCWideString@@QBEPBGXZ',0x99af10:'?GetAsCharArray@CWideString@@QBEPBGXZ',0xbfeb8a:'_floor',0xac20a0:'?Lock@CVertexBufferBase@@QAEPAEKKK@Z',0xac20b0:'?Unlock@CVertexBufferBase@@QAEXXZ',0xa0abe0:'?DrawCurrentBuffersAsTris@CRenderManagerCore@@QAEXPAPBVCTexture@@KKK@Z'}
constants={0x122f59c:'_FontHalf',0x122ded8:'_FontOne',0x122dcb4:'_FontUnsignedBias'}
data=EXE.read_bytes();original=read_virtual_data(data,0xab7c20,1910);assert original[-3:]==b'\xc2\x18\0';(b/'original.bin').write_bytes(original)
for address,name in constants.items():
 expected={'_FontHalf':.5,'_FontOne':1.,'_FontUnsignedBias':4294967296.}[name]
 assert read_virtual_data(data,address,4)==struct.pack('<f',expected)
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;specs=[]
for i in md.disasm(original,0xab7c20):
 if i.mnemonic=='call' and i.operands[0].type==2:specs.append((i.address-0xab7c20+i.imm_offset,targets[i.operands[0].imm],1))
 for address,name in constants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:specs.append((i.address-0xab7c20+bytes(i.bytes).index(value),name,0))
def patch_map(path,mapping,half_to_one=False):
 m=read_map(mapping)
 patches=[(site,m['_FontOne' if half_to_one and name=='_FontHalf' else name],kind) for site,name,kind in specs]
 path.write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));return patches
(b/'patches.json').write_text(json.dumps(patch_map(b/'patches.bin',b/'probe.map'),indent=2))
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=60)
standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr)
(b/'standalone.json').write_text(json.dumps(standalone_record,indent=2));assert not standalone.returncode,standalone_record
x=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
positive=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(positive,indent=2));print(positive,flush=True)
assert not x.returncode,positive
# Qualification: fresh objects equal the accepted Render/vertex objects, original vertex bytes exact, final PE bytes exact.
pe=pefile.PE(str(exe));image=pe.OPTIONAL_HEADER.ImageBase
assert mapped['??_ECVertexBufferWin32@@UAEPAXI@Z']==mapped['??_GCVertexBufferWin32@@UAEPAXI@Z']
def body(c,symbol):
 section,offset=c.symbols[symbol];raw,rels=c.sections[section]
 if symbol=='??_7CVertexBufferWin32@@6B@':
  assert offset in (0,4) and len(raw)==offset+20
  return raw[offset:],{site-offset:value for site,value in rels.items() if site>=offset}
 assert offset==0;return raw,rels
def reference(receipt,source):
 path=next(Path(p) for p,name in receipt['object_sources'].items() if Path(name).resolve()==source.resolve());assert sha(path)==receipt['objects'][str(path)];return Coff(path)
linked=set();final_bytes=0
def linked_check(index,symbol):
 global final_bytes
 if symbol in linked:return
 linked.add(symbol);raw,rels=body(Coff(objects[index]),symbol);fixed=bytearray(raw)
 for site,(name,kind) in rels.items():
  assert kind in (6,20)
  struct.pack_into('<I',fixed,site,(mapped[name]+struct.unpack_from('<i',raw,site)[0]-(mapped[symbol]+site+4 if kind==20 else 0))&0xffffffff)
 assert pe.get_data(mapped[symbol]-image,len(raw))==bytes(fixed),symbol
 final_bytes+=len(raw)
render_symbols={
 'CStaticFontBank_Render_00ab7c20.cpp':'?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z',
 'CStaticFontBank_GetStringExtents_00ab7b00.cpp':'?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PBG_N@Z',
 'CCharString_operatorcharconstptr_0099af00.cpp':'??BCWideString@@QBEPBGXZ',
 'CCharString_operatorcharconstptr2_0099af10.cpp':'?GetAsCharArray@CWideString@@QBEPBGXZ',
 'CFontManager_GetFreeVertexBuffer_009e2ce0.cpp':'?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ',
}
for filename,symbol in render_symbols.items():
 index=next(i for i,p in enumerate(sources) if p.name==filename);assert body(Coff(objects[index]),symbol)==body(reference(render,sources[index]),symbol),symbol;linked_check(index,symbol)
addresses={row['symbol']:int(row['address'],16) for row in vertex['qualification']['proof']}
addresses.update({'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc,'??_ECVertexBufferWin32@@UAEPAXI@Z':0x9e2890})
proof=[];unlinked=[]
for row in vertex['qualification']['proof']:
 symbol=row['symbol'];source=next(Path(name) for p,name in vertex['object_sources'].items() if symbol in Coff(Path(p)).symbols and ('GCVertexBuffer' in name if symbol.startswith(('??_G','??_7')) else True))
 index=next(i for i,p in enumerate(sources) if p.resolve()==source.resolve());raw,rels=body(Coff(objects[index]),symbol);assert len(raw)==row['bytes'];assert (raw,rels)==body(reference(vertex,source),symbol),symbol
 fixed=bytearray(raw)
 for site,(name,kind) in rels.items():
  assert kind in (6,20)
  struct.pack_into('<I',fixed,site,(addresses[name]+struct.unpack_from('<i',raw,site)[0]-(int(row['address'],16)+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==read_virtual_data(data,int(row['address'],16),len(raw)),symbol
 if symbol in mapped:linked_check(index,symbol)
 else:unlinked.append(symbol)
 proof.append(dict(symbol=symbol,address=row['address'],bytes=len(raw),original_exact=True,linked=symbol in mapped))
assert set(unlinked)=={'?IsBusy@CVertexBufferBase@@QBE_NXZ','?Sizeof@CVertexBufferBase@@QBEKXZ'},unlinked
assert final_bytes==2633,final_bytes
qualification=dict(final_pe_code_and_vtable_bytes_exact=final_bytes,original_vertex_bytes_exact=sum(row['bytes'] for row in proof),linked_vertex_bytes=sum(row['bytes'] for row in proof if row['linked']),vertex_proof=proof,qualification_builds={name:receipt['build'] for name,receipt in qualifications.items()})
assert qualification['original_vertex_bytes_exact']==406 and qualification['linked_vertex_bytes']==396,qualification
# Controls: every behavioral fault is relinked with its own map and freshly bound original patch list.
out=b/'controls';out.mkdir();records=[]
def reject(name,items,missing=False):
 fault=out/('fault'+str(len(records))+'.exe');mapping=fault.with_suffix('.map')
 x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(mapping),'/out:'+str(fault),*['/include:'+name for name in targets.values()],*map(str,items)],env=env,capture_output=True,text=True)
 if missing:
  assert x.returncode,(name,x.stdout,x.stderr);record=dict(name=name,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr
  patches=fault.with_suffix('.patches.bin');patch_map(patches,mapping)
  x=subprocess.run([str(fault),str(b/'original.bin'),str(patches)],cwd=out,capture_output=True,text=True,timeout=60)
  record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr)
  assert x.returncode==80 and 'FONT_RENDER_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(name,'rejected',flush=True)
def mutate(name,index,old,new,exact=False):
 text=sources[index].read_text();count=text.count(old);assert (count==1 if exact else count>0),(name,count)
 path=out/(name+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj')
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out);assert not x.returncode,(name,x.stdout,x.stderr)
 items=list(objects);items[index]=obj;reject(name,items)
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
for name,old,new in mutants:mutate(name,0,old,new)
helper_mutants=[
 ('full-byte-length','CVertexBuffer_Dispatch_00a63240.cpp','if(bytes==0)bytes=ByteSize;','if(bytes==0)bytes=0;'),
 ('driver-flags','CVertexBuffer_Dispatch_00a63240.cpp','reinterpret_cast<void**>(&data),flags','reinterpret_cast<void**>(&data),8'),
 ('driver-range','CVertexBuffer_Dispatch_00a63240.cpp','Buffer->Lock(offset,bytes,','Buffer->Lock(bytes,offset,'),
 ('initial-length','CVertexBufferWin32_Initialise_00a63150.cpp','long bytes = count * stride;','long bytes = count * stride + 1;'),
 ('initial-stride','CVertexBufferWin32_Initialise_00a63150.cpp','VertexStride = static_cast<unsigned char>(stride);','VertexStride = 27;'),
 ('initial-type','CVertexBufferWin32_Initialise_00a63150.cpp','VertexType = type;','VertexType = 0;'),
 ('release-missing','CVertexBufferWin32_Uninitialise_00a63200.cpp','reinterpret_cast<IUnknown*>(Buffer)->Release();',';'),
 ('clear-missing','CVertexBufferWin32_Uninitialise_00a63200.cpp','Buffer = 0;',';'),
 ('cleanup-missing','CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp','    Uninitialise();','    ;'),
]
for name,filename,old,new in helper_mutants:mutate(name,next(i for i,p in enumerate(sources) if p.name==filename),old,new,True)
for basename in ('CStaticFontBank_Render_00ab7c20.cpp','CStaticFontBank_GetStringExtents_00ab7b00.cpp','CCharString_operatorcharconstptr_0099af00.cpp','CCharString_operatorcharconstptr2_0099af10.cpp','CFontManager_GetFreeVertexBuffer_009e2ce0.cpp','CVertexBuffer_Dispatch_00a63240.cpp','CVertexBufferWin32_GCVertexBufferWin32_009e2890.cpp','CVertexBufferWin32_Initialise_00a63150.cpp','CVertexBufferWin32_Uninitialise_00a63200.cpp','CRenderManagerCore_WaitWhileResourceIsBusy_00a0aa70.cpp'):
 index=next(i for i,p in enumerate(sources) if p.name==basename)
 reject('missing-'+basename,[p for i,p in enumerate(objects) if i!=index],True)
patches=out/'original-alignment.patches.bin';patch_map(patches,b/'probe.map',True)
x=subprocess.run([str(exe),str(b/'original.bin'),str(patches)],cwd=out,capture_output=True,text=True,timeout=60)
assert x.returncode==80 and 'FONT_RENDER_FAILURE' in x.stderr,x
records.append(dict(name='original-half-to-one',code=x.returncode,stdout=x.stdout,stderr=x.stderr))
assert len(records)==45,len(records)
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(p)==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=241920,original_paths=483840,repeat=True,controls=records,run=positive,standalone=standalone_record,build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},**qualification,boundary='Accepted CPU glyph Render joined to actual Win32 vertex-buffer constructor/Initialise/DoLock/DoUnlock/Uninitialise/scalar cleanup and PC busy wait. Synthetic COM CreateVertexBuffer/Lock/Unlock/Release plus stream/draw callbacks; three owned fake resources balance. Borrowed font storage, cold Prepare and controlled draw remain boundaries. No actual graphics API, device state realization, rasterization or game certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None:(R/'rebuild/compile-gate/native-static-font-render-buffer.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_RENDER_BUFFER_GATE_PASS cases=241920 original_paths=483840 controls='+str(len(records))+' final_bytes='+str(final_bytes),b,flush=True)
