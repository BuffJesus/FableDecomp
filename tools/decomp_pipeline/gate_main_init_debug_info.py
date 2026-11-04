"""Candidate-required main metadata initialization composed with native size query."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_init_debug_info as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp');args=parser.parse_args();work=Path(tempfile.mkdtemp(prefix='main-init-debug-'));env=check_one.env();objects=[]
    for path in [args.source,'rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/tests/integration/MainInitDebugInfo_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    report=check(objects[0],'?InitDebugInfo@CMemoryAllocator@@IAEXPAX_NJ@Z',0x9f28d0,77,2,{'?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z':0x9f2390});assert report['grade']=='MATCH'
    blob=objects[1].read_bytes()
    for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot')]:
        needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
    objects[1].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<8I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(2):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_INIT_DEBUG_INFO_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-init-debug-info.json').write_text(json.dumps(dict(provider=report,source=args.source,cases=len(rows),size_queries=len(rows)*2,omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='real GetAllocationSize called twice, first return ignored0/32/ffffffff, second20/24/64/256 determines trailer; full prestates/final states and untouched payload; requested0/1/7/max/min/negative bits, wrapping per-ID counter; leading/trailing ffffffff guards, trailing current trackingID and requested size; debug false/true does not change metadata; callback switches table context for second query and tracking ID255 then7; current tracking reloaded after second query; balanced recorded lock calls; validated/normalized test vtable pointer',boundary='recorded critical-section imports and size-class slot3; main size query is genuine native provider; actual OS metadata composition pending'),indent=2)+'\n',encoding='utf-8')
    print(f'MAIN_INIT_DEBUG_INFO_PASS bytes=77 cases={len(rows)} size_queries={len(rows)*2} omitted=2')
if __name__=='__main__':main()
