"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_persist_slong
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9e/CPersistTraitsSLONG_TransferIn_009e2590.cpp','00/9b/CStringParser_ReadNextItem_009ba080.cpp','00/9b/CStringParser_ReadNextNumber_009b9ee0.cpp','00/9b/CStringParser_ReadNextString_009b9fc0.cpp','00/9b/CStringParser_ReadNextSymbol_009b99c0.cpp','00/9b/CStringParser_ReadAsString_009ba6f0.cpp','00/9b/CStringParser_ReadAsInteger_009ba540.cpp','00/9b/CStringParser_SkipWhitespace_009b9c60.cpp','00/9b/CStringParser_CalcInQuotesTyped_009b9570.cpp','00/9b/CStringParser_PeekNextItemType_009ba900.cpp','00/9b/CParsedItem_InitAsInteger_009b9700.cpp','00/9b/CParsedItem_InitAsFloat_009b9770.cpp','00/9b/CStringParser_SkipPastNextItemIfIs_009b9e00.cpp']
def main():
    retail_cases=oracle_persist_slong.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9e/CPersistTraitsSLONG_TransferIn_009e2590_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?TransferIn@CPersistTraitsSLONG'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_SLONG_OK cases=11' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/persist-slong.json').write_text(json.dumps(dict(address='009e2590',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=511,linked_cases=11,retail_oracle_cases=retail_cases,native_sources=len(sources),mocked_boundaries='string storage, symbol lookup/sort, error reporting'),indent=2)+'\n');print('PERSIST_SLONG_PASS cases=11 sources='+str(len(sources))+' bytes='+str(len(b)))
if __name__=='__main__':main()
