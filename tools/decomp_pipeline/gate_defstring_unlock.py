"""Verify string-table unlock reconstruction against retail and linked fixtures."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]

def main():
    work=Path(tempfile.mkdtemp(prefix='defstring-unlock-'))
    env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    source=ROOT/'rebuild/src/compiled/00/9d/CDefStringTable_Unlock_009d4f10.cpp'
    test=ROOT/'rebuild/tests/00/9d/CDefStringTable_Unlock_009d4f10_test.cpp'
    obj=work/'unlock.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);key=next(k for k in coff.symbols if k.startswith('?Unlock@'))
    section,offset=coff.symbols[key];body,relocations=coff.sections[section];patched=bytearray(body)
    for position,(symbol,kind) in relocations.items():
        target=0x995e70 if symbol.startswith('?PeekData@') else 0x994b40 if symbol.startswith('?GetLength@CMemory') else 0x99ebf0 if symbol.startswith('??0CCharString') else 0x519230 if symbol.startswith('??A') else 0x99eae0 if symbol.startswith('??1CCharString') else None
        assert target is not None and kind==20,(symbol,kind)
        struct.pack_into('<I',patched,position,(target+struct.unpack_from('<i',body,position)[0]-0x9d4f10-position-4)&0xffffffff)
    raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),0x9d4f10)
    assert len(body)==119 and bytes(patched)==raw[at:at+119]
    exe=work/'test.exe';result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(source),str(test)],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'DEFSTRING_UNLOCK_OK cases=7' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/defstring-unlock.json').write_text(json.dumps(dict(address='009d4f10',grade='RELOCATION_MATCH',bytes=119,linked_cases=7,mocked_boundaries='stream storage, string lifetime and map insertion; actual unlock body and inline length access'),indent=2)+'\n')
    print('DEFSTRING_UNLOCK_PASS cases=7 bytes=119')

if __name__=='__main__':main()
