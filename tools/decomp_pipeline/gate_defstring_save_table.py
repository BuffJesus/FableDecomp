"""Verify SaveTable lock state, native map cleanup and file lifetime."""
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off

ROOT=Path(__file__).resolve().parents[2]

def main():
    work=Path(tempfile.mkdtemp(prefix='defstring-save-table-'))
    env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    sources=[ROOT/'rebuild/src/compiled'/s for s in ['00/9d/CDefStringTable_SaveTable_009d4ce0.cpp','00/43/STL_DefStringMapErase_0043020a.cpp']]
    test=ROOT/'rebuild/tests/00/9d/CDefStringTable_SaveTable_009d4ce0_test.cpp'
    obj=work/'save.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);key=next(k for k in coff.symbols if k.startswith('?SaveTable@CDefStringTable'))
    section,offset=coff.symbols[key];body,relocations=coff.sections[section]
    targets={'??0CAFile@@QAE@XZ':0x99a2f0,'??_7CDiskFileWin32@@6B@':0x122d06c,'??0CWideString@@QAE@XZ':0x99aed0,'?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z':0x99a6a0,'??0CFileDataOutputStream@@QAE@PAVCAFile@@@Z':0x994400,'?WriteTable@CDefStringTable@@QAEXAAVCDataOutputStream@@@Z':0x9d4a50,'??1CFileDataOutputStream@@UAE@XZ':0x994430,'?Close@CDiskFileWin32@@UAEXXZ':0x99a920,'??1CWideString@@QAE@XZ':0x99b510,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300}
    patched=bytearray(body)
    for position,(symbol,kind) in relocations.items():
        target=0x43020a if symbol.startswith('?_M_erase') else targets[symbol]
        addend=struct.unpack_from('<i',body,position)[0]
        assert kind in (6,20)
        value=target+addend-(0x9d4ce0+position+4 if kind==20 else 0)
        struct.pack_into('<I',patched,position,value&0xffffffff)
    raw=EXE.read_bytes();retail_offset=va_to_off(pe_sections(raw),0x9d4ce0)
    assert len(body)==200 and bytes(patched)==raw[retail_offset:retail_offset+200]
    exe=work/'test.exe'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe)]+[str(s) for s in sources]+[str(test)],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'DEFSTRING_SAVE_TABLE_OK cases=5' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/defstring-save-table.json').write_text(json.dumps(dict(address='009d4ce0',grade='RELOCATION_MATCH',compiled_bytes=len(body),retail_bytes=200,linked_cases=5,mocked_boundaries='disk and output-stream operations, table serialization and string destruction; native string-map erase'),indent=2)+'\n')
    print('DEFSTRING_SAVE_TABLE_PASS cases=5 bytes='+str(len(body)))

if __name__=='__main__':main()
