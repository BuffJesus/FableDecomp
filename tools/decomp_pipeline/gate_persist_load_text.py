"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_persist_load_text

from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9b/CPersistContext_TransferObjectLoadText_009bba40.cpp','00/46/CStringParser_CopyConstructor_0046381b.cpp','00/9b/CStringParser_SkipUntilWholeString_009b9aa0.cpp','00/9b/CStringParser_SkipPastWholeString_009b9c00.cpp','00/9b/CStringParser_SetStringPos_009b9940.cpp','00/9b/CStringParser_ReadAsSymbol_009ba790.cpp']
def main():
    retail=oracle_persist_load_text.collect()
    from gate_persist_slong import NAMES as PARSERS
    NAMES.extend(n for n in PARSERS[1:] if n not in NAMES and "PeekNextItemType" not in n and "SkipPastNextItemIfIs" not in n and "ReadAsInteger" not in n)
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/CPersistContext_TransferObjectLoadText_009bba40_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?TransferObjectLoadText@CPersistContext'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_TEXT_LOAD_OK cases=6' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/persist-load-text.json').write_text(json.dumps(dict(address='009bba40',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=417,linked_cases=6,retail_cases=retail,native_sources=len(sources),mocked_boundaries='string storage, field transfer and error callbacks'),indent=2)+'\n')
    print('PERSIST_TEXT_LOAD_PASS cases=6 bytes='+str(len(b)))
if __name__=='__main__':main()

