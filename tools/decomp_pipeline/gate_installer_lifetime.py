import sys,subprocess,json,hashlib,argparse,ast,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import build_installer_lifetime as build
import check_one,gate_zlib_deflate_core as core
from replay_file_installer_constructor_os_scratch import STRINGS
ap=argparse.ArgumentParser();ap.add_argument('--singleton',action='store_true');ap.add_argument('--workers',action='store_true');ap.add_argument('--buffers',action='store_true');ap.add_argument('--constructor',action='store_true');args=ap.parse_args()
if args.singleton:args.workers=True
if args.workers:args.constructor=True
if args.constructor:args.buffers=True
w=build.w;objects=[build.obj]
fixture=ROOT/'rebuild/tests/integration'/('FileInstallerNativeSingleton_test.cpp'if args.singleton else 'FileInstallerNativeWorkers_test.cpp'if args.workers else 'FileInstallerLifetime_test.cpp'if args.constructor else 'FileInstallerDestructorBuffers_test.cpp'if args.buffers else 'FileInstallerDestructorContainers_test.cpp')
sources=[ROOT/'rebuild/src/compiled'/p for p in STRINGS]+[ROOT/'rebuild/src/compiled'/p for p in ['00/4a/InstallerCompletedFileSet_Erase_004aba70.cpp','00/9c/InstallerActiveFileList_Clear_009c71a0.cpp','00/9c/InstallerCountedFileList_Clear_009c6cc0.cpp','00/a3/CCompressorZlib_CCompressorZlib_00a3acc0.cpp']]+[core.SOURCE,core.TREES]+[ROOT/'rebuild/src/compiled'/p for p in core.SUPPORT]+[ROOT/'rebuild/src/compiled/00/c0'/p for p in ['Zlib_zinflateInit_00c02310.cpp','Zlib_zinflateInit2_00c02240.cpp','Zlib_zinflateReset_00c021f0.cpp','Zlib_zinflateEnd_00c038d0.cpp']]+[ROOT/'rebuild/src/compiled/00/a6'/p for p in ['CCriticalSectionLock_Constructor_00a62920.cpp','CCriticalSectionLock_Destructor_00a629a0.cpp','CCriticalSectionLock_Release_00a62960.cpp','CCriticalSectionLock_Lock_00a62980.cpp']]+[fixture]
if args.buffers:
 from gate_main_allocator_os import SOURCES
 tree=ast.parse((ROOT/'tools/decomp_pipeline/gate_installer_cleanup_os.py').read_text())
 function=next(n for n in tree.body if isinstance(n,ast.FunctionDef)and n.name=='main');assignment=function.body[0]
 extra=eval(compile(ast.Expression(assignment.value),'<allocator source list>','eval'),{'SOURCES':SOURCES})
 sources.extend(ROOT/p for p in extra);sources.append(ROOT/'rebuild/tests/integration/InstallerDestructorAllocatorSupport.cpp')
if args.constructor:
 from gate_retail_bank_alias_insertion import SOURCES as aliases
 sources.extend(p for p in aliases[:-1]if p.stem not in ['STL_Mfind_0042b467','STL_Rb_global_Rebalance_0042971d'])
 sources.append(ROOT/'rebuild/src/compiled/00/9c/CFileInstaller_Constructor_009c5890.cpp')
if args.workers:
 sources.extend(ROOT/'rebuild/src/compiled'/p for p in ['01/3c/FileInstaller_Data_013ca818.cpp','00/9c/CFileInstaller_Workers_009c5870.cpp','00/9c/CFileInstaller_ThreadMain_009c5690.cpp','00/9c/CFileInstaller_DecompressionThreadMain_009c4230.cpp','00/a3/CCompressorZlib_HasStreamInput_00a3a7e0.cpp'])
if args.singleton:
 sources.extend(ROOT/'rebuild/src/compiled'/p for p in ['00/40/CFileInstallerSingleton_Get_00404440.cpp','00/40/CFileInstallerSingleton_Release_00404490.cpp','00/40/FileInstallerCountedReset_00403db0.cpp','00/40/FileInstallerDestroy_00403630.cpp'])
sources=list(dict.fromkeys(sources));required=[0]
for i,source in enumerate(sources):
 obj=w/('composition%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT']+(['/DFABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT']if args.buffers else [])+['/Fo'+str(obj),str(source)],build.env,w);assert r.returncode==0,(source,r.stdout,r.stderr)
 if source.name=='CCompressorZlib_CCompressorZlib_00a3acc0.cpp':
  blob=obj.read_bytes();old=b'??1CBaseClass@@UAE@XZ\0';new=b'@BDtor@4\0';assert blob.count(old)==1;obj.write_bytes(blob.replace(old,new+b'\0'*(len(old)-len(new))))
 if source.name=='CFileInstaller_Constructor_009c5890.cpp' and not args.workers:
  raw=bytearray(obj.read_bytes());ptr,count=struct.unpack_from('<II',raw,8);start=ptr+count*18;needle=b'__beginthread\0';value=b'_ThreadRecording\0';pos=raw.index(needle,start);delta=len(value)-len(needle);offset=pos-start
  for n in range(count):
   at=ptr+n*18
   if raw[at:at+4]==b'\0'*4:
    oldOffset=struct.unpack_from('<I',raw,at+4)[0]
    if oldOffset>offset:struct.pack_into('<I',raw,at+4,oldOffset+delta)
  struct.pack_into('<I',raw,start,struct.unpack_from('<I',raw,start)[0]+delta);obj.write_bytes(raw[:pos]+value+raw[pos+len(needle):])
 objects.append(obj)
 if source.name in ['CFileInstallerSingleton_Get_00404440.cpp','CFileInstallerSingleton_Release_00404490.cpp','FileInstallerCountedReset_00403db0.cpp','FileInstallerDestroy_00403630.cpp','FileInstaller_Data_013ca818.cpp','CFileInstaller_Workers_009c5870.cpp','CFileInstaller_ThreadMain_009c5690.cpp','CFileInstaller_DecompressionThreadMain_009c4230.cpp','CCompressorZlib_HasStreamInput_00a3a7e0.cpp','CFileInstaller_Constructor_009c5890.cpp','InstallerCompletedFileSet_Erase_004aba70.cpp','InstallerActiveFileList_Clear_009c71a0.cpp','InstallerCountedFileList_Clear_009c6cc0.cpp','CBasicRetailBankAliasMap_Erase_009aa830.cpp','CCompressorZlib_CCompressorZlib_00a3acc0.cpp','FileInstallerClearBuffers_009c5110.cpp','FileInstallerBufferArray_009c7150.cpp','FileInstallerDestroyBufferRange_009c6d60.cpp']:required.append(len(objects)-1)
exe=w/'composition.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
for omit in required:
 r=subprocess.run(cmd+[str(p)for i,p in enumerate(objects)if i!=omit],env=build.env,capture_output=True,text=True);assert r.returncode!=0,omit
r=subprocess.run(cmd+list(map(str,objects)),env=build.env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
r=subprocess.run([str(exe)],capture_output=True,text=True,timeout=45);assert r.returncode==0 and (('INSTALLER_NATIVE_SINGLETON_OK cases=64 workers=128 events=256' in r.stdout) if args.singleton else ('INSTALLER_NATIVE_WORKERS_OK cases=64 workers=128 events=256' in r.stdout) if args.workers else ('INSTALLER_LIFETIME_OK'if args.constructor else 'INSTALLER_DESTRUCTOR_BUFFERS_OK'if args.buffers else 'INSTALLER_DESTRUCTOR_CONTAINERS_OK')+' cases=96 fixture_threads=96 events=384'in r.stdout),(r.returncode,r.stdout,r.stderr)
report=dict(cases=96,fixture_threads=96,event_handles=384,omission_checks=len(required),boundary='Real destructor, lock/Windows synchronization, recovered alias/completed/active/counted cleanup and string ownership; real zlib stream lifetimes and CRT release. Shared fixture file object final owner destroys once after streams. ClearBuffers/range and compressor base remain recorded; full constructor/game worker loops deferred.',providers=[dict(source=str(p),sha256=hashlib.sha256(p.read_bytes()).hexdigest())for p in [ROOT/'rebuild/src/compiled/00/9c/CFileInstaller_Destructor_009c5170.cpp']+sources])
if args.buffers:report['boundary']='Real destructor, lock/Windows synchronization, recovered containers/strings/zlib and full ClearBuffers/array/range/30-class allocator composition. Two live owned buffers and null middle chunk, actual GlobalAlloc/GlobalFree handles; main allocation count zero after each destruction. Compressor base recorded; constructor/game workers deferred; original retained allocator chunks fixture-released after shutdown.'
if args.constructor:report.update(recorded_constructor_thread_calls=192,boundary='Actual polymorphic constructor and destructor in96 lifetimes, constructor creates all five sentinels/fourWindows events and real .biz->.big alias through recovered insertion. Fixture adds real containers/strings/zlib/mainallocator buffers; actual destructor releases all ownership and events. Constructor thread launch recorded, 96fixtureshutdown threads execute; compressor base completion and CFileInfo object body remain fixture boundaries.')
if args.workers:report.update(cases=64,fixture_threads=0,recorded_constructor_thread_calls=0,native_workers=128,event_handles=256,boundary='Actual constructor, CRT _beginthread, native worker entries and both recovered loops with real Windows locks/waits/signals, actual destructor and empty container cleanup. All processing callbacks abort if reached; no files queued in this idle-startup/shutdown composition.')
if args.singleton:report.update(boundary='Actual singleton Get/cached Get/Release, counted control allocation and virtual destruction, actual CRT allocation, constructor and both CRT worker loops, real Windows handles. Empty file queues; processing callbacks abort if reached.')
(ROOT/'rebuild/compile-gate'/('installer-native-singleton.json'if args.singleton else 'installer-native-workers.json'if args.workers else 'installer-lifetime.json'if args.constructor else 'destructor-buffers.json'if args.buffers else 'destructor-containers.json')).write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
