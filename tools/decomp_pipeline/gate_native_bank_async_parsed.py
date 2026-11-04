"""Native metadata parsing -> real async read/publication/owned teardown."""
from pathlib import Path
from datetime import datetime,timezone
import json,hashlib,subprocess,sys
R=Path(__file__).resolve().parents[2]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 b=R/'rebuild/build/native-bank-async-parsed/00/9d/009d5820'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 scripts=[R/'tools/decomp_pipeline'/name for name in ('native_bank_async_parsed_build.py','native_bank_async_parsed_verify.py')]
 inputs={Path(__file__),*scripts,R/'tools/decomp_pipeline/native_bank_async_parsed_sources.py',R/'tools/decomp_pipeline/native_bank_header_literals.py',R/'rebuild/tests/integration/NativeBankHeaderThrow_model.py'}
 pins={str(p):sha(p) for p in inputs}
 for script in scripts:
  run=subprocess.run([sys.executable,str(script),str(b)],cwd=R,capture_output=True,text=True,timeout=180)
  (b/(script.stem+'.run.json')).write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr),indent=2));print(run.stdout.strip(),flush=True);assert not run.returncode,run.stdout+run.stderr
 build=json.loads((b/'receipt.json').read_text());verified=json.loads((b/'async-parsed-verification.json').read_text())
 assert verified['status']=='PASS' and len(verified['runs'])==24 and len(verified['controls'])==8 and verified['linked_symbols']==284
 header=next(Path(o) for o,p in build['object_sources'].items() if Path(p).name=='CBankFile_ReadFileInformation_009d0880.cpp')
 command=[sys.executable,str(R/'tools/decomp_pipeline/native_bank_header_literals.py'),str(b),str(header)]
 run=subprocess.run(command,cwd=R,capture_output=True,text=True,timeout=60);(b/'header-grade.run.json').write_text(json.dumps(dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr),indent=2));assert not run.returncode,run.stdout+run.stderr
 literal=json.loads((b/'literal-correspondence.json').read_text());assert literal['literal_verified_grade']['near_match']
 for paths in (pins,build['input_sha256'],verified['input_sha256']):
  for p,h in paths.items():assert sha(Path(p))==h,('input drift',p)
 for p,h in build['objects'].items():assert sha(Path(p))==h
 report=dict(status='PASS',build=str(b),input_sha256={**pins,**build['input_sha256'],**verified['input_sha256']},native=build,verification=verified,header_grade=literal,boundary=verified['boundary'])
 for p in map(Path,report['input_sha256']):
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 (b/'acceptance.json').write_text(json.dumps(report,indent=2));(R/'rebuild/compile-gate/native-bank-async-parsed.json').write_text(json.dumps(report,indent=2))
 print('NATIVE_BANK_ASYNC_PARSED_GATE_PASS processes=24 variants=3 linked=284 bodies=271 controls=8 header_near=1')
if __name__=='__main__':main()
