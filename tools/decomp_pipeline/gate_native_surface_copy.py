from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess,struct
R=Path(__file__).resolve().parents[2]
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();w=args.staging.resolve() if args.staging else None;b=args.output.resolve() if args.output else R/'rebuild/build/native-surface-copy/00/9f/009f4390'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
helpers=['00/9f/CSurface_CanDoProperSignedConversion_009f35d0.cpp', '00/9e/CPixelFormat_IsSigned_009e3970.cpp', '00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp', '00/9e/CPixelFormat_Initialise_009e3830.cpp', '01/29/CPixelFormat_Table_0129ba40.cpp'];sources=[w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/9f/CSurface_CopyFromMemory_009f4390.cpp',w/'Equivalent.cpp' if w else R/'rebuild/src/compiled/00/9e/CPixelFormat_IsEquivalentTyped_009e37f0.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeSurfaceCopy_test.cpp'];overlay=w/'include' if w else R/'rebuild/include';inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,overlay/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(overlay):q=inc/p.relative_to(overlay)
 elif p.is_relative_to(R/'rebuild/include'):q=inc/p.relative_to(R/'rebuild/include')
 else:continue
 q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
inputs.update(inc.rglob('*.h'))
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b);(b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapped[m[1]]=int(m[2],16)
targets={0x9e3830:'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3970:'?IsSigned@CPixelFormat@@QBE_NXZ',0x9f35d0:'?CanDoProperSignedConversion@CSurface@@AAE_NPBVC2DBoxI@@0ABVCPixelFormat@@@Z',0x9f36e0:'?CopyFromUnsignedDataToSignedSurface@CSurface@@AAEXPBXABVCPixelFormat@@J@Z',0x9e37f0:'?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z',0x9f3b00:'?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z',0xbfefc8:'_D3DXLoadSurfaceFromMemory@40',0x129ba40:'?GD3DFormats@@3QBVCD3DFormatInfo@@B'}
original=read_virtual_data(EXE.read_bytes(),0x9f4390,507);(b/'original.bin').write_bytes(original);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;patches=[]
for i in md.disasm(original,0x9f4390):
 if i.mnemonic=='call' and i.operands[0].type==2:
  address=int(i.op_str,16);patches.append((i.address-0x9f4390+i.imm_offset,mapped[targets[address]],1))
 else:
  for address in (0x129ba40,):
   value=struct.pack('<I',address)
   if value in i.bytes:
    site=bytes(i.bytes).index(value);patches.append((i.address-0x9f4390+site,mapped[targets[address]],0))
(b/'patches.bin').write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));(b/'patches.json').write_text(json.dumps(patches,indent=2))
c=Coff(objects[0]);sec,off=c.symbols['?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z'];raw,rels=c.sections[sec];grade=compare(raw,original,tuple(rels),retail_relocations('009f4390',507));assert grade.near_match,grade
runs=[]
for argv in ([],[b/'original.bin',b/'patches.bin']):
 x=subprocess.run([str(exe),*map(str,argv)],capture_output=True,text=True,timeout=30);record=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);runs.append(record);(b/'runs.json').write_text(json.dumps(runs,indent=2));print(record,flush=True);assert not x.returncode,record
# Independent original equality bytes, complete table, and final linked contributions.
import pefile
from gate_render_state_static_data import coff_data
other=Coff(objects[1]);equivalent='?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z';os,oo=other.symbols[equivalent];odata,orels=other.sections[os];assert oo==0 and len(odata)==16 and not orels and odata==read_virtual_data(EXE.read_bytes(),0x9e37f0,16)
table='?GD3DFormats@@3QBVCD3DFormatInfo@@B';assert coff_data(Coff(objects[-2]),table,1692)==read_virtual_data(EXE.read_bytes(),0x129ba40,1692)
pe=pefile.PE(str(exe));symbol='?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z';fixed=bytearray(raw)
for at,(name,kind) in rels.items():
 assert kind in (6,20);struct.pack_into('<I',fixed,at,(mapped[name]+struct.unpack_from('<i',raw,at)[0]-(mapped[symbol]+at+4 if kind==20 else 0))&0xffffffff)
assert off==0 and pe.get_data(mapped[symbol]-pe.OPTIONAL_HEADER.ImageBase,len(raw))==bytes(fixed)
assert pe.get_data(mapped[equivalent]-pe.OPTIONAL_HEADER.ImageBase,16)==odata
out=b/'controls';out.mkdir();text=sources[0].read_text();records=[];r={'object_sources':{str(o):str(p) for o,p in zip(objects,sources)}}
def reject(label,obj=None,index=0,missing=False):
 items=[p for i,p in enumerate(objects) if not(missing and i==index)]
 if obj:items[index]=obj
 exe=out/(label+'.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*map(str,items)],env=env,capture_output=True,text=True)
 if missing:assert x.returncode;record=dict(name=label,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr;x=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);record=dict(name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);assert x.returncode==80 and 'SURFACE_COPY_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(label,'rejected',flush=True)
def compile(path,label):
 obj=out/(label+'.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out);assert not x.returncode,x.stdout+x.stderr;return obj
specs=[('destination-width','destinationBox->right-destinationBox->left','destinationBox->right'),('destination-height',' : GetHeight();',' : GetWidth();'),('source-width','sourceBox->right-sourceBox->left','sourceBox->right'),('signed-source-only','GetPixelFormat().IsSigned() || format.IsSigned()','format.IsSigned()'),('both-signed','GetPixelFormat().IsSigned() && format.IsSigned()','GetPixelFormat().IsSigned() || format.IsSigned()'),('skip-eligibility','CanDoProperSignedConversion(destinationBox,sourceBox,format)','true'),('wrong-signed-pitch','CopyFromUnsignedDataToSignedSurface(data,format,pitch)','CopyFromUnsignedDataToSignedSurface(data,format,0)'),('direct-dimensions','sourceWidth==destinationWidth && sourceHeight==destinationHeight','true'),('direct-rectangles','CopyFromMemory(destinationBox,data,sourceBox,pitch)','CopyFromMemory(sourceBox,data,destinationBox,pitch)'),('destination-rectangle','destination.right=destinationBox->right','destination.right=destinationBox->left'),('alias-destination-rectangle','dp=&destination','dp=reinterpret_cast<const RECT*>(destinationBox)'),('alias-source-rectangle','sp=&source','sp=reinterpret_cast<const RECT*>(sourceBox)'),('wrong-d3d-format','GD3DFormats[format.FormatIndex].D3DFormat','GD3DFormats[0].D3DFormat'),('wrong-filter',',sp,filter,0)',',sp,0,0)'),('wrong-pitch','].D3DFormat,pitch,0','].D3DFormat,0,0'),('color-key',',sp,filter,0)',',sp,filter,1)'),('cached-surface','    long destinationWidth=','    IDirect3DSurface9* savedSurface=PD3DSurface;\n    long destinationWidth=')]
# Cached receiver control also substitutes only the final D3DX argument.

for label,old,new in specs:
 # First-glyph replacement is intentionally confined to its assignment, not NoGlyphs.
 count=text.count(old);assert count==1,(label,count);path=out/(label+'.cpp');body=text.replace(old,new,1);body=body.replace('D3DXLoadSurfaceFromMemory(PD3DSurface,','D3DXLoadSurfaceFromMemory(savedSurface,') if label=='cached-surface' else body;body+='\nunsigned long (CSurface::*KeepHeight)()const=&CSurface::GetHeight;\n' if label=='destination-height' else '';path.write_text(body);reject(label,compile(path,label))
equivalent=sources[1].read_text()
for label,body in [('equivalent-always','return true;'),('equivalent-never','return false;'),('equivalent-inverted','return FormatIndex!=other.FormatIndex;')]:
 path=out/(label+'.cpp');path.write_text(equivalent.replace('return FormatIndex==other.FormatIndex;',body));reject(label,compile(path,label),index=1)
for index in (0,1):reject('missing-provider-'+str(index),index=index,missing=True)
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=30);assert not again.returncode and again.stdout==runs[1]['stdout']
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',build=str(b),cases=118496,original_cases=118496,grade=grade.to_dict(),equivalent_bytes_exact=16,final_pe_candidate_bytes_exact=len(raw)+16,table_bytes_exact=1692,repeat=True,controls=records,runs=runs,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Surface copy parent routing, actual format table/queries and signed eligibility. COM GetDesc, signed conversion leaf, direct-copy leaf and D3DX loader are adapters. Valid descriptions despite varying HRESULT;46x46formats,two placements,seven box pairs,four live mutation modes. Full1KiB state/event comparisons after each callback plus pointer/rectangle/format/pitch/filter arguments. No actual pixel upload/conversion or GPU claim.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-surface-copy.json').write_text(json.dumps(report,indent=2))
print('NATIVE_SURFACE_COPY_GATE_PASS cases=118496 original=118496 controls='+str(len(records)),b,flush=True)
