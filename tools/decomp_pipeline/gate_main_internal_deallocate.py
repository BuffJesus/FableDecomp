"""Candidate-required main tracked free with real main size-query composition."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_internal_deallocate as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InternalDeallocate_009f2920.cpp');args=parser.parse_args();work=Path(tempfile.mkdtemp(prefix='main-internal-free-'));env=check_one.env();objects=[]
    for path in [args.source,'rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/tests/integration/MainInternalDeallocate_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    report=check(objects[0],'?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z',0x9f2920,155,5,{'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390,'?g_FableAllocatorRangeStart_013CAAA4@@3KA':0x13caaa4,'?g_FableAllocatorRangeEnd_013CAAA8@@3KA':0x13caaa8,'?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A':0x1396868});assert report['grade']=='MATCH'
    blob=objects[1].read_bytes()
    for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[1].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<6I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(2):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_INTERNAL_DEALLOCATE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-internal-deallocate.json').write_text(json.dumps(dict(provider=report,source=args.source,cases=len(rows),valid_cases=sum(row['args'][1]!=2 for row in rows),invalid_cases=sum(row['args'][1]==2 for row in rows),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='seeded from independently verified successful retail allocation metadata; header/special-range IDs; two genuine main size queries then cached allocator slot3; first0 ignored,second256 locates metadata,third0/1/4/7/64/256 controls exact CD fill; tracking subtraction from stored ID independent of current tracking99; first callback changes table context, third query still uses cached allocator and changes its vtable, subsequent slot1 reload invokes alternate free; free callback sets NoAllocations0 then original decrement wrapsffffffff; invalidID255 no calls or writes; complete call/final states and four validated test vtable cells normalized',boundary='recorded class slot3/slot1 and critical-section imports; main GetAllocationSize genuine; actual native allocation/free OS composition pending'),indent=2)+'\n',encoding='utf-8')
    print(f'MAIN_INTERNAL_DEALLOCATE_PASS bytes=155 cases={len(rows)} omitted=2')
if __name__=='__main__':main()
