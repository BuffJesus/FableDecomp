"""Retail byte/relocation and linked behavior gate for native chunk freeing."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_chunk_free as oracle
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SPLIT='00/a6/VariableAllocatorChunk_FreeArea_00a658e0.cpp'
REMOVE='00/a6/VariableAllocatorChunk_RemoveAreaFromFreeList_00a65060.cpp'
INSERT='00/a6/VariableAllocatorChunk_InsertAreaIntoFreeList_00a650f0.cpp'
METHODS=[(SPLIT,'?FreeArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z',0xa658e0,79,2),
 (SPLIT,'?ConsolidateAdjacentFreeAreas@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z',0xa65590,248,12),
 (SPLIT,'?RemoveAreaFromAllocatedList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z',0xa64ce0,55,0),
 (REMOVE,'?RemoveAreaFromFreeList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z',0xa65060,138,1),
 (INSERT,'?InsertAreaIntoFreeList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z',0xa650f0,188,1)]
TARGETS={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8,
 '?ConsolidateAdjacentFreeAreas@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z':0xa65590,
 '?RemoveAreaFromFreeList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z':0xa65060,
 '?InsertAreaIntoFreeList@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@2@@Z':0xa650f0,
 '?RemoveFromTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z':0xa64ef0,
 '?InsertIntoTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@0@Z':0xa64e70}

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-chunk-free-'));env=check_one.env();objects=[];coffs={};reports=[]
    def compile(path):
        obj=work/(path.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,work)
        assert r.returncode==0,r.stdout+r.stderr;objects.append(obj);return Coff(obj)
    for source in [SPLIT,REMOVE,INSERT]:coffs[source]=compile(ROOT/'rebuild/src/compiled'/source)
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for source,symbol,address,length,count in METHODS:
        c=coffs[source];section,offset=c.symbols[symbol];body,relocs=c.sections[section];assert offset==0 and len(relocs)==count
        patched=bytearray(body)
        for p,(name,kind)in relocs.items():
            assert name in TARGETS and kind in [6,20];value=TARGETS[name]+struct.unpack_from('<I',body,p)[0]
            if kind==20:value-=address+p+4
            struct.pack_into('<I',patched,p,value&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==length and patched==raw[at:at+length],source
        reports.append(dict(address=f'{address:08x}',bytes=length,relocations=count,grade='MATCH',source=source))
    compile(ROOT/'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp')
    compile(ROOT/'rebuild/tests/integration/VariableChunkFree_test.cpp')
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for r in rows:
        data.extend(struct.pack('<4I',r['mode'],r['alignment'],r['area'],len(r['events'])))
        data.extend(r['initial']);data.extend(r['after'])
        for address,args,state in r['events']:
            data.extend(struct.pack('<4I',address,oracle.BASE,*((args+(0,))[:2])));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(4):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True)
        assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    assert r.returncode==0 and f'VARIABLE_CHUNK_FREE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-chunk-free.json').write_text(json.dumps(dict(providers=reports,linked_cases=len(rows),retail_cases=len(rows),native_sources=4,omitted_provider_checks=4,compared_bytes_per_snapshot=oracle.WIDTH,boundary='Tree insertion/removal record their arguments and complete pre-call state; tree mutation not claimed',invariants='maximal contiguous free run containing target; retained lowest address/ID; merged data includes absorbed headers; exact free/allocated list counts, links, extrema and byte totals; allocated unlink clears its list links'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_CHUNK_FREE_PASS bytes=708 cases={len(rows)} omitted=4')

if __name__=='__main__':main()
