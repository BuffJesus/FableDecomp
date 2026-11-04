"""Candidate-required aligned null return/fault comparison with actual native locks."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_aligned_failure as oracle
ROOT=Path(__file__).resolve().parents[2]
def main():
    p=argparse.ArgumentParser();p.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_Aligned_009f2be0.cpp');args=p.parse_args();work=Path(tempfile.mkdtemp(prefix='main-aligned-failure-'));env=check_one.env();objects=[]
    for path in [args.source,'rebuild/tests/integration/MainAlignedFailure_test.cpp']:
        source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
    rows=oracle.collect();data=struct.pack('<I',len(rows))+b''.join(struct.pack('<4I',*row)for row in rows);binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
    r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe),str(binary)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_ALIGNED_FAILURE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
    (ROOT/'rebuild/compile-gate/main-aligned-failure.json').write_text(json.dumps(dict(source=args.source,cases=len(rows),omitted_provider_checks=1,result=r.stdout.strip(),invariants='retail independently predicted null internal allocation write fault address/size/value and create-enter-allocate-leave ordering; native SEH confirms write access violation at same low address after actual critical section balanced; sizes0/1/236/ffffffff, alignments1/2/4/8/16/256/4096, singleton absent/present',boundary='internal allocation failure recorded; actual Win32 locks, no claim of forced OS exhaustion'),indent=2)+'\n',encoding='utf-8');print(r.stdout.strip())
if __name__=='__main__':main()
