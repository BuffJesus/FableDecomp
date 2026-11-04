"""Fresh exact root and real RB child composition gate."""
import hashlib,json,subprocess,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/decomp_pipeline'))
from pe_oracle import EXE
R=Path(__file__).resolve().parents[2]
r=subprocess.run([sys.executable,str(R/'rebuild/tests/integration/NativeAsyncPointerTreeRangeErase_test.py')],capture_output=True,text=True,timeout=600)
assert r.returncode==0,r.stdout+r.stderr
assert 'NATIVE_ASYNC_POINTER_TREE_RANGE_ERASE_PASS 4448 16648 80 704' in r.stdout
p=json.loads((R/'rebuild/compile-gate/native-async-pointer-tree-range-erase.json').read_text())
assert p['status']=='NATIVE_ASYNC_POINTER_TREE_RANGE_ERASE_PASS' and p['link_executable_executed'] is False
assert len(p['providers'])==7 and [x['bytes'] for x in p['providers']]==[189,51,572,26,26,64,64]
assert all(x['grade']=='RELOCATION_MATCH' and all(y['addend']==0 for y in x['fixups']) for x in p['providers'])
assert len(p['source_omissions'])==4 and p['link_control_passed']
assert p['translation_cache_policy']=='Separate immutable Unicorn machines'
assert p['caller_abi']['argument_words']==2 and p['caller_abi']['byvalue_iterators'] is True and p['caller_abi']['callee_cleanup']==8 and p['caller_abi']['caller_stack_cleanup_after_call']==0
assert p['phases']['baseline']['model']['cases']==4448 and p['phases']['baseline']['model']['snapshots']==16648
assert p['phases']['supplement']['model']['supplement_cases']==80 and p['phases']['supplement']['model']['snapshots']==704
for phase in p['phases'].values():assert phase['mode_provider_hits']['original']==phase['mode_provider_hits']['compiled']
assert p['input_sha256_before']==p['input_sha256_after'] and p['toolchain_sha256_before']==p['toolchain_sha256_after']
for name,digest in p['input_sha256_after'].items():assert hashlib.sha256((R/name).read_bytes()).hexdigest()==digest,name
for name,digest in p['toolchain_sha256_after'].items():assert hashlib.sha256(Path(name).read_bytes()).hexdigest()==digest,name
assert hashlib.sha256(EXE.read_bytes()).hexdigest()==p['original_exe_sha256']
print('NATIVE_ASYNC_POINTER_TREE_RANGE_ERASE_GATE_PASS')
