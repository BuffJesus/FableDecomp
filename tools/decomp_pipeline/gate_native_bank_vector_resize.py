"""Fresh bank-vector resize roots, native checksum chain and bounded mutation oracle."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,subprocess,hashlib
import check_one as V
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankVectorResize_model as M
SCRIPTS=[R/'tools/decomp_pipeline'/('native_bank_vector_resize_'+n+'.py') for n in ('build','verify','controls')]
SOURCES=[R/'rebuild/src/compiled'/p for p in ('00/9d/STLBankRuntimeVector_Resize_009d4010.cpp','00/9d/STLBankUpdateVector_Resize_009d3df0.cpp','00/49/STLCharStringVector_Resize_0049b760.cpp','00/46/UlongVector_Resize_00464931.cpp','00/46/UlongVector_Storage_0046496f.cpp','00/46/UlongVector_Overflow_00464aab.cpp')]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 inputs={Path(__file__),Path(M.__file__),EXE,*SCRIPTS,*SOURCES,R/'rebuild/tests/integration/NativeBankVectorResize_test.cpp'}
 inputs.update(p for p in (R/'rebuild/include').rglob('*') if p.is_file());inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
 inputs.update(p for directory in (V.VC/'bin',V.VC/'lib') for p in directory.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-vector-resize/00/9d/009d4010'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.parent.mkdir(parents=True,exist_ok=True);b.with_suffix('.inputs.json').write_text(json.dumps(pins,indent=2)+'\n')
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 for script in SCRIPTS:
  run=subprocess.run([sys.executable,str(script),str(b)],cwd=R,capture_output=True,text=True,timeout=120);save(script.stem+'.run.json',dict(returncode=run.returncode,stdout=run.stdout,stderr=run.stderr));assert not run.returncode,run.stdout+run.stderr;print(script.stem+' PASS',flush=True)
 model=M.compare(b);oracle=M.oracle_controls(b);assert model['cases']==400 and len(oracle)==10
 native=json.loads((b/'native-controls.json').read_text());verify=json.loads((b/'verification.json').read_text());run=json.loads((b/'run.json').read_text());assert len(native['controls'])==10 and len(verify['records'])==9 and len(verify['omissions'])==6 and 'cases=200' in run['stdout']
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 objects=[b/(name+'.obj') for name in ('runtime','updates','checksums','checksum-storage','checksum-overflow','symbols','Fixture')]
 report=dict(status='PASS',grade='MATCH',build=str(b),model=model,oracle_controls=oracle,native_controls=native['controls'],host_cases=200,records=verify['records'],omissions=verify['omissions'],input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='Four exact resize roots: runtime87B, update-pointer87B, symbols101B, existing checksum62B. Native runtime copy and complete five-body checksum chain give9 final verified bodies.400 full4KiB differential cases and200 guarded native cases include shrink/equal/grow and fill aliases. Other fill operations and string construction/destruction/erasure controlled. Checksum host cases use spare capacity; existing full unsigned-long gate covers growth. No exception-unwinding claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-vector-resize.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_VECTOR_RESIZE_PASS model=400 host=200 bodies=9 controls=20 grade=MATCH')
if __name__=='__main__':main()
