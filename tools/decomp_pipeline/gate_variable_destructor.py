"""Destructor/thunk retail parity and native chunk-release boundary replay."""
import json,struct,subprocess,tempfile,sys
from pathlib import Path
import check_one,oracle_variable_destructor as oracle
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from gate_fixed_allocator_lifetime import SLOTS
ROOT=Path(__file__).resolve().parents[2]
SOURCES=['00/a6/CMemoryAllocatorVariableSize_Destructor_00a65690.cpp','00/a6/CMemoryAllocatorVariableSize_DeleteChunk_00a65390.cpp','01/3d/VariableAllocatorAlignmentData_013d28d4.cpp','00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp','00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp']
def main():
    work=Path(tempfile.mkdtemp(prefix='variable-destructor-'));env=check_one.env();objects=[]
    native_tree='--native-tree'in sys.argv
    sources=SOURCES+(['00/a6/CMemoryAllocatorVariableSize_RemoveFromTree_00a64ef0.cpp','00/a6/CMemoryAllocatorVariableSize_RemoveFromTreeFixup_00a64910.cpp']if native_tree else[])
    for source in sources+['fixture']:
        path=ROOT/('rebuild/tests/integration/VariableDestructor_test.cpp'if source=='fixture'else'rebuild/src/compiled/'+source)
        obj=work/(path.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT']+(['/DFABLE_NATIVE_TREE']if native_tree else[])+['/Fo'+str(obj),str(path)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    targets={'??_7CMemoryAllocatorVariableSize@@6B@':0x129db1c,'?g_FableVariableAllocatorAlignment_013D28D4@@3KA':0x13d28d4,'?g_FableVariableAllocatorAlignmentShift_013D28D8@@3KA':0x13d28d8,'?DeleteChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryAreaChunk@1@@Z':0xa65390,'??1CMemoryAllocatorVariableSize@@UAE@XZ':0xa65690,'??3CMemoryAllocatorBase@@SIXPAX@Z':0xa66750,'?RemoveFromTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z':0xa64ef0,'__imp__GlobalMemoryStatus@4':0x143fe20,'__imp__VirtualFree@12':0x143fd40}
    reports=[check(objects[0],'??1CMemoryAllocatorVariableSize@@UAE@XZ',0xa65690,74,4,targets),check(objects[0],'??_GCMemoryAllocatorVariableSize@@UAEPAXI@Z',0xa65c60,28,2,targets),check(objects[1],'?DeleteChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryAreaChunk@1@@Z',0xa65390,126,3,targets)]
    slots=[s.replace('CMemoryAllocatorFixedSize','CMemoryAllocatorVariableSize')for s in SLOTS];slots[13]='?ConsolidateFreeAreas@CMemoryAllocatorBase@@UAEXXZ';Coff(objects[0]).table('??_7CMemoryAllocatorVariableSize@@6B@',slots)
    assert all(r['grade']=='MATCH'for r in reports)
    if native_tree:
        fixup='?RemoveFromTreeFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@PBVCDeletedNodeChildInfo@@@Z';right='?RotateRight@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z'
        reports.extend([check(objects[5],'?RemoveFromTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z',0xa64ef0,305,1,{fixup:0xa64910}),check(objects[6],fixup,0xa64910,757,1,{right:0xa646e0})])
    for i in [1,4]:
        blob=objects[i].read_bytes()
        for original,replacement in [('__imp__GlobalMemoryStatus@4','_StatusSlot'),('__imp__VirtualFree@12','_FreeSlot')]:
            needle=original.encode()+b'\0';value=replacement.encode()+b'\0'
            if needle in blob:
                assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
        objects[i].write_bytes(blob)
    rows=oracle.collect(native_tree=native_tree);data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        data.extend(struct.pack('<7I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
        for *args,state in row['events']:data.extend(struct.pack('<6I',*args));data.extend(state)
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib','/alternatename:_RetailTable=??_7CMemoryAllocatorVariableSize@@6B@','/alternatename:@TestDeleting@12=??_GCMemoryAllocatorVariableSize@@UAEPAXI@Z']
    for omitted in range(len(objects)-1):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=60);assert r.returncode==0 and f'VARIABLE_DESTRUCTOR_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    report=ROOT/('rebuild/compile-gate/variable-destructor-native-tree.json'if native_tree else'rebuild/compile-gate/variable-destructor.json')
    report.write_text(json.dumps(dict(providers=reports,cases=len(rows),native_tree=native_tree,boundary_calls=sum(len(row['events'])for row in rows),omitted_provider_checks=len(objects)-1,vtable_slots=16,compared_bytes_per_snapshot=oracle.WIDTH,invariants='head chunk deletion until empty including Permanent; tree removal order; two status calls per chunk with same buffer; DemandPaged callback mutation reloaded; alignment initialized once then callback changes persist; VirtualFree success/failure ignored; deleting flags0/1/2/3, returned owner and optional object release; entire call/final states; owner vtable validated before normalization'+('; native red-black membership/order/parents/black-depth and removed tree links checked; retail-built trees, equal/high-bit keys; final root empty'if native_tree else''),boundary='real recovered DeleteChunk and operator delete; '+('real native tree deletion/fixup/rotations; only OS calls recorded'if native_tree else'tree removal and OS calls recorded')+'; other virtuals fail fast; no actual OS release claimed'),indent=2)+'\n',encoding='utf-8')
    print(f'VARIABLE_DESTRUCTOR_PASS bytes=102 cases={len(rows)} calls={sum(len(row["events"])for row in rows)} omitted={len(objects)-1} native_tree={int(native_tree)}')
if __name__=='__main__':main()
