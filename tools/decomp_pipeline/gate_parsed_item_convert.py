"""Parsed-item string-conversion dispatch and precision contract."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
    w=Path(tempfile.mkdtemp(prefix='parsed-convert-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'c.obj';s=ROOT/'rebuild/src/compiled/00/9b/CParsedItem_ConvertToString_009b98b0.cpp';t=ROOT/'rebuild/tests/00/9b/CParsedItem_ConvertToString_009b98b0_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(o);k=next(k for k in c.symbols if k.startswith('?ConvertToString@'));sec,off=c.symbols[k];b,rels=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSED_CONVERT_STRING_OK cases=5' in r.stdout
    (ROOT/'rebuild/compile-gate/parsed-item-convert.json').write_text(json.dumps(dict(address='009b98b0',grade='BEHAVIORAL',compiled_bytes=len(b),linked_cases=5,mocked_boundaries='numeric formatting and string storage'),indent=2)+'\n');print('PARSED_CONVERT_PASS cases=5')
if __name__=='__main__':main()
