"""Shared boot ctor/destroy providers required; full retail memory-state replay."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_progress_counted_ctor as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
 p=argparse.ArgumentParser();p.add_argument('--source',default='rebuild/src/compiled/00/40/ProgressCountedConstructor_00403d20.cpp');p.add_argument('--destroy-source',default='rebuild/src/compiled/00/40/ProgressDestroy_00403600.cpp');args=p.parse_args();work=Path(tempfile.mkdtemp(prefix='progress-counted-'));env=check_one.env();objects=[]
 for path in [args.source,args.destroy_source,'rebuild/tests/integration/ProgressCounted_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 reports=[check(objects[0],'??0CCountedProgressDisplay@@QAE@PAVCProgressDisplay@@@Z',0x403d20,78,2,{'?FableOperatorNew@@YAPAXI@Z':0xbfea1a,'?FableDestroyProgressDisplay_00403600@@YIXPAX@Z':0x403600}),check(objects[1],'?FableDestroyProgressDisplay_00403600@@YIXPAX@Z',0x403600,11,0,{})]
 rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
 for row in rows:
  data.extend(struct.pack('<5I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
  for state in row['events']:data.extend(state)
 binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 for omitted in range(2):
  r=subprocess.run(link+[str(o)for i,o in enumerate(objects)if i!=omitted],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe),str(binary)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'PROGRESS_COUNTED_NATIVE_OK cases={len(rows)} destroy_cases=2'in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/progress-counted.json').write_text(json.dumps(dict(providers=reports,source=args.source,destroy_source=args.destroy_source,cases=len(rows),destroy_cases=2,omitted_provider_checks=2,compared_bytes_per_snapshot=oracle.WIDTH,boundary='constructor allocation boundary recorded; callback pointer validated and normalized; genuine destroy executes null path and real CRT heap polymorphic delete; ownership copy/release composition pending'),indent=2)+'\n');print(r.stdout.strip())
if __name__=='__main__':main()
