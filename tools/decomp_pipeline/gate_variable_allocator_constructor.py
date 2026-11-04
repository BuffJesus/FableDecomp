"""Match native variable constructor/vtable/data and all initial chunk branches."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_allocator_constructor
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from gate_fixed_allocator_lifetime import SLOTS
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_Constructor_00a65930.cpp'
DATA='01/3d/VariableAllocatorAlignmentData_013d28d4.cpp'

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-allocator-'));env=check_one.env();objects=[]
    def compile(source):
        obj=work/(Path(source).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj);return Coff(obj)
    coff=compile(SOURCE);section,offset=coff.symbols['??0CMemoryAllocatorVariableSize@@QAE@PAXJ0JKKK@Z'];body,relocs=coff.sections[section];assert offset==0;patched=bytearray(body)
    targets={'??0CMemoryAllocatorBase@@QAE@K@Z':0xa65ad0,'??_7CMemoryAllocatorVariableSize@@6B@':0x129db1c,'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8,'?NewChunkDemandPaged@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@PAXJ@Z':0xa65830,'?NewChunk@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@J@Z':0xa65720}
    assert len(relocs)==6
    for position,(name,kind)in relocs.items():
        assert name in targets and kind in [6,20],(name,kind);value=targets[name]+struct.unpack_from('<I',body,position)[0]
        if kind==20:value-=0xa65930+position+4
        struct.pack_into('<I',patched,position,value&0xffffffff)
    raw=EXE.read_bytes();sections=pe_sections(raw);at=va_to_off(sections,0xa65930);assert len(body)==138 and bytes(patched)==raw[at:at+138]
    slots=[s.replace('CMemoryAllocatorFixedSize','CMemoryAllocatorVariableSize')for s in SLOTS];slots[13]='?ConsolidateFreeAreas@CMemoryAllocatorBase@@UAEXXZ';coff.table('??_7CMemoryAllocatorVariableSize@@6B@',slots)
    at=va_to_off(sections,0x129db1c);assert list(struct.unpack('<16I',raw[at:at+64]))==[0xa659c0,0xa65a50,0xa65260,0xa651b0,0xa651e0,0xa64500,0xa65210,0xa64db0,0xa64530,0xa64560,0xa64590,0xa652d0,0xa64e20,0xa65b50,0xa65330,0xa65c60]
    data=compile(DATA)
    for symbol,address in [('?g_FableVariableAllocatorAlignment_013D28D4@@3KA',0x13d28d4),('?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA',0x13d28d8)]:
        section,offset=data.symbols[symbol];values,relocs=data.sections[section];assert not relocs and coff_data(data,symbol,4)==b'\0'*4
        retail_section=next(s for s in sections if s[1]<=address-0x400000<s[1]+s[2]);assert address-0x400000-retail_section[1]>=retail_section[4] and address+4-0x400000<=retail_section[1]+retail_section[2]
    compile('00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp');compile('00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp')
    compile('00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp')
    retail=oracle_variable_allocator_constructor.collect();fixture=(ROOT/'rebuild/tests/integration/VariableAllocatorConstructor_test.cpp').read_text(encoding='utf-8')
    def array(name,rows,width):return 'static unsigned long '+name+'['+str(len(rows))+']['+str(width)+']={'+','.join('{'+','.join(str(v&0xffffffff)+'UL'for v in row)+'}'for row in rows)+'};\n'
    def bytes_array(name,rows,width):return 'static const unsigned char '+name+'['+str(len(rows))+']['+str(width)+']={'+','.join('{'+','.join(str(v)for v in bytes.fromhex(row))+'}'for row in rows)+'};\n'
    table=array('arguments',oracle_variable_allocator_constructor.CASES,6)+bytes_array('expected',[r['bytes']for r in retail],0xc38)+bytes_array('expectedChunk',[r['chunk']for r in retail],0x34)+array('expectedGlobals',[struct.unpack('<2I',bytes.fromhex(r['globals']))for r in retail],2)
    for name,values in [('expectedCalls',[len(r['events'])for r in retail]),('expectedKind',[r['events'][0]['kind']if r['events']else 0 for r in retail]),('expectedMemory',[r['events'][0]['args'][0]if r['events']and r['events'][0]['kind']==2 else 0 for r in retail]),('expectedSize',[r['events'][0]['args'][-1]if r['events']else 0 for r in retail])]:table+='static const unsigned long '+name+'[40]={'+','.join(str(v)+'UL'for v in values)+'};\n'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',table),encoding='utf-8');testobj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(testobj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(5):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'VARIABLE_ALLOCATOR_CTOR_OK cases=40'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/variable-allocator-constructor.json').write_text(json.dumps(dict(address='00a65930',grade='RELOCATION_MATCH',bytes=138,zero_filled_data_bytes=8,linked_cases=40,retail_cases=40,vtable_slots=16,object_size=0xc38,chunk_size=0x34,area_size=0x20,omitted_provider_checks=5,mocked_boundaries='NewChunk/NewChunkDemandPaged record constructor calls; unrecovered virtuals/destructor abort if invoked; no heap/chunk creation claimed',retail_results=retail),indent=2)+'\n',encoding='utf-8')
    print('VARIABLE_ALLOCATOR_CONSTRUCTOR_PASS bytes=138 data=8 cases=40 slots=16 omitted=5')

if __name__=='__main__':main()
