"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_definition_enum
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9a/CDefinitionManager_ParseEnumForSymbols_009acee0.cpp','00/9b/CStringParser_ReadNextItem_009ba080.cpp','00/9b/CStringParser_ReadNextNumber_009b9ee0.cpp','00/9b/CStringParser_ReadNextString_009b9fc0.cpp','00/9b/CStringParser_ReadNextSymbol_009b99c0.cpp','00/9b/CStringParser_ReadAsString_009ba6f0.cpp','00/9b/CStringParser_ReadAsSymbol_009ba790.cpp','00/9b/CStringParser_SkipWhitespace_009b9c60.cpp','00/9b/CStringParser_CalcInQuotesTyped_009b9570.cpp','00/9b/CStringParser_PeekNextItem_009ba8c0.cpp','00/9b/CStringParser_PeekNextItemType_009ba900.cpp','00/9b/CParsedItem_Accessors_009b9870.cpp','00/9b/CParsedItem_InitAsInteger_009b9700.cpp','00/9b/CParsedItem_InitAsFloat_009b9770.cpp','00/9b/CPersistContext_TextConstructor_009badd0.cpp']
def main():
    retail_cases=oracle_definition_enum.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9a/CDefinitionManager_ParseEnumForSymbols_009acee0_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?ParseEnumForSymbols'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFINITION_ENUM_PARSER_OK cases=8' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/definition-enum-parser.json').write_text(json.dumps(dict(address='009acee0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=1017,linked_cases=8,retail_oracle_cases=retail_cases,native_sources=len(sources),mocked_boundaries='string storage, symbol map publication, error reporting, expression TransferIn'),indent=2)+'\n');print('DEFINITION_ENUM_PARSER_PASS cases=8 sources='+str(len(sources))+' bytes='+str(len(b)))
if __name__=='__main__':main()
