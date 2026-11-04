"""Fresh genuine public-static provider dispatch; no scratch dependencies."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
run=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeDirtyDisc_test.py')],capture_output=True,text=True,timeout=120)
assert run.returncode==0,run.stdout+run.stderr
assert 'NATIVE_DIRTY_DISC_PASS' in run.stdout
report=json.loads((R/'rebuild/compile-gate/native-dirty-disc.json').read_text())
assert report['status']=='NATIVE_DIRTY_DISC_PASS' and report['grade']=='MATCH' and report['root_bytes']=='c3' and report['root_relocations']==[]
assert report['root_symbol']=='?ProcessDirtyDiscError@CThreadedFileManager@@SIXXZ'
assert [r['cases'] for r in report['abi']]==[32,32]
for p,h in report['input_after'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
for p,h in report['compiler_after'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
for p,h in report['fresh_objects_after'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
print('NATIVE_DIRTY_DISC_GATE_PASS 32 32')
