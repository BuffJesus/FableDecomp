"""Exact native fixed chunk-list methods and complete retail state replay."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_fixed_chunk_lists as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorFixedSize_ChunkLists_00a65e70.cpp'
def main():
    work=Path(tempfile.mkdtemp(prefix='fixed-chunk-lists-'));env=check_one.env();objects=[]
    for path in ['rebuild/src/compiled/'+SOURCE,'rebuild/tests/integration/FixedChunkLists_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    reports=[check(objects[0],'?'+name+'@CMemoryAllocatorFixedSize@@IAEXPAVCMemoryAreaChunk@1@@Z',address,length,0,{})for name,address,length in [('RemoveChunkFromList',0xa65e70,69),('InsertChunkAtListHead',0xa65ec0,58),('InsertChunkAtListTail',0xa65f00,58)]]
    assert all(r['grade']=='MATCH'for r in reports)
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<2I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FIXED_CHUNK_LISTS_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/fixed-chunk-lists.json').write_text(json.dumps(dict(providers=reports,cases=len(rows),omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='empty/singleton/three/seven nodes; head/middle/tail removal; head/tail insertion into empty/nonempty chains; forward/reversed order; head/tail and reciprocal links independently predicted; detached links deliberately preserved on removal; current allocator chunk/counters/unrelated storage unchanged',boundary='none; actual native list mutations'),indent=2)+'\n',encoding='utf-8')
    print(f'FIXED_CHUNK_LISTS_PASS bytes=185 cases={len(rows)} omitted=1')
if __name__=='__main__':main()
