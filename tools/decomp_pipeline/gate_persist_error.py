"""Retail parser error formatting and callback ABI gate."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
TARGETS={'?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,'??H@YI?AVCCharString@@ABV0@PBD@Z':0x99f600,'??H@YI?AVCCharString@@ABV0@0@Z':0x99f570,'??1CCharString@@QAE@XZ':0x99eae0}
def main():
    w=Path(tempfile.mkdtemp(prefix='definition-error-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];o=w/'e.obj';s=ROOT/'rebuild/src/compiled/00/9b/CPersistContext_PersistError_009baa90.cpp';t=ROOT/'rebuild/tests/00/9b/CPersistContext_PersistError_009baa90_test.cpp'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(s)],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(o);k=next(k for k in c.symbols if k.startswith('?PersistError@'));sec,off=c.symbols[k];b,rels=c.sections[sec];p=bytearray(b);assert len(b)==170 and k=='?PersistError@CPersistContext@@QBEXABVCStringParser@@ABVCCharString@@@Z'
    for pos,(target,kind) in rels.items():
        if kind==6:dest=0x129b084 if 'error' in target else 0x122e020 if '?$CJ' in target else 0x122e028
        else:dest=TARGETS[target]
        struct.pack_into('<I',p,pos,(dest+struct.unpack_from('<i',b,pos)[0]-0x9baa90-pos-4 if kind==20 else dest)&0xffffffff)
    raw=EXE.read_bytes();a=va_to_off(pe_sections(raw),0x9baa90);parity=bytes(p)==raw[a:a+170]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe'),str(s),str(t)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_ERROR_OK cases=2' in r.stdout
    (ROOT/'rebuild/compile-gate/persist-error.json').write_text(json.dumps(dict(address='009baa90',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=170,linked_cases=2),indent=2)+'\n')
    print('PERSIST_ERROR_PASS bytes=170 cases=2')
if __name__=='__main__':main()
