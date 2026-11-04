"""Byte-exact virtual deallocation and chunk destruction with real native lists."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_deallocation as oracle
from gate_variable_chunk_free import METHODS as FREE_METHODS,TARGETS as FREE_TARGETS
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
DEALLOC='00/a6/CMemoryAllocatorVariableSize_Deallocate_00a65a50.cpp'
DELETE='00/a6/CMemoryAllocatorVariableSize_DeleteChunk_00a65390.cpp'
METHODS=[(DEALLOC,'?Deallocate@CMemoryAllocatorVariableSize@@UAEXPAX@Z',0xa65a50,78,4),
 (DELETE,'?DeleteChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryAreaChunk@1@@Z',0xa65390,126,3)]+FREE_METHODS
TARGETS=dict(FREE_TARGETS,**{METHODS[1][1]:0xa65390,
 '?FreeArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z':0xa658e0,
 '__imp__GlobalMemoryStatus@4':0x143fe20,'__imp__VirtualFree@12':0x143fd40})

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-deallocation-'));env=check_one.env();objects=[];coffs={};by_source={};reports=[]
    def compile(path):
        obj=work/(path.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,work)
        assert r.returncode==0,r.stdout+r.stderr;objects.append(obj);return Coff(obj)
    for source in dict.fromkeys(row[0]for row in METHODS):
        coffs[source]=compile(ROOT/'rebuild/src/compiled'/source);by_source[source]=objects[-1]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for source,symbol,address,length,count in METHODS:
        c=coffs[source];section,offset=c.symbols[symbol];body,relocs=c.sections[section];assert offset==0 and len(relocs)==count;patched=bytearray(body)
        for p,(target,kind)in relocs.items():
            assert target in TARGETS and kind in [6,20];v=TARGETS[target]+struct.unpack_from('<I',body,p)[0]
            if kind==20:v-=address+p+4
            struct.pack_into('<I',patched,p,v&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==length and patched==raw[at:at+length],source
        reports.append(dict(address=f'{address:08x}',bytes=length,relocations=count,grade='MATCH',source=source))
    # Recording import slots are redirected only after original retail relocation validation.
    obj=by_source[DELETE];blob=obj.read_bytes()
    for original,replacement in [('__imp__GlobalMemoryStatus@4','_StatusSlot'),('__imp__VirtualFree@12','_FreeSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1 and len(value)<=len(needle)
        blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    obj.write_bytes(blob)
    compile(ROOT/'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp')
    compile(ROOT/'rebuild/tests/integration/VariableDeallocation_test.cpp')
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<8I',*row['args'],len(row['events']),*row['globals']));data.extend(row['initial']);data.extend(row['after'])
        for kind,a,b,c,state in row['events']:data.extend(struct.pack('<4I',kind,a,b,c));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(objects)-1):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    assert r.returncode==0 and f'VARIABLE_DEALLOCATION_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-deallocation.json').write_text(json.dumps(dict(providers=reports,linked_cases=len(rows),retail_cases=len(rows),native_sources=len(objects)-1,omitted_provider_checks=len(objects)-1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='data pointer conversion/current alignment; real unlink/free/coalescing; destroy only last nonpermanent allocation; free-area tree-removal order; head/middle/tail/singleton chunk unlink; two status calls sharing a buffer; release only nondemand-paged storage; status mutation of DemandPaged reloaded; VirtualFree success/failure ignored',boundary='Tree insertion/removal and GlobalMemoryStatus/VirtualFree record arguments and complete pre-call state; real native list/coalescing methods linked; no real OS memory release claimed'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_DEALLOCATION_PASS bytes=912 cases={len(rows)} native_sources={len(objects)-1} omitted={len(objects)-1}')

if __name__=='__main__':main()
