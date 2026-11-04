import argparse,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,oracle_file_installer_constructor as oracle
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2]
def main():
 p=argparse.ArgumentParser();p.add_argument('--source',default='work/FileInstallerConstructor_candidate.cpp');args=p.parse_args();work=Path(tempfile.mkdtemp(prefix='installer-ctor-'));env=check_one.env();objects=[]
 for path in [args.source,'rebuild/tests/integration/FileInstallerConstructor_test.cpp']:
  source=ROOT/path;obj=work/(source.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr;objects.append(obj)
 targets={'?g_FileInstallerVTable_0129B43C@@3EA':0x129b43c,'?FableInstallerContainerAllocate@@YAPAXK@Z':0xbfea0e,'__imp__InitializeCriticalSection@4':0x143fee4,'__imp__CreateEventW@16':0x143fdc8,'?g_FileInstallerCompressedExtension_0129B448@@3QBDB':0x129b448,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'?g_FileInstallerPlainExtension_0129B440@@3QBDB':0x129b440,'??ACBasicRetailBankAliasMap@@QAEAAVCCharString@@ABV1@@Z':0x9ac700,'??4CCharString@@QAEABV0@PBD@Z':0x99efe0,'??1CCharString@@QAE@XZ':0x99eae0,'?FableInstallerWorker_009C5870@@YAXPAX@Z':0x9c5870,'?FableInstallerDecompressionWorker_009C4420@@YAXPAX@Z':0x9c4420,'__beginthread':0xbfed3a}
 report=check(objects[0],'??0CFileInstaller@@QAE@XZ',0x9c5890,389,18,targets)
 blob=objects[0].read_bytes()
 for original,replacement in [('__imp__InitializeCriticalSection@4','_InitSlot'),('__imp__CreateEventW@16','_EventSlot'),('__beginthread','_ThreadRecording')]:
  needle=original.encode()+b'\0';value=replacement.encode()+b'\0';assert blob.count(needle)==1
  # COFF long-name string entries can grow: update string-table lengths/offsets below.
  if len(value)>len(needle):
   from gate_game_component_abi import Coff
   raw=bytearray(blob);ptr,count=struct.unpack_from('<II',raw,8);start=ptr+count*18;pos=raw.index(needle,start);delta=len(value)-len(needle);offset=pos-start
   for i in range(count):
    at=ptr+i*18
    if raw[at:at+4]==b'\0'*4:
     old=struct.unpack_from('<I',raw,at+4)[0]
     if old>offset:struct.pack_into('<I',raw,at+4,old+delta)
   struct.pack_into('<I',raw,start,struct.unpack_from('<I',raw,start)[0]+delta);blob=bytes(raw[:pos])+value+bytes(raw[pos+len(needle):])
  else:blob=blob.replace(needle,value+b'\0'*(len(needle)-len(value)))
 objects[0].write_bytes(blob);rows=oracle.collect();data=bytearray(struct.pack('<I',len(rows)))
 for row in rows:
  data.extend(struct.pack('<3I',*row['args']));data.extend(row['initial']);data.extend(row['after'])
  for *values,state in row['events']:data.extend(struct.pack('<4I',*values));data.extend(state)
 binary=work/'retail.bin';binary.write_bytes(data);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
 r=subprocess.run(link+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(link+[str(o)for o in objects],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe),str(binary)],cwd=work,capture_output=True,text=True,timeout=30);assert r.returncode==0 and f'FILE_INSTALLER_CTOR_NATIVE_OK cases={len(rows)}'in r.stdout,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/file-installer-constructor.json').write_text(json.dumps(dict(provider=report,source=args.source,cases=len(rows),omitted_provider_checks=1,compared_bytes_per_snapshot=oracle.WIDTH,boundary='allocation, OS, strings/map and thread boundaries recorded; constructor executes; normalized vtable validated, argument identities validated; no actual worker launches'),indent=2)+'\n');print(r.stdout.strip())
if __name__=='__main__':main()
