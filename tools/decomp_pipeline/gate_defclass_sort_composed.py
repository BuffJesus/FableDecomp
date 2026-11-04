"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one




from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/STL_Sort_DefClassEntry_009b9490.cpp', '00/9b/STL_IntrosortLoop_DefClassEntry_009b9320.cpp', '00/9b/STL_InsertionSort_DefClassEntry_009b8f70.cpp', '00/9b/STLSort_DefClassInfo_linear_insert_009b8c00.cpp', '00/9b/STLSort_DefClassInfo_unguarded_linear_insert_009b76f0.cpp', '00/9b/STL_UnguardedInsertionSort_DefClassEntry_009b80c0.cpp', '00/9b/STL_UnguardedPartition_DefClassEntry_009b8630.cpp', '00/9b/STL_PartialSort_DefClassEntry_009b9030.cpp']
def main():
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/STL_DefClassSortComposed_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('??$sort@'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFCLASS_SORT_COMPOSED_OK cases=27' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/defclass-sort-composed.json').write_text(json.dumps(dict(linked_cases=27,native_sources=8,paths='quicksort, insertion and forced zero-depth heap fallback',mocked_boundaries='string storage'),indent=2)+'\n')
    print('DEFCLASS_SORT_COMPOSED_PASS cases=27 sources=8')
if __name__=='__main__':main()
