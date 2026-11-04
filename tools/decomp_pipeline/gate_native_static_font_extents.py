"""Static font extents: typed candidate, original instructions and independent model."""
from pathlib import Path
from datetime import datetime,timezone
import argparse,hashlib,json,re,struct,subprocess,sys
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
SYMBOL='?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PBG_N@Z'
HELPERS=['00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','00/99/CCharString_Destructor_0099eae0.cpp','00/99/CCharString_UnassignString_0099e9b0.cpp','00/9a/CFontBank_Preload_009a0b20.cpp','00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp']
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();stage=args.staging.resolve() if args.staging else None
 b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-extents/00/ab/00ab7b00'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 overlay=stage/'include' if stage else R/'rebuild/include';source=stage/'early-return.cpp' if stage else R/'rebuild/src/compiled/00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp';fixture=stage/'CatalogFixture.cpp' if stage else R/'rebuild/tests/integration/NativeStaticFontExtents_test.cpp';sources=[R/'rebuild/src/compiled'/p for p in HELPERS]
 inputs={Path(__file__),EXE};seen=set()
 def closure(p):
  p=p.resolve()
  if p in seen:return
  seen.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,overlay/name,R/'rebuild/include'/name) if q.is_file()))
 for p in sources+[source,fixture]:closure(p)
 for p in seen:
  if p.is_relative_to(overlay):target=inc/p.relative_to(overlay)
  elif p.is_relative_to(R/'rebuild/include'):target=inc/p.relative_to(R/'rebuild/include')
  else:continue
  target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects={}
 def compile(p,label,text=None):
  snapshot=b/(label+'.cpp');snapshot.write_text(p.read_text() if text is None else text);obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b);(b/(label+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr,source=str(p))));assert not x.returncode,x.stdout+x.stderr;objects[str(obj)]=sha(obj);return obj
 def link(items,label):
  exe=b/(label+'.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(exe),'/map:'+str(b/(label+'.map')),*map(str,items)],cwd=b,env=env,capture_output=True,text=True);(b/(label+'.link.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));return x,exe
 def run(exe,argv=()):
  x=subprocess.run([str(exe),*map(str,argv)],cwd=b,capture_output=True,text=True,timeout=20);return dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)
 root=compile(source,'candidate');c=Coff(root);sec,off=c.symbols[SYMBOL];raw,rels=c.sections[sec];assert not off
 retail=EXE.read_bytes();original=read_virtual_data(retail,0xab7b00,287);grade=compare(raw,original,tuple(rels),retail_relocations('00ab7b00',287));assert grade.near_match,grade
 assert struct.unpack('<I',read_virtual_data(retail,0x129ead4+4,4))[0]==0xab7b00
 assert read_virtual_data(retail,0x122dedc,4)==bytes(4) and read_virtual_data(retail,0x122dcb4,4)==struct.pack('<f',4294967296.0)
 # Fixed retail image has no PE relocation records here; validate the two audited absolute float operands directly.
 assert original[6:8]==bytes.fromhex('d905') and struct.unpack_from('<I',original,8)[0]==0x122dedc
 assert original[45:47]==bytes.fromhex('d805') and struct.unpack_from('<I',original,47)[0]==0x122dcb4
 (b/'original.bin').write_bytes(original);support=[compile(p,'helper'+str(i)) for i,p in enumerate(sources)];host=compile(fixture,'host');x,exe=link([root,*support,host],'probe');assert not x.returncode,x.stdout+x.stderr
 positive=run(exe,[b/'original.bin']);(b/'run.json').write_text(json.dumps(positive,indent=2));assert positive['code']==0 and 'cases=5377 original=5377 model=5377 pc53_nearest=1' in positive['stdout'],positive
 standalone=run(exe);assert standalone['code']==0 and 'cases=5377 original=0 model=5377 pc53_nearest=1' in standalone['stdout'],standalone
 repeat=run(exe,[b/'original.bin']);assert repeat==positive
 text=source.read_text();controls=[]
 specs=[('line-height','FontHeight + 1','FontHeight'),('ignore-first-line','if (firstLineOnly) return C2DExtentsF(maxX, maxY);','if (false) return C2DExtentsF(maxX, maxY);'),('no-line-reset','            currentX = 0.0f;','            ;'),('ignore-low-bound','static_cast<long>(character) < MinChar','false'),('ignore-high-bound','static_cast<long>(character) > MaxChar','false'),('wrong-bank','character >> 6','character >> 7'),('ignore-offset','glyph->Offset + currentX + glyph->Advance','currentX + glyph->Advance'),('width-as-advance','glyph->Offset + currentX + glyph->Advance','glyph->Offset + currentX + glyph->Width'),('glyph-upper-bound','index < bank.NoGlyphs','index <= bank.NoGlyphs'),('last-line-only','return C2DExtentsF(maxX, maxY);','return C2DExtentsF(currentX, currentY);'),('wide-accumulator','volatile float currentX','volatile double currentX')]
 for label,old,new in specs:
  assert text.count(old)==(2 if label=='last-line-only' else 1),(label,text.count(old));fault=compile(source,label,text.replace(old,new));x,fault_exe=link([fault,*support,host],label);assert not x.returncode,x.stdout+x.stderr;rejected=run(fault_exe,[b/'original.bin']);assert rejected['code']==80 and 'FONT_EXTENTS_FAILURE' in rejected['stderr'],(label,rejected);controls.append(dict(name=label,**rejected))
 omitted,_=link([*support,host],'missing-provider');assert omitted.returncode and 'GetStringExtents' in omitted.stdout+omitted.stderr;controls.append(dict(name='missing-provider',link_rejected=True))
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p,h in objects.items():assert sha(Path(p))==h,('object drift',p)
 report=dict(status='PASS',build=str(b),symbol=SYMBOL,grade=grade.to_dict(),cases=5377,original_cases=5377,controls=controls,repeat=True,run=positive,standalone=standalone,input_sha256=pins,objects=objects,source=str(source),object=str(root),relocations={str(k):v for k,v in rels.items()},boundary='Synthetic static glyph/text records, PC53-nearest only. Native extents ABI, signed glyph metrics, missing glyph/range/newline/height-wrap behavior and per-character float rounding compare with original instructions and independent model. Concrete constructor/destructor, other glyph services and borrowed glyph storage are fixture boundaries. No asset acquisition, glyph upload/rasterization or complete font lifecycle certification.')
 (b/'receipt.json').write_text(json.dumps(report,indent=2))
 if args.output is None and stage is None:(R/'rebuild/compile-gate/native-static-font-extents.json').write_text(json.dumps(report,indent=2))
 print('NATIVE_STATIC_FONT_EXTENTS_GATE_PASS cases=5377 original=5377 model=5377 controls=12 near=1 pc53_nearest=1',b,flush=True)
if __name__=='__main__':main()
