"""Direct near-match deletion fixup, captured at real retail call boundaries."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_tree_deletion as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
SOURCE='00/a6/CMemoryAllocatorVariableSize_RemoveFromTreeFixup_00a64910.cpp'

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-tree-fixup-'));env=check_one.env();objects=[]
    for source in [ROOT/'rebuild/src/compiled'/SOURCE,ROOT/'rebuild/tests/integration/VariableTreeFixup_test.cpp']:
        obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work)
        assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    right='?RotateRight@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z'
    parity=check(objects[0],'?RemoveFromTreeFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@PBVCDeletedNodeChildInfo@@@Z',0xa64910,757,1,{right:0xa646e0})
    rotation=check(objects[0],right,0xa646e0,84,0,{})
    assert rotation['grade']=='MATCH'
    rows=oracle.collect_fixups();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:data.extend(struct.pack('<2I',row['area'],row['info']));data.extend(row['before']);data.extend(row['after'])
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0,'fixup omission linked'
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    assert r.returncode==0 and f'VARIABLE_TREE_FIXUP_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-tree-fixup.json').write_text(json.dumps(dict(parity=parity,rotation=rotation,cases=len(rows),direct_guard_cases=24,omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,invariants='retail deletion sequences independently validate membership, signed size/address order, root black, parent links, black depth and no red-red/cycles; direct fixup captures compare full states; descriptor copied unchanged from retail caller stack; nullable/inactive descriptor guards preserve storage except final blackening',boundary='none inside fixup; real rotations; no RemoveFromTree reconstruction claim'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_TREE_FIXUP_PASS grade={parity["grade"]} similarity={parity["opcode_similarity"]:.6f} cases={len(rows)} omitted=1')

if __name__=='__main__':main()
