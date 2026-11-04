"""Recovered native allocator methods with actual Win32 memory lifetime."""
import ast,json,subprocess,tempfile
from pathlib import Path
import check_one
ROOT=Path(__file__).resolve().parents[2]
def main():
    # Reuse the production composition's provider set without executing its CLI.
    tree=ast.parse((ROOT/'tools/decomp_pipeline/gate_variable_existing_allocations.py').read_text(encoding='utf-8'))
    sources=list(dict.fromkeys(node.value for node in ast.walk(tree)if isinstance(node,ast.Constant)and isinstance(node.value,str)and node.value.startswith('rebuild/src/compiled/')and node.value.endswith('.cpp')))
    sources+=['rebuild/src/compiled/'+path for path in [
        '00/a6/CMemoryAllocatorVariableSize_Allocate_00a659c0.cpp',
        '00/a6/CMemoryAllocatorVariableSize_Deallocate_00a65a50.cpp',
        '00/a6/CMemoryAllocatorVariableSize_DeleteChunk_00a65390.cpp',
        '00/a6/CMemoryAllocatorVariableSize_Destructor_00a65690.cpp',
        '00/a6/CMemoryAllocatorVariableSize_Constructor_00a65930.cpp',
        '00/a6/CMemoryAllocatorVariableSize_NewChunk_00a65720.cpp',
        '00/a6/CMemoryAllocatorVariableSize_NewChunkDemandPaged_00a65830.cpp',
        '00/a6/VariableAllocatorChunk_Init_00a65410.cpp',
        '00/a6/CMemoryAllocatorBase_Constructor_00a65ad0.cpp',
        '00/a6/CMemoryAllocatorBase_ConsolidateFreeAreas_00a65b50.cpp',
        '00/a6/CMemoryAllocatorBase_OperatorNew_00a66730.cpp',
        '00/a6/CMemoryAllocatorBase_OperatorDelete_00a66750.cpp',
        '00/a6/CMemoryAllocatorVariableSize_Totals_00a64db0.cpp',
        '00/9f/CMemoryAllocator_OnFailedAllocation_009f1f00.cpp']]
    sources=list(dict.fromkeys(sources));work=Path(tempfile.mkdtemp(prefix='variable-os-lifetime-'));env=check_one.env();objects=[]
    for source in sources+['rebuild/tests/integration/VariableOSLifetime_test.cpp']:
        path=ROOT/source;obj=work/(path.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=60);assert r.returncode==0 and 'VARIABLE_OS_LIFETIME_OK scenarios=54 operations=1215 validations=1269'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/variable-os-lifetime.json').write_text(json.dumps(dict(grade='NATIVE_OS_LIFETIME_PASS',native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=54,operations=1215,validations=1269,sources=sources,result=r.stdout.strip(),invariants='actual Win32 reserve/commit/query/release; recovered constructor and all native virtual allocations/deallocations/destruction; all three constructor-supported alignments4/8/16; empty/OS-owned permanent/caller-owned demand-paged initial memory; forward/reverse/permuted frees or live allocations at destruction; payload preservation/nonoverlap; tree membership/order/parents/colour/black depth; list counts/links/extrema; data/header conservation; native totals/count/size/enumeration; last allocation releases nonpermanent chunk; destructor releases OS chunks and allocator object but retains caller-owned memory',boundary='no recording imports or algorithm substitutes; real Windows APIs; successful read/write heap paths only; constructor unsupported alignments use retail invalid shift9999999 and are excluded; no forced OS failure or main singleton construction/validation claim; prior method parity/retail fixtures remain separate evidence'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'VARIABLE_OS_LIFETIME_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
