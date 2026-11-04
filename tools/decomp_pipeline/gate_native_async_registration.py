"""Proposed fresh-source native registration dispatch."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];test=R/'rebuild/tests/integration/NativeAsyncRegistration_test.py'
r=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600);assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_ASYNC_REGISTRATION_PASS 480 1136 20' in r.stdout,r.stdout
p=R/'rebuild/compile-gate/native-async-registration.json';report=json.loads(p.read_text());assert report['status']=='NATIVE_ASYNC_REGISTRATION_PASS' and report['grade']=='NEAR_MATCH'
assert report['compiled_grade']['near_match'] and report['compiled_grade']['opcode_similarity']==1 and len(report['relocation_audit'])==3 and all(x['addend']==0 for x in report['relocation_audit'])
assert report['source_omission']['missing_failed'] and report['source_omission']['provider_link_passed']
for name,digest in report['input_sha256'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in report['toolchain_sha256'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
print('NATIVE_ASYNC_REGISTRATION_GATE_PASS',480,1136,20)
