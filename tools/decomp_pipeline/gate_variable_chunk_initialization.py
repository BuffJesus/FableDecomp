"""Match demand-paged chunk, Init and free-list insertion with composed retail state."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_chunk_initialization as oracle
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
PROVIDERS=[
    ('00/a6/CMemoryAllocatorVariableSize_NewChunkDemandPaged_00a65830.cpp','?NewChunkDemandPaged@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@PAXJ@Z',0xa65830,108,5),
    ('00/a6/VariableAllocatorChunk_Init_00a65410.cpp','?Init@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAXJPAV2@@Z',0xa65410,110,3),
    ('00/a6/VariableAllocatorChunk_InsertAreaIntoFreeList_00a650f0.cpp','?InsertAreaIntoFreeList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z',0xa650f0,188,1)]
TARGETS={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,
         '?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8,
         '?Init@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAXJPAV2@@Z':0xa65410,
         '?InsertAreaIntoFreeList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z':0xa650f0,
         '?InsertIntoTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@0@Z':0xa64e70}

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-chunk-'));env=check_one.env();objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    def compile(source):
        obj=work/(source.stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert result.returncode==0,result.stdout+result.stderr;objects.append(obj);return Coff(obj)
    for source,symbol,address,length,relocation_count in PROVIDERS:
        coff=compile(ROOT/'rebuild/src/compiled'/source);section,offset=coff.symbols[symbol]
        body,relocs=coff.sections[section];assert offset==0 and len(relocs)==relocation_count;patched=bytearray(body)
        for position,(name,kind)in relocs.items():
            assert name in TARGETS and kind in [6,20],(name,kind)
            value=TARGETS[name]+struct.unpack_from('<I',body,position)[0]
            if kind==20:value-=address+position+4
            struct.pack_into('<I',patched,position,value&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==length and bytes(patched)==raw[at:at+length],source
        reports.append(dict(address=f'{address:08x}',bytes=length,relocations=relocation_count,grade='RELOCATION_MATCH',source=source))
    compile(ROOT/'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp')
    compile(ROOT/'rebuild/tests/integration/VariableChunkInitialization_test.cpp')
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<5I',*(v&0xffffffff for v in row['args'])))
        data.extend(struct.pack('<3I',*(row['event'][key]for key in ['owner','root','area'])))
        data.extend(bytes.fromhex(row['event']['before']));data.extend(bytes.fromhex(row['after']))
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(4):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True)
        assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'VARIABLE_CHUNK_INITIALIZATION_OK cases=116'in result.stdout,(result.returncode,result.stdout,result.stderr)
    (ROOT/'rebuild/compile-gate/variable-chunk-initialization.json').write_text(json.dumps(dict(providers=reports,linked_cases=len(rows),retail_cases=len(rows),native_sources=4,omitted_provider_checks=4,compared_bytes_per_snapshot=oracle.WIDTH,boundary='InsertIntoTree records owner/root/area and full memory state; tree mutations and OS chunk acquisition not claimed',scenarios=[dict(args=row['args'])for row in rows]),indent=2)+'\n',encoding='utf-8')
    print('VARIABLE_CHUNK_INITIALIZATION_PASS bytes=406 cases=116 native_sources=4 omitted=4')

if __name__=='__main__':main()
