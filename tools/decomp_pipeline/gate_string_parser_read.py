"""ReadAsString retail parity and three return-path contracts."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
TARGETS={'??0CCharString@@QAE@XZ':0x99e4b0,'?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,'??0CCharString@@QAE@ABV0@@Z':0x99ec30,'??1CCharString@@QAE@XZ':0x99eae0,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'?Error@CStringParser@@IAEXABVCCharString@@@Z':0x9b95f0}
def main():
    w=Path(tempfile.mkdtemp(prefix='read-string-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'r.obj';s=ROOT/'rebuild/src/compiled/00/9b/CStringParser_ReadAsString_009ba6f0.cpp';t=ROOT/'rebuild/tests/00/9b/CStringParser_ReadAsString_009ba6f0_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(o);k=next(k for k in c.symbols if k.startswith('?ReadAsString@'));sec,off=c.symbols[k];b,rels=c.sections[sec];p=bytearray(b);assert len(b)==145 and off==0
    for pos,(target,kind) in rels.items():
        dest=(0x129b2ec if 'Error' in target else 0x122d70e) if target.startswith('??_C@') else TARGETS[target]
        assert kind in (6,20)
        struct.pack_into('<I',p,pos,(dest+struct.unpack_from('<i',b,pos)[0]-0x9ba6f0-pos-4 if kind==20 else dest)&0xffffffff)
    raw=EXE.read_bytes();a=va_to_off(pe_sections(raw),0x9ba6f0);assert bytes(p)==raw[a:a+len(b)]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_READ_STRING_OK cases=3' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-read.json').write_text(json.dumps(dict(address='009ba6f0',grade='RELOCATION_MATCH',bytes=145,linked_cases=3),indent=2)+'\n')
    print('PARSER_READ_STRING_PASS bytes=145 cases=3')
if __name__=='__main__':main()
