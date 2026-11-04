"""Enum recovery linked to native token-reader implementations."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import oracle_persist_byte_dispatch
import struct
from pe_oracle import EXE,pe_sections,va_to_off



from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/40/CPersistContext_TransferSignedChar_00405a60.cpp','00/40/CPersistContext_ByteTransferIn_004059f0.cpp','00/40/CPersistContext_ByteTransferOut_00405a10.cpp','00/9e/CPersistTraitsSLONG_TransferOut_009e2560.cpp']
def main():
    retail=oracle_persist_byte_dispatch.collect()
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9b/../40/CPersistContext_TransferSignedChar_00405a60_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?TransferSignedChar@CPersistContext'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PERSIST_BYTE_DISPATCH_OK cases=5' in r.stdout,r.stdout
    raw=EXE.read_bytes();sections=pe_sections(raw)
    targets={'?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z':0x9e2560,'?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,'??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,'??1CCharString@@QAE@XZ':0x99eae0}
    exact=[]
    for index,va,size,prefix in [(2,0x405a10,31,'?TransferOut@?$CPersistContext'),(3,0x9e2560,39,'?TransferOut@CPersistTraitsSLONG')]:
        obj=w/('exact'+str(index)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[index])],e,w);assert r.returncode==0,r.stdout+r.stderr;c=Coff(obj);key=next(k for k in c.symbols if k.startswith(prefix));sec,off=c.symbols[key];body,rels=c.sections[sec];patched=bytearray(body)
        for pos,(sym,kind) in rels.items():
            assert kind==20;struct.pack_into('<I',patched,pos,(targets[sym]+struct.unpack_from('<i',body,pos)[0]-va-pos-4)&0xffffffff)
        offset=va_to_off(sections,va);assert len(body)==size and bytes(patched)==raw[offset:offset+size];exact.append(dict(address=hex(va),bytes=size,grade='RELOCATION_MATCH'))
    (ROOT/'rebuild/compile-gate/persist-byte-dispatch.json').write_text(json.dumps(dict(address='00405a60',grade='BEHAVIORAL',compiled_bytes=len(b),linked_cases=5,retail_cases=retail,exact_providers=exact,mocked_boundaries='CRC, binary IO, text field dispatch, integer formatter/input and string storage'),indent=2)+'\n')
    print('PERSIST_BYTE_DISPATCH_PASS cases=5 bytes='+str(len(b)))
if __name__=='__main__':main()

