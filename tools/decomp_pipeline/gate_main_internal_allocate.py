"""Candidate-required main allocation dispatch with real metadata and size query."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_internal_allocate as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalAllocate_009f2a40.cpp');args=parser.parse_args();work=Path(tempfile.mkdtemp(prefix='main-internal-allocate-'));env=check_one.env();objects=[]
    for path in [args.source,'rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/tests/integration/MainInternalAllocate_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    report=check(objects[0],'?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z',0x9f2a40,122,3,{'?InitDebugInfo@CMemoryAllocator@@IAEXPAX_NJ@Z':0x9f28d0,'?g_FableAllocatorRangeStart_013CAAA4@@3KA':0x13caaa4,'?g_FableAllocatorRangeEnd_013CAAA8@@3KA':0x13caaa8});assert report['grade']=='MATCH'
    blob=objects[2].read_bytes()
    for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[2].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<11I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(3):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_INTERNAL_ALLOCATE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-internal-allocate.json').write_text(json.dumps(dict(provider=report,source=args.source,cases=len(rows),success_cases=sum(not row['args'][3]for row in rows),null_cases=sum(bool(row['args'][3])for row in rows),omitted_provider_checks=3,compared_bytes_per_snapshot=oracle.WIDTH,invariants='signed clamp0/negative/min, ordinary lookup boundaries and signed-max addition; categories0/9 and max0/32/64; true/false bool; null return and successful prefix/trailer metadata/per-ID counters; cached selected lookup entry and postcallback ID/range reads; callback changes category lookup/max/counter/tracking/ID/range/table context; real main InitDebugInfo/GetAllocationSize, two virtual size queries and balanced recorded locks; counters ffffffff->0 or callback7->8; special range skips byteID write; complete independently predicted call/final states, two validated vtable pointer normalizations',boundary='class slot0/slot3 and lock imports recorded; large size cases do not claim actual OS allocation; real main metadata and size-query providers execute; actual main allocation/free OS composition pending'),indent=2)+'\n',encoding='utf-8')
    print(f'MAIN_INTERNAL_ALLOCATE_PASS bytes=122 cases={len(rows)} omitted=3')
if __name__=='__main__':main()
