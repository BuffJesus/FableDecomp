"""Fresh canonical Close gate dispatch; functional child interfaces disclosed."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeThreadedClose_test.py')],capture_output=True,text=True,timeout=180)
assert p.returncode==0,p.stdout+p.stderr
assert 'NATIVE_THREADED_CLOSE_PASS RELOCATION_MATCH 144 360' in p.stdout
report=json.loads((R/'rebuild/compile-gate/native-threaded-file-close.json').read_text())
assert report['status']=='NATIVE_THREADED_CLOSE_PASS' and report['grade']=='RELOCATION_MATCH'
assert report['root_symbol']=='?Close@CThreadedFile@@QAEXXZ' and len(report['relocations'])==3 and all(r['addend']==0 for r in report['relocations'])
for run in (report['original_run'],report['compiled_run']):assert run['cases']==144 and run['snapshots']==360 and run['skip_cases']==36 and run['called_cases']==108
for name in ('input_after','compiler_after','object_after','linked_after'):
 for path,digest in report[name].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==digest,path
print('NATIVE_THREADED_FILE_CLOSE_GATE_PASS 144 360')
