"""Near-match native deletion/fixup with full retail tree mutation sequences."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_tree_deletion as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
REMOVE='00/a6/CMemoryAllocatorVariableSize_RemoveFromTree_00a64ef0.cpp'
FIXUP='00/a6/CMemoryAllocatorVariableSize_RemoveFromTreeFixup_00a64910.cpp'

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-tree-deletion-'));env=check_one.env();objects=[]
    for source in [ROOT/'rebuild/src/compiled'/REMOVE,ROOT/'rebuild/src/compiled'/FIXUP,ROOT/'rebuild/tests/integration/VariableTreeDeletion_test.cpp']:
        obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    fixup='?RemoveFromTreeFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@PBVCDeletedNodeChildInfo@@@Z'
    right='?RotateRight@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z'
    reports=[check(objects[0],'?RemoveFromTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z',0xa64ef0,305,1,{fixup:0xa64910}),
             check(objects[1],fixup,0xa64910,757,1,{right:0xa646e0})]
    rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<I',len(row['operations'])));data.extend(row['initial'])
        for area,state in zip(row['operations'],row['snapshots']):data.extend(struct.pack('<I',area));data.extend(state)
    guards=oracle.collect_guards();data.extend(struct.pack('<I',len(guards)))
    for row in guards:data.extend(struct.pack('<2I',row['area'],row['info']));data.extend(row['before']);data.extend(row['after'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in [0,1]:
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    operations=sum(len(row['operations'])for row in rows)
    assert r.returncode==0 and f'VARIABLE_TREE_DELETION_OK sequences={len(rows)} operations={operations} guards={len(guards)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-tree-deletion.json').write_text(json.dumps(dict(providers=reports,sequences=len(rows),operations=operations,direct_guard_cases=len(guards),omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,invariants='membership, signed(size,address) order, root black, parent links, black depth, no red-red/cycles; removed tree links cleared while size/list/ID remain; complete retail/linked snapshots after each delete; null/inactive descriptor guards',boundary='none; production deletion/fixup/rotations'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_TREE_DELETION_PASS sequences={len(rows)} operations={operations} guards={len(guards)} omitted=2')

if __name__=='__main__':main()
