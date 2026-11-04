"""Verify base bank read-open state, cleanup and notification against original."""
import hashlib,json,re,sys
from datetime import datetime,timezone
from pathlib import Path
import check_one as V
from pe_oracle import EXE
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'rebuild/tests/integration'))
from NativeBankBaseOpenRead_model import compare_bank_open
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/9d/CBankFile_OpenFileReadOnly_009d1090.cpp'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 build=R/'rebuild/build/native-bank-base-open-read/00/9d/009d1090'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();source=build/'root.cpp'
 inputs={Path(__file__),EXE,R/'rebuild/tests/integration/NativeBankBaseOpenRead_model.py'};visited=set()
 inputs.update(R/'tools/decomp_pipeline'/name for name in ('threaded_file_open_counters_oracle.py','threaded_file_open_write_oracle.py','check_one.py','gate_game_component_abi.py','pe_oracle.py'))
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 closure(SOURCE)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(V.VC/'bin'/name for name in ('cl.exe','c1xx.dll','c2.dll'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins);source.write_bytes(SOURCE.read_bytes());(build/'gate-source.py').write_bytes(Path(__file__).read_bytes());(build/'model-source.py').write_bytes((R/'rebuild/tests/integration/NativeBankBaseOpenRead_model.py').read_bytes())
 for p in visited:
  if p==SOURCE:continue
  target=include/p.relative_to(R/'rebuild/include');target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=';'.join(map(str,(V.VC/'include',include)))
 obj=build/'root.obj';cmd=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(include),'/Fo'+str(obj),str(source)]
 result=V.cl(cmd,env,build);save('compile.json',dict(command=cmd,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
 for line in result.stdout.splitlines():
  if 'including file:' in line.lower():
   p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(V.VC/'include') or p.is_relative_to(build)
 report=compare_bank_open(obj);assert report['cases']==432 and len(report['controls'])==9
 assert all(sha(Path(p))==h for p,h in pins.items()),'Input drift'
 report.update(build=str(build),input_sha256=pins,object_sha256=sha(obj),source_sha256=sha(SOURCE))
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-base-open-read.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_BASE_OPEN_READ_PASS bytes=204 cases=432 controls=9 grade=MATCH');print('RECEIPT',build/'receipt.json')

if __name__=='__main__':main()
