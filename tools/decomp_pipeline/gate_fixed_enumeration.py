"""Exact fixed-area enumeration and native/recorded virtual-count state replay."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_enumeration as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorFixedSize_Enumeration_00a660a0.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='fixed-enumeration-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetNoAllocatedAreas_00a65e50.cpp','rebuild/tests/integration/FixedEnumeration_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    reports=[check(objects[0],'?'+name+'@CMemoryAllocatorFixedSize@@'+suffix,address,length,0,{})for name,suffix,address,length in [('GetAllAllocatedAreas','UAEKPAPAXK@Z',0xa660a0,89),('GetAllocatedArea','UAEPAXJ@Z',0xa66100,90)]]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<6I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FIXED_ENUMERATION_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-enumeration.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),native_count_cases=sum(row['args'][0]==0 and row['args'][1]==0 for row in rows),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='empty/one/three/seven chunks; empty/single id254/sparse forward-backward ID chains; ff sentinel; stride32/64; reversed chunks; bulk max ignored including zero; unconditional virtual count call and ignored return; real native count with wrapping unrelated metadata; callback changes head/stride before traversal; every valid index and negative/out-of-range misses; full call/final states and output buffer writes',boundary='ABI-compatible count thunk invokes actual recovered count or records seam; one vtable binding validated before pointer normalization; caller output buffer holds all entries'),indent=2)+'\n',encoding='utf-8')
    print(f'FIXED_ENUMERATION_PASS bytes=179 cases={len(rows)} native_counts={sum(row["args"][0]==0 and row["args"][1]==0 for row in rows)} omitted=2')
if __name__=='__main__':main()
