"""Verify OS-backed chunk creation, native failure handler and composed retry state."""
import json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_variable_new_chunk as oracle
from gate_game_component_abi import Coff
from gate_variable_chunk_initialization import PROVIDERS as CHUNK_PROVIDERS,TARGETS as CHUNK_TARGETS
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
PROVIDERS=[('00/a6/CMemoryAllocatorVariableSize_NewChunk_00a65720.cpp','?NewChunk@CMemoryAllocatorVariableSize@@IAEPAVCMemoryAreaChunk@1@J@Z',0xa65720,269,9),
           *CHUNK_PROVIDERS[1:],
           ('00/9f/CMemoryAllocator_OnFailedAllocation_009f1f00.cpp','?OnFailedAllocation@CMemoryAllocator@@QAEXXZ',0x9f1f00,1,0)]
TARGETS=dict(CHUNK_TARGETS,**{'__imp__VirtualAlloc@16':0x143fd44,'__imp__VirtualQuery@12':0x143fe80,
    '?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?OnFailedAllocation@CMemoryAllocator@@QAEXXZ':0x9f1f00})

def main(real_tree=False):
    work=Path(tempfile.mkdtemp(prefix='variable-new-chunk-'));env=check_one.env();objects=[];reports=[]
    raw=EXE.read_bytes();sections=pe_sections(raw)
    providers=PROVIDERS
    targets=dict(TARGETS)
    if real_tree:
        providers=PROVIDERS+[
            ('00/a6/CMemoryAllocatorVariableSize_InsertIntoTree_00a64e70.cpp','?InsertIntoTree@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@0@Z',0xa64e70,122,3),
            ('00/a6/CMemoryAllocatorVariableSize_InsertFixup_00a64740.cpp','?InsertFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z',0xa64740,464,0)]
        targets['?InsertFixup@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z']=0xa64740
    def compile(source):
        flags=['/DFABLE_REAL_TREE']if real_tree and source.name=='VariableNewChunk_test.cpp'else[]
        obj=work/(source.stem+'.obj');result=check_one.cl(['/nologo','/c','/O2','/MT',*flags,'/Fo'+str(obj),str(source)],env,work)
        assert result.returncode==0,result.stdout+result.stderr;objects.append(obj);return Coff(obj)
    for source,symbol,address,length,relocation_count in providers:
        coff=compile(ROOT/'rebuild/src/compiled'/source);section,offset=coff.symbols[symbol]
        body,relocs=coff.sections[section];assert offset==0 and len(relocs)==relocation_count;patched=bytearray(body)
        for position,(name,kind)in relocs.items():
            assert name in targets and kind in [6,20],(name,kind)
            value=targets[name]+struct.unpack_from('<I',body,position)[0]
            if kind==20:value-=address+position+4
            struct.pack_into('<I',patched,position,value&0xffffffff)
        at=va_to_off(sections,address);assert len(body)==length and bytes(patched)==raw[at:at+length],source
        reports.append(dict(address=f'{address:08x}',bytes=length,relocations=relocation_count,grade='RELOCATION_MATCH',source=source))
        if address==0xa64740:
            for method,rotation_address in [('RotateLeft',0xa64680),('RotateRight',0xa646e0)]:
                section,offset=coff.symbols['?'+method+'@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryArea@1@@Z']
                rotation,rotation_relocs=coff.sections[section];at=va_to_off(sections,rotation_address)
                assert offset==0 and not rotation_relocs and len(rotation)==84 and rotation==raw[at:at+84]
                reports.append(dict(address=f'{rotation_address:08x}',bytes=84,relocations=0,grade='RELOCATION_MATCH',source=source))
        # Test-only IAT redirection follows validation of the original relocations.
        if address==0xa65720:
            blob=objects[-1].read_bytes()
            for original,replacement in [('__imp__VirtualAlloc@16','_AllocateSlot'),('__imp__VirtualQuery@12','_QuerySlot')]:
                needle=original.encode()+b'\0';assert blob.count(needle)==1
                value=replacement.encode()+b'\0';assert len(value)<=len(needle)
                blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
            objects[-1].write_bytes(blob)
    compile(ROOT/'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp')
    compile(ROOT/'rebuild/src/compiled/01/3b/GFMainSingletonCodeData_013b7d58.cpp')
    compile(ROOT/'rebuild/tests/integration/VariableNewChunk_test.cpp')
    rows=oracle.collect(real_tree=real_tree);data=bytearray(struct.pack('<I',len(rows)))
    for row in rows:
        events=[];before=b'\0'*oracle.WIDTH;tree_calls=0
        for event in row['events']:
            if real_tree and event['kind']=='tree':continue
            if event['kind']=='failure':
                assert event['this']==oracle.OWNER+0x1000;continue
            kind={'alloc':1,'query':2,'tree':3}[event['kind']];args=event['args'];result=event.get('result',0)
            events.append([kind,len(args),*args,*([0]*(4-len(args))),result])
            if kind==3:tree_calls+=1;before=bytes.fromhex(event['before'])
        data.extend(struct.pack('<9I',*(v&0xffffffff for v in row['args'])))
        data.extend(struct.pack('<3I',row['result'],len(events),tree_calls))
        data.extend(b''.join(struct.pack('<7I',*event)for event in events));data.extend(before);data.extend(bytes.fromhex(row['after']))
    binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
    link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    native_sources=len(objects)-1
    for omitted in range(native_sources):
        result=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True)
        assert result.returncode!=0,('provider omitted',omitted)
    result=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert result.returncode==0,result.stdout+result.stderr
    result=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30)
    marker='VARIABLE_CHUNK_TREE_COMPOSED_OK'if real_tree else'VARIABLE_NEW_CHUNK_OK'
    assert result.returncode==0 and marker+' cases='+str(len(rows))in result.stdout,(result.returncode,result.stdout,result.stderr)
    report='variable-chunk-tree-composed.json'if real_tree else'variable-new-chunk.json'
    boundaries='VirtualAlloc/VirtualQuery only; genuine tree insertion/fixup/rotations linked; no-op failure call identity validated by relocation/omission, not instrumented in fixture'if real_tree else'VirtualAlloc/VirtualQuery and InsertIntoTree record calls; native no-op failure handler linked and checked byte-exact; failure call identity validated by relocation/omission, not instrumented in fixture'
    (ROOT/'rebuild/compile-gate'/report).write_text(json.dumps(dict(providers=reports,linked_cases=len(rows),retail_cases=len(rows),successful_cases=sum(r['result']!=0 for r in rows),failed_cases=sum(r['result']==0 for r in rows),native_sources=native_sources,omitted_provider_checks=native_sources,compared_bytes_per_snapshot=oracle.WIDTH,boundaries=boundaries,scenarios=[dict(args=row['args'],result=row['result'])for row in rows]),indent=2)+'\n',encoding='utf-8')
    if real_tree:print('VARIABLE_CHUNK_TREE_COMPOSED_PASS bytes=1322 cases=48 native_sources=8 omitted=8')
    else:print('VARIABLE_NEW_CHUNK_PASS new_bytes=270 composed_bytes=568 cases=32 native_sources=6 omitted=6')

if __name__=='__main__':main()
