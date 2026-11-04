"""Verify native allocator cleanup, direct class release, and retail ordering."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
from gate_memory_allocator_data import SOURCE as DATA_SOURCE
import oracle_allocator_destructor
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9f/CMemoryAllocator_Destructor_009f2310.cpp','00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-destructor-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for name,prefix,address,size in zip(NAMES,['??1CMemoryAllocator@@IAE@XZ','??3CMemoryAllocatorBase@@SIXPAX@Z'],[0x9f2310,0xa66750],[116,15]):
        obj=work/(Path(name).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/name)],env,work);assert result.returncode==0,result.stdout+result.stderr
        coff=Coff(obj);section,offset=coff.symbols[prefix];body,relocs=coff.sections[section];patched=bytearray(body)
        for position,(symbol,kind)in relocs.items():
            target=0xbfea14 if symbol=='_free'else 0xa66750 if symbol.startswith('??3CMemoryAllocatorBase')else 0x1396868 if symbol.startswith('?AllocatorInfo@')else 0x13caa70 if symbol.startswith('?g_CMemoryAllocatorCriticalSection')else 0x143fde4 if symbol=='__imp__DeleteCriticalSection@4'else 0x143fd40 if symbol=='__imp__VirtualFree@12'else None
            assert target is not None and kind in(6,20),(symbol,kind)
            addend=struct.unpack_from('<i',body,position)[0];value=target+addend-(address+position+4 if kind==20 else 0);struct.pack_into('<I',patched,position,value&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==size and bytes(patched)==raw[at:at+size],(name,len(body),size,patched.hex(),raw[at:at+size].hex())
        objects.append(obj);reports.append(dict(address=f'{address:08x}',bytes=size,grade='RELOCATION_MATCH',source=name))
    data=work/'data.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(data),str(ROOT/'rebuild/src/compiled'/DATA_SOURCE)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(data)
    retail=oracle_allocator_destructor.collect();fixture=(ROOT/'rebuild/tests/integration/MemoryAllocatorDestructor_test.cpp').read_text(encoding='utf-8')
    expected='static const unsigned long expected[8][64]={'+','.join('{'+','.join(str(v)for v in row['order'])+'}'for row in retail)+'};\n'
    expected+='static const unsigned long expectedCalls[8]={'+','.join(str(len(row['order']))for row in retail)+'};\n'
    expected+='static const unsigned long expectedCount[8]={'+','.join(str(row['final_count'])for row in retail)+'};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',expected),encoding='utf-8');obj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj)
    # Redirect only the fixture copies' IAT symbol references. /alternatename
    # can bind real kernel32 imports before the recording aliases are used.
    # The native bodies/relocations above were verified before this boundary hook.
    for index in range(2):
        native=objects[index].read_bytes()
        for original,replacement in [(b'__imp__VirtualFree@12',b'_ReleaseRecording'),(b'__imp__DeleteCriticalSection@4',b'_DestroyRecording')]:
            if original+b'\0'in native:
                assert native.count(original+b'\0')==1
                native=native.replace(original+b'\0',replacement+bytes(len(original)-len(replacement)+1))
        hooked=work/(objects[index].stem+'.fixture.obj');hooked.write_bytes(native);objects[index]=hooked
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omitted in range(3):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'MEMORY_ALLOCATOR_DTOR_OK cases=8'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/allocator-destructor.json').write_text(json.dumps(dict(providers=reports,linked_cases=8,retail_cases=8,omitted_provider_checks=3,retail_results=retail,mocked_boundaries='CRT free, Win32 VirtualFree and DeleteCriticalSection; native destructor, class-specific release and static data'),indent=2)+'\n',encoding='utf-8')
    print('ALLOCATOR_DESTRUCTOR_PASS cases=8 bytes=131 omitted=3')

if __name__=='__main__':main()
