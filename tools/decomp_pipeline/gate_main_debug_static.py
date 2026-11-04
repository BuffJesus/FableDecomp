import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_main_debug_static as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
 p=argparse.ArgumentParser();p.add_argument('--source',default='rebuild/src/compiled/00/9f/CMemoryAllocator_AllocateDebug_009f2b90.cpp');args=p.parse_args();work=Path(tempfile.mkdtemp(prefix='main-debug-static-'));env=check_one.env();objects=[]
 for path in [args.source,'rebuild/tests/integration/MainDebugStatic_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 report=check(objects[0],'?Allocate@CMemoryAllocator@@SIPAXKKPBDK0@Z',0x9f2b90,79,8,{'?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,'?Create@CMemoryAllocator@@KIXXZ':0x9f2870,'?InternalAllocate@CMemoryAllocator@@IAEPAXJKPBDJ0@Z':0x9f2ac0,'?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A':0x13caa70,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10});assert report['grade']=='MATCH'
 blob=objects[0].read_bytes()
 for original,replacement in [('__imp__EnterCriticalSection@4','_EnterSlot'),('__imp__LeaveCriticalSection@4','_LeaveSlot')]:
  needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1;blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
 objects[0].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
 for row in rows:
  data.extend(struct.pack('<6I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
  for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
 binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe),str(binary)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'MAIN_DEBUG_STATIC_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/main-debug-static.json').write_text(json.dumps(dict(provider=report,source=args.source,cases=len(rows),omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,boundary='Create/protected debug overload and lock imports recorded; genuine static wrapper executes'),indent=2)+'\n');print(r.stdout.strip())
if __name__=='__main__':main()
