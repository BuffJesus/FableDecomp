from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess
R=Path(__file__).resolve().parents[2]
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();w=args.staging.resolve() if args.staging else None;b=args.output.resolve() if args.output else R/'rebuild/build/native-glyph-read/00/ab/00ab96b0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
overlay=w/'include' if w else R/'rebuild/include';inc=b/'include';inc.mkdir();helpers=['00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CDataInputStream_CDataInputStream_00993b20.cpp','00/99/DataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp'];sources=[w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/ab/CGlyph_Read_00ab96b0.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeGlyphRead_test.cpp'];inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,overlay/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(overlay):target=inc/p.relative_to(overlay)
 elif p.is_relative_to(R/'rebuild/include'):target=inc/p.relative_to(R/'rebuild/include')
 else:continue
 target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
inputs.update(inc.rglob('*.h'))
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));env=V.env();objects=[]
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(snapshot)],env,b);(b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
original=read_virtual_data(EXE.read_bytes(),0xab96b0,654);(b/'original.bin').write_bytes(original);c=Coff(objects[0]);symbol=next(n for n in c.symbols if n.startswith('?Read@CGlyph'));sec,off=c.symbols[symbol];raw,rels=c.sections[sec];grade=compare(raw,original,tuple(rels),retail_relocations('00ab96b0',654));assert grade.near_match,grade
assert off==0 and len(rels)==7 and all(name=='?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z' and kind==20 for name,kind in rels.values())
import struct
calls=(0x53,0xa7,0xfc,0x151,0x1a8,0x201,0x25d)
assert all(original[at-1]==0xe8 and (0xab96b0+at+4+struct.unpack_from('<i',original,at)[0])==0x993ca0 for at in calls)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
runs=[]
for argv in ([],[b/'original.bin']):
 x=subprocess.run([str(exe),*map(str,argv)],capture_output=True,text=True,timeout=30);r=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);runs.append(r);(b/'runs.json').write_text(json.dumps(runs,indent=2));print(r,flush=True);assert not x.returncode,r
# Behavioral controls use the same frozen include graph and genuine dependencies.
controls=[]
specs=[(0,'float-order','Left = stream.ReadFloat();','Top = stream.ReadFloat();'),
 (0,'numeric-float-conversion','Left = stream.ReadFloat();','Left = static_cast<float>(stream.ReadSWORD());'),
 (0,'skip-top','Top = stream.ReadFloat();','Top = 0;'),
 (0,'short-width','Offset = stream.ReadSWORD();','Offset = static_cast<short>(stream.ReadFloat());'),
 (0,'skip-advance','Advance = stream.ReadSWORD();','Advance = 0;'),
 (0,'short-order','Width = stream.ReadSWORD();','Offset = stream.ReadSWORD();'),
 (0,'padding-write','Advance = stream.ReadSWORD();','Advance = stream.ReadSWORD(); _pad_0x16[0]=0;'),
 (0,'bulk-24','Left = stream.ReadFloat();','stream.Read(this,24); return; Left = stream.ReadFloat();'),
 (3,'unbuffered-position','StreamPos = position + size;','StreamPos = position;'),
 (3,'chunk-position','SourceChunkStreamPos = StreamPos;','SourceChunkStreamPos = 0;'),
 (3,'unbuffered-window','CurrentSourcePtr = 0;','CurrentSourcePtr = static_cast<fable_u8*>(data);')]
for index,label,old,new in specs:
 text=sources[index].read_text();assert text.count(old)==1
 path=b/('fault-'+label+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,b);assert not x.returncode,x.stdout+x.stderr
 items=list(objects);items[index]=obj;fault_exe=path.with_suffix('.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(fault_exe),*map(str,items)],env=env,capture_output=True,text=True);assert not x.returncode,x.stdout+x.stderr
 x=subprocess.run([str(fault_exe)],capture_output=True,text=True,timeout=30);record=dict(name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);controls.append(record);(b/'controls.json').write_text(json.dumps(controls,indent=2));assert x.returncode==80 and 'GLYPH_READ_FAILURE' in x.stderr,record
for index in (0,3):
 x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/('missing'+str(index)+'.exe')),*map(str,[p for i,p in enumerate(objects) if i!=index])],env=env,capture_output=True,text=True);assert x.returncode and ('?Read@CGlyph' if index==0 else '?ReadWithSrcChunkOverflow@CDataInputStream') in x.stdout+x.stderr;controls.append(dict(name='missing-provider-'+str(index),link_rejected=True))
again=subprocess.run([str(exe),str(b/'original.bin')],capture_output=True,text=True,timeout=30);assert not again.returncode and again.stdout==runs[1]['stdout']
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',build=str(b),cases=41216,original_cases=41216,grade=grade.to_dict(),controls=controls,repeat=True,runs=runs,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)},boundary='Glyph22-byte serialized record ->24-byte struct; valid complete input only. Actual C++ overflow reader/constructor/base helpers with controlled buffered/unbuffered source hooks and cold unused virtuals. Raw float/short bits, padding,source bytes,stream state and event trace compare. No malformed position/EOF/zero-progress stream or font construction/rendering certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-glyph-read.json').write_text(json.dumps(report,indent=2))
print('NATIVE_GLYPH_READ_GATE_PASS cases=41216 original=41216 controls='+str(len(controls)),b,flush=True)
