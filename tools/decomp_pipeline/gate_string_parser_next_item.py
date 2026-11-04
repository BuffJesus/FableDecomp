"""Retail token dispatch parity and linked branch checks."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
TARGETS={'?SkipWhitespace@CStringParser@@QAEJXZ':0x9b9c60,'_isdigit':0xbfeda6,'_isalpha':0xbfedb8,'?ReadNextSymbol@CStringParser@@IAE_NAAVCParsedItem@@@Z':0x9b99c0,'?ReadNextString@CStringParser@@IAE_NAAVCParsedItem@@@Z':0x9b9fc0,'?ReadNextNumber@CStringParser@@IAE_NAAVCParsedItem@@@Z':0x9b9ee0}
def main():
    w=Path(tempfile.mkdtemp(prefix='next-item-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'n.obj';s=ROOT/'rebuild/src/compiled/00/9b/CStringParser_ReadNextItem_009ba080.cpp';t=ROOT/'rebuild/tests/00/9b/CStringParser_ReadNextItem_009ba080_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(o);k=next(k for k in c.symbols if k.startswith('?ReadNextItem@'));sec,off=c.symbols[k];b,rels=c.sections[sec];p=bytearray(b);assert len(b)==136 and len(rels)==7
    for pos,(target,kind) in rels.items():
        assert kind==20;struct.pack_into('<I',p,pos,(TARGETS[target]+struct.unpack_from('<i',b,pos)[0]-0x9ba080-pos-4)&0xffffffff)
    raw=EXE.read_bytes();a=va_to_off(pe_sections(raw),0x9ba080);parity = bytes(p)==raw[a:a+len(b)]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_NEXT_ITEM_OK cases=9' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-next-item.json').write_text(json.dumps(dict(address='009ba080',grade='RELOCATION_MATCH' if parity else 'BEHAVIORAL',bytes=136,linked_cases=9),indent=2)+'\n')
    print('PARSER_NEXT_ITEM_PASS bytes=136 cases=9')
if __name__=='__main__':main()
