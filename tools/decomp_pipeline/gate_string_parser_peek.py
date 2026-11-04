"""Lookahead cursor restore and failure-side-effect contracts."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
    w=Path(tempfile.mkdtemp(prefix='parser-peek-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];reports=[]
    sources=[ROOT/'rebuild/src/compiled/00/9b'/name for name in ['CStringParser_PeekNextItem_009ba8c0.cpp','CStringParser_PeekNextItemType_009ba900.cpp']];test=ROOT/'rebuild/tests/00/9b/CStringParser_Peek_009ba8c0_test.cpp';raw=EXE.read_bytes();sections=pe_sections(raw)
    targets={'?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,'??0CCharString@@QAE@XZ':0x99e4b0,'??1CCharString@@QAE@XZ':0x99eae0}
    for source,address in zip(sources,[0x9ba8c0,0x9ba900]):
        obj=w/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],e,w);assert r.returncode==0,r.stdout+r.stderr
        c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?Peek'));sec,off=c.symbols[k];b,rels=c.sections[sec];patched=bytearray(b)
        for pos,(target,kind) in rels.items():
            assert kind==20;struct.pack_into('<I',patched,pos,(targets[target]+struct.unpack_from('<i',b,pos)[0]-address-pos-4)&0xffffffff)
        start=va_to_off(sections,address);parity=bytes(patched)==raw[start:start+len(b)];reports.append(dict(address=hex(address),bytes=len(b),grade='RELOCATION_MATCH' if parity else 'BEHAVIORAL'))
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_PEEK_OK cases=4' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-peek.json').write_text(json.dumps(dict(providers=reports,linked_cases=4),indent=2)+'\n');print('PARSER_PEEK_PASS cases=4 '+str(reports))
if __name__=='__main__':main()
