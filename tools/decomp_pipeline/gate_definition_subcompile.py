"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_definition_subcompile
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_CompileSubDefinition_009ae150.cpp','00/40/CStringParser_Constructor_00404720.cpp','00/9b/CStringParser_InitTyped_009b9530.cpp','00/9b/CPersistContext_TextConstructor_009badd0.cpp']
def main():
    retail_cases=oracle_definition_subcompile.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_CompileSubDefinition_009ae150_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?CompileSubDefinition'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_SUBCOMPILE_OK cases=4' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-subcompile.json').write_text(json.dumps(dict(address='009ae150',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=568,linked_cases=4,retail_oracle_cases=retail_cases,native_sources=len(sources),mocked_boundaries='string storage, definition Load/InstantiateSubDef/Copy/Transfer, parent subdef lookup/removal and unknown-symbol checks'),indent=2)+'\n');print('DEFINITION_SUBCOMPILE_PASS cases=4 sources='+str(len(sources))+' bytes='+str(len(b)))
if __name__=='__main__':main()
