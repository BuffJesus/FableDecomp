"""Verify full retail main constructor parity and composed native allocator behavior."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_allocator_constructor
from gate_fixed_allocator_lifetime import LEAVES
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]

def main():
    work=Path(tempfile.mkdtemp(prefix='main-allocator-behavior-'));env=check_one.env();objects=[]
    sources=[ROOT/'rebuild/src/compiled/00/9f/CMemoryAllocator_Constructor_009f20b0.cpp']
    sources += [ROOT/'rebuild/src/compiled'/source for source in ['00/a6/CMemoryAllocatorFixedSize_Constructor_00a65ff0.cpp','00/a6/CMemoryAllocatorFixedSize_Destructor_00a65d20.cpp','00/a6/CMemoryAllocatorVariableSize_Constructor_00a65930.cpp','00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp','00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp','00/a6/CMemoryAllocatorBase_OperatorNew_00a66730.cpp','00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp','00/9f/InitializeMemorySystem_009f1d20.cpp','01/39/MemoryAllocatorData_01396868.cpp','01/3d/VariableAllocatorAlignmentData_013d28d4.cpp']]
    sources += [ROOT/'rebuild/src/compiled/00/a6'/('CMemoryAllocatorFixedSize_%s_%08x.cpp'%(name,address))for name,address in LEAVES]
    redirects={'__imp__InitializeCriticalSection@4':'_IC0','__imp__EnterCriticalSection@4':'_IC1','__imp__LeaveCriticalSection@4':'_IC2','__imp__VirtualAlloc@16':'_IVM','__imp__CreateFileA@28':'_IOpen','__imp__GetLastError@0':'_IError','_malloc':'_Malloc'}
    for source in sources:
        obj=work/(source.stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert result.returncode==0,result.stdout+result.stderr
        if source == sources[0]:
            coff=Coff(obj);section,offset=coff.symbols['??0CMemoryAllocator@@IAE@PAX@Z'];body,relocs=coff.sections[section];assert offset==0;patched=bytearray(body)
            targets={'?InitializeMemorySystem@@YIXXZ': 10427680, '?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A': 20539496, '?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A': 20753008, '__imp__InitializeCriticalSection@4': 21233380, '__imp__EnterCriticalSection@4': 21233164, '__imp__LeaveCriticalSection@4': 21233168, '??2CMemoryAllocatorBase@@SIPAXI@Z': 10905392, '??0CMemoryAllocatorFixedSize@@QAE@PAXKKKKK@Z': 10903536, '??0CMemoryAllocatorVariableSize@@QAE@PAXJ0JKKK@Z': 10901808, '__allmul': 12577616, '_malloc': 12577294}
            assert len(relocs)==16
            for position,(name,kind)in relocs.items():
                assert name in targets and kind in [6,20],(name,kind)
                value=targets[name]+struct.unpack_from('<I',body,position)[0]
                if kind==20:value-=0x9f20b0+position+4
                struct.pack_into('<I',patched,position,value&0xffffffff)
            raw=EXE.read_bytes();at=va_to_off(pe_sections(raw),0x9f20b0)
            assert len(body)==598 and bytes(patched)==raw[at:at+598]
        blob=obj.read_bytes()
        for old,new in redirects.items():
            needle=old.encode()+b'\0'
            if needle not in blob:continue
            assert blob.count(needle)==1 and len(new)<len(old)+1,(source,old);replacement=new.encode()+b'\0';blob=blob.replace(needle,replacement+b'\0'*(len(needle)-len(replacement)))
        obj.write_bytes(blob);objects.append(obj)
    fixture=ROOT/'rebuild/tests/integration/MainAllocatorConstructor_test.cpp';testobj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(fixture)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(testobj)
    rows=oracle_main_allocator_constructor.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<5I',row['pattern'],row['fail']&0xffffffff,len(row['allocations']),len(row['events']),row['lock_depth']))
        data.extend(b''.join(struct.pack('<7I',*fields)for fields in row['input_table']));data.extend(bytes.fromhex(row['owner']));data.extend(bytes.fromhex(row['table']));data.extend(b''.join(struct.pack('<4I',*event)for event in row['events']))
        for area in row['allocations']:
            data.extend(struct.pack('<3I',area['size'],area['kind'],int(area['bytes']is not None)))
            if area['bytes']is not None:data.extend(bytes.fromhex(area['bytes']))
    oracle=work/'retail.bin';oracle.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1,3,4,6,8,9,10]:
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe),str(oracle)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'MAIN_ALLOCATOR_CTOR_OK cases=16'in result.stdout,(result.returncode,result.stdout,result.stderr)
    report=ROOT/'rebuild/compile-gate/main-allocator-constructor.json';report.write_text(json.dumps(dict(address='009f20b0',grade='RELOCATION_MATCH',bytes=598,cases=16,native_sources=21,omitted_provider_checks=8,mocked_boundaries='critical sections, VirtualAlloc/malloc, missing boot.ini open; unrecovered virtuals/chunk helpers abort if called; vptr addresses normalized',retail_results=rows),indent=2)+'\n',encoding='utf-8')
    print('MAIN_ALLOCATOR_CONSTRUCTOR_PASS bytes=598 cases=16 native_sources=21 omitted=8')

if __name__=='__main__':main()
