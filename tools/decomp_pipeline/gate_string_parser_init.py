"""Typed retail parser Init byte parity and untouched-field validation."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
    work=Path(tempfile.mkdtemp(prefix='parser-init-gate-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    source=ROOT/'rebuild/src/compiled/00/9b/CStringParser_InitTyped_009b9530.cpp';test=ROOT/'rebuild/tests/00/9b/CStringParser_InitTyped_009b9530_test.cpp';obj=work/'init.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);symbol=next(k for k in c.symbols if k.startswith('?Init@'));section,offset=c.symbols[symbol];body,relocs=c.sections[section]
    assert len(body)==55 and len(relocs)==1 and offset==0
    raw=EXE.read_bytes();start=va_to_off(pe_sections(raw),0x9b9530);retail=raw[start:start+55];patched=bytearray(body)
    pos,(target,kind)=next(iter(relocs.items()));assert target=='??4CCharString@@QAEAAV0@ABV0@@Z' and kind==20
    destination=0x0099efb0
    struct.pack_into('<i',patched,pos,destination-0x009b9530-pos-4)
    assert bytes(patched)==retail
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(work/'test.exe'),str(source),str(test)],env,work);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(work/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSER_INIT_TYPED_OK cases=2' in r.stdout
    (ROOT/'rebuild/compile-gate/string-parser-init.json').write_text(json.dumps(dict(address='009b9530',grade='RELOCATION_MATCH',bytes=55,linked_cases=2),indent=2)+'\n')
    print('PARSER_INIT_TYPED_PASS bytes=55 cases=2')
if __name__=='__main__':main()
