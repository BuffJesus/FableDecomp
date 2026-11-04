"""Fresh genuine wrapper-provider correction, not child/layout recovery."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE
r=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeThreadedFileFinishHandle_test.py')],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_THREADED_FILE_FINISH_HANDLE_PASS 200 560 160 25' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-threaded-file-finish-handle.json').read_text())
assert p['status']=='NATIVE_THREADED_FILE_FINISH_HANDLE_PASS' and p['root_symbol']=='?FinishAndGetFileHandle@CThreadedFile@@QAEPAXXZ'
assert (p['bytes'],p['instructions'],p['caller_abi_cases'])==(25,10,25) and p['relocated_exact']
assert p['fixups']=={'11':['?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A',6,0],'16':['?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z',20,0]}
m=p['matrix'];assert (m['cases'],m['snapshots_per_mode'],m['typed_child_callbacks_per_mode'])==(200,560,160)
assert m['mode_hits']=={'original':{'0x98ec70':200,'0x98e7d0':160},'compiled':{'0x98ec70':200,'0x98e7d0':160}}
assert all(p[n] is False for n in ('link_executable_executed','manager_link_abort_executed','global_link_storage_executed'))
o=p['source_omission'];assert o['missing_failed'] and o['provider_link_passed']
assert any(o['symbol'] in line and ('LNK2001' in line or 'LNK2019' in line) for line in o['diagnostic'].splitlines())
assert p['input_sha256_before']==p['input_sha256_after'] and p['toolchain_sha256_before']==p['toolchain_sha256_after'] and len(p['toolchain_sha256_after'])==5
assert p['object_sha256_before']==p['object_sha256_after']
for n,h in p['input_sha256_after'].items():assert hashlib.sha256((R/n).read_bytes()).hexdigest()==h,n
for n,h in p['toolchain_sha256_after'].items():assert hashlib.sha256(Path(n).read_bytes()).hexdigest()==h,n
for n,h in p['object_sha256_after'].items():assert hashlib.sha256(Path(n).read_bytes()).hexdigest()==h,n
assert p['original_exe_sha256_before']==p['original_exe_sha256_after']==hashlib.sha256(EXE.read_bytes()).hexdigest()
assert p['original_body_sha256']==p['compiled_body_sha256']
print('NATIVE_THREADED_FILE_FINISH_HANDLE_GATE_PASS')
