"""Retail/native fixed chunk initialization/release and process-heap seam."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_chunk_lifetime as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='fixed-chunk-lifetime-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/tests/integration/FixedChunkLifetime_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    reports=[check(objects[0],'?DeleteChunk@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z',0xa66160,105,2,{'__imp__GetProcessHeap@0':0x143fd3c,'__imp__HeapFree@12':0x143fe8c}),check(objects[0],'?CreateChunk@CMemoryAllocatorFixedSize@@IAEPAVCMemoryAreaChunk@1@PAX@Z',0xa661d0,137,0,{})]
    # Redirect only the test object after checking the original import relocations.
    blob=objects[0].read_bytes()
    for original,replacement in [('__imp__GetProcessHeap@0','_GetHeapSlot'),('__imp__HeapFree@12','_HeapFreeSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[0].write_bytes(blob)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<7I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *args,state in row['events']:data.extend(struct.pack('<4I',*args));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FIXED_CHUNK_LIFETIME_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-chunk-lifetime.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),creation_cases=sum(row['args'][0]==0 for row in rows),release_cases=sum(row['args'][0]==1 for row in rows),omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='unaligned raw pointer aligned to owner alignment; raw pointer retained; zero/ordinary/254/255/256/257 area counts, byte-ID wrap and ff sentinel preserved; ascending initialization/prepended free-list headers, all untouched bytes preserved; deletion unlinks every position and clears current only if matching; empty counter preserved absent callback; process heap handle/flags1/original raw pointer captured; heap callback mutates stored pointer/current/empty counter after argument load; result ignored; complete call/final snapshots',boundary='GetProcessHeap/HeapFree recorded; all native chunk/list/area initialization code executes; no actual process-heap lifetime claim'),indent=2)+'\n',encoding='utf-8')
    print(f'FIXED_CHUNK_LIFETIME_PASS cases={len(rows)} creation={sum(row["args"][0]==0 for row in rows)} release={sum(row["args"][0]==1 for row in rows)} create_similarity={reports[1]["opcode_similarity"]:.6f} omitted=1')
if __name__=='__main__':main()
