"""Candidate-required native aligned API replay against independent retail states."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_aligned as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_Aligned_009f2be0.cpp');args=parser.parse_args();work=Path(tempfile.mkdtemp(prefix='main-aligned-'));env=check_one.env();objects=[]
    for path in [args.source,'rebuild/tests/integration/MainAligned_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?Create@CMemoryAllocator@@KIXXZ':0x9f2870,'?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z':0x9f2a40,'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,'?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A':0x13caa70,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10,'?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,'??1CMemoryAllocator@@IAE@XZ':0x9f2310,'__imp__VirtualFree@12':0x143fd40}
    reports=[check(objects[0],'?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z',0x9f2be0,79,8,targets),check(objects[0],'?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z',0x9f2b20,108,11,targets)]
    assert all(r['grade']=='MATCH'for r in reports)
    blob=objects[0].read_bytes()
    for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot'),('__imp__VirtualFree@12','_FreeSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[0].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<11I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0,'provider omitted'
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_ALIGNED_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-aligned.json').write_text(json.dumps(dict(providers=reports,source=args.source,cases=len(rows),allocation_cases=sum(row['args'][0]==0 for row in rows),release_cases=sum(row['args'][0]==1 for row in rows),omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,boundary='Create/internal allocation/free/destructor and Win32 lock/free imports recorded; native wrappers execute; actual OS composition pending',invariants='complete independent retail call/final snapshots; wrapped size/alignment arithmetic; creation, owner switches, captured original prefix before lock; aligned prefix write after unlock; null release, counter underflow, fresh singleton deferred deletion, cached deletion owner, ignored VirtualFree result'),indent=2)+'\n',encoding='utf-8')
    print(f'MAIN_ALIGNED_PASS bytes=187 cases={len(rows)} omitted=1')
if __name__=='__main__':main()
