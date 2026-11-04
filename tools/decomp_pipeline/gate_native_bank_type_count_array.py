"""Exact bank type-count vector construction, guarded host allocation and oracle."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,hashlib,re,struct
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankTypeCountArray_model as M
SOURCE=R/'rebuild/src/compiled/00/9d/STLBankTypeCountVector_Constructor_009d2af0.cpp'
FIXTURE=R/'rebuild/tests/integration/NativeBankTypeCountArray_test.cpp'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 inputs={Path(__file__),Path(M.__file__),SOURCE,FIXTURE,EXE};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 closure(SOURCE);closure(FIXTURE)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-type-count-array/00/9d/009d2af0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root.cpp' if p==SOURCE else 'Fixture.cpp') if p in (SOURCE,FIXTURE) else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
 for name in ('root','Fixture'):
  result=V.cl(['/nologo','/c','/O2','/MT',*(['/Dmalloc=FixtureTypeCountMalloc'] if name=='root' else []),'/I'+str(inc),'/Fo'+str(b/(name+'.obj')),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 report=M.compare(b/'root.obj');assert report['cases']==35 and len(report['controls'])==5
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),str(b/'root.obj'),str(b/'Fixture.obj'),'kernel32.lib'];result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=15);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=14' in result.stdout,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 c=Coff(b/'root.obj');s,o=c.symbols[M.SYMBOL];body,rels=c.sections[s];entry=mapping[M.SYMBOL];fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4))&0xffffffff)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body));start=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 mutations=[('allocation-width',0x9d2b12,b'\xbd'),('first-word',0x9d2b39,b'\x90'*2),('second-word',0x9d2b3b,b'\x90'*3),('stride',0x9d2b40,b'\x04'),('finish',0x9d2b44,b'\x90'*3)]
 for n,(name,address,replacement) in enumerate(mutations):
  changed=bytearray(raw);offset=start+address-M.ENTRY;changed[offset:offset+len(replacement)]=replacement;exe=b/('control-%d.exe'%n);exe.write_bytes(changed);result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=15);save('control-%d.run.json'%n,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'TYPE_COUNT_FAILURE' in result.stderr,(name,result.returncode,result.stderr)
 missing=['/out:'+str(b/'missing-root.exe') if s.startswith('/out:') else s for s in command if s!=str(b/'root.obj') and not s.startswith('/map:')];result=subprocess.run(missing,cwd=b,env=env,capture_output=True,text=True);save('missing-root.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report.update(build=str(b),host_cases=14,native_controls=[x[0] for x in mutations],input_sha256=pins,object_sha256={str(p):sha(p) for p in (b/'root.obj',b/'Fixture.obj')},executable_sha256=sha(b/'probe.exe'),missing_root_rejected=True)
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-type-count-array.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_TYPE_COUNT_ARRAY_PASS bytes=94 model=35 host=14 controls=10 grade=MATCH')
if __name__=='__main__':main()
