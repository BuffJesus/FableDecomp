"""Fixed chunk creation/deletion with actual Windows process-heap release."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    work=Path(tempfile.mkdtemp(prefix='fixed-chunk-os-'));env=check_one.env();objects=[]
    sources=['rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLifetime_00a66160.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ChunkLists_00a65e70.cpp','rebuild/tests/integration/FixedChunkOS_test.cpp']
    for source in sources:
        path=ROOT/source;obj=work/(path.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    parity=check(objects[0],'?DeleteChunk@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z',0xa66160,105,2,{'__imp__GetProcessHeap@0':0x143fd3c,'__imp__HeapFree@12':0x143fe8c})
    blob=objects[0].read_bytes()
    for original,replacement in [('__imp__GetProcessHeap@0','_GetHeapSlot'),('__imp__HeapFree@12','_HeapFreeSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[0].write_bytes(blob);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'FIXED_CHUNK_OS_OK cases=84 actual_releases=84'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-chunk-os.json').write_text(json.dumps(dict(grade='NATIVE_PROCESS_HEAP_PASS',release_parity=parity,cases=84,actual_releases=84,omitted_provider_checks=2,invariants='real HeapAlloc/GetProcessHeap/HeapFree and whole-heap HeapValidate; alignment4/8/16,stride16/64,counts0/1/3/254/255/256/257; independently predicted complete heap-block initialization and untouched payload; original raw pointer release with flags1, success required; current reset only if equal and signed empty counter unchanged; actual native head insertion/unlink',boundary='thin test wrappers validate arguments then invoke actual Windows APIs; raw allocator context, no full fixed allocator constructor/Allocate/Deallocate or failure-path claim'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print('FIXED_CHUNK_OS_PASS cases=84 omitted=2')
if __name__=='__main__':main()
