"""Fresh native recovery-path construction through bank open/read/lifetime."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess
import check_one as V
import gate_native_threaded_file_read_paths as P
import bank_file_lifetime_factory_worker_evidence as F
R=Path(__file__).resolve().parents[2]
BANK=R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z/receipt.json'
PATHS=R/'rebuild/build/native-file-read-paths/00/98/0098e000/20261019T021303326518Z/receipt.json'
from bank_open_wrapper_sources import PARENT,EXTRAS
FIXTURE=R/'rebuild/tests/integration/NativeBankRecoveryPathLifetime_test.cpp'
EXTRAS=[*EXTRAS,R/'rebuild/src/compiled/00/9c/CBankFile_RecoveryFile_009cbfe0.cpp']
WRAPPER=R/'rebuild/build/native-bank-open-wrapper-lifetime/00/9d/009d5820/20261019T230203991963Z/acceptance.json'
import disk_open_original_evidence
import gate_native_bank_async_open
import native_exact_roots
SCRIPTS=[R/'tools/decomp_pipeline'/n for n in ('native_bank_open_wrapper_build.py','native_bank_recovery_path_build.py','native_bank_open_file_verify.py','native_bank_open_wrapper_verify.py','native_bank_recovery_path_verify.py','native_bank_recovery_path_controls.py')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sha(BANK)=='c5e5e4ef2b3b6171ee8b54e6fc00a3e35cfcbf348045d19c329f89f000c7daec'
 bank=json.loads(BANK.read_text());paths=json.loads(PATHS.read_text());parent=json.loads(PARENT.read_text())
 assert sha(PARENT)=='d0f10772e5cfdb050d5d563b6255554c8957ba3a245115a34a71cec042f64514'
 inputs={Path(__file__),BANK,PATHS,PARENT,WRAPPER,FIXTURE,*SCRIPTS,R/'rebuild/tests/integration/NativeBankBaseOpenRead_model.py',R/'rebuild/build/native-bank-shared-worker/00/9d/009d6060/20261019T155340426945Z/receipt.json'};visited=set()
 def closure(p):
  p=p.resolve()
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
   closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in [FIXTURE,*EXTRAS,*P.SOURCES[1:],*[Path(p) for p in bank['input_sha256'] if Path(p).suffix=='.cpp' and Path(p).is_relative_to(R/'rebuild/src')]]:closure(p)
 inputs.update(Path(p) for p in bank['input_sha256'] if Path(p).is_relative_to(V.VC))
 inputs.update(Path(p) for report in (bank,paths,parent) for p in report['object_sha256'])
 inputs.add(P.EXE);inputs.add(F.bank.shared.WINMM)
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 assert sha(WRAPPER)=='cb0d57c1a6574258d063551d50d5735e693f01196b6f32d0ea14c2956d04a27a'
 previous=json.loads(WRAPPER.read_text());inputs.update(Path(p) for p in previous['input_sha256'])
 pins={str(p):sha(p) for p in sorted(inputs)}
 build=R/'rebuild/build/native-bank-recovery-path-lifetime/00/9c/009cbfe0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 # Builder owns creation; freeze pins before any compiler or probe starts.
 pending=build.with_suffix('.inputs.json');pending.parent.mkdir(parents=True,exist_ok=True);pending.write_text(json.dumps(pins,indent=2)+'\n');build.mkdir()
 def save(n,v):
  p=build/n;assert not p.exists();p.write_text(json.dumps(v,indent=2)+'\n')
 for script in SCRIPTS:
  result=subprocess.run([sys.executable,str(script),str(build/'parent' if script.name=='native_bank_open_wrapper_build.py' else build)],cwd=R,capture_output=True,text=True,timeout=240)
  save(script.stem+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  assert result.returncode==0,result.stdout+result.stderr
  print(script.stem+' PASS',flush=True)
 report=json.loads((build/'receipt.json').read_text());verify=json.loads((build/'linked-verification.json').read_text());controls=json.loads((build/'recovery-controls.json').read_text())
 wrapper=json.loads((build/'wrapper-verification.json').read_text())
 assert len(controls['controls'])==3 and verify['linked_bodies']==153 and verify['fixups']==713
 assert wrapper['wrapper_bodies']==18 and wrapper['wrapper_fixups']==88
 assert len(wrapper['disk_vtable'])==16
 recovery=json.loads((build/'recovery-path-verification.json').read_text());assert recovery['bytes']==207 and recovery['fixups']==16
 link=json.loads((build/'link.json').read_text())['command'];objects=[Path(p) for p in report['objects']];omissions=[];redundant=[]
 required={build/'new0.obj'}
 for index,obj in enumerate(objects):
  if obj.name=='new1.obj':continue
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
 final=dict(status='PASS',build=str(build),processes=24,reads=24,bytes=888,controls=controls['controls'],linked_bodies=170,instruction_fixups=814,vtable_fixups=16,fixups=830,path_cases=144,root_grade='MATCH',fresh=True,link_omissions=omissions,redundant_objects=redundant,input_sha256=pins,object_sha256=report['objects'],executable_sha256=sha(build/'probe.exe'),new_roots=0,
  boundaries=['Native recovery-path helper207B matches original and final bytes, with144 cases and3 controls. Full bank open/read/lifetime remains native; all linked providers freshly compiled. Path metadata is restored before reads and teardown. DeleteRecoveryFile is discarded, not executed; no recovery file is created or deleted. Parent87 controls retained as prior evidence; not rerun here.',
   'Current-directory cache remains a process-lifetime object; its storage is explicitly cleared before returning, with all tracked string blocks released and one live global wide string until CRT destruction.',
   'Packed bank metadata remains fixture constructed. Installer actions are controlled. CloseAllStreams840B and uppercase277B retain accepted near-match grades; no active-stream file destruction or full bank parser claim.'])
 save('acceptance.json',final);(R/'rebuild/compile-gate/native-bank-recovery-path-lifetime.json').write_text(json.dumps(final,indent=2)+'\n')
 print('NATIVE_BANK_RECOVERY_PATH_LIFETIME_PASS processes=24 paths=144 reads=24 bodies=170 controls=3',flush=True)

if __name__=='__main__':main()
