"""Bounded existing-owner composition gate; no new recovery claim."""
import json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
fixture=R/'rebuild/tests/integration/NativeGraphicInfoFrameInitialiseComposition_test.py'
result=subprocess.run([sys.executable,str(fixture)],cwd=R,capture_output=True,text=True,timeout=120)
assert result.returncode==0,(result.returncode,result.stdout,result.stderr)
assert 'INITIALISE_COMPOSITION_PASS 2250 5730' in result.stdout,result.stdout
report=json.loads((R/'rebuild/compile-gate/native-graphic-info-frame-initialise-composition.json').read_text())
assert (report['cases'],report['snapshots'])==(2250,5730)
assert len(report['providers'])==4 and len(report['omission_checks'])==4
assert all(p['complete_decode'] and p['near_match'] for p in report['providers'])
assert [p['grade'] for p in report['providers']]==['MATCH','MATCH','MATCH','NEAR_MATCH']
assert all(p['missing_failed'] and p['provider_link_passed'] for p in report['omission_checks'])
assert report['input_sha256'] and report['original_exe_sha256']
print(result.stdout.strip())
print('NATIVE_GRAPHIC_INFO_FRAME_INITIALISE_COMPOSITION_GATE_PASS')
