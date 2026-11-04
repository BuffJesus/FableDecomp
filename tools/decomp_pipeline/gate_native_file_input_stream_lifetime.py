"""Fresh native buffered-input lifetime, original/final bytes and defect controls."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,struct,subprocess
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from file_input_stream_sources import SOURCES,ROOTS,TARGETS
R=Path(__file__).resolve().parents[2]
FIXTURE=R/'rebuild/tests/integration/NativeFileInputStreamLifetime_test.cpp'
HELPERS=[R/'tools/decomp_pipeline'/('native_file_input_stream_'+name+'.py') for name in ('verify','controls')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 inputs={Path(__file__),EXE,*HELPERS};visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in [FIXTURE,*SOURCES]:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(p for directory in (V.VC/'lib',V.VC/'bin') for p in directory.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-file-input-stream-lifetime/00/99/00994700'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins)
 for p in visited:
  if p.suffix=='.h':
   q=inc/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else b/p.name;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];providers=[]
 for index,p in enumerate([*SOURCES,FIXTURE]):
  snap=b/p.name;snap.write_bytes(p.read_bytes());obj=b/('provider%d.obj'%index)
  result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(inc),'/Fo'+str(obj),str(snap)],env,b);save('compile%d.json'%index,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
  objects.append(obj)
  if p!=FIXTURE:providers.append(dict(source=str(p),object=str(obj)))
 save('providers.json',providers);original=EXE.read_bytes();records=[]
 for index,symbol,address,size in ROOTS:
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert not off and len(body)==size;patched=bytearray(body)
  for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(patched)==read_virtual_data(original,address,size),symbol
  records.append(dict(symbol=symbol,address=hex(address),bytes=size,match=True,object=str(objects[index]),source=str(SOURCES[index]),relocations=rels))
 save('original-audit.json',dict(status='PASS',roots=records,targets=TARGETS))
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects),'kernel32.lib']
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True,timeout=30);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=15);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=24 allocations=24 frees=24 reads=78 seeks=90' in result.stdout,result.stdout+result.stderr
 for helper in HELPERS:
  result=subprocess.run([sys.executable,str(helper),str(b)],cwd=R,capture_output=True,text=True,timeout=120);save(helper.stem+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 linked=json.loads((b/'linked-verification.json').read_text());controls=json.loads((b/'controls-verification.json').read_text());assert linked['linked_bodies']==8 and len(controls['controls'])==6
 omissions=[];redundant=[]
 for index,obj in enumerate(objects[:-1]):
  exe=b/('omit%d.exe'%index);cmd=['/out:'+str(exe) if s.startswith('/out:') else s for s in command if s!=str(obj) and not s.startswith('/map:')]
  result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True,timeout=30);save('omit%d.link.json'%index,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  (omissions if result.returncode else redundant).append(index)
  if index!=2:assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout),index
 for name,h in pins.items():assert sha(Path(name))==h,('input drift',name)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 receipt=dict(status='PASS',build=str(b),cases=24,reads=78,allocations=24,frees=24,original_bodies=13,linked_bodies=8,controls=controls['controls'],omissions=omissions,redundant=redundant,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),new_roots=0,boundary='Native file-input constructor/read/seek/window refill/direct read/close/destruction. Backing CAFile and base lifetime controlled; unused virtual methods fail-fast. Five unreferenced helper copies discarded; caller expansions and eight active final bodies exact. No real disk read or bank header parser claim.')
 save('receipt.json',receipt);(R/'rebuild/compile-gate/native-file-input-stream-lifetime.json').write_text(json.dumps(receipt,indent=2)+'\n');print('NATIVE_FILE_INPUT_STREAM_LIFETIME_PASS cases=24 original=13 linked=8 controls=6')

if __name__=='__main__':main()
