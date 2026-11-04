"""Verify definition string-table destruction and its owning STLport map."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]

def main():
    work=Path(tempfile.mkdtemp(prefix='defstring-dtor-'))
    env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled'/s for s in ['00/9d/CDefStringTable_Destructor_009d4e90.cpp','00/43/STL_DefStringMapErase_0043020a.cpp']]
    test=ROOT/'rebuild/tests/00/9d/CDefStringTable_Destructor_009d4e90_test.cpp';obj=work/'dtor.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);key=next(k for k in coff.symbols if k.startswith('??1CDefStringTable'))
    section,offset=coff.symbols[key];body,relocations=coff.sections[section];patched=bytearray(body)
    for position,(symbol,kind) in relocations.items():
        target=0x43020a if symbol.startswith('?_M_erase') else 0x99b510 if symbol.startswith('??1CWideString') else 0x996b30 if symbol.startswith('??1CMemoryDataOutputStream') else 0xbfea14 if symbol=='_free' else None
        assert target is not None and kind==20,(symbol,kind)
        struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-0x9d4e90-position-4)&0xffffffff)
    raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),0x9d4e90)
    assert len(body)==114 and bytes(patched)==raw[at:at+114]
    exe=work/'test.exe';result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe)]+[str(s)for s in sources]+[str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'DEFSTRING_DTOR_OK cases=4' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/defstring-destructor.json').write_text(json.dumps(dict(address='009d4e90',grade='RELOCATION_MATCH',bytes=114,linked_cases=4,mocked_boundaries='filename, memory-stream and string destruction; native map erase/header deallocation; stream fixture reinserts a node to verify the second cleanup'),indent=2)+'\n')
    print('DEFSTRING_DTOR_PASS cases=4 bytes=114')

if __name__=='__main__':main()
