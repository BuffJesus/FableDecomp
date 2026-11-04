"""Compose native Unlock with STLport string-map insertion and erase."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9d/CDefStringTable_Unlock_009d4f10.cpp','00/51/STLMapCCharStringlong_operator_00519230.cpp','00/47/STLStringLongTree_Mlowerbound_0047547d.cpp','00/51/STLMapCCharStringlong_insert_unique_hint_00516af0.cpp','00/43/STL_insertunique_00431606.cpp','00/43/STL_Minsert_004316cf.cpp','00/43/global_Alloc_00431773.cpp','00/42/STL_Rb_global_Rebalance_0042971d.cpp','00/43/STL_DefStringMapErase_0043020a.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='defstring-unlock-composed-'))
    env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled'/name for name in NAMES]
    test=ROOT/'rebuild/tests/integration/DefStringUnlock_composed_test.cpp';exe=work/'test.exe'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe)]+[str(s)for s in sources]+[str(test)],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'DEFSTRING_UNLOCK_COMPOSED_OK cases=14' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/defstring-unlock-composed.json').write_text(json.dumps(dict(linked_cases=14,native_providers=NAMES,mocked_boundaries='memory-stream storage and string lifetime; tree insertion, balancing, lookups and erasure are native'),indent=2)+'\n')
    print('DEFSTRING_UNLOCK_COMPOSED_PASS cases=14 providers='+str(len(NAMES)))

if __name__=='__main__':main()
