from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,struct,subprocess,hashlib
R=Path(__file__).resolve().parents[2];
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();w=args.staging.resolve() if args.staging else None;b=args.output.resolve() if args.output else R/'rebuild/build/native-font-virtual-extents/00/43/00437210'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
source=w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/43/CFontBank_GetVStringExtents_00437210.cpp';fixture=w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeFontVirtualExtents_test.cpp';sources=[source,R/'rebuild/src/compiled/00/43/CFontBank_GetVStringWidth_0043e4d0.cpp']+[R/'rebuild/src/compiled'/p for p in ['00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','00/99/CCharString_Destructor_0099eae0.cpp','00/99/CCharString_UnassignString_0099e9b0.cpp','00/9a/CFontBank_Preload_009a0b20.cpp','00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp','00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp','00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp','00/99/CWideString_Constructor_0099aed0.cpp','00/99/CWideString_Destructor_0099b510.cpp','00/99/CCharString_operatorcharconstptr2_0099af10.cpp','00/9e/Global_VirtualCoordinateHelpers_009e1d00.cpp','00/9e/Global_GFAYToVY_009e1dd0.cpp']]+[fixture]
overlay=w/'include' if w else R/'rebuild/include';inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
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
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));objects=[];env=V.env()
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(snapshot)],env,b);(b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
originals=[];proof=[]
for i,(address,size) in enumerate([(0x437210,76),(0x43e4d0,69)]):
 c=Coff(objects[i]);symbol=next(n for n in c.symbols if 'GetVString' in n);sec,offset=c.symbols[symbol];raw,rels=c.sections[sec];fixed=bytearray(raw);targets={'?GetAsCharArray@CWideString@@QBEPBGXZ':0x99af10,'?GFAXToVX@@YIMM@Z':0x9e1da0,'?GFAYToVY@@YIMM@Z':0x9e1dd0};assert offset==0 and len(raw)==size and len(rels)==3
 for at,(name,kind) in rels.items():assert kind==20;struct.pack_into('<I',fixed,at,(targets[name]+struct.unpack_from('<i',raw,at)[0]-address-at-4)&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),address,size);assert bytes(fixed)==original;path=b/('original'+str(i)+'.bin');path.write_bytes(original);originals.append(path);proof.append(dict(address=hex(address),size=size,symbol=symbol,relocated_bytes_exact=True))
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
runs=[]
for runArgs in ([],originals):
 x=subprocess.run([str(exe),*map(str,runArgs)],capture_output=True,text=True,timeout=20);r=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);runs.append(r);(b/'runs.json').write_text(json.dumps(runs,indent=2));print(r,flush=True);assert not x.returncode,r
# All fault cases compile against the same frozen header snapshot and actual native dependencies.
controls=[]
specs=[(0,'no-X','extents.X = GFAXToVX(extents.X);',';'),(0,'no-Y','extents.Y = GFAYToVY(extents.Y);',';'),(0,'wrong-X-axis','GFAXToVX(extents.X)','GFAYToVY(extents.X)'),(0,'first-line','GetAsCharArray(), false','GetAsCharArray(), true'),(0,'mutate-receiver','{\n','{\n    const_cast<CFontBank*>(this)->FontHeight=0;\n'),(1,'no-Y-side-effect','extents.Y = GFAYToVY(extents.Y);',';'),(1,'return-Y','return extents.X;','return extents.Y;'),(1,'first-line','GetAsCharArray(), false','GetAsCharArray(), true'),(1,'no-X','extents.X = GFAXToVX(extents.X);',';'),(1,'mutate-receiver','{\n','{\n    const_cast<CFontBank*>(this)->FontHeight=0;\n')]
for index,label,old,new in specs:
 text=sources[index].read_text();assert text.count(old)==1;path=b/('fault'+str(index)+'-'+label+'.cpp');path.write_text(text.replace(old,new));obj=path.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,b);assert not x.returncode,x.stdout+x.stderr
 items=list(objects);items[index]=obj;fault_exe=path.with_suffix('.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(fault_exe),*map(str,items)],env=env,capture_output=True,text=True);assert not x.returncode,x.stdout+x.stderr;x=subprocess.run([str(fault_exe)],capture_output=True,text=True,timeout=20);record=dict(root=index,name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);controls.append(record);assert x.returncode==80 and 'FONT_VIRTUAL_EXTENTS_FAILURE' in x.stderr,record
for index in (0,1):
 x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/('missing'+str(index)+'.exe')),*map(str,[p for i,p in enumerate(objects) if i!=index])],env=env,capture_output=True,text=True);assert x.returncode and 'GetVString' in x.stdout+x.stderr;controls.append(dict(root=index,name='missing-provider',link_rejected=True))
again=subprocess.run([str(exe),*map(str,originals)],capture_output=True,text=True,timeout=20);assert not again.returncode and again.stdout==runs[1]['stdout']
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',build=str(b),proof=proof,runs=runs,cases=512,original_cases=512,controls=controls,repeat=True,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(p):str(source) for p,source in zip(objects,sources)},boundary='Actual typed aggregate extents and width wrappers, static metrics and coordinate/string helpers. PC53-nearest; 4 screen sizes,4 virtual sizes,4 font heights,8 text cases. Both X and Y conversion calls execute even for width-only result. Concrete glyph construction/render services, display/system state and shared borrowed wide-data views remain fixture boundaries; no complete ownership or rasterization certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-font-virtual-extents.json').write_text(json.dumps(report,indent=2))
print('NATIVE_FONT_VIRTUAL_EXTENTS_GATE_PASS cases=512 original=512 controls=12 exact_bytes=145 pc53_nearest=1',b,flush=True)
