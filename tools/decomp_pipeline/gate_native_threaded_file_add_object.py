"""Fresh genuine AddObject wrapper validation; manager is explicit boundary."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
r=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeThreadedFileAddObject_test.py')],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_THREADED_FILE_ADD_OBJECT_PASS 19 6 144 432 80' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-threaded-file-add-object.json').read_text())
assert p['status']=='NATIVE_THREADED_FILE_ADD_OBJECT_PASS' and p['root_symbol']=='?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z'
assert (p['bytes'],p['instructions'],p['cases'],p['snapshots_per_mode'],p['caller_abi_cases'])==(19,6,144,432,80) and p['relocated_exact']
assert p['fixups']=={'7':['?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A',6,0],'12':['?AddObject@CThreadedFileManager@@QAEKABVCThreadedFileObjectDesc@@AAVCThreadedFile@@@Z',20,0]}
assert p['mode_hits']=={'original':{'0x98f350':144,'0x98f0f0':144},'compiled':{'0x98f350':144,'0x98f0f0':144}}
assert p['executable_cache_policy']=='Separate immutable UCs with code installed before translation.'
assert p['original_body_sha256']==p['compiled_body_sha256']=='ee70d1b43e7f4617175c3cdb3072161e9bc0842a8290f5886bf65d12f10cba62'
assert p['provider_link_control_passed'] and p['link_executable_executed'] is False
assert any(p['root_symbol'] in line and ('LNK2001' in line or 'LNK2019' in line) for line in p['source_omission_diagnostic'].splitlines())
assert p['input_sha256_before']==p['input_sha256_after'] and p['toolchain_sha256_before']==p['toolchain_sha256_after'] and len(p['toolchain_sha256_after'])==5
assert p['object_sha256_before']==p['object_sha256_after'] and p['linked_before']==p['linked_after']
for table in ('input_sha256_after','toolchain_sha256_after','object_sha256_after','linked_after'):
 for name,h in p[table].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==h,name
for name,raw in p['raw_coff'].items():assert Path(name).read_bytes().hex()==raw
print('NATIVE_THREADED_FILE_ADD_OBJECT_GATE_PASS')
