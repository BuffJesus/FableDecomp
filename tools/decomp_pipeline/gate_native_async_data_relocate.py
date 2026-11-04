"""Fresh native secondary override validation."""
import hashlib,json,subprocess,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/decomp_pipeline'))
R=Path(__file__).resolve().parents[2]
test=R/'rebuild/tests/integration/NativeAsyncDataRelocate_test.py'
r=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_ASYNC_DATA_RELOCATE_PASS 25200 126000 75600' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-async-data-relocate.json').read_text())
assert p['status']=='NATIVE_ASYNC_DATA_RELOCATE_PASS' and p['link_executable_executed'] is False
assert (p['cases'],p['arena_snapshots'],p['lock_snapshots'])==(25200,126000,75600)
assert len(p['providers'])==3 and len(p['source_omissions'])==3
assert all(row['missing_failed'] and row['provider_link_passed'] for row in p['source_omissions'])
assert all(set(row)=={'0x9d5670','0xa62920','0xa629a0'} and all(n==25200 for n in row.values()) for row in p['mode_provider_hits'].values())
assert p['input_sha256_before']==p['input_sha256_after'] and p['toolchain_sha256_before']==p['toolchain_sha256_after']
for name,digest in p['input_sha256_after'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in p['toolchain_sha256_after'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
assert p['original_exe_sha256']==p['original_exe_sha256_after']
from pe_oracle import EXE
assert hashlib.sha256(EXE.read_bytes()).hexdigest()==p['original_exe_sha256_after']
assert all(set(row)=={'enter','buffer_phase','data_phase','leave','final'} and all(n==25200 for n in row.values()) for row in p['mode_phase_counts'].values())
print('NATIVE_ASYNC_DATA_RELOCATE_GATE_PASS')
