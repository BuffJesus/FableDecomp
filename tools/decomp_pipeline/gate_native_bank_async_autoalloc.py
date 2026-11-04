"""Fresh native async auto-allocation dispatch; children remain explicit boundaries."""
import hashlib,json,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
test=ROOT/'rebuild/tests/integration/NativeBankAsyncAutoAlloc_test.py'
r=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_BANK_ASYNC_AUTOALLOC_PASS 110772 544254' in r.stdout,r.stdout
path=ROOT/'rebuild/compile-gate/native-bank-async-autoalloc.json'
report=json.loads(path.read_text())
assert report['status']=='COMPILED_ROOT_ACTUAL_SETUP_DELETER_MODEL_PASS'
assert (report['cases'],report['snapshots'])==(110772,544254)
assert report['compiled_grade']['near_match'] and report['compiled_grade']['opcode_similarity']==0.967741935483871
assert len(report['relocation_audit'])==8 and all(q['addend']==0 for q in report['relocation_audit'])
assert report['source_omission']['missing_failed'] and report['source_omission']['provider_link_passed']
assert report['prerequisite_omission']['missing_failed'] and report['prerequisite_omission']['provider_link_passed']
for rel,sha in report['input_sha256'].items():assert hashlib.sha256((ROOT/rel).read_bytes()).hexdigest()==sha,rel
for name,sha in report['toolchain_sha256'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==sha,name
report['gate_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
path.write_text(json.dumps(report,indent=2)+'\n')
print('NATIVE_BANK_ASYNC_AUTOALLOC_GATE_PASS',report['cases'],report['snapshots'])
