"""Fresh genuine base virtual callback identity/ABI gate dispatch."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeThreadedBaseCallbacks_test.py')],capture_output=True,text=True,timeout=180)
assert p.returncode==0,p.stdout+p.stderr
assert 'NATIVE_BASE_CALLBACKS_PASS 768 384' in p.stdout
report=json.loads((R/'rebuild/compile-gate/native-threaded-base-callbacks.json').read_text())
assert report['status']=='NATIVE_BASE_CALLBACKS_PASS' and report['grade']=='MATCH'
assert report['root_bodies']=={'begin':'b001c20800','completion':'c20400'}
for mode in ('original_run','compiled_run'):assert [x['cases'] for x in report[mode]['runs']]==[768,384]
assert [x['actual_caller_cases'] for x in report['caller_runs']]==[4,4] and len(report['omissions'])==2
for field in ('input_after','compiler_after','objects_after','linked_after'):
 for path,digest in report[field].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==digest,path
print('NATIVE_THREADED_BASE_CALLBACKS_GATE_PASS 768 384 4 4')
