"""Actual memory-stream lifetime/overflow, explicit external LZO boundary."""
import json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
result=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeMemoryStreamComposition_test.py')],cwd=R,capture_output=True,text=True,timeout=180)
assert not result.returncode,(result.returncode,result.stdout,result.stderr)
assert 'MEMORY_STREAM_COMPOSITION_PASS 8064 28752' in result.stdout,result.stdout
report=json.loads((R/'rebuild/compile-gate/native-memory-stream-composition.json').read_text())
assert (report['cases'],report['snapshots'])==(8064,28752)
assert (report['installed_providers'],report['executed_providers'],report['unique_provider_sources'])==(10,9,8)
assert len(report['providers'])==10 and all(p['grade']=='MATCH' and p['complete_decode'] for p in report['providers'])
assert len(report['omission_checks'])==10 and len({p['symbol'] for p in report['omission_checks']})==10
assert all(p['missing_failed'] and p['provider_link_passed'] for p in report['omission_checks'])
hits=report['provider_hits'];assert hits['retail']==hits['compiled']
assert sum(v>0 for v in hits['retail'].values())==9
assert hits['retail']['?InternalReadNextSourceChunk@CDataInputStream@@EAEXAAPBEAAJ@Z']==0
assert report['input_sha256'] and report['original_exe_sha256'] and report['memory_vtable_sha256']
print(result.stdout.strip())
print('NATIVE_MEMORY_STREAM_COMPOSITION_GATE_PASS')
