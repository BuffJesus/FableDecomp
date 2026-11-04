"""Native boot counted ownership composition; real CRT allocation boundary."""
import json,subprocess,tempfile
from pathlib import Path
import check_one
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
 sources=['rebuild/src/compiled/01/22/Global_ProgressDisplay_012241b0.cpp','rebuild/src/compiled/00/40/ProgressCountedConstructor_00403d20.cpp','rebuild/src/compiled/00/40/ProgressDestroy_00403600.cpp','rebuild/src/compiled/00/9e/Global_SetProgressDisplay_009e9fd0.cpp','rebuild/src/compiled/00/9e/Global_GetProgressDisplay_009ea060.cpp'];work=Path(tempfile.mkdtemp(prefix='progress-counted-life-'));env=check_one.env();objects=[]
 for path in sources+['rebuild/tests/integration/ProgressCountedLifetime_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 targets={'?g_FableProgressDisplay_013CAA38@@3UCCountedProgressDisplay@@A':0x13caa38,'?FableOperatorDelete@@YAXPAX@Z':0xbfe9bc}
 proofs=[check(objects[3],'?SetProgressDisplay@@YIXPAUCCountedProgressDisplay@@@Z',0x9e9fd0,133,12,targets),check(objects[4],'?GetProgressDisplay@@YI?AUCCountedProgressDisplay@@XZ',0x9ea060,28,2,targets)]
 exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 for omitted in range(5):
  r=subprocess.run(link+[str(o)for i,o in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PROGRESS_COUNTED_LIFETIME_OK scenarios=32 destroyed=32 blocks_released=32'in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/progress-counted-lifetime.json').write_text(json.dumps(dict(providers=proofs,sources=sources,scenarios=32,omitted_provider_checks=5,result=r.stdout.strip(),invariants='real ctor/destroy and existing singleton Set/Get, native inline copy/destructor; shared counts1/2/3/4/3/2/1 then final object virtual deletion and control block release exactly once; singleton empty after clearing',boundary='FableOperatorNew/Delete fixture forwards real CRT operator new/delete; polymorphic test object, no actual texture-bearing CProgressDisplay destructor claim'),indent=2)+'\n');print(r.stdout.strip())
if __name__=='__main__':main()
