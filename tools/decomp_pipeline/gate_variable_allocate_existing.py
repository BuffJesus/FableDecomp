"""Match native allocation dispatch and composed best-fit query with a recorded split."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_allocate_existing as oracle
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_AllocateMemoryFromExistingChunks_00a656e0.cpp'
QUERY='00/a6/CMemoryAllocatorVariableSize_TreeQueries_00a64c10.cpp'
METHODS=[(SOURCE,'?AllocateMemoryFromExistingChunks@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@1@K@Z',0xa656e0,59,2),
 (QUERY,'?GetTreeAreaWithSizeGreaterThanOrEqualTo@CMemoryAllocatorVariableSize@@IBEPAVCMemoryArea@1@J@Z',0xa64c10,55,0)]
TARGETS={METHODS[1][1]:0xa64c10,'?AllocAreaFromArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@2@PAV32@J@Z':0xa65480}

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-allocate-existing-'));env=check_one.env();objects=[];coffs={};reports=[]
    for source in [SOURCE,QUERY]:
        obj=work/(Path(source).stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work)
        assert r.returncode==0,r.stdout+r.stderr;objects.append(obj);coffs[source]=Coff(obj)
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for source,symbol,address,length,count in METHODS:
        c=coffs[source];section,offset=c.symbols[symbol];body,relocs=c.sections[section];assert offset==0 and len(relocs)==count;patched=bytearray(body)
        for p,(target,kind)in relocs.items():
            assert target in TARGETS and kind==20
            struct.pack_into('<I',patched,p,(TARGETS[target]+struct.unpack_from('<I',body,p)[0]-address-p-4)&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==length and patched==raw[at:at+length]
        reports.append(dict(address=f'{address:08x}',bytes=length,relocations=count,grade='MATCH',source=source))
    fixture=work/'fixture.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fixture),str(ROOT/'rebuild/tests/integration/VariableAllocateExisting_test.cpp')],env,work)
    assert r.returncode==0,r.stdout+r.stderr;objects.append(fixture)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<7I',row['request'],row['returned'],row['result'],row['calls'],*row['event']));data.extend(row['initial'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    assert r.returncode==0 and f'VARIABLE_ALLOCATE_EXISTING_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-allocate-existing.json').write_text(json.dumps(dict(providers=reports,linked_cases=len(rows),retail_cases=len(rows),native_sources=2,omitted_provider_checks=2,compared_unchanged_bytes=oracle.WIDTH,invariants='unsigned alignment wrap; signed minimum eligible size and selected-node membership; original parent chunk/area/aligned-size boundary arguments; exact null/non-null boundary return propagation; full owner/tree state unchanged',boundary='AllocAreaFromArea recorded; deletion and splitting not claimed by this dispatch gate'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_ALLOCATE_EXISTING_PASS bytes=114 cases={len(rows)} omitted=2')

if __name__=='__main__':main()
