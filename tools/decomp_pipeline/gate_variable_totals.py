"""Complete retail parity and linked state/return validation for count queries."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_totals as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_Totals_00a64db0.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='variable-totals-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','rebuild/tests/integration/VariableTotals_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8}
    reports=[check(objects[0],'?'+name+'@CMemoryAllocatorVariableSize@@UAEKXZ',address,length,2,targets)for name,address,length in [('GetTotalMemoryAllocated',0xa64db0,100),('GetHeaderOverhead',0xa64e20,74),('GetTotalMemoryControlledByAllocator',0xa65210,79)]]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<4I',*row['args']));data.extend(row['initial'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'VARIABLE_TOTALS_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-totals.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='independent allocated-list data sum plus metadata-based area/chunk headers; controlled total uses TotalDataSize plus one area/chunk header per chunk, ignoring allocation metadata and ExtraMemUsedByOS; aligned32/52-byte headers at six alignments; unsigned return wrap/signed data boundaries; metadata deliberately differs from actual list length; zero/one/three/seven chunks and allocated areas; stale global replacement; full storage unchanged',boundary='none; native raw-storage statistics, no complete heap consistency claim'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_TOTALS_PASS bytes=253 cases={len(rows)} omitted=2')
if __name__=='__main__':main()
