"""Check exact retail allocator aggregation and live-count linked behavior."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_allocator_statistics
from gate_memory_allocator_data import SOURCE as DATA_SOURCE
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9f/CMemoryAllocator_GetTotalMemoryAllocated_009f2450.cpp','00/9f/CMemoryAllocator_GetNoAllocatedAreas_009f2550.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-statistics-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for name,address,prefix in zip(NAMES,[0x9f2450,0x9f2550],['?GetTotalMemoryAllocated@','?GetNoAllocatedAreas@']):
        source=ROOT/'rebuild/src/compiled'/name;obj=work/(source.stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert result.returncode==0,result.stdout+result.stderr
        coff=Coff(obj);key=next(k for k in coff.symbols if k.startswith(prefix));section,offset=coff.symbols[key];body,relocs=coff.sections[section];patched=bytearray(body)
        for position,(symbol,kind)in relocs.items():
            target=0x13caa70 if symbol.startswith('?g_CMemoryAllocatorCriticalSection')else 0x143fe0c if symbol=='__imp__EnterCriticalSection@4'else 0x143fe10 if symbol=='__imp__LeaveCriticalSection@4'else 0x1396868 if symbol.startswith('?AllocatorInfo@')else None
            assert target is not None and kind==6,(symbol,kind)
            struct.pack_into('<I',patched,position,(target+struct.unpack_from('<I',body,position)[0])&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==77 and bytes(patched)==raw[at:at+77],(name,len(body),patched.hex(),raw[at:at+77].hex())
        objects.append(obj);reports.append(dict(address=f'{address:08x}',bytes=77,grade='RELOCATION_MATCH',source=name))
    data=work/'allocator-data.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(data),str(ROOT/'rebuild/src/compiled'/DATA_SOURCE)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(data)
    base=work/'base-ctor.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(base),str(ROOT/'rebuild/src/compiled/00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp')],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(base)
    retail=oracle_allocator_statistics.collect()
    fixture=(ROOT/'rebuild/tests/integration/AllocatorStatistics_test.cpp').read_text(encoding='utf-8')
    expectations='static const unsigned long expected[18][3]={'+','.join('{'+','.join(str(v)for v in [r['result'],len(r['order']),r['final_count']])+'}'for r in retail)+'};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',expectations),encoding='utf-8');obj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    objects.append(obj);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(2):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',NAMES[omitted])
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'ALLOCATOR_STATISTICS_OK cases=18' in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/allocator-statistics.json').write_text(json.dumps(dict(providers=reports,grade='RELOCATION_MATCH',linked_cases=18,retail_cases=18,omitted_provider_checks=2,mocked_boundaries='recording size-class allocators; native table/lock storage; real Win32 critical section',retail_results=retail),indent=2)+'\n',encoding='utf-8')
    print('ALLOCATOR_STATISTICS_PASS cases=18 bytes=154 omitted=2')

if __name__=='__main__':main()
