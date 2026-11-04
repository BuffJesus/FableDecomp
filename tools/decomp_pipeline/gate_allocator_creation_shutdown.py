"""Verify allocator creation and deferred shutdown against retail bodies."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
from gate_memory_allocator_data import SOURCE as DATA_SOURCE
import oracle_allocator_creation_shutdown
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/9f/CMemoryAllocator_Create_009f2870.cpp','00/9f/MemoryAllocatorShutdown_009f2d20.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-create-shutdown-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    targets={'__imp__VirtualAlloc@16':0x143fd44,'__imp__GetProcessHeap@0':0x143fd3c,'__imp__GlobalMemoryStatus@4':0x143fe20,'__imp__VirtualFree@12':0x143fd40,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?MemoryUseBeforeAllocator@CMemoryAllocator@@1KA':0x13caa88,'?g_FableAllocatorMemoryBaseline_013CAAB0@@3KA':0x13caab0,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,'??0CMemoryAllocator@@IAE@PAX@Z':0x9f20b0,'??1CMemoryAllocator@@IAE@XZ':0x9f2310}
    for source,symbol,address,size in zip(NAMES,['?Create@CMemoryAllocator@@KIXXZ','?FableMemoryAllocatorShutdown_009f2d20@@YAXXZ'],[0x9f2870,0x9f2d20],[95,61]):
        obj=work/(Path(source).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work);assert result.returncode==0,result.stdout+result.stderr
        coff=Coff(obj);section,offset=coff.symbols[symbol];body,relocs=coff.sections[section];patched=bytearray(body)
        for position,(name,kind)in relocs.items():
            assert name in targets and kind in(6,20),(name,kind)
            value=targets[name]+struct.unpack_from('<i',body,position)[0]-(address+position+4 if kind==20 else 0);struct.pack_into('<I',patched,position,value&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==size and bytes(patched)==raw[at:at+size],(source,len(body),size,patched.hex(),raw[at:at+size].hex())
        objects.append(obj);reports.append(dict(address=f'{address:08x}',bytes=size,grade='RELOCATION_MATCH',source=source))
    for source in ['00/9f/CMemoryAllocator_Destructor_009f2310.cpp','00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp',DATA_SOURCE,'01/3b/GFMainSingletonCodeData_013b7d58.cpp']:
        obj=work/(Path(source).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj)
    retail=oracle_allocator_creation_shutdown.collect()
    fixture=(ROOT/'rebuild/tests/integration/AllocatorCreationShutdown_test.cpp').read_text(encoding='utf-8')
    expected='static const unsigned long expected[10][8]={'+','.join('{'+','.join(str(v)for v in [row['created'],*row['baseline'],row['create_flag'],row['first_owner'],row['first_flag'],row['final_owner'],row['final_flag']])+'}'for row in retail)+'};\n'
    expected+='static const unsigned long expectedCalls[10]={'+','.join(str(len(row['order']))for row in retail)+'};\n'
    expected+='static const unsigned long expectedOrder[10][16]={'+','.join('{'+','.join(str(v)for v in row['order'])+'}'for row in retail)+'};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',expected),encoding='utf-8');obj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    for index in range(len(objects)):
        body=objects[index].read_bytes()
        for original,replacement in [(b'__imp__VirtualAlloc@16',b'_AllocateRecording'),(b'__imp__GetProcessHeap@0',b'_HeapRecording'),(b'__imp__GlobalMemoryStatus@4',b'_StatusRecording'),(b'__imp__DeleteCriticalSection@4',b'_DestroyRecording'),(b'__imp__VirtualFree@12',b'_ReleaseRecording')]:
            if original+b'\0'in body:
                assert body.count(original+b'\0')==1
                body=body.replace(original+b'\0',replacement+bytes(len(original)-len(replacement)+1))
        hooked=work/(objects[index].stem+'.fixture.obj');hooked.write_bytes(body);objects[index]=hooked
    objects.append(obj);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omitted in range(6):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'ALLOCATOR_CREATION_SHUTDOWN_OK cases=10'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/allocator-creation-shutdown.json').write_text(json.dumps(dict(providers=reports,linked_cases=10,retail_cases=10,omitted_provider_checks=6,retail_results=retail,mocked_boundaries='allocator constructor and Win32 APIs; native create, shutdown, full allocator destructor, base release and static data',limitation='retail 009F2D20 has no discovered callers or address-taken references; no live exit registration claim'),indent=2)+'\n',encoding='utf-8')
    print('ALLOCATOR_CREATION_SHUTDOWN_PASS bytes=156 cases=10 omitted=6')

if __name__=='__main__':main()
