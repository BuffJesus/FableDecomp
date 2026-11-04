"""Production native best-fit/split/tree/free composition against retail."""
import sys,struct,subprocess,json,tempfile
from pathlib import Path
root=Path(__file__).resolve().parents[2]
import check_one,oracle_variable_existing_allocations as oracle
free_cycles='--free-cycles'in sys.argv
size_queries='--size-queries'in sys.argv
largest_free='--largest-free'in sys.argv
fragmentation='--fragmentation'in sys.argv
enumeration='--enumeration'in sys.argv
validation='--validate'in sys.argv
work=Path(tempfile.mkdtemp(prefix='variable-allocation-composition-'));env=check_one.env();objects=[]
sources=['rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_AllocateMemoryFromExistingChunks_00a656e0.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_RemoveFromTree_00a64ef0.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_RemoveFromTreeFixup_00a64910.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_AllocAreaFromArea_00a65480.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_RemoveAreaFromFreeList_00a65060.cpp',
 'rebuild/src/compiled/00/a6/VariableAllocatorChunk_InsertAreaIntoFreeList_00a650f0.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_InsertIntoTree_00a64e70.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_InsertFixup_00a64740.cpp',
 'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_TreeQueries_00a64c10.cpp',
 'rebuild/src/compiled/01/3d/VariableAllocatorAlignmentData_013d28d4.cpp',
 'rebuild/tests/integration/VariableExistingAllocations_test.cpp']
if free_cycles:sources.insert(-1,'rebuild/src/compiled/00/a6/VariableAllocatorChunk_FreeArea_00a658e0.cpp')
if size_queries:sources.insert(-1,'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_AllocationSizes_00a651b0.cpp')
if largest_free:sources.insert(-1,'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_GetLargestFreeAreaSize_00a64500.cpp')
if fragmentation:
    sources[-1:-1]=['rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_GetFragmentation_00a64590.cpp','rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Counts_00a64530.cpp']
if enumeration:sources.insert(-1,'rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Enumeration_00a65260.cpp')
if validation:sources[-1:-1]=['rebuild/src/compiled/00/a6/CMemoryAllocatorVariableSize_Validate_00a65330.cpp','rebuild/src/compiled/00/9f/CMemoryAllocator_InternalValidateArea_009f1f40.cpp','rebuild/src/compiled/01/3b/GFMainSingletonCodeData_013b7d58.cpp']
for path in sources:
    source=root/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT']+(['/DFABLE_FREE_CYCLES']if free_cycles else[])+(['/DFABLE_SIZE_QUERIES']if size_queries else[])+(['/DFABLE_LARGEST_FREE']if largest_free else[])+(['/DFABLE_FRAGMENTATION']if fragmentation else[])+(['/DFABLE_ENUMERATION']if enumeration else[])+(['/DFABLE_VALIDATE']if validation else[])+['/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
rows=oracle.collect(free_cycles=free_cycles,size_queries=size_queries,largest_free=largest_free,fragmentation=fragmentation,enumeration=enumeration,validation=validation);data=bytearray(struct.pack('<I',len(rows)))
for row in rows:
    data.extend(struct.pack('<2I',row['alignment'],len(row['steps'])));data.extend(row['initial'])
    for step in row['steps']:
        if free_cycles:data.extend(struct.pack('<3I',step['mode'],step['request'],step['result']))
        else:data.extend(struct.pack('<2I',step['request'],step['result']))
        data.extend(step['after'])
        if size_queries:data.extend(struct.pack('<2I',*step.get('queries',(0,0))))
        if largest_free:data.extend(struct.pack('<I',step['largest']))
        if fragmentation:data.extend(struct.pack('<I',step['fragmentation']))
        if enumeration:
            pointers=step['enumeration'];data.extend(struct.pack('<I',len(pointers)))
            data.extend(struct.pack('<'+str(len(pointers))+'I',*pointers))
binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe'
link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omitted in range(len(objects)-1):
    r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',omitted)
r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe),str(binary)],capture_output=True,text=True,timeout=30);assert r.returncode==0,r.stdout+r.stderr
calls=sum(len(row['steps'])for row in rows);marker='VARIABLE_ALLOCATION_FREE_CYCLES_OK'if free_cycles else'VARIABLE_EXISTING_ALLOCATIONS_OK';assert f'{marker} sequences={len(rows)} calls={calls}'in r.stdout;print(r.stdout.strip())
invariants='independent minimum eligible size; split-tail placement and whole-area size; list counts/links/extrema; tree membership and red-black invariants; byte/header conservation; no overlap; ID preservation; unchanged state on miss or signed split overflow'
if free_cycles:invariants+='; free cycles restore original seven free extents with zero allocated nodes'
report=root/'rebuild/compile-gate'/('variable-allocation-free-cycles.json'if free_cycles else'variable-existing-allocations.json')
if size_queries:report=root/'rebuild/compile-gate/variable-composed-size-queries.json'
if largest_free:report=root/'rebuild/compile-gate/variable-composed-free-stats.json'
if fragmentation:report=root/'rebuild/compile-gate/variable-composed-fragmentation.json'
if enumeration:report=root/'rebuild/compile-gate/variable-composed-enumeration.json'
if validation:report=root/'rebuild/compile-gate/variable-composed-validation.json'
report.write_text(json.dumps(dict(grade='COMPOSED_NATIVE_PASS',sequences=len(rows),calls=calls,allocation_calls=sum(step['mode']==0 for row in rows for step in row['steps']),free_calls=sum(step['mode']==1 for row in rows for step in row['steps']),free_cycles=free_cycles,native_sources=len(objects)-1,omitted_provider_checks=len(objects)-1,compared_bytes_per_snapshot=oracle.WIDTH,invariants=invariants,boundary='none in existing-chunk allocation/freeing; production best-fit, split, lists, coalescing, deletion/fixup/insertion/rotations; initial states retail-built; no OS acquisition/destruction or virtual entry claim'),indent=2)+'\n',encoding='utf-8')
if size_queries:
    query_count=2*sum(step['mode']==0 and bool(step['result'])for row in rows for step in row['steps'])
    assert f'VARIABLE_COMPOSED_SIZE_QUERIES_OK queries={query_count}'in r.stdout
    payload=json.loads(report.read_text(encoding='utf-8'));payload.update(size_query_calls=query_count,size_query_invariants='both native pointer queries on every successful real split/whole-area allocation; exact retail results and unchanged full post-allocation state');report.write_text(json.dumps(payload,indent=2)+'\n',encoding='utf-8')
if largest_free:
    assert f'VARIABLE_COMPOSED_LARGEST_FREE_OK queries={calls}'in r.stdout
    payload=json.loads(report.read_text(encoding='utf-8'));payload.update(largest_free_calls=calls,largest_free_invariants='cached query compared against independent live free-list maximum after every allocation/free; complete state and globals preserved');report.write_text(json.dumps(payload,indent=2)+'\n',encoding='utf-8')
if fragmentation:
    assert f'VARIABLE_COMPOSED_FRAGMENTATION_OK queries={calls}'in r.stdout
    payload=json.loads(report.read_text(encoding='utf-8'));payload.update(fragmentation_calls=calls,fragmentation_invariants='real native virtual counts after every allocation/free; independent live free-list guard; float32 zero/infinity and x87 divide-by-zero flag; full state and alignment preserved; both vtable bindings validated before normalization');report.write_text(json.dumps(payload,indent=2)+'\n',encoding='utf-8')
if enumeration:
    indexed=sum(len(step['enumeration'])+4 for row in rows for step in row['steps'])
    assert f'VARIABLE_COMPOSED_ENUMERATION_OK bulk={calls} indexed={indexed}'in r.stdout
    payload=json.loads(report.read_text(encoding='utf-8'));payload.update(enumeration_bulk_calls=calls,enumeration_index_calls=indexed,enumeration_invariants='real allocated-list order after every allocation/free; all valid indices plus four misses; zero bulk max still writes all pointers; output tail and full heap preserved');report.write_text(json.dumps(payload,indent=2)+'\n',encoding='utf-8')
if validation:
    assert f'VARIABLE_COMPOSED_VALIDATE_OK queries={calls}'in r.stdout
    payload=json.loads(report.read_text(encoding='utf-8'));payload.update(validation_calls=calls,validation_invariants='actual native validation and shipping InternalValidateArea no-op after every allocation/free; full live heap, globals and singleton preserved');report.write_text(json.dumps(payload,indent=2)+'\n',encoding='utf-8')
print(f'VARIABLE_EXISTING_ALLOCATIONS_PASS free_cycles={int(free_cycles)} calls={calls} native_sources={len(objects)-1} omitted={len(objects)-1}')
