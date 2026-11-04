"""Successful fresh-allocation composition; eight executed, nine audited providers."""
import json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
result=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeGraphicInfoFrameInitialiseAllocation_test.py')],cwd=R,capture_output=True,text=True,timeout=180)
assert not result.returncode,(result.returncode,result.stdout,result.stderr)
assert 'INITIALISE_ALLOCATION_COMPOSITION_PASS 12288 24576' in result.stdout,result.stdout
report=json.loads((R/'rebuild/compile-gate/native-graphic-info-frame-initialise-allocation.json').read_text())
assert (report['cases'],report['snapshots'])==(12288,24576)
assert (report['installed_providers'],report['executed_providers'])==(9,8)
assert len(report['omission_checks'])==9 and all(p['missing_failed'] and p['provider_link_passed'] for p in report['omission_checks'])
assert [p['grade'] for p in report['providers']]==['MATCH','MATCH','MATCH','NEAR_MATCH','NEAR_MATCH','MATCH','MATCH','MATCH','MATCH']
assert all(p['near_match'] and p['complete_decode'] for p in report['providers'])
assert report['unexecuted_provider']=='?Uninitialise@CGraphicFrameData@@QAEXXZ'
assert report['input_sha256'] and report['original_exe_sha256']
assert (report['baseline']['cases'],report['baseline']['snapshots'])==(12288,24576)
assert (report['supplement']['cases'],report['supplement']['snapshots'])==(15360,43008)
assert (report['total_cases'],report['total_snapshots'])==(27648,67584)
assert 'INITIALISE_ALLOCATION_SUPPLEMENT_PASS 15360 43008' in result.stdout
assert report['provider_hits']['retail']==report['provider_hits']['compiled']
assert report['baseline_provider_hits']['retail']==report['baseline_provider_hits']['compiled']
assert len(report['provider_hits']['retail'])==9
assert sum(bool(n) for n in report['provider_hits']['retail'].values())==8
assert report['provider_hits']['retail']['0x9fcba0']==0
assert all(len(p['direct_target_relocations'])==1 for p in report['omission_checks'][4:])
print(result.stdout.strip())
print('NATIVE_GRAPHIC_INFO_FRAME_INITIALISE_ALLOCATION_GATE_PASS')
