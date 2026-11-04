"""Verify original hierarchy, ctor/dtor/thunk bytes and linked retail snapshots."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_allocator_lifetime
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
CTOR='00/a6/CMemoryAllocatorFixedSize_Constructor_00a65ff0.cpp'
DTOR='00/a6/CMemoryAllocatorFixedSize_Destructor_00a65d20.cpp'
LEAVES=[('ConsolidateFreeAreas',0xa65d30),('GetAllocationSize',0xa65d40),('GetTotalMemoryControlledByAllocator',0xa65d50),('GetHeaderOverhead',0xa65d70),('GetTotalMemoryAllocated',0xa65da0),('GetAllocationSizeIncludingHeader',0xa65de0),('GetLargestFreeAreaSize',0xa65df0),('GetNoFreeAreas',0xa65e20),('GetNoAllocatedAreas',0xa65e50),('GetFragmentation',0xa66550)]
SLOTS=['?Allocate@CMemoryAllocatorFixedSize@@UAEPAXK@Z','?Deallocate@CMemoryAllocatorFixedSize@@UAEXPAX@Z','?GetAllAllocatedAreas@CMemoryAllocatorFixedSize@@UAEKPAPAXK@Z','?GetAllocationSize@CMemoryAllocatorFixedSize@@UAEKPAX@Z','?GetAllocationSizeIncludingHeader@CMemoryAllocatorFixedSize@@UAEKPAX@Z','?GetLargestFreeAreaSize@CMemoryAllocatorFixedSize@@UAEKXZ','?GetTotalMemoryControlledByAllocator@CMemoryAllocatorFixedSize@@UAEKXZ','?GetTotalMemoryAllocated@CMemoryAllocatorFixedSize@@UAEKXZ','?GetNoFreeAreas@CMemoryAllocatorFixedSize@@UAEKXZ','?GetNoAllocatedAreas@CMemoryAllocatorFixedSize@@UAEKXZ','?GetFragmentation@CMemoryAllocatorFixedSize@@UAEMXZ','?GetAllocatedArea@CMemoryAllocatorFixedSize@@UAEPAXJ@Z','?GetHeaderOverhead@CMemoryAllocatorFixedSize@@UAEKXZ','?ConsolidateFreeAreas@CMemoryAllocatorFixedSize@@UAEXXZ','?Validate@CMemoryAllocatorFixedSize@@UAEXXZ','??_ECMemoryAllocatorFixedSize@@UAEPAXI@Z']
ADDRESSES=[0xa66450,0xa662a0,0xa660a0,0xa65d40,0xa65de0,0xa65df0,0xa65d50,0xa65da0,0xa65e20,0xa65e50,0xa66550,0xa66100,0xa65d70,0xa65d30,0xa663a0,0xa66600]

def main():
    work=Path(tempfile.mkdtemp(prefix='fixed-allocator-'));env=check_one.env();objects=[];raw=EXE.read_bytes();sections=pe_sections(raw)
    def compile(source):
        obj=work/(Path(source).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj);return Coff(obj)
    def match(coff,symbol,address,length):
        section,offset=coff.symbols[symbol];body,relocs=coff.sections[section];assert offset==0;patched=bytearray(body)
        targets={'??0CMemoryAllocatorBase@@QAE@K@Z':0xa65ad0,'??_7CMemoryAllocatorFixedSize@@6B@':0x129db64,'??3CMemoryAllocatorBase@@SIXPAX@Z':0xa66750}
        for pos,(target,kind)in relocs.items():
            assert target in targets and kind in [6,20],(target,kind)
            value=targets[target]+struct.unpack_from('<I',body,pos)[0]
            if kind==20:value-=address+pos+4
            struct.pack_into('<I',patched,pos,value&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==length and bytes(patched)==raw[at:at+length],(symbol,len(body),patched.hex(),raw[at:at+length].hex())
    ctor=compile(CTOR);match(ctor,'??0CMemoryAllocatorFixedSize@@QAE@PAXKKKKK@Z',0xa65ff0,169);ctor.table('??_7CMemoryAllocatorFixedSize@@6B@',SLOTS)
    at=va_to_off(sections,0x129db64);assert list(struct.unpack('<16I',raw[at:at+64]))==ADDRESSES
    dtor=compile(DTOR);match(dtor,'??1CMemoryAllocatorFixedSize@@UAE@XZ',0xa65d20,7)
    # The vtable's vector deleting name is a weak alias of the scalar body.
    match(dtor,'??_GCMemoryAllocatorFixedSize@@UAEPAXI@Z',0xa66600,27)
    compile('00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp');compile('00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp');compile('00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp')
    for name,address in LEAVES:
        coff=compile('00/a6/CMemoryAllocatorFixedSize_%s_%08x.cpp'%(name,address));symbol=next(s for s in SLOTS if s.startswith('?'+name+'@'));section,offset=coff.symbols[symbol];body,relocs=coff.sections[section];assert offset==0
        # The fragmentation getter has an absolute retail constant, not a reloc.
        assert not relocs;at=va_to_off(sections,address);assert body==raw[at:at+len(body)],(name,body.hex(),raw[at:at+len(body)].hex())
    retail=oracle_fixed_allocator_lifetime.collect();fixture=(ROOT/'rebuild/tests/integration/FixedAllocatorLifetime_test.cpp').read_text(encoding='utf-8')
    table='static const unsigned long arguments[15][5]={'+','.join('{'+','.join(str(v)+'UL' for v in row)+'}'for row in oracle_fixed_allocator_lifetime.CASES)+'};\n'
    table+='static const unsigned char expected[30][0xc4c]={\n'+',\n'.join('{'+','.join(str(v)for v in bytes.fromhex(row['bytes']))+'}'for row in retail)+'\n};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',table),encoding='utf-8');testobj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    objects.append(testobj);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1,2,4]:
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'FIXED_ALLOCATOR_LIFETIME_OK cases=30'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/fixed-allocator-lifetime.json').write_text(json.dumps(dict(grade='RELOCATION_MATCH',constructor_bytes=169,destructor_bytes=7,deleting_thunk_bytes=27,linked_cases=30,retail_cases=30,vtable_slots=16,object_size=0xc4c,chunk_size=0x18,omitted_provider_checks=4,mocked_boundaries='five virtual fixture boundaries abort if called; no heap operations exercised; recovered native providers compose in gate_fixed_os_lifetime.py',retail_results=retail),indent=2)+'\n',encoding='utf-8')
    print('FIXED_ALLOCATOR_LIFETIME_PASS bytes=203 cases=30 leaves=10 slots=16 omitted=4')

if __name__=='__main__':main()
