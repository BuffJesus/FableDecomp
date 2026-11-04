"""Fresh genuine priority wrapper gate; bank-first limitation remains disclosed."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
r=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeThreadedFilePriority_test.py')],capture_output=True,text=True,timeout=1200)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_THREADED_FILE_PRIORITY_PASS 10 2 8448 16896 48' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-threaded-file-priority.json').read_text());before=json.loads((R/'rebuild/compile-gate/native-threaded-file-priority.before.json').read_text());after=json.loads((R/'rebuild/compile-gate/native-threaded-file-priority.after.json').read_text())
assert p['status']=='NATIVE_THREADED_FILE_PRIORITY_PASS' and p['root_symbol']=='?SetObjectPriority@CThreadedFile@@QAEXKW4EThreadedFilePriority@@@Z'
assert (p['bytes'],p['instructions'],p['cases'],p['checkpoints_per_mode'],p['caller_abi_cases'])==(10,2,8448,16896,48) and p['relocated_exact']
assert p['fixups']=={'1':['?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A',6,0],'6':['?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z',20,0]}
assert p['original_body_sha256']==p['compiled_body_sha256']=='06463d12f63389d1ac31e0b9db878e7a72d88e055acca97129e04e92ee4c5112'
expected=dict(cases=8448,checkpoints=16896,hits={'0x98f0e0':8448,'0x98eff0':8448},named_enum_bit_patterns=5376,raw_unnamed_or_out_of_range_machine_patterns=3072,typed_caller_operational_enum_domain=list(range(1,7)),count7_sentinel_machine_cases=768)
assert p['mode_execution']=={m:expected for m in ('original','compiled')}
assert p['executable_cache_policy']=='Separate immutable UCs with code installed before translation.'
assert p['provider_link_control_passed'] and p['link_executable_executed'] is False
assert p['supported_include_order']=='file/GF/gamebank/bank_async_entry fresh TU compiled'
assert 'Not whole-header-order PASS' in p['bank_first_baseline_failure_disclosure']
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
assert len(p['raw_coff'])==4
print('NATIVE_THREADED_FILE_PRIORITY_GATE_PASS')
