"""Typed quote helper parity and symbol extraction behavior."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
    w=Path(tempfile.mkdtemp(prefix='parser-symbol-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled/00/9b'/name for name in ['CStringParser_ReadNextSymbol_009b99c0.cpp','CStringParser_CalcInQuotesTyped_009b9570.cpp']];test=ROOT/'rebuild/tests/00/9b/CStringParser_ReadNextSymbol_009b99c0_test.cpp'
    reports=[]
    for source in sources:
        obj=w/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],e,w);assert r.returncode==0,r.stdout+r.stderr
        c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?ReadNextSymbol@') or k.startswith('?CalcInQuotes@'));sec,off=c.symbols[k];b,rels=c.sections[sec]
        grade='BEHAVIORAL'
        if k.startswith('?CalcInQuotes@'):
            assert k=='?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z' and len(b)==43 and not rels
            raw=EXE.read_bytes();a=va_to_off(pe_sections(raw),0x9b9570);assert b==raw[a:a+43];grade='MATCH'
        reports.append(dict(symbol=k,bytes=len(b),grade=grade))
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s) for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_SYMBOL_OK cases=5' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-symbol.json').write_text(json.dumps(dict(providers=reports,linked_cases=5),indent=2)+'\n')
    print('PARSER_SYMBOL_PASS cases=5 quote_bytes=43')
if __name__=='__main__':main()
