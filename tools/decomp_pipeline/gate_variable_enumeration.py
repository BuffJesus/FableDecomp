"""Exact parity and candidate-required native allocator enumeration replay."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_enumeration as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_Enumeration_00a65260.cpp'

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-enumeration-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','rebuild/tests/integration/VariableEnumeration_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8}
    reports=[check(objects[0],'?'+name+'@CMemoryAllocatorVariableSize@@'+suffix,address,length,relocs,targets)for name,suffix,address,length,relocs in [('GetAllAllocatedAreas','UAEKPAPAXK@Z',0xa65260,108,4),('GetAllocatedArea','UAEPAXJ@Z',0xa652d0,96,2)]]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<5I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'VARIABLE_ENUMERATION_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-enumeration.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),bulk_cases=sum(row['args'][0]==0 for row in rows),index_cases=sum(row['args'][0]==1 for row in rows),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='six alignments; empty/one/three/seven chunks; empty/single/mixed allocated lists; reversed chunk/list order; unrelated cached count and signed-boundary sizes; max ignored including zero; every valid global index and negative/out-of-range misses; exact output-buffer writes and untouched surrounding storage; stale globals replaced',boundary='none; output buffer has capacity for all list entries, as required by retail despite ignored max'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_ENUMERATION_PASS bytes=204 cases={len(rows)} omitted=2')
if __name__=='__main__':main()
