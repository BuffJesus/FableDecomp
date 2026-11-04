"""Fresh read-begin native chain/model validation; no scratch dependencies."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
test=R/'rebuild/tests/integration/NativeAsyncReadBegin_test.py'
result=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600)
assert result.returncode==0,result.stdout+result.stderr
assert 'NATIVE_ASYNC_READ_BEGIN_PASS 10368 34560 34560' in result.stdout,result.stdout
path=R/'rebuild/compile-gate/native-async-read-begin.json';report=json.loads(path.read_text())
assert report['status']=='NATIVE_ASYNC_READ_BEGIN_PASS'
assert (report['original_cases'],report['original_arena_snapshots'],report['original_lock_snapshots'],report['compiled_cases'],report['compiled_arena_snapshots'],report['compiled_lock_snapshots'])==(10368,34560,34560,10368,34560,34560)
assert len(report['providers'])==3 and [p['bytes'] for p in report['providers']]==[92,54,25]
assert all(p['grade']=='RELOCATION_MATCH' and all(r['addend']==0 for r in p['fixups']) for p in report['providers'])
assert [len(p['fixups']) for p in report['providers']]==[4,2,1]
assert len(report['source_omissions'])==3 and {p['symbol'] for p in report['source_omissions']}=={p['symbol'] for p in report['providers']}
assert all(p['missing_failed'] and p['provider_link_passed'] for p in report['source_omissions'])
assert report['link_executable_executed'] is False and report['separate_immutable_uc_instances'] is True
expected_providers={'0x9d5440':10368,'0xa62920':10368,'0xa629a0':10368}
assert report['original_provider_hits']==report['compiled_provider_hits']==expected_providers
assert report['original_callback_hits']==report['compiled_callback_hits']==report['model_callback_counts']=={'try':10368,'setup':6912,'leave':6912}
assert report['source_sha256']==report['source_sha256_after'] and report['toolchain_sha256_before']==report['toolchain_sha256_after']
assert report['original_exe_sha256_before']==report['original_exe_sha256_after']
assert report['actual_iat_imports']=={'0x143fd30':{'dll':'KERNEL32.dll','name':'TryEnterCriticalSection'},'0x143fe0c':{'dll':'KERNEL32.dll','name':'EnterCriticalSection'},'0x143fe10':{'dll':'KERNEL32.dll','name':'LeaveCriticalSection'}}
for name,digest in report['source_sha256'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in report['toolchain_sha256_before'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
print('NATIVE_ASYNC_READ_BEGIN_GATE_PASS')
