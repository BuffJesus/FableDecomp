"""Definition allocation, indexing, ownership and publication contracts."""
import json,subprocess,tempfile
from pathlib import Path
import check_one,oracle_definition_subinstantiate
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    retail_cases=oracle_definition_subinstantiate.collect()
    w=Path(tempfile.mkdtemp(prefix='definition-instantiate-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'i.obj';s=ROOT/'rebuild/src/compiled/00/9a/CDefinitionManager_InstantiateSubDef_009adae0.cpp';t=ROOT/'rebuild/tests/00/9a/CDefinitionManager_InstantiateSubDef_009adae0_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(o);k=next(k for k in c.symbols if k.startswith('?InstantiateSubDef@'));sec,off=c.symbols[k];b,rels=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_INSTANTIATE_SUB_OK cases=4' in r.stdout,(r.returncode,r.stdout)
    (ROOT/'rebuild/compile-gate/definition-subinstantiate.json').write_text(json.dumps(dict(address='009adae0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=417,linked_cases=4,retail_oracle_cases=retail_cases,mocked_boundaries='class lookup, allocator/vtable, string interning and parent LoadDef/AddSubDef'),indent=2)+'\n');print('DEFINITION_INSTANTIATE_SUB_PASS cases=4 bytes='+str(len(b)))
if __name__=='__main__':main()
