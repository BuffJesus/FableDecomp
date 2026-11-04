"""Scratch behavior replay; deliberately does not certify code parity or promotion."""
import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_debug_internal as oracle
ROOT=Path(__file__).resolve().parents[2]
def main():
 p=argparse.ArgumentParser();p.add_argument('--source',default='work/MainDebugAllocateInternal_candidate.cpp');args=p.parse_args();work=Path(tempfile.mkdtemp(prefix='main-debug-internal-'));env=check_one.env();objects=[]
 for path in [args.source,'rebuild/tests/integration/MainDebugInternal_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 blob=objects[0].read_bytes();original=b'@__security_check_cookie@4\0';replacement=b'@DebugCookieRecording@4\0';assert blob.count(original)==1 and len(replacement)<=len(original);objects[0].write_bytes(blob.replace(original,replacement+b'\0'*(len(original)-len(replacement))))
 rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
 for row in rows:
  data.extend(struct.pack('<6I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
  for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
 binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe),str(binary)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_DEBUG_INTERNAL_NATIVE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/main-debug-internal-scratch.json').write_text(json.dumps(dict(source=args.source,cases=len(rows),omitted_provider_checks=1,result=r.stdout.strip(),compared_bytes_per_snapshot=oracle.WIDTH,boundary='bool internal allocation/main size/cookie checker recorded, genuine five-argument C++ overload executes; scratch provider with compiler-generated GS cookie, no promotion or cookie-mismatch rejection claim'),indent=2)+'\n');print(r.stdout.strip())
if __name__=='__main__':main()
