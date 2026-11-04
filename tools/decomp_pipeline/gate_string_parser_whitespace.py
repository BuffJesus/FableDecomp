"""Parser whitespace cursor and untouched-line contracts."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    w=Path(tempfile.mkdtemp(prefix='parser-space-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'s.obj';s=ROOT/'rebuild/src/compiled/00/9b/CStringParser_SkipWhitespace_009b9c60.cpp';t=ROOT/'rebuild/tests/00/9b/CStringParser_SkipWhitespace_009b9c60_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(o);k=next(k for k in c.symbols if k.startswith('?SkipWhitespace@'));sec,off=c.symbols[k];b,rels=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_WHITESPACE_OK cases=7' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-whitespace.json').write_text(json.dumps(dict(address='009b9c60',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=124,linked_cases=7),indent=2)+'\n')
    print('PARSER_WHITESPACE_PASS cases=7')
if __name__=='__main__':main()
