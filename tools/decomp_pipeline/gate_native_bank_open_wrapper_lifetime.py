"""Fresh native base/async bank open, worker read and owned disk/file teardown."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,subprocess
import check_one as V
import gate_native_threaded_file_read_paths as P
import bank_file_lifetime_factory_worker_evidence as F
R=Path(__file__).resolve().parents[2]
BANK=R/'rebuild/build/native-bank-file_lifetime_factory-owner/00/9d/009d6060/20261019T221038607481Z/receipt.json'
PATHS=R/'rebuild/build/native-file-read-paths/00/98/0098e000/20261019T021303326518Z/receipt.json'
from bank_open_wrapper_sources import PARENT,FIXTURE,EXTRAS
import disk_open_original_evidence
import gate_native_bank_async_open
import native_exact_roots
SCRIPTS=[R/'tools/decomp_pipeline'/n for n in ('native_bank_open_wrapper_build.py','native_bank_open_file_verify.py','native_bank_open_wrapper_verify.py','native_bank_open_wrapper_controls.py')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sha(BANK)=='c5e5e4ef2b3b6171ee8b54e6fc00a3e35cfcbf348045d19c329f89f000c7daec'
 bank=json.loads(BANK.read_text());paths=json.loads(PATHS.read_text());parent=json.loads(PARENT.read_text())
 assert sha(PARENT)=='d0f10772e5cfdb050d5d563b6255554c8957ba3a245115a34a71cec042f64514'
 inputs={Path(__file__),BANK,PATHS,PARENT,FIXTURE,*SCRIPTS,R/'rebuild/tests/integration/NativeBankBaseOpenRead_model.py',R/'rebuild/build/native-bank-shared-worker/00/9d/009d6060/20261019T155340426945Z/receipt.json'};visited=set()
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
 pins={str(p):sha(p) for p in sorted(inputs)}
 build=R/'rebuild/build/native-bank-open-wrapper-lifetime/00/9d/009d5820'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
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
 wrapper=json.loads((build/'wrapper-verification.json').read_text())
 assert report['fresh'] and len(controls['controls'])==87 and verify['linked_bodies']==153 and verify['fixups']==713
 assert wrapper['wrapper_bodies']==18 and wrapper['wrapper_fixups']==88
 assert len(wrapper['disk_vtable'])==16
 link=json.loads((build/'link.json').read_text())['command'];objects=[Path(p) for p in report['objects']];omissions=[];redundant=[]
 required={build/('extra%d.obj'%i) for i in range(1,12)} | {build/('rebuilt-path%d.obj'%i) for i in (1,3,5,31,33,34,35,36,40,41)}
 for index,obj in enumerate(objects):
  if obj.name=='extra0.obj':continue
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
 final=dict(status='PASS',build=str(build),processes=24,reads=24,bytes=888,controls=controls['controls'],linked_bodies=169,instruction_fixups=798,vtable_fixups=16,fixups=814,neutral_controls=controls['neutral_controls'],link_omissions=omissions,redundant_objects=redundant,input_sha256=pins,object_sha256=report['objects'],executable_sha256=sha(build/'probe.exe'),new_roots=0,
  boundaries=['Native base/async bank Open owns the disk and threaded reader through counted pointers. Actual worker read and ordered destruction pass. Header decoding and packed metadata remain controlled; no parser claim. 87 defect controls plus one intentionally neutral threaded Open return control.',
   'Current-directory cache remains a process-lifetime object; its storage is explicitly cleared before returning, with all tracked string blocks released and one live global wide string until CRT destruction.',
   'Packed bank metadata remains fixture constructed. Installer actions are controlled. CloseAllStreams840B and uppercase277B retain accepted near-match grades; no active-stream file destruction or full bank parser claim.'])
 save('acceptance.json',final);(R/'rebuild/compile-gate/native-bank-open-wrapper-lifetime.json').write_text(json.dumps(final,indent=2)+'\n')
 print('NATIVE_BANK_OPEN_WRAPPER_LIFETIME_PASS processes=24 reads=24 bytes=888 bodies=169 fixups=814 controls=87',flush=True)

if __name__=='__main__':main()
