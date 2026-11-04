"""Main size query with real size-class providers and actual Windows locking."""
import argparse,json,subprocess,tempfile
from pathlib import Path
import check_one
from gate_main_allocator_os import SOURCES
from pe_oracle import EXE,pe_sections
ROOT=Path(__file__).resolve().parents[2]
def main():
    parser=argparse.ArgumentParser();parser.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_GetAllocationSize_009f2390.cpp');args=parser.parse_args();sources=SOURCES+[args.source,'rebuild/src/compiled/01/3c/AllocatorRangeData_013caaa4.cpp']
    # Both globals occupy the PE section's zero-filled virtual tail, not raw data.
    raw=EXE.read_bytes();rva=0x13caaa4-0x400000
    section=next(s for s in pe_sections(raw)if s[1]<=rva and rva+8<=s[1]+s[2]);assert rva-section[1]>=section[4]
    work=Path(tempfile.mkdtemp(prefix='main-size-os-'));env=check_one.env();objects=[]
    for path in sources+['rebuild/tests/integration/MainAllocatorOS_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');flags=['/DFABLE_NATIVE_MAIN_SIZE']if path.endswith('MainAllocatorOS_test.cpp')else[]
        r=check_one.cl(['/nologo','/c','/O2','/MT']+flags+['/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    for omitted in range(len(sources)):
        r=subprocess.run(link+[str(p)for i,p in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0,('provider omitted',sources[omitted])
    r=subprocess.run(link+[str(p)for p in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'MAIN_ALLOCATOR_OS_OK scenarios=6'in r.stdout and 'MAIN_SIZE_OS_QUERIES_OK queries=4320'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-allocation-size-os.json').write_text(json.dumps(dict(native_sources=len(sources),omitted_provider_checks=len(sources),source=args.source,scenarios=6,size_queries=4320,result=r.stdout.strip(),invariants='real main singleton constructor and all30 size classes; main size query dispatches actual fixed/variable slot3 using area-header row IDs; actual special range ID0 path; critical-section recursion preserved on valid return and incremented on null/invalid early return, fixture explicitly balances leaked entries; all previous real OS payload/size-class lifetime/shutdown invariants',boundary='direct size-class allocations supply rowID explicitly; no claim for main InternalAllocate/InitDebugInfo/InternalDeallocate yet; no mocked imports; actual main size-query lock leaks deliberately balanced by fixture'),indent=2)+'\n',encoding='utf-8')
    print(r.stdout.strip());print(f'MAIN_ALLOCATION_SIZE_OS_PASS sources={len(sources)} omitted={len(sources)} queries=4320')
if __name__=='__main__':main()
