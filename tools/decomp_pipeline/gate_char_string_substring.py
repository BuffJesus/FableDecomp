"""Retail substring assignment ownership order and call parity."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
    w=Path(tempfile.mkdtemp(prefix='char-substring-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'s.obj';s=ROOT/'rebuild/src/compiled/00/99/CCharString_SetAsSubString_0099ef40.cpp';t=ROOT/'rebuild/tests/00/99/CCharString_SetAsSubString_0099ef40_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(o);k=next(k for k in c.symbols if k.startswith('?SetAsSubString@'));sec,off=c.symbols[k];b,rels=c.sections[sec];p=bytearray(b);assert len(b)==45 and len(rels)==2
    targets={'?UnassignString@CCharString@@AAEXXZ':0x99e9b0,'?AllocStringData@CCharString@@AAEPAVCCharStringData@@PBDJ@Z':0x99ea60}
    for pos,(target,kind) in rels.items():
        assert kind==20;struct.pack_into('<I',p,pos,(targets[target]+struct.unpack_from('<i',b,pos)[0]-0x99ef40-pos-4)&0xffffffff)
    raw=EXE.read_bytes();a=va_to_off(pe_sections(raw),0x99ef40);assert bytes(p)==raw[a:a+45]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'CHAR_SUBSTRING_OK cases=5' in r.stdout
    (ROOT/'rebuild/compile-gate/char-string-substring.json').write_text(json.dumps(dict(address='0099ef40',grade='RELOCATION_MATCH',bytes=45,linked_cases=5),indent=2)+'\n')
    print('CHAR_SUBSTRING_PASS bytes=45 cases=5')
if __name__=='__main__':main()
