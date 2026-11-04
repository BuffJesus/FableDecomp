"""Real main metadata initialization on actual fixed/variable OS allocations."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_InitDebugInfo_009f28d0.cpp');args=parser.parse_args()
    sources=SOURCES+['rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp','rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp',args.source]
    work=Path(tempfile.mkdtemp(prefix='main-init-os-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainAllocatorOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');flags=['/DFABLE_NATIVE_MAIN_SIZE','/DFABLE_NATIVE_MAIN_INIT']if path.endswith('MainAllocatorOS_test.cpp')else[]
        r=check_one.cl(['/nologo','/c','/O2','/MT']+flags+['/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_INIT_OS_METADATA_OK checks=936'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-init-debug-info-os.json').write_text(json.dumps(dict(source=args.source,native_sources=len(sources),omitted_provider_checks=len(sources),scenarios=6,metadata_checks=936,result=r.stdout.strip(),invariants='real initializer and main size query on actual fixed/variable allocations with capacity>=20; independently predicted entire allocated block and complete main owner; prefix/trailer guards, row tracking ID/requested metadata and per-ID counter changes; untouched bytes and real balanced lock recursion; all existing main OS lifetime/size-query checks',boundary='direct size-class allocations supply row ID explicitly; fixture restores payload after checking metadata to retain prior lifetime invariants; no claim for main InternalAllocate/InternalDeallocate yet'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_INIT_DEBUG_INFO_OS_PASS sources={len(sources)} omitted={len(sources)}')
if __name__=='__main__':main()
