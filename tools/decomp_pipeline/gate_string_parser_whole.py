"""Typed parser keyword search, linked behavior and retail execution gate."""
import json, subprocess, tempfile
from pathlib import Path
import check_one
import oracle_string_parser_whole
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    work=Path(tempfile.mkdtemp(prefix='parser-whole-gate-'));env=check_one.env()
    env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled/00/9b'/name for name in ['CStringParser_SkipUntilWholeString_009b9aa0.cpp','CStringParser_SkipPastWholeString_009b9c00.cpp']]
    test=ROOT/'rebuild/tests/00/9b/CStringParser_WholeString_009b9aa0_test.cpp'
    reports=[]
    for source in sources:
        obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert r.returncode==0,r.stdout+r.stderr
        c=Coff(obj);symbol=next(k for k in c.symbols if k.startswith('?Skip'))
        section,offset=c.symbols[symbol];body,relocs=c.sections[section]
        reports.append(dict(symbol=symbol,bytes=len(body),relocations=len(relocs)))
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(work/'test.exe')]+[str(s) for s in sources]+[str(test)],env,work)
    assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(work/'test.exe')],capture_output=True,text=True,timeout=30)
    assert r.returncode==0 and 'WHOLE_STRING_OK cases=6' in r.stdout,r.stdout
    oracle_string_parser_whole.main()
    (ROOT/'rebuild/compile-gate/string-parser-whole.json').write_text(json.dumps(dict(grade='BEHAVIORAL',providers=reports,linked_cases=6,retail_oracle_cases=6),indent=2)+'\n')
    print('PARSER_WHOLE_PASS linked=6 retail=6')
if __name__=='__main__':main()
