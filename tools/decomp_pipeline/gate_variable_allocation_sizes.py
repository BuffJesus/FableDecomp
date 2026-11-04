"""Complete retail parity and linked state/return validation for count queries."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_allocation_sizes as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_AllocationSizes_00a651b0.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='variable-allocation-sizes-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','rebuild/tests/integration/VariableAllocationSizes_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8}
    reports=[check(objects[0],'?'+name+'@CMemoryAllocatorVariableSize@@UAEKPAX@Z',address,length,2,targets)for name,address,length in [('GetAllocationSize',0xa651b0,44),('GetAllocationSizeIncludingHeader',0xa651e0,46)]]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<5I',*row['args']));data.extend(row['initial'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'VARIABLE_ALLOCATION_SIZES_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-allocation-sizes.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='data pointer conversion using aligned32-byte header; six alignments; three header positions; zero/ordinary/signed-boundary/wrapping data sizes; optional header addition with unsigned return wrap; stale alignment globals replaced; complete storage unchanged',boundary='none; native raw-storage pointer queries; null is outside retail query precondition'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_ALLOCATION_SIZES_PASS bytes=90 cases={len(rows)} omitted=2')
if __name__=='__main__':main()
