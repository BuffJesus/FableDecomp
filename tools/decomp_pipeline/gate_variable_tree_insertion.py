"""Verify native rotations, fixup and insertion with full retail trees and invariants."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_tree_insertion as oracle
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['00/a6/CMemoryAllocatorVariableSize_InsertIntoTree_00a64e70.cpp','00/a6/CMemoryAllocatorVariableSize_InsertFixup_00a64740.cpp']
METHODS=[('InsertIntoTree',0xa64e70,122,3),('InsertFixup',0xa64740,464,0),('RotateLeft',0xa64680,84,0),('RotateRight',0xa646e0,84,0)]

def main():
    work=Path(tempfile.mkdtemp(prefix='variable-tree-'));env=check_one.env();objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    for source in SOURCES:
        obj=work/(Path(source).stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled'/source)],env,work)
        assert result.returncode==0,result.stdout+result.stderr;objects.append(obj);coff=Coff(obj)
        for method,address,length,count in METHODS:
            symbol='?'+method+'@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@'+('0@Z'if method=='InsertIntoTree'else'@Z')
            if symbol not in coff.symbols:continue
            section,offset=coff.symbols[symbol];body,relocs=coff.sections[section];assert offset==0 and len(relocs)==count;patched=bytearray(body)
            for position,(name,kind)in relocs.items():
                assert kind==20 and name=='?InsertFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z'
                value=0xa64740+struct.unpack_from('<I',body,position)[0]-address-position-4
                struct.pack_into('<I',patched,position,value&0xffffffff)
            at=va_to_off(sections,address);assert len(body)==length and bytes(patched)==raw[at:at+length],method
            reports.append(dict(address=f'{address:08x}',bytes=length,relocations=count,grade='RELOCATION_MATCH',source=source))
    assert len(reports)==4
    fixture=ROOT/'rebuild/tests/integration/VariableTreeInsertion_test.cpp';obj=work/'fixture.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(fixture)],env,work);assert result.returncode==0,result.stdout+result.stderr;objects.append(obj)
    rows=oracle.collect();operations=oracle.collect_operations();data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<2I',row['pattern'],len(row['sequence'])))
        data.extend(b''.join(struct.pack('<2I',index,value&0xffffffff)for index,value in row['sequence']))
        data.extend(b''.join(bytes.fromhex(snapshot)for snapshot in row['snapshots']))
    data.extend(struct.pack('<I',len(operations)))
    for row in operations:
        data.extend(struct.pack('<2I',row['mode'],row['argument']));data.extend(bytes.fromhex(row['before']));data.extend(bytes.fromhex(row['after']))
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(2):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'VARIABLE_TREE_INSERTION_OK sequences=26 snapshots=530 operations=28'in result.stdout,(result.returncode,result.stdout,result.stderr)
    report=dict(providers=reports,sequences=len(rows),snapshots=530,direct_operations=len(operations),native_sources=2,omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,boundaries='none in tree operations; fixture uses Win32 only to reserve fixed-address test storage',invariants='root black, parent links, signed size/address ordering, no cycles, no red-red links, equal black depth after every insertion',cases=[dict(sequence=row['sequence'],pattern=row['pattern'])for row in rows])
    (ROOT/'rebuild/compile-gate/variable-tree-insertion.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print('VARIABLE_TREE_INSERTION_PASS bytes=754 sequences=26 snapshots=530 operations=28 native_sources=2 omitted=2')

if __name__=='__main__':main()
