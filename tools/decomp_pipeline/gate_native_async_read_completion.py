"""Fresh completion native-chain validation; no scratch evidence dependencies."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
test=R/'rebuild/tests/integration/NativeAsyncReadCompletion_test.py'
result=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600)
assert result.returncode==0,result.stdout+result.stderr
assert 'NATIVE_ASYNC_READ_COMPLETION_PASS 1600 4800 4800' in result.stdout,result.stdout
path=R/'rebuild/compile-gate/native-async-read-completion.json';report=json.loads(path.read_text())
assert report['status']=='NATIVE_ASYNC_READ_COMPLETION_PASS'
assert report['translation_cache_policy']=='Explicit ctl_remove_cache after every executable body write'
assert set(report['execution_counts'])=={'original','compiled'}
assert all(set(counts)=={'0x9d54a0','0xa62920','0xa629a0'} and all(n==1600 for n in counts.values()) for counts in report['execution_counts'].values())
assert (report['original_cases'],report['original_snapshots'],report['original_lock_snapshots'],report['compiled_cases'],report['compiled_snapshots'],report['compiled_lock_snapshots'])==(1600,4800,4800,1600,4800,4800)
assert len(report['providers'])==3 and all(p['grade']=='RELOCATION_MATCH' and all(r['addend']==0 for r in p['fixups']) for p in report['providers'])
assert [p['bytes'] for p in report['providers']]==[63,54,25]
assert len(report['source_omissions'])==3 and {p['symbol'] for p in report['source_omissions']}=={p['symbol'] for p in report['providers']}
assert all(p['missing_failed'] and p['provider_link_passed'] for p in report['source_omissions']) and report['link_executable_executed'] is False
assert report['source_sha256']==report['source_sha256_after'] and report['toolchain_sha256_before']==report['toolchain_sha256_after']
for name,digest in report['source_sha256'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in report['toolchain_sha256_before'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
print('NATIVE_ASYNC_READ_COMPLETION_GATE_PASS')
