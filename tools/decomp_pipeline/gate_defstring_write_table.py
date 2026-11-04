"""Verify native definition string-table output and relocated retail bytes."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
import struct
from pe_oracle import EXE,pe_sections,va_to_off





from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9d/CDefStringTable_WriteTable_009d4a50.cpp','00/99/CMemoryDataOutputStream_Clear_00996b60.cpp','00/43/STL_DefStringMapErase_0043020a.cpp']
def main():
    w=Path(tempfile.mkdtemp(prefix='enum-parser-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE'];sources=[ROOT/'rebuild/src/compiled'/n for n in NAMES];test=ROOT/'rebuild/tests/00/9d/CDefStringTable_WriteTable_009d4a50_test.cpp';obj=w/'enum.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
    c=Coff(obj);k=next(k for k in c.symbols if k.startswith('?WriteTable@CDefStringTable'));sec,off=c.symbols[k];b,relocs=c.sections[sec]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEFSTRING_WRITE_TABLE_OK cases=3' in r.stdout,r.stdout
    patched=bytearray(b);targets={'?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,'?GetLength@CMemoryDataOutputStream@@QBEKXZ':0x994b40,'?PeekData@CMemoryDataOutputStream@@QBEPBXXZ':0x995e70}
    for pos,(symbol,kind) in relocs.items():
        assert kind==20;struct.pack_into('<I',patched,pos,(targets[symbol]+struct.unpack_from('<i',b,pos)[0]-0x9d4a50-pos-4)&0xffffffff)
    raw=EXE.read_bytes();offset=va_to_off(pe_sections(raw),0x9d4a50);assert len(b)==87 and bytes(patched)==raw[offset:offset+87]
    (ROOT/'rebuild/compile-gate/defstring-write-table.json').write_text(json.dumps(dict(address='009d4a50',grade='RELOCATION_MATCH',bytes=87,linked_cases=3,mocked_boundaries='memory stream storage and scalar output'),indent=2)+'\n')
    print('DEFSTRING_WRITE_TABLE_PASS cases=3 bytes='+str(len(b)))
if __name__=='__main__':main()
