"""Verify the native allocator frame boundary, including its actual ABI."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-new-frame-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE']
    source=ROOT/'rebuild/src/compiled/00/9f/CMemoryAllocator_NewFrame_009f26d0.cpp';obj=work/'provider.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);key='?NewFrame@CMemoryAllocator@@QAEXXZ';section,offset=coff.symbols[key];body,relocs=coff.sections[section];patched=bytearray(body)
    for position,(symbol,kind)in relocs.items():
        target=0x13caa70 if symbol.startswith('?g_CMemoryAllocatorCriticalSection')else 0x143fe0c if symbol=='__imp__EnterCriticalSection@4'else 0x143fe10 if symbol=='__imp__LeaveCriticalSection@4'else None
        assert target is not None and kind==6,(symbol,kind)
        struct.pack_into('<I',patched,position,target+struct.unpack_from('<I',body,position)[0])
    raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),0x9f26d0);assert len(body)==23 and bytes(patched)==raw[at:at+23]
    test=ROOT/'rebuild/tests/00/9f/CMemoryAllocator_NewFrame_009f26d0_test.cpp';exe=work/'fixture.exe'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(test),'kernel32.lib'],env,work);assert result.returncode!=0,'omitted provider linked'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(test),str(obj),'kernel32.lib'],env,work);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'NEW_FRAME_OK cases=3'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/allocator-new-frame.json').write_text(json.dumps(dict(address='009f26d0',grade='RELOCATION_MATCH',bytes=23,linked_cases=3,omitted_provider_checks=1,mocked_boundaries='none; real Win32 critical section'),indent=2)+'\n',encoding='utf-8')
    print('ALLOCATOR_NEW_FRAME_PASS cases=3 bytes=23 omitted=1')

if __name__=='__main__':main()
