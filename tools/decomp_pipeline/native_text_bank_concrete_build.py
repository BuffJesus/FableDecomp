from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,subprocess,struct,hashlib
R=Path(__file__).resolve().parents[2];sys.path[:0]=[str(R/'tools/decomp_pipeline'),str(R/'rebuild/tests/integration')]
import check_one as V
from native_text_bank_concrete_sources import SPECS,WINMM
from gate_game_component_abi import Coff
from pe_oracle import EXE
b=Path(sys.argv[1]).resolve();b.mkdir(parents=True);print(b,flush=True)
sources=[R/'rebuild/src/compiled'/source for source,flags in SPECS];profiles={R/'rebuild/src/compiled'/source:flags for source,flags in SPECS}
sources.append(R/'rebuild/tests/integration/NativeBankAsyncHandleBridge.cpp');profiles[R/'rebuild/tests/integration/NativeBankAsyncHandleBridge.cpp']=['/Dmalloc=FixtureParseMalloc','/Dfree=FixtureParseFree']
fixture=R/'rebuild/tests/integration/NativeTextBankConcrete_test.cpp';seen=set();inc=b/'include';inc.mkdir()

def closure(p):
 if p in seen:return
 seen.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name,R/'rebuild/tests/integration'/name) if q.is_file()))
for p in sources+[fixture]:closure(p)
for p in seen:
 if p in sources or p==fixture:continue
 dest=inc/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else inc/p.name;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(p.read_bytes())
inputs=set(seen)|{Path(__file__),R/'tools/decomp_pipeline/native_text_bank_concrete_sources.py',EXE,WINMM}
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];object_sources={}
for i,p in enumerate(sources+[fixture]):
 snap=b/('Fixture.cpp' if p==fixture else 'root%d.cpp'%i);snap.write_bytes(p.read_bytes());obj=snap.with_suffix('.obj');flags=[]
 if p!=fixture:flags=profiles[p]
 result=V.cl(['/nologo','/c','/O2','/MT',*flags,'/I'+str(inc),'/Fo'+str(obj),str(snap)],env,b)
 (b/(snap.stem+'.compile.json')).write_text(json.dumps(dict(source=str(p),flags=flags,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)));assert not result.returncode,result.stdout+result.stderr
 objects.append(obj);object_sources[str(obj)]=str(p)
command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib',str(WINMM)];result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)));assert not result.returncode,result.stdout+result.stderr
raw=bytearray((i*37+23)&255 for i in range(0x9000));raw[:32]=struct.pack('<8I',0x42424242,0x73,106,1,8,3,1,64)
symbol=b'native_async_entry';entry=struct.pack('<3I',1,0,1)+struct.pack('<6I',0x2a2a0003,3,0,37,0x313,0)+struct.pack('<I',len(symbol))+symbol+struct.pack('<3I',0,0,4)+struct.pack('<i',-5);raw[64:64+len(entry)]=entry;(b/'synthetic.bin').write_bytes(raw);(b/'owned-writes.bin').write_bytes(b'')
result=subprocess.run([str(b/'probe.exe'),str(b/'synthetic.bin'),str(b/'owned-writes.bin')],cwd=b,capture_output=True,text=True,timeout=20);(b/'run.json').write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)));print(result.stdout,result.stderr,flush=True);assert not result.returncode
assert not [p for p,h in pins.items() if sha(Path(p))!=h], 'input drift'
(b/'receipt.json').write_text(json.dumps(dict(status='BUILT_HOST_PASS',input_sha256=pins,generated_files={str(p):sha(p) for p in (b/'synthetic.bin',b/'owned-writes.bin')},object_sources=object_sources,objects={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in objects},run=dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr)),indent=2))
