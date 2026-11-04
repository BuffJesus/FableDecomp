"""Native identifier and number-scanner boundary contracts."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    w=Path(tempfile.mkdtemp(prefix='token-readers-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled/00/9b'/name for name in ['CStringParser_ReadNextNumber_009b9ee0.cpp','CStringParser_ReadNextString_009b9fc0.cpp','CStringParser_CalcInQuotesTyped_009b9570.cpp']];test=ROOT/'rebuild/tests/00/9b/CStringParser_TokenReaders_009b9ee0_test.cpp';reports=[]
    for source in sources[:2]:
        obj=w/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],e,w);assert r.returncode==0,r.stdout+r.stderr
        c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?ReadNext'));sec,off=c.symbols[k];b,rels=c.sections[sec];reports.append(dict(symbol=k,bytes=len(b),grade='BEHAVIORAL'))
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s) for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_TOKEN_READERS_OK cases=12' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-token-readers.json').write_text(json.dumps(dict(providers=reports,linked_cases=12,mocked_boundaries='string storage and parsed numeric conversion'),indent=2)+'\n')
    print('PARSER_TOKEN_READERS_PASS cases=12')
if __name__=='__main__':main()
