"""Verify typed disk-file counted ownership against original and host execution."""
import hashlib,json,re,sys
from datetime import datetime,timezone
from pathlib import Path
import check_one as V
from pe_oracle import EXE
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'rebuild/tests/integration'))
from NativeDiskCountedReset_model import compare_disk_reset
import disk_counted_reset_host_evidence as host
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/9d/CCountedPointer_DiskFile_Reset_009d1ee0.cpp'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 build=R/'rebuild/build/native-disk-counted-reset/00/9d/009d1ee0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');build.mkdir(parents=True)
 include=build/'include';include.mkdir();source=build/'root.cpp'
 inputs={Path(__file__),EXE,R/'rebuild/tests/integration/NativeDiskCountedReset_model.py',R/'rebuild/tests/integration/NativeDiskCountedReset_state.py',R/'tools/decomp_pipeline/threaded_file_counted_delete_oracle.py'};visited=set();inputs.add(Path(host.__file__))
 inputs.update(R/'tools/decomp_pipeline'/name for name in ('threaded_file_open_counters_oracle.py','threaded_file_open_write_oracle.py','check_one.py','gate_game_component_abi.py','pe_oracle.py'))
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 closure(SOURCE);closure(R/'rebuild/tests/integration/NativeDiskCountedReset_test.cpp')
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(V.VC/'bin'/name for name in ('cl.exe','link.exe','c1xx.dll','c2.dll'))
 inputs.update(V.VC/'lib'/n for n in ('libcmt.lib','oldnames.lib','kernel32.lib'))
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins);source.write_bytes(SOURCE.read_bytes());(build/'gate-source.py').write_bytes(Path(__file__).read_bytes());(build/'model-source.py').write_bytes((R/'rebuild/tests/integration/NativeDiskCountedReset_model.py').read_bytes())
 for p in visited:
  if p==SOURCE:continue
  target=build/'fixture.cpp' if p.name=='NativeDiskCountedReset_test.cpp' else include/p.relative_to(R/'rebuild/include');target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=';'.join(map(str,(V.VC/'include',include)))
 obj=build/'root.obj';cmd=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(include),'/Fo'+str(obj),str(source)]
 result=V.cl(cmd,env,build);save('compile.json',dict(command=cmd,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
 for line in result.stdout.splitlines():
  if 'including file:' in line.lower():
   p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(V.VC/'include') or p.is_relative_to(build)
 report=compare_disk_reset(obj);assert report['cases']==400 and len(report['controls'])==3
 report.update(host.validate(build,obj,env,save))
 report['executable_sha256']=sha(build/'probe.exe')
 assert all(sha(Path(p))==h for p,h in pins.items()),'Input drift'
 provenance={}
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   target=build/'python-provenance'/p.relative_to(R);target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes());provenance[str(target)]=sha(target)
 report['python_provenance_sha256']=provenance
 report.update(build=str(build),input_sha256=pins,object_sha256=sha(obj),source_sha256=sha(SOURCE))
 save('receipt.json',report);(R/'rebuild/compile-gate/native-disk-counted-reset.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_DISK_COUNTED_RESET_PASS reset=144 delete=256 host=36 controls=9 grade=MATCH');print('RECEIPT',build/'receipt.json')

if __name__=='__main__':main()
