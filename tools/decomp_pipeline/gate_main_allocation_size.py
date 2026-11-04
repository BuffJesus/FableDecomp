"""Candidate-required main size-query ABI, callback and lock-depth replay."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_allocation_size as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp');args=parser.parse_args();work=Path(tempfile.mkdtemp(prefix='main-allocation-size-'));env=check_one.env();objects=[]
    for path in [args.source,'rebuild/tests/integration/MainAllocationSize_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    report=check(objects[0],'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z',0x9f2390,110,7,{'?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A':0x13caa70,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10,'?g_FableAllocatorRangeStart_013CAAA4@@3KA':0x13caaa4,'?g_FableAllocatorRangeEnd_013CAAA8@@3KA':0x13caaa8,'?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A':0x1396868});assert report['grade']=='MATCH'
    blob=objects[0].read_bytes()
    for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[0].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<7I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_ALLOCATION_SIZE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-allocation-size.json').write_text(json.dumps(dict(provider=report,source=args.source,cases=len(rows),valid_cases=sum(row['valid']for row in rows),lock_leak_cases=sum(row['args'][5]!=row['args'][1]for row in rows),omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='independent full owner/storage/table/range predictions; null/header/rangeID0/1/29/255/below-start/at-end/reversed-range; byte IDs0/29/30/255 and allocator count0/1/30; exact return0/ffffffff; nested lock depth0/1; Enter callback changes count/header/range/table target; slot3 callback changes owner/header/range/table; argument original pointer-4, this/order/prestates; original null/invalid early returns leak lock; one validated native vtable function address normalized',boundary='critical-section imports and slot3 size-class method recorded; no actual OS locking or size-class implementation claim in this focused gate'),indent=2)+'\n',encoding='utf-8')
    print(f'MAIN_ALLOCATION_SIZE_PASS bytes=110 cases={len(rows)} valid={sum(row["valid"]for row in rows)} leaked={sum(row["args"][5]!=row["args"][1]for row in rows)} omitted=1')
if __name__=='__main__':main()
