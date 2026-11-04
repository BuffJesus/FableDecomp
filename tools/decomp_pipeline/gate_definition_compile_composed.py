"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_definition_compile
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_CompileSubDefinition_009ae150.cpp','00/9a/CDefinitionManager_CompileDefinition_009ae7c0.cpp','00/9b/CStringParser_ReadNextItem_009ba080.cpp','00/9b/CStringParser_ReadNextNumber_009b9ee0.cpp','00/9b/CStringParser_ReadNextString_009b9fc0.cpp','00/9b/CStringParser_ReadNextSymbol_009b99c0.cpp','00/9b/CStringParser_ReadAsString_009ba6f0.cpp','00/9b/CStringParser_SkipWhitespace_009b9c60.cpp','00/9b/CStringParser_CalcInQuotesTyped_009b9570.cpp','00/9b/CStringParser_PeekNextItem_009ba8c0.cpp','00/9b/CParsedItem_Accessors_009b9870.cpp','00/9b/CParsedItem_InitAsInteger_009b9700.cpp','00/9b/CParsedItem_InitAsFloat_009b9770.cpp','00/9b/CStringParser_SkipUntilString_009b9a20.cpp','00/9b/CStringParser_SkipPastString_009b9ba0.cpp','00/9b/CStringParser_ReadAsStringUntilString_009ba330.cpp','00/9b/CStringParser_SetStringPos_009b9940.cpp','00/9b/CStringParser_SkipPastNextItemIfIs_009b9e00.cpp','00/40/CStringParser_Constructor_00404720.cpp','00/9b/CStringParser_InitTyped_009b9530.cpp','00/9b/CPersistContext_TextConstructor_009badd0.cpp']
def main():
    retail_cases=oracle_definition_compile.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_CompileDefinitionComposed_009ae7c0_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[1])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?CompileDefinition'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_COMPILE_COMPOSED_OK cases=7' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-compile-composed.json').write_text(json.dumps(dict(address='009ae7c0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=1456,linked_cases=7,retail_oracle_cases=retail_cases,native_sources=len(sources),mocked_boundaries='string storage and SetChar, symbol lookup, definition Load/Copy/Transfer, InstantiateSubDef, parent subdef lookup and unknown-symbol checks'),indent=2)+'\n');print('DEFINITION_COMPILE_COMPOSED_PASS cases=7 sources='+str(len(sources))+' bytes='+str(len(b)))
if __name__=='__main__':main()
