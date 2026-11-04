"""Complete retail parity and linked state/return validation for count queries."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_counts as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_Counts_00a64530.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='variable-counts-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','rebuild/tests/integration/VariableCounts_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8}
    reports=[check(objects[0],'?'+name+'@CMemoryAllocatorVariableSize@@UAEKXZ',address,length,2,targets)for name,address,length in [('GetNoFreeAreas',0xa64530,48),('GetNoAllocatedAreas',0xa64560,43)]]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<4I',*row['args']));data.extend(row['initial'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'VARIABLE_COUNTS_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-counts.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='free metadata sum with unsigned wrap; allocated query preserves retail zero despite nonzero metadata; empty/one/three/seven chunks; four alignments replacing stale globals; full storage unchanged',boundary='none; raw-storage native count queries, no complete heap consistency claim'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_COUNTS_PASS bytes=91 cases={len(rows)} omitted=2')
if __name__=='__main__':main()
