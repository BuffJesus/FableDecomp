"""Compose filename introsort, insertion and heap fallback over owned strings."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_widestring_heap import UNITS
ROOT=Path(__file__).resolve().parents[2]
NAMES=[unit[0]for unit in UNITS]+['STL_Sort_WideString_009b8440.cpp','STL_Introsort_WideString_009b7cf0.cpp','STL_InsertionSort_WideString_009b68d0.cpp','STL_UnguardedInsertionSort_WideString_009b2ac0.cpp','STL_UnguardedPartition_WideString_009b3580.cpp','STL_PartialSort_WideString_009b6a40.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='wide-string-sort-composed-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled/00/9b'/name for name in NAMES]
    test=ROOT/'rebuild/tests/integration/WideStringSort_composed_test.cpp';exe=work/'test.exe'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe)]+[str(s)for s in sources]+[str(test)],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'WIDESTRING_SORT_COMPOSED_OK cases=32' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/widestring-sort-composed.json').write_text(json.dumps(dict(linked_cases=32,native_providers=NAMES,mocked_boundaries='wide-string storage, reference-counted copy/assignment/destruction and lexical comparison; all sort and heap providers are native',checks='sorted names, partial prefix, object identity permutation, reference counts and complete storage release'),indent=2)+'\n')
    print('WIDESTRING_SORT_COMPOSED_PASS cases=32 providers=10')

if __name__=='__main__':main()
