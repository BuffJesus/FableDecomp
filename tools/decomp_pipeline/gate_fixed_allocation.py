"""Fixed allocator candidate-required full-state retail/native allocation/free sequences."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_allocation as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_Allocation_00a662a0.cpp');args=parser.parse_args()
    work=Path(tempfile.mkdtemp(prefix='fixed-allocation-'));env=check_one.env();objects=[]
    sources=[args.source,'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_NewChunk_00a65f40.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLists_00a65e70.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_OnFailedAllocation_009f1f00.cpp','rebuild/tests/integration/FixedAllocation_test.cpp']
    for path in sources:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?NewChunk@CMemoryAllocatorFixedSize@@IAEPAVCMemoryAreaChunk@1@XZ':0xa663d0,'?RemoveChunkFromList@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z':0xa65e70,'?InsertChunkAtListHead@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z':0xa65ec0,'?InsertChunkAtListTail@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z':0xa65f00,'?DeleteChunk@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z':0xa66160}
    reports=[check(objects[0],'?Allocate@CMemoryAllocatorFixedSize@@UAEPAXK@Z',0xa66450,250,3,targets),check(objects[0],'?Deallocate@CMemoryAllocatorFixedSize@@UAEXPAX@Z',0xa662a0,242,3,targets)]
    for obj in objects[1:3]:
        blob=obj.read_bytes()
        for original,replacement in [('__imp__GetProcessHeap@0','_GetHeapSlot'),('__imp__HeapAlloc@12','_HeapAllocSlot'),('__imp__HeapSize@12','_HeapSizeSlot'),('__imp__HeapFree@12','_HeapFreeSlot')]:
            needle=original.encode()+b'\0';value=replacement.encode()+b'\0'
            if needle in blob:assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
        obj.write_bytes(blob)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<8I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(objects)-1):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FIXED_ALLOCATION_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-allocation.json').write_text(json.dumps(dict(providers=reports,source=args.source,sequences=54,sequence_operations=1668,forced_failure_cases=18,signed_counter_cases=12,operations=len(rows),allocations=sum(row['args'][1]==0 for row in rows),heap_boundary_calls=sum(len(row['events'])for row in rows),omitted_provider_checks=len(objects)-1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='independent chunk/area membership model and predicted full storage; native state persists across each sequence; align4/8/16, stride16/64, area count1/3/7, initial0/1/3 forward/reversed chunks and null/head/tail current selection; two allocate/free cycles with forward/reverse/permuted frees; requested size ignored including0/ffffffff; full chunks move to tail, freed formerly-full chunks move to head; byte links and stale free-head previous IDs; signed empty counter negative/zero/positive/max/wrap boundaries, real DeleteChunk with no counter decrement; real NewChunk/CreateChunk/list providers; exact heap calls/order/prestates and raw-pointer releases; forced null HeapAlloc, zero HeapSize, unsigned64 overflow: absent head faults at address0x10 via native SEH; surviving full head returns null and remains current',boundary='heap imports recorded and native shipping failure handler linked; direct raw-context invalid alignment0 used only to force unsigned overflow for zero-slack cases; actual OS lifetime is separate gate_fixed_os_lifetime.py'),indent=2)+'\n',encoding='utf-8')
    print(f'FIXED_ALLOCATION_PASS sequences=54 operations={len(rows)} failure_cases=18 counter_cases=12 heap_calls={sum(len(row["events"])for row in rows)} omitted={len(objects)-1}')
if __name__=='__main__':main()
