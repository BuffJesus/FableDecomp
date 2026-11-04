"""Match native class allocation/release and exercise real Win32 pages."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
NAMES=['00/a6/CMemoryAllocatorBase_OperatorNew_00a66730.cpp','00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp']

def main():
    work=Path(tempfile.mkdtemp(prefix='allocator-virtual-memory-'));env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[];reports=[];raw=EXE.read_bytes();sections=pe_sections(raw)
    for source,symbol,address,size,target in zip(NAMES,['??2CMemoryAllocatorBase@@SIPAXI@Z','??3CMemoryAllocatorBase@@SIXPAX@Z'],[0xa66730,0xa66750],[17,15],[0x143fd44,0x143fd40]):
        obj=work/(Path(source).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work);assert result.returncode==0,result.stdout+result.stderr
        coff=Coff(obj);section,offset=coff.symbols[symbol];body,relocs=coff.sections[section];patched=bytearray(body);assert len(relocs)==1
        for position,(name,kind)in relocs.items():
            assert name==('__imp__VirtualAlloc@16'if address==0xa66730 else '__imp__VirtualFree@12')and kind==6
            struct.pack_into('<I',patched,position,target+struct.unpack_from('<I',body,position)[0])
        at=va_to_off(sections,address);assert len(body)==size and bytes(patched)==raw[at:at+size]
        objects.append(obj);reports.append(dict(address=f'{address:08x}',bytes=size,grade='RELOCATION_MATCH',source=source))
    test=ROOT/'rebuild/tests/00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750_test.cpp';obj=work/'fixture.obj';result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(test)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(2):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert result.returncode==0 and 'ALLOCATOR_BASE_DELETE_OK cases=4'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/allocator-virtual-memory.json').write_text(json.dumps(dict(providers=reports,linked_cases=4,omitted_provider_checks=2,mocked_boundaries='none; real VirtualAlloc/VirtualFree/VirtualQuery',behavior=result.stdout.strip()),indent=2)+'\n',encoding='utf-8')
    print('ALLOCATOR_VIRTUAL_MEMORY_PASS cases=4 bytes=32 omitted=2')

if __name__=='__main__':main()
