"""Fresh exact wrapper and original-manager boundary proof; link EXE never runs."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from pe_oracle import EXE
test=R/'rebuild/tests/integration/NativeThreadedFileRemoveObject_test.py'
r=subprocess.run([sys.executable,str(test)],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_THREADED_FILE_REMOVE_OBJECT_PASS 4224 25248 30' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-threaded-file-remove-object.json').read_text())
assert p['status']=='NATIVE_THREADED_FILE_REMOVE_OBJECT_PASS'
assert p['root_symbol']=='?RemoveObject@CThreadedFile@@QAE_NK@Z'
assert (p['bytes'],p['instructions'],p['cases'],p['snapshots_per_chain'],p['caller_abi_cases'])==(10,2,4224,25248,30)
assert p['relocated_exact'] and p['fixups']=={'1':['?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A',6,0],'6':['?RemoveObject@CThreadedFileManager@@QAE_NK@Z',20,0]}
for mode in ('original','compiled'):
 assert p['mode_provider_hits'][mode]=={'wrapper':{'0x98ec90':4224,'0x98eb20':4224},'manager':{'0x98ec90':0,'0x98eb20':4224}}
assert all(p[n] is False for n in ('link_executable_executed','manager_link_abort_executed','global_link_storage_executed'))
o=p['source_omission'];assert o['missing_failed'] and o['provider_link_passed']
assert any(o['symbol'] in line and ('LNK2001' in line or 'LNK2019' in line) for line in o['diagnostic'].splitlines())
assert p['input_sha256_before']==p['input_sha256_after'] and p['toolchain_sha256_before']==p['toolchain_sha256_after']
assert len(p['toolchain_sha256_after'])==5
assert p['object_sha256_before']==p['object_sha256_after']==p['object_sha256']
for name,digest in p['input_sha256_after'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in p['toolchain_sha256_after'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
for name,digest in p['object_sha256'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
assert p['original_exe_sha256_before']==p['original_exe_sha256_after']==hashlib.sha256(EXE.read_bytes()).hexdigest()
assert p['imports']['0x143fdf8'][1]=='GlobalFree'
print('NATIVE_THREADED_FILE_REMOVE_OBJECT_GATE_PASS')
