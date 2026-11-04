"""Verify independent retail counters in mixed read/write publication sequences."""
import hashlib,json,re
from datetime import datetime,timezone
from pathlib import Path
import check_one as V
from pe_oracle import EXE
from threaded_file_open_counters_oracle import compare_counters
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFile_Open_0098e1e0.cpp'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 build=R/'rebuild/build/native-file-open-counters/00/98/0098e1e0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();source=build/'root.cpp'
 inputs={Path(__file__),EXE};visited=set()
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
 save('input-pins.json',pins);source.write_bytes(SOURCE.read_bytes());(build/'gate-source.py').write_bytes(Path(__file__).read_bytes())
 for p in visited:
  if p==SOURCE:continue
  target=include/p.relative_to(R/'rebuild/include');target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=';'.join(map(str,(V.VC/'include',include)))
 obj=build/'root.obj';cmd=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(include),'/Fo'+str(obj),str(source)]
 result=V.cl(cmd,env,build);save('compile.json',dict(command=cmd,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
 for line in result.stdout.splitlines():
  if 'including file:' in line.lower():
   p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(V.VC/'include') or p.is_relative_to(build)
 report=compare_counters(obj);assert report['cases']==1024 and report['publication_blocks']==6144
 assert all(sha(Path(p))==h for p,h in pins.items()),'Input drift'
 report.update(build=str(build),input_sha256=pins,object_sha256=sha(obj),source_sha256=sha(SOURCE),independent_counters=['013bc9e8','013bc9ec'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-file-open-counters.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_FILE_OPEN_COUNTERS_PASS sequences=1024 blocks=6144 roots=2 counters=2 controls=1');print('RECEIPT',build/'receipt.json')

if __name__=='__main__':main()
