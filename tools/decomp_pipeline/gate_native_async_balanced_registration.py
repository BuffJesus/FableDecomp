"""Fresh canonical balanced native registration validation only; no new credit."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];test=R/'rebuild/tests/integration/NativeAsyncBalancedRegistration_test.py'
r=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600);assert r.returncode==0,r.stdout+r.stderr
assert 'FRESH_BALANCED_REGISTRATION_COMPOSITION_PASS 4128 10752' in r.stdout,r.stdout
p=R/'rebuild/compile-gate/native-async-balanced-registration.json';report=json.loads(p.read_text());assert report['status']=='FRESH_BALANCED_REGISTRATION_COMPOSITION_PASS'
assert (report['unique_operations'],report['snapshots_per_root_model'])==(4128,10752)
assert len(report['source_omissions'])==3 and all(x['missing_failed'] for x in report['source_omissions']) and report['link_control_passed']
assert len(report['mandatory_translation_units'])==3
assert report['translation_cache_policy']=='Explicit ctl_remove_cache after every executable body write'
assert report['control_returncode']==0 and report['link_executable_executed'] is False
assert report['original_malloc_import']['dll'].lower()=='msvcr71.dll' and report['original_malloc_import']['symbol']=='malloc'
for kind in ('original','compiled'):
 assert report['coverage'][kind]['decrement']==784
 assert all(report['coverage'][kind][name]>0 for name in ('left','right','left_uncle_red','right_uncle_red','root_replacements','duplicates'))
 assert all(x>0 for x in report['nullmalloc_faults'][kind].values())
assert report['input_sha256_before']==report['input_sha256_after']
assert report['toolchain_sha256_before']==report['toolchain_sha256_after'] and len(report['toolchain_sha256_after'])==5
for name,digest in report['input_sha256_after'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in report['toolchain_sha256_after'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
print('NATIVE_ASYNC_BALANCED_REGISTRATION_GATE_PASS',4128,10752)
