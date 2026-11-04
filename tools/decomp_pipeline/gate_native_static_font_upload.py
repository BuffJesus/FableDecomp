from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess,struct
R=Path(__file__).resolve().parents[2]
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();w=args.staging.resolve() if args.staging else None;b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-upload/20261024'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
helpers=['00/99/CBaseClass_CBaseClass_0099a2d0.cpp', '00/99/CInitBaseClass_IsValid_0099a330.cpp', '00/99/CCharString_DefaultConstructor_0099e4b0.cpp', '00/99/CCharString_Destructor_0099eae0.cpp', '00/99/CCharString_UnassignString_0099e9b0.cpp', '00/99/CCharString_Constructor_0099ebf0.cpp', '00/99/CCharString_AllocStringData_0099ea60.cpp', '00/99/CCharString_CopyAssignment_0099efb0.cpp', '00/99/CCharString_ConversionOperator_0099e4c0.cpp', '00/9a/CCharString_InitFromCStr_009a0590.cpp', '00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp', '00/9a/CFontBank_Preload_009a0b20.cpp', '00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp', '00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp', '00/ab/CStaticFontBank_Lifetime_00ab7a70.cpp', '00/ab/CGlyph_Read_00ab96b0.cpp', '00/99/CDataInputStream_CDataInputStream_00993b20.cpp', '00/99/DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp', '00/99/CMemoryDataInputStream_ReadNullTerminatedString_00995d30.cpp', '00/99/CMemoryDataInputStream_DestructorTyped_00993b10.cpp', '00/99/CCharString_CCharString_0099ec30.cpp', '00/9e/CPixelFormat_InitialiseRGBA_009e3790.cpp', '00/9b/CDisplayManager_GetBestCompressedAlphaTextureFormat_009be870.cpp', '00/9f/CTexture_Initialise_009fa280.cpp', '00/9f/CTexture_PreallocatedStorage_009fa230.cpp', '00/9e/CPixelFormat_GetColourDepth_009e3820.cpp', '00/9e/CPixelFormat_Initialise_009e3830.cpp', '01/29/CPixelFormat_Table_0129ba40.cpp', '00/9f/CTexture_GetSurfaceLevel_009f9e00.cpp', '00/9f/CSurface_CopyConstructor_009f2d60.cpp', '00/9f/CSurface_InitialiseFromTextureMipmap_009f2f10.cpp', '00/9f/CSurface_Uninitialise_009f2e20.cpp', '00/9f/CTexture_Uninitialise_009f9f70.cpp', '00/9f/CSurface_CopyFromMemory_009f4390.cpp', '00/9e/CPixelFormat_IsEquivalentTyped_009e37f0.cpp', '00/9f/CSurface_CanDoProperSignedConversion_009f35d0.cpp', '00/9e/CPixelFormat_IsSigned_009e3970.cpp', '00/9e/CPixelFormat_GetByteColourDepth_009e3800.cpp'];sources=[R/'rebuild/src/compiled/00/ab/CStaticFontBank_Constructor_00ab8e10.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeStaticFontUpload_test.cpp'];inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,inc/name,R/'rebuild/include'/name) if q.is_file()))
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
qualifications={}
for name,cases,controls in [('native-static-font-constructor.json',90,27),('native-surface-copy.json',118496,22)]:
 path=R/'rebuild/compile-gate'/name;inputs.add(path);qualification=json.loads(path.read_text());assert qualification['status']=='PASS' and qualification['cases']==qualification['original_cases']==cases and len(qualification['controls'])==controls
 for source,digest in qualification['input_sha256'].items():
  p=Path(source)
  if p.is_relative_to(R/'rebuild/include') or p.is_relative_to(R/'rebuild/src/compiled') or p.is_relative_to(R/'rebuild/tests/integration'):
   assert hashlib.sha256(p.read_bytes()).hexdigest()==digest,source
 qualifications[name]=dict(build=qualification['build'],cases=cases,controls=controls,grade=qualification['grade'])
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/GS','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b);(b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapped[m[1]]=int(m[2],16)
targets={0x99a2f0:'??0CBaseClassNonCopyable@@QAE@XZ',0x99e4b0:'??0CCharString@@QAE@XZ',0x9cc240:'?GetEntryDataSize@CBankFile@@QBEKK@Z',0xbfea0e:'_malloc',0x9cc2a0:'?ReadEntryData@CBankFile@@QAEXKKPAXK@Z',0x994930:'??0CMemoryDataInputStream@@QAE@PBXK@Z',0x995d30:'?ReadNullTerminatedString@CMemoryDataInputStream@@UAE?AVCCharString@@XZ',0x99efb0:'??4CCharString@@QAEAAV0@ABV0@@Z',0x99eae0:'??1CCharString@@QAE@XZ',0x993ca0:'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z',0xbfeb22:'??_U@YAPAXI@Z',0xab96b0:'?Read@CGlyph@@QAEXAAVCDataInputStream@@@Z',0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0x9e3790:'?Initialise@CPixelFormat@@QAEXJJJJ_N@Z',0x9be870:'?GetBestCompressedAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z',0xbfea1a:'??2@YAPAXI@Z',0x9fa280:'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z',0x9f9e00:'?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z',0x9f4390:'?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z',0x9f2e20:'?Uninitialise@CSurface@@QAEXXZ',0xbfea14:'_free',0x993b10:'??1CMemoryDataInputStream@@UAE@XZ',0xbfe9f9:'@__security_check_cookie@4',0x139c8a8:'___security_cookie',0x129ab38:'??_7CFontBank@@6B@',0x129ead4:'??_7CStaticFontBank@@6B@',0x122f84c:'??_7CSurface@@6B@'}
original=read_virtual_data(EXE.read_bytes(),0xab8e10,2197);(b/'original.bin').write_bytes(original);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;patches=[]
for i in md.disasm(original,0xab8e10):
 if i.mnemonic=='call':
  assert i.op_str.startswith('0x');address=int(i.op_str,16);patches.append((i.address-0xab8e10+i.imm_offset,mapped[targets[address]],1))
 else:
  for address in (0x139c8a8,0x129ab38,0x129ead4,0x122f84c):
   value=struct.pack('<I',address)
   if value in i.bytes:
    site=bytes(i.bytes).index(value);patches.append((i.address-0xab8e10+site,mapped[targets[address]],0))
(b/'patches.bin').write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));(b/'patches.json').write_text(json.dumps(patches,indent=2))
c=Coff(objects[0]);sec,off=c.symbols['??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z'];raw,rels=c.sections[sec];grade=compare(raw,original,tuple(rels),retail_relocations('00ab8e10',2197));assert grade.near_match,grade
runs=[]
for argv in ([],[b/'original.bin',b/'patches.bin']):
 x=subprocess.run([str(exe),*map(str,argv)],capture_output=True,text=True,timeout=30);record=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);runs.append(record);(b/'runs.json').write_text(json.dumps(runs,indent=2));print(record,flush=True);assert not x.returncode,record
provider_objects={p.name:o for o,p in zip(objects,sources)}
targets={'??_7CSurface@@6B@':0x122f84c,'?InitialiseFromTextureMipmap@CSurface@@QAEXPAUIDirect3DSurface9@@@Z':0x9f2f10,'??0CSurface@@QAE@ABV0@@Z':0x9f2d60,'?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,'?CalcByteLength@CTexture@@AAEXXZ':0x9f9ee0,'?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820}
specs=[('CPixelFormat_InitialiseRGBA_009e3790.cpp','?Initialise@CPixelFormat@@QAEXJJJJ_N@Z',0x9e3790),('CDisplayManager_GetBestCompressedAlphaTextureFormat_009be870.cpp','?GetBestCompressedAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z',0x9be870),('CTexture_Initialise_009fa280.cpp','?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z',0x9fa280),('CTexture_PreallocatedStorage_009fa230.cpp','?CalcByteLength@CTexture@@AAEXXZ',0x9f9ee0),('CPixelFormat_GetColourDepth_009e3820.cpp','?GetColourDepth@CPixelFormat@@QBEKXZ',0x9e3820),('CPixelFormat_Initialise_009e3830.cpp','?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z',0x9e3830),('CTexture_GetSurfaceLevel_009f9e00.cpp','?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z',0x9f9e00),('CSurface_CopyConstructor_009f2d60.cpp','??0CSurface@@QAE@ABV0@@Z',0x9f2d60),('CSurface_InitialiseFromTextureMipmap_009f2f10.cpp','?InitialiseFromTextureMipmap@CSurface@@QAEXPAUIDirect3DSurface9@@@Z',0x9f2f10),('CSurface_Uninitialise_009f2e20.cpp','?Uninitialise@CSurface@@QAEXXZ',0x9f2e20),('CTexture_Uninitialise_009f9f70.cpp','?Uninitialise@CTexture@@QAEXXZ',0x9f9f70)]
proof=[]
for name,symbol,address in specs:
 c=Coff(provider_objects[name]);section,offset=c.symbols[symbol];raw,relocs=c.sections[section];assert offset==0;fixed=bytearray(raw)
 for site,(target,kind) in relocs.items():
  assert kind in (6,20);struct.pack_into('<I',fixed,site,(targets[target]+struct.unpack_from('<i',raw,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),address,len(raw));record=dict(source=name,symbol=symbol,address=hex(address),bytes=len(raw),exact=bytes(fixed)==original);proof.append(record);print(record,flush=True)
assert all(p['exact'] for p in proof)
# Verify linked candidate contributions at their final addresses, not just their COFF inputs.
import pefile
from gate_render_state_static_data import coff_data
assert coff_data(Coff(provider_objects['CPixelFormat_Table_0129ba40.cpp']),'?GD3DFormats@@3QBVCD3DFormatInfo@@B',1692)==read_virtual_data(EXE.read_bytes(),0x129ba40,1692)
pe=pefile.PE(str(exe));final_bytes=0
final_specs=[(provider_objects[name],symbol) for name,symbol,address in specs]+[(objects[0],'??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z'),(provider_objects['CSurface_CopyFromMemory_009f4390.cpp'],'?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z'),(provider_objects['CPixelFormat_IsEquivalentTyped_009e37f0.cpp'],'?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z')]
for obj,symbol in final_specs:
 coff=Coff(obj);section,offset=coff.symbols[symbol];payload,relocations=coff.sections[section];assert offset==0;fixed=bytearray(payload)
 for site,(target,kind) in relocations.items():
  assert kind in (6,20);struct.pack_into('<I',fixed,site,(mapped[target]+struct.unpack_from('<i',payload,site)[0]-(mapped[symbol]+site+4 if kind==20 else 0))&0xffffffff)
 assert pe.get_data(mapped[symbol]-pe.OPTIONAL_HEADER.ImageBase,len(payload))==bytes(fixed);final_bytes+=len(payload)
out=b/'controls';out.mkdir();r={'object_sources':{str(o):str(p) for o,p in zip(objects,sources)}};records=[]
def index(name):return next(i for i,p in enumerate(objects) if Path(r['object_sources'][str(p)]).name==name)
def reject(label,items,missing=False):
 exe=out/(label+'.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*map(str,items)],env=env,capture_output=True,text=True)
 if missing:assert x.returncode;record=dict(name=label,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr;x=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);record=dict(name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);assert x.returncode==80 and 'STATIC_FONT_CTOR_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(label,'rejected',flush=True)
specs=[('rgba-format','CPixelFormat_InitialiseRGBA_009e3790.cpp','FormatIndex=index;','FormatIndex=0;'),('compression-format','CDisplayManager_GetBestCompressedAlphaTextureFormat_009be870.cpp','D3DFMT_DXT3','D3DFMT_DXT5'),('texture-format','CTexture_Initialise_009fa280.cpp','GD3DFormats[format.FormatIndex]','GD3DFormats[1]'),('texture-pool','CTexture_Initialise_009fa280.cpp','(D3DPOOL)pool','D3DPOOL_SYSTEMMEM'),('texture-owner','CTexture_Initialise_009fa280.cpp','AllocationSource=1;',';'),('skip-byte-accounting','CTexture_Initialise_009fa280.cpp','CalcByteLength();',';'),('wrong-byte-accounting','CTexture_PreallocatedStorage_009fa230.cpp','unsigned long bytes=','unsigned long bytes=8+'),('uncleared-byte-count','CTexture_PreallocatedStorage_009fa230.cpp','ByteLength=0;',';'),('surface-level','CTexture_GetSurfaceLevel_009f9e00.cpp','GetSurfaceLevel(level,','GetSurfaceLevel(level+1,'),('surface-copy-reference','CSurface_CopyConstructor_009f2d60.cpp','PD3DSurface->AddRef();',';'),('surface-release','CSurface_Uninitialise_009f2e20.cpp','PD3DSurface->Release();',';'),('texture-release','CTexture_Uninitialise_009f9f70.cpp','PD3DTexture->Release();',';'),('copy-filter','CSurface_CopyFromMemory_009f4390.cpp',',sp,filter,0)',',sp,0,0)')]
for label,name,old,new in specs:
 i=index(name);text=Path(r['object_sources'][str(objects[i])]).read_text();assert text.count(old)==(2 if label=='compression-format' else 1),(label,text.count(old));path=out/(label+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/GS','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out);assert not x.returncode,x.stdout+x.stderr;items=list(objects);items[i]=obj;reject(label,items)
for name in ('CStaticFontBank_Constructor_00ab8e10.cpp','CPixelFormat_InitialiseRGBA_009e3790.cpp','CDisplayManager_GetBestCompressedAlphaTextureFormat_009be870.cpp','CTexture_Initialise_009fa280.cpp','CTexture_PreallocatedStorage_009fa230.cpp','CTexture_GetSurfaceLevel_009f9e00.cpp','CSurface_CopyFromMemory_009f4390.cpp','CTexture_Uninitialise_009f9f70.cpp','CPixelFormat_Table_0129ba40.cpp'):
 i=index(name);reject('missing-'+name,[p for n,p in enumerate(objects) if n!=i],True)
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=30);assert not again.returncode and again.stdout==runs[1]['stdout']
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',build=str(b),cases=90,original_cases=90,controls=records,repeat=True,grade=grade.to_dict(),providers=proof,provider_bytes_exact=sum(p['bytes'] for p in proof),table_bytes_exact=1692,final_pe_verified_bytes=final_bytes,qualifications=qualifications,runs=runs,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Generated valid complete font entry ->actual constructor/glyph/Name/stream/format negotiation/texture initialization/byte accounting/mip surface lifetime/surface-copy routing -> COM and D3DX callbacks. Bank bytes and borrowed system/display storage are adapters. Only successful compression/CreateTexture with one mip and populated descriptions here; no real GPU/image conversion/retail font/OOM or rendering certification.390 guarded allocations per candidate run,780 across candidate+original; borrowed malloc vectors use host CRT.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-static-font-upload.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_UPLOAD_GATE_PASS cases=90 original=90 controls='+str(len(records)),b,flush=True)
