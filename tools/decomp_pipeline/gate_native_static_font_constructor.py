from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess,struct
R=Path(__file__).resolve().parents[2]
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();w=args.staging.resolve() if args.staging else None;b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-constructor/00/ab/00ab8e10'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
helpers=[
  "00/99/CBaseClass_CBaseClass_0099a2d0.cpp",
  "00/99/CInitBaseClass_IsValid_0099a330.cpp",
  "00/99/CCharString_DefaultConstructor_0099e4b0.cpp",
  "00/99/CCharString_Destructor_0099eae0.cpp",
  "00/99/CCharString_UnassignString_0099e9b0.cpp",
  "00/99/CCharString_Constructor_0099ebf0.cpp",
  "00/99/CCharString_AllocStringData_0099ea60.cpp",
  "00/99/CCharString_CopyAssignment_0099efb0.cpp",
  "00/99/CCharString_ConversionOperator_0099e4c0.cpp",
  "00/9a/CCharString_InitFromCStr_009a0590.cpp",
  "00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp",
  "00/9a/CFontBank_Preload_009a0b20.cpp",
  "00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp",
  "00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp",
  "00/ab/CStaticFontBank_Lifetime_00ab7a70.cpp",
  "00/ab/CGlyph_Read_00ab96b0.cpp",
  "00/99/CDataInputStream_CDataInputStream_00993b20.cpp",
  "00/99/DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp",
  "00/99/CMemoryDataInputStream_ReadNullTerminatedString_00995d30.cpp",
  "00/99/CMemoryDataInputStream_DestructorTyped_00993b10.cpp",
  "00/99/CCharString_CCharString_0099ec30.cpp"
];sources=[w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/ab/CStaticFontBank_Constructor_00ab8e10.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeStaticFontConstructor_test.cpp'];overlay=w/'include' if w else R/'rebuild/include';inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
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
# Verify the accepted source contribution survives linking with all its relocations.
import pefile
symbol='??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z';fixed=bytearray(raw)
for at,(name,kind) in rels.items():
 assert kind in (6,20);struct.pack_into('<I',fixed,at,(mapped[name]+struct.unpack_from('<i',raw,at)[0]-(mapped[symbol]+at+4 if kind==20 else 0))&0xffffffff)
pe=pefile.PE(str(exe));assert off==0 and pe.get_data(mapped[symbol]-pe.OPTIONAL_HEADER.ImageBase,len(raw))==bytes(fixed)
out=b/'controls';out.mkdir();text=sources[0].read_text();records=[];r={'object_sources':{str(o):str(p) for o,p in zip(objects,sources)}}
def reject(label,obj=None,index=0,missing=False):
 items=[p for i,p in enumerate(objects) if not(missing and i==index)]
 if obj:items[index]=obj
 exe=out/(label+'.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*map(str,items)],env=env,capture_output=True,text=True)
 if missing:assert x.returncode;record=dict(name=label,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr;x=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);record=dict(name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);assert x.returncode==80 and 'STATIC_FONT_CTOR_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(label,'rejected',flush=True)
def compile(path,label):
 obj=out/(label+'.obj');x=V.cl(['/nologo','/c','/O2','/GS','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out);assert not x.returncode,x.stdout+x.stderr;return obj
specs=[('dimensions-copy','*reinterpret_cast<const C2DExtentsI*>(&TextureSize_X)','C2DExtentsI(TextureSize_X,TextureSize_Y)'),('skip-zero-banks','memset(GlyphBanks,0,sizeof(GlyphBanks));',';'),('font-height','FontHeight=stream.ReadULONG();','FontHeight=stream.ReadULONG()+1;'),('weight','Weight=stream.ReadValue<long>();','Weight=0; stream.ReadValue<long>();'),('italic','Italics=stream.ReadEBOOL();','Italics=!stream.ReadEBOOL();'),('max-height','MaxHeight=stream.ReadULONG();','MaxHeight=stream.ReadULONG()+1;'),('texture-width','TextureSize_X=stream.ReadValue<long>();','TextureSize_X=stream.ReadValue<long>()+1;'),('min-char','MinChar=stream.ReadValue<long>();','MinChar=stream.ReadValue<long>()+1;'),('first-glyph','static_cast<unsigned short>(stream.ReadULONG());','static_cast<unsigned short>(stream.ReadULONG()+1);'),('extra-glyph-allocation','new CGlyph[glyphBank.NoGlyphs]','new CGlyph[glyphBank.NoGlyphs+1]'),('padding-write','glyphBank.Glyphs[j].Read(stream);','{glyphBank.Glyphs[j].Read(stream); glyphBank.Glyphs[j]._pad_0x16[0]=0;}'),('skip-texture-header','stream.Read(textureHeader,18);',';'),('nonzero-tail','CArray<char> pixels(textureBytes);','CArray<char> pixels(textureBytes); memset(pixels.GetAsCArray(),0xff,textureBytes);'),('skip-first-system','    GFGetSystemManager();','    ;'),('rgba-depth','rgba.Initialise(8,8,8,8,false)','rgba.Initialise(4,8,8,8,false)'),('compressed-depth','GetBestCompressedAlphaTextureFormat(16,compressed)','GetBestCompressedAlphaTextureFormat(8,compressed)'),('wrong-format','),1,compressed,0,', '),1,rgba,0,'),('wrong-pool','SURFACE_POOL_MANAGED','SURFACE_POOL_SYSTEM_MEM'),('zero-levels','),1,compressed,0,','),0,compressed,0,'),('null-destination-box','CopyFromMemory(&destinationBox,','CopyFromMemory(0,'),('wrong-pitch','rgba,TextureSize_X*4,','rgba,TextureSize_Y*4,'),('wrong-filter','*4,0xffffffff','*4,0'),('total-count-write','    memset(GlyphBanks,','    TotalNoGlyphs=0; memset(GlyphBanks,'),('manager-write','    memset(GlyphBanks,','    PManager=0; memset(GlyphBanks,')]
for label,old,new in specs:
 # First-glyph replacement is intentionally confined to its assignment, not NoGlyphs.
 count=text.count(old);assert count==(2 if label=='first-glyph' else 1),(label,count);path=out/(label+'.cpp');path.write_text(text.replace(old,new,1));reject(label,compile(path,label))
for name in (sources[0].name,'CGlyph_Read_00ab96b0.cpp','CMemoryDataInputStream_ReadNullTerminatedString_00995d30.cpp'):
 index=next(i for i,p in enumerate(objects) if Path(r['object_sources'][str(p)]).name==name);reject('missing-'+name,index=index,missing=True)
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=30);assert not again.returncode and again.stdout==runs[1]['stdout']
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',build=str(b),cases=90,original_cases=90,grade=grade.to_dict(),final_pe_candidate_bytes_exact=len(raw),repeat=True,controls=records,runs=runs,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Generated complete valid font records only; actual C++ constructor,glyph reader,Name/base/stream and owned glyph-array cleanup. Borrowed bank payload adapter, format negotiation,texture creation/surface upload and system/display are controlled. Guarded new/new[] allocations balance; malloc-backed vectors use host CRT. No malformed-input/OOM/real GPU or retail font asset claim.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-static-font-constructor.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_CONSTRUCTOR_GATE_PASS cases=90 original=90 controls='+str(len(records)),b,flush=True)
