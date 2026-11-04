"""CompileString directive ordering and exact class-filter contracts."""
import json,subprocess,tempfile
from pathlib import Path
import check_one,gate_definition_enum_parser,oracle_definition_compile_string
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    retail_cases=oracle_definition_compile_string.collect()
    w=Path(tempfile.mkdtemp(prefix='compile-string-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'c.obj';source=ROOT/'rebuild/src/compiled/00/9a/CDefinitionManager_CompileString_009af1e0.cpp';test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_CompileString_009af1e0_test.cpp'
    names=[n for n in gate_definition_enum_parser.NAMES if 'ParseEnumForSymbols' not in n and 'CPersistContext_Text' not in n];names+=['00/9b/CStringParser_InitTyped_009b9530.cpp','00/9b/CStringParser_SkipUntilWholeString_009b9aa0.cpp','00/9b/CStringParser_SkipPastWholeString_009b9c00.cpp','00/9b/CStringParser_SetStringPos_009b9940.cpp'];sources=[source]+[ROOT/'rebuild/src/compiled'/n for n in names]
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(source)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(o);k=next(k for k in c.symbols if k.startswith('?CompileString@'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_COMPILE_STRING_OK cases=6' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-compile-string.json').write_text(json.dumps(dict(address='009af1e0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=670,linked_cases=6,retail_oracle_cases=retail_cases,native_sources=len(sources),mocked_boundaries='string storage and CompileDefinition'),indent=2)+'\n');print('DEFINITION_COMPILE_STRING_PASS cases=6 sources='+str(len(sources)))
if __name__=='__main__':main()
