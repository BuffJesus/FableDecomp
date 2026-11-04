"""Fresh native path/open, bank worker read and ordered file/bank teardown."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess
import check_one as V
import gate_native_threaded_file_read_paths as P
import bank_file_lifetime_factory_worker_evidence as F
R=Path(__file__).resolve().parents[2]
BANK=R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z/receipt.json'
PATHS=R/'rebuild/build/native-file-read-paths/00/98/0098e000/20261019T021303326518Z/receipt.json'
FIXTURE=R/'rebuild/tests/integration/NativeOpenFileBankLifetimeOwner_test.cpp'
SCRIPTS=[R/'tools/decomp_pipeline'/('native_bank_open_file_'+s+'.py') for s in ('build','verify','controls')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sha(BANK)=='c5e5e4ef2b3b6171ee8b54e6fc00a3e35cfcbf348045d19c329f89f000c7daec'
 bank=json.loads(BANK.read_text());paths=json.loads(PATHS.read_text())
 inputs={Path(__file__),BANK,PATHS,FIXTURE,*SCRIPTS};visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
   closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in [FIXTURE,*P.SOURCES[1:],*[Path(p) for p in bank['input_sha256'] if Path(p).suffix=='.cpp' and Path(p).is_relative_to(R/'rebuild/src')]]:closure(p)
 inputs.update(Path(p) for p in bank['input_sha256'] if Path(p).is_relative_to(V.VC))
 inputs.update(Path(p) for report in (bank,paths) for p in report['object_sha256'])
 inputs.add(P.EXE);inputs.add(F.bank.shared.WINMM)
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in sorted(inputs)}
 build=R/'rebuild/build/native-bank-open-file-lifetime/00/98/0098e1e0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 # Builder owns creation; freeze pins before any compiler or probe starts.
 pending=build.with_suffix('.inputs.json');pending.parent.mkdir(parents=True,exist_ok=True);pending.write_text(json.dumps(pins,indent=2)+'\n')
 def save(n,v):
  p=build/n;assert not p.exists();p.write_text(json.dumps(v,indent=2)+'\n')
 for script in SCRIPTS:
  result=subprocess.run([sys.executable,str(script),str(build)],cwd=R,capture_output=True,text=True,timeout=240)
  save(script.stem+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  assert result.returncode==0,result.stdout+result.stderr
  print(script.stem+' PASS',flush=True)
 report=json.loads((build/'receipt.json').read_text());verify=json.loads((build/'linked-verification.json').read_text());controls=json.loads((build/'controls-verification.json').read_text())
 assert report['fresh'] and len(controls['controls'])==75 and verify['linked_bodies']==151 and verify['fixups']==706
 link=json.loads((build/'link.json').read_text())['command'];objects=[Path(p) for p in report['objects']];omissions=[];redundant=[]
 required={build/('path%d.obj'%i) for i in (1,3,5,31,33,34,35,36,40,41)}
 for index,obj in enumerate(objects):
  if obj.name=='fixture.obj':continue
  exe=build/('omit-%d.exe'%index)
  command=[('/out:'+str(exe)) if arg.startswith('/out:') else arg for arg in link if arg!=str(obj) and not arg.startswith('/map:')]
  result=subprocess.run(command,cwd=build,env=V.env(),capture_output=True,text=True,timeout=30)
  save('omit-%d.link.json'%index,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  (omissions if result.returncode else redundant).append(str(obj))
  if obj in required and result.returncode==0:
   run=subprocess.run([str(exe),str(build/'synthetic.bin'),str(build/'owned-writes.bin')],cwd=build,capture_output=True,text=True,timeout=15)
   save('omit-%d.run.json'%index,dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr));assert run.returncode==80 and 'NATIVE_WORKER_FAILURE' in run.stderr,obj
 for p,h in pins.items():assert sha(Path(p))==h,('Input drift',p)
 for p,h in report['objects'].items():assert sha(Path(p))==h
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   snap=build/'python-provenance'/p.relative_to(R);snap.parent.mkdir(parents=True,exist_ok=True);snap.write_bytes(p.read_bytes())
 final=dict(status='PASS',build=str(build),processes=24,reads=24,bytes=888,controls=controls['controls'],linked_bodies=151,fixups=706,link_omissions=omissions,redundant_objects=redundant,input_sha256=pins,object_sha256=report['objects'],executable_sha256=sha(build/'probe.exe'),new_roots=0,
  boundaries=['Native path/string conversion and cached Open supply the handle and filename for actual bank factory/worker read and ordered file/bank destruction. D: drive selects native device1. Six added open controls plus69 inherited ownership controls pass.',
   'Current-directory cache remains a process-lifetime object; its storage is explicitly cleared before returning, with all tracked string blocks released and one live global wide string until CRT destruction.',
   'Packed bank metadata remains fixture constructed. Installer actions are controlled. CloseAllStreams840B and uppercase277B retain accepted near-match grades; no active-stream file destruction or full bank parser claim.'])
 save('acceptance.json',final);(R/'rebuild/compile-gate/native-bank-open-file-lifetime.json').write_text(json.dumps(final,indent=2)+'\n')
 print('NATIVE_BANK_OPEN_FILE_LIFETIME_PASS processes=24 reads=24 bytes=888 bodies=151 fixups=706 controls=75',flush=True)

if __name__=='__main__':main()
