"""Candidate-required retail/native heap acquisition and chunk creation replay."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_new_chunk as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_NewChunk_00a65f40.cpp');args=parser.parse_args()
    work=Path(tempfile.mkdtemp(prefix='fixed-new-chunk-'));env=check_one.env();objects=[]
    paths=[args.source,'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp','rebuild/tests/integration/FixedNewChunk_test.cpp']
    for path in paths:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    reports=[check(objects[0],'?InternalAlloc@CMemoryAllocatorFixedSize@@IAEKKPAPAX@Z',0xa65f40,100,3,{'__imp__GetProcessHeap@0':0x143fd3c,'__imp__HeapAlloc@12':0x143fe88,'__imp__HeapSize@12':0x143fe84}),check(objects[0],'?NewChunk@CMemoryAllocatorFixedSize@@IAEPAVCMemoryAreaChunk@1@XZ',0xa663d0,122,4,{'?InternalAlloc@CMemoryAllocatorFixedSize@@IAEKKPAPAX@Z':0xa65f40,'?CreateChunk@CMemoryAllocatorFixedSize@@IAEPAVCMemoryAreaChunk@1@PAX@Z':0xa661d0,'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?OnFailedAllocation@CMemoryAllocator@@QAEXXZ':0x9f1f00})]
    assert all(r['grade']=='MATCH'for r in reports)
    # Test-only IAT redirection, after parity of the original imports is established.
    blob=objects[0].read_bytes()
    for original,replacement in [('__imp__GetProcessHeap@0','_GetHeapSlot'),('__imp__HeapAlloc@12','_HeapAllocSlot'),('__imp__HeapSize@12','_HeapSizeSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[0].write_bytes(blob)
    # The lifetime object's unrelated DeleteChunk imports remain ordinary Win32 imports.
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<9I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(2):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FIXED_NEW_CHUNK_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    os_objects=[]
    for path in ['rebuild/tests/integration/FixedNewChunkOS_test.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_OnFailedAllocation_009f1f00.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;os_objects.append(obj)
    providers=objects[:2]+os_objects[1:];os_exe=work/'os.exe';os_link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(os_exe),'kernel32.lib']
    for omitted in range(len(providers)):
        r=subprocess.run(os_link+[str(p)for i,p in enumerate(providers)if i!=omitted]+[str(os_objects[0])],env=env,capture_output=True,text=True);assert r.returncode!=0,('OS provider omitted',omitted)
    r=subprocess.run(os_link+[str(p)for p in providers]+[str(os_objects[0])],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(os_exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'FIXED_NEW_CHUNK_OS_OK cases=42 allocations=84'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-new-chunk.json').write_text(json.dumps(dict(providers=reports,source=args.source,cases=len(rows),internal_cases=sum(row['args'][0]==0 for row in rows),new_chunk_cases=sum(row['args'][0]==1 for row in rows),os_cases=42,os_allocations=84,omitted_provider_checks=5,compared_bytes_per_snapshot=oracle.WIDTH,invariants='independently predicted overflow boundaries and output preservation; alignment slack; heap flags1 and exact requested bytes; null allocation and HeapSize0/ordinary/ffffffff; cached GetProcessHeap import; callback owner alignment/geometry/list/counter/singleton mutation; complete boundary/final snapshots; real recovered CreateChunk and head insertion; OS mode supported alignment4/8/16, count0/1/3/254/255/256/257, stride16/64, two-chunk lists and complete heap-block initialization/untouched payload; real DeleteChunk and heap validation',boundary='recorded mode mocks Win32 imports and failure handler; OS mode wrappers call actual GetProcessHeap/HeapAlloc/HeapSize, native shipping failure handler and DeleteChunk/HeapFree; raw allocator context, no Allocate/Deallocate integration claim'),indent=2)+'\n',encoding='utf-8')
    print(f'FIXED_NEW_CHUNK_PASS bytes=222 cases={len(rows)} os_cases=42 os_allocations=84 omitted=5')
if __name__=='__main__':main()
