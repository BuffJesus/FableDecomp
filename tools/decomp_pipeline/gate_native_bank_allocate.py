"""Exact bank allocation root with native release helpers and bounded mutation oracle."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,hashlib
import check_one as V
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankAllocate_model as M
SCRIPTS=[R/'tools/decomp_pipeline'/('native_bank_allocate_'+n+'.py') for n in ('build','verify','controls')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 inputs={Path(__file__),Path(M.__file__),EXE,*SCRIPTS,R/'rebuild/src/compiled/00/9c/CBankFile_AllocateBuffers_009ceae0.cpp',R/'rebuild/tests/integration/NativeBankAllocate_test.cpp'}
 inputs.update(p for p in (R/'rebuild/include').rglob('*') if p.is_file());inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(p for directory in (V.VC/'bin',V.VC/'lib') for p in directory.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-allocate/00/9c/009ceae0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.parent.mkdir(parents=True,exist_ok=True);b.with_suffix('.inputs.json').write_text(json.dumps(pins,indent=2)+'\n')
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 for script in SCRIPTS:
  run=subprocess.run([sys.executable,str(script),str(b)],cwd=R,capture_output=True,text=True,timeout=120);save(script.stem+'.run.json',dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr));assert not run.returncode,run.stdout+run.stderr;print(script.stem+' PASS',flush=True)
 model=M.compare(b/'candidate.obj');oracle=M.oracle_controls();assert model['cases']==3584 and len(oracle)==8
 native=json.loads((b/'native-controls.json').read_text());verify=json.loads((b/'verification.json').read_text());run=json.loads((b/'run.json').read_text());assert len(native['controls'])==8 and len(verify['records'])==4 and 'cases=192 copy_cases=6' in run['stdout']
 command=json.loads((b/'link.json').read_text())['command'];command=[s for s in command if s!=str(b/'candidate.obj') and not s.startswith('/map:')];command=['/out:'+str(b/'missing-root.exe') if s.startswith('/out:') else s for s in command]
 missing=subprocess.run(command,cwd=b,env=V.env(),capture_output=True,text=True,timeout=30);save('missing-root.link.json',dict(returncode=missing.returncode,stdout=missing.stdout,stderr=missing.stderr));assert missing.returncode and ('LNK2001' in missing.stdout or 'LNK2019' in missing.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',grade='MATCH',build=str(b),model=model,oracle_controls=oracle,native_controls=native['controls'],host_cases=192,copy_cases=6,records=verify['records'],input_sha256=pins,object_sha256={str(p):sha(p) for p in (b/'candidate.obj',b/'Fixture.obj')},executable_sha256=sha(b/'probe.exe'),missing_root_rejected=True,boundary='Exact375B bank allocation root,90B runtime release,95B symbol release and74B runtime copy.192 host allocation cases and6 guarded copy cases; actual CRT free forwarded by counted observer. Resize and string/tree erasure controlled.3584 full12KiB differential cases include helper mutation/reload behavior. No allocation failure or exception-unwinding claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-allocate.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_ALLOCATE_PASS bytes=375 model=3584 host=192 copy=6 bodies=4 controls=16 grade=MATCH')
if __name__=='__main__':main()
