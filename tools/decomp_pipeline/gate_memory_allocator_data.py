"""Validate allocator data against raw retail and zero-filled section storage."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCE='01/39/MemoryAllocatorData_01396868.cpp'
TABLE='?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A'
LOCK='?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A'
EXTRA=[(0x13caa88,'?MemoryUseBeforeAllocator@CMemoryAllocator@@1KA',4),(0x13caab0,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA',4),(0x13caa94,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA',1)]

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-data-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=work/'data.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/SOURCE)],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),0x1396868);expected=raw[at:at+840]
    assert coff_data(coff,TABLE,840)==expected
    assert coff_data(coff,LOCK,24)==bytes(24)
    section,offset=coff.symbols[TABLE];assert not coff.sections[section][1],'table has relocations'
    with pefile.PE(str(EXE))as pe:
        for address,symbol,size in [(0x13caa70,LOCK,24)]+EXTRA:
            assert coff_data(coff,symbol,size)==bytes(size)
            rva=address-pe.OPTIONAL_HEADER.ImageBase
            section=next(s for s in pe.sections if s.VirtualAddress<=rva and rva+size<=s.VirtualAddress+s.Misc_VirtualSize)
            assert section.Name.rstrip(b'\0')==b'.data'and rva-section.VirtualAddress>=section.SizeOfRawData
    fixture=(ROOT/'rebuild/tests/integration/MemoryAllocatorData_test.cpp').read_text(encoding='utf-8')
    table='static const unsigned long expected[30][7]={'+','.join('{'+','.join(str(x)for x in row)+'}'for row in struct.iter_unpack('<7I',expected))+'};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',table),encoding='utf-8');exe=work/'fixture.exe'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(test),'kernel32.lib'],env,work);assert result.returncode!=0,'omitted data provider linked'
    result=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(exe),str(test),str(obj),'kernel32.lib'],env,work);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'MEMORY_ALLOCATOR_DATA_OK rows=30'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/memory-allocator-data.json').write_text(json.dumps(dict(addresses=['01396868','013caa70']+[f'{a:08x}'for a,_,_ in EXTRA],data_bytes=873,rows=30,omitted_provider_checks=1,coverage_claim='native data only; no additional function claim',source=SOURCE,behavior=result.stdout.strip(),mocked_boundaries='none; real Win32 critical section'),indent=2)+'\n',encoding='utf-8')
    print('MEMORY_ALLOCATOR_DATA_PASS bytes=873 rows=30 omitted=1')

if __name__=='__main__':main()
