"""Match base initialization, full layout and its fifteen-slot vtable."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
import oracle_allocator_base_constructor
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp'

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-base-ctor-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=work/'ctor.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/SOURCE)],env,work);assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);section,offset=coff.symbols['??0CMemoryAllocatorBase@@QAE@K@Z'];body,relocs=coff.sections[section];patched=bytearray(body)
    for position,(name,kind)in relocs.items():
        assert name=='??_7CMemoryAllocatorBase@@6B@'and kind==6,(name,kind)
        struct.pack_into('<I',patched,position,0x129dadc+struct.unpack_from('<I',body,position)[0])
    raw=EXE.read_bytes();sections=pe_sections(raw);at=va_to_off(sections,0xa65ad0);assert len(body)==113 and bytes(patched)==raw[at:at+113],(len(body),patched.hex(),raw[at:at+113].hex())
    expected=['__purecall']*15;expected[13]='?ConsolidateFreeAreas@CMemoryAllocatorBase@@UAEXXZ';coff.table('??_7CMemoryAllocatorBase@@6B@',expected)
    at=va_to_off(sections,0x129dadc);retail_table=list(struct.unpack('<15I',raw[at:at+60]));assert retail_table[13]==0xa65b50 and len(set(retail_table[:13]+retail_table[14:]))==1
    helper=work/'consolidate.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(helper),str(ROOT/'rebuild/src/compiled/00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp')],env,work);assert result.returncode==0,result.stdout+result.stderr
    retail=oracle_allocator_base_constructor.collect();fixture=(ROOT/'rebuild/tests/integration/AllocatorBaseConstructor_test.cpp').read_text(encoding='utf-8');table='static const unsigned char expected[20][0xc1c]={\n'+',\n'.join('{'+','.join(str(v)for v in bytes.fromhex(row['bytes']))+'}'for row in retail)+'\n};'
    test=work/'fixture.cpp';test.write_text(fixture.replace('// RETAIL_EXPECTATIONS',table),encoding='utf-8');testobj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(testobj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr
    objects=[obj,helper,testobj];exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omitted in range(2):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'ALLOCATOR_BASE_CTOR_OK cases=20'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/allocator-base-constructor.json').write_text(json.dumps(dict(address='00a65ad0',bytes=113,grade='RELOCATION_MATCH',linked_cases=20,retail_cases=20,omitted_provider_checks=2,vtable_slots=15,base_size=0xc1c,mocked_boundaries='none; derived fixture supplies abstract virtual implementations',retail_results=retail),indent=2)+'\n',encoding='utf-8')
    print('ALLOCATOR_BASE_CONSTRUCTOR_PASS bytes=113 cases=20 omitted=2')

if __name__=='__main__':main()
