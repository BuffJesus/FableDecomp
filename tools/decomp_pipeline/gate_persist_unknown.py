"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_persist_unknown
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/CPersistContext_CheckUnknownSymbols_009bbbf0.cpp','00/46/CStringParser_CopyConstructor_0046381b.cpp','00/9b/CStringParser_SkipWhitespace_009b9c60.cpp','00/9b/CStringParser_NextItemExists_009ba4f0.cpp','00/9b/CStringParser_ReadAsStringUntilWhitespace_009ba4a0.cpp','00/9b/CStringParser_SkipUntilWhitespace_009b9ce0.cpp','00/9b/CStringParser_CalcInQuotesTyped_009b9570.cpp']
def main():
    retail_cases=oracle_persist_unknown.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/CPersistContext_CheckUnknownSymbols_009bbbf0_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?CheckUnknownSymbols@CPersistContext'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_UNKNOWN_OK cases=5' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/persist-unknown.json').write_text(json.dumps(dict(address='009bbbf0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=207,linked_cases=5,retail_oracle_cases=retail_cases,native_sources=len(sources),mocked_boundaries='string storage, contextual error reporting'),indent=2)+'\n');print('PERSIST_UNKNOWN_PASS cases=5 sources='+str(len(sources))+' bytes='+str(len(b)))
if __name__=='__main__':main()
