"""Actual linked VC7.1 CRT cookie match/mismatch, isolated subprocesses."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
ROOT=Path(__file__).resolve().parents[2]
def main():
 work=Path(tempfile.mkdtemp(prefix='main-debug-cookie-'));env=check_one.env();objects=[]
 for path in ['work/MainDebugAllocateInternal_candidate.cpp','rebuild/tests/integration/MainDebugCookie_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 rows=[]
 for mode in [0,1]:
  for cookie in [0,0xbb40e64e,0xffffffff]:
   r=subprocess.run([str(exe),str(mode),hex(cookie)],cwd=work,capture_output=True,text=True,timeout=15)
   assert 'COOKIE_BOUNDARY_REACHED'in r.stderr,(mode,cookie,r.returncode,r.stdout,r.stderr)
   if not mode:assert r.returncode==0 and 'MAIN_DEBUG_COOKIE_RETURN_OK'in r.stdout
   else:assert r.returncode==3 and 'MAIN_DEBUG_COOKIE_RETURN_OK'not in r.stdout and 'CRT_COOKIE_MISMATCH_DETECTED'in r.stderr,(r.returncode,r.stdout,r.stderr)
   rows.append(dict(mutation=mode,cookie=cookie,exit_code=r.returncode,stdout=r.stdout,stderr=r.stderr))
 (ROOT/'rebuild/compile-gate/main-debug-cookie-scratch.json').write_text(json.dumps(dict(cases=6,rows=rows,omitted_provider_checks=1,boundary='actual unmodified linked CRT cookie checker invokes documented security error handler on mismatch; fixture handler verifies buffer-overrun code and exits3 to suppress interactive CRT UI; InternalAllocate bool/size queries recorded; scratch provider, no production promotion'),indent=2)+'\n');print('MAIN_DEBUG_COOKIE_OK cases=6 accepted=3 rejected=3')
if __name__=='__main__':main()

