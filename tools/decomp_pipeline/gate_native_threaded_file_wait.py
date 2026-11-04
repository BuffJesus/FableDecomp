"""Fresh genuine file Wait gate; no historical first-probe acceptance."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
r=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeThreadedFileWait_test.py')],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_THREADED_FILE_WAIT_PASS 10 2 144 288 48' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-threaded-file-wait.json').read_text());before=json.loads((R/'rebuild/compile-gate/native-threaded-file-wait.before.json').read_text());after=json.loads((R/'rebuild/compile-gate/native-threaded-file-wait.after.json').read_text())
assert p['status']=='NATIVE_THREADED_FILE_WAIT_PASS' and p['root_symbol']=='?WaitForObjectToFinish@CThreadedFile@@QAEXK@Z'
assert (p['bytes'],p['instructions'],p['cases'],p['checkpoints_per_mode'],p['caller_abi_cases'])==(10,2,144,288,48) and p['relocated_exact']
assert p['fixups']=={'1':['?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A',6,0],'6':['?WaitForObjectToFinish@CThreadedFileManager@@QAEXK@Z',20,0]}
assert p['original_body_sha256']==p['compiled_body_sha256']=='38584797badbf57e7bc984b3257179de8b218de18657983c83737666ab8479ba'
assert p['mode_execution']=={m:dict(cases=144,checkpoints=288,root_hits=144,child_hits=144) for m in ('original','compiled')}
assert p['executable_cache_policy']=='Separate immutable UCs with code installed before translation.'
assert p['provider_link_control_passed'] and p['link_executable_executed'] is False
assert any(p['root_symbol'] in line and ('LNK2001' in line or 'LNK2019' in line) for line in p['source_omission_diagnostic'].splitlines())
assert before['kind']=='FRESH_CANONICAL_BEFORE_COMPILE' and before['input_sha256']==p['input_sha256_before'] and before['toolchain_sha256']==p['toolchain_sha256_before']
assert after['status']=='TERMINAL' and after['error'] is None
for name in ('input','toolchain','object','linked'):assert not after[name+'_drift']
assert p['input_sha256_before']==p['input_sha256_after']==after['input_after']
assert p['toolchain_sha256_before']==p['toolchain_sha256_after']==after['toolchain_after'] and len(p['toolchain_sha256_after'])==5
assert p['object_sha256_before']==p['object_sha256_after']==after['object_after'] and p['linked_before']==p['linked_after']==after['linked_after']
for table in ('input_sha256_after','toolchain_sha256_after','object_sha256_after','linked_after'):
 for name,h in p[table].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==h,name
for name,raw in p['raw_coff'].items():assert Path(name).read_bytes().hex()==raw
print('NATIVE_THREADED_FILE_WAIT_GATE_PASS')
