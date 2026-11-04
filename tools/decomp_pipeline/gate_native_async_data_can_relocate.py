"""Fresh native secondary override validation."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
test=R/'rebuild/tests/integration/NativeAsyncDataCanRelocate_test.py'
r=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_ASYNC_DATA_CAN_RELOCATE_PASS 6000 18000 18000' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-async-data-can-relocate.json').read_text())
assert p['status']=='NATIVE_ASYNC_DATA_CAN_RELOCATE_PASS' and p['link_executable_executed'] is False
assert (p['cases'],p['snapshots_per_root'],p['lock_snapshots_per_root'])==(6000,18000,18000)
assert len(p['providers'])==3 and len(p['source_omissions'])==3 and p['link_control_passed']
assert all(set(row)=={'0x9d55d0','0xa62920','0xa629a0'} and all(n==6000 for n in row.values()) for row in p['mode_provider_hits'].values())
assert p['input_sha256_before']==p['input_sha256_after'] and p['toolchain_sha256_before']==p['toolchain_sha256_after']
for name,digest in p['input_sha256_after'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in p['toolchain_sha256_after'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
print('NATIVE_ASYNC_DATA_CAN_RELOCATE_GATE_PASS')
