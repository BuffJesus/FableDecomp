"""Definition allocation, indexing, ownership and publication contracts."""
import json,subprocess,tempfile
from pathlib import Path
import check_one,oracle_definition_instantiate
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    retail_cases=oracle_definition_instantiate.collect()
    w=Path(tempfile.mkdtemp(prefix='definition-instantiate-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'i.obj';s=ROOT/'rebuild/src/compiled/00/9a/CDefinitionManager_InstantiateDef_009adc90.cpp';t=ROOT/'rebuild/tests/00/9a/CDefinitionManager_InstantiateDef_009adc90_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(o);k=next(k for k in c.symbols if k.startswith('?InstantiateDef@'));sec,off=c.symbols[k];b,rels=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_INSTANTIATE_OK cases=5' in r.stdout,(r.returncode,r.stdout)
    (ROOT/'rebuild/compile-gate/definition-instantiate.json').write_text(json.dumps(dict(address='009adc90',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=382,linked_cases=5,retail_oracle_cases=retail_cases,mocked_boundaries='class lookup, allocator/vtable, string interning, symbol-map publication'),indent=2)+'\n');print('DEFINITION_INSTANTIATE_PASS cases=5 bytes='+str(len(b)))
if __name__=='__main__':main()
