"""Actual manager construction/startup/destruction around owned native IO."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_MEM,X86_OP_IMM
import check_one as V
import threaded_process_oracle as worker
import gate_native_threaded_read_completion as callback
import gate_native_threaded_worker_begin as begin
import gate_native_threaded_worker_exit as exit_gate
import gate_native_threaded_manager_shutdown as shutdown
import gate_native_threaded_file_manager_remove as finish_gate
import gate_native_installer_pause as installer_pause
import installer_unpause_oracle as installer_unpause
from near_match import compare,retail_relocations
import threaded_manager_construct_oracle as construction
import threaded_manager_destroy_oracle as destruction
import gate_native_threaded_manager_update as update
import threaded_query_oracle as queries
from types import SimpleNamespace
finish=SimpleNamespace(ROOTS=[r for r in finish_gate.ROOTS if r["name"]=="on_finish_object"],ADDRESS=finish_gate.ADDRESS)
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2]
WINMM=Path('C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x86/winmm.lib')
FIXTURE=R/'rebuild/tests/integration/NativeThreadedWorkerOwner_test.cpp'
ABI=FIXTURE.with_name('NativeThreadedReadAPC_win32.h')
SOURCES=[FIXTURE,worker.SOURCE,callback.SOURCE,callback.DATA_SOURCE,begin.F,exit_gate.F,shutdown.F,finish.ROOTS[0]["source"]]
SOURCES.extend(R/"rebuild/src/compiled"/p for p in ("00/40/CFileInstallerSingleton_Get_00404440.cpp","00/9c/CFileInstaller_Pause_009c31e0.cpp","00/9c/CFileInstaller_Unpause_009c3250.cpp","01/3c/CFileInstaller_DirtyDisc_013ca814.cpp","00/a6/CCriticalSectionLock_Constructor_00a62920.cpp","00/a6/CCriticalSectionLock_Release_00a62960.cpp","00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp"))
SOURCES.extend(R/'rebuild/src/compiled'/p for p in ('00/9e/global_GFGetTime_009e1bc0.cpp','01/3c/GFGetTimeData_013caa00.cpp'))
SOURCES.extend(R/'rebuild/src/compiled'/p for p in ('00/98/CThreadedFileManager_Constructor_0098f3a0.cpp','00/98/CThreadedFileManager_Destructor_0098e690.cpp','00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp','00/98/CThreadedFileManager_SetDeviceTimeSlice_0098e170.cpp','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp'))
SOURCES.extend((queries.SOURCE,queries.FIND_SOURCE))
SOURCES.append(R/'rebuild/src/compiled/00/98/CThreadedFileObject_Constructor_0098df00.cpp')

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-worker-owner/00/98/0098ecd0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 build.mkdir(parents=True,exist_ok=False)
 inc=build/'include';inc.mkdir();src=build/'sources';src.mkdir()
 inputs={Path(__file__),EXE,ABI,WINMM,Path(worker.__file__),Path(callback.__file__),Path(begin.__file__),Path(exit_gate.__file__),Path(shutdown.__file__),Path(finish_gate.__file__),Path(installer_pause.__file__),Path(installer_unpause.__file__),Path(construction.__file__),Path(destruction.__file__),Path(update.__file__),Path(queries.__file__)}
 inputs.update((V.VC/"include").rglob("*.h"))
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py'):
  inputs.add(R/'tools/decomp_pipeline'/name)
 visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   if name=='original_fixups.h':assert p==FIXTURE;continue
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None)
   assert q,(p,name)
   closure(q)
 for p in SOURCES:closure(p)
 for p in [V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll')]+[V.VC/'lib'/n for n in ('kernel32.lib','winmm.lib','libcmt.lib','oldnames.lib')]:
  if p.is_file():inputs.add(p)
 pins={str(p):sha(p) for p in sorted(inputs)}
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('input-pins.json',pins);(build/'gate-source.py').write_bytes(Path(__file__).read_bytes())
 for p in visited-set(SOURCES):
  q=inc/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else src/p.name
  q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 snapshots=[]
 for i,p in enumerate(SOURCES):
  q=src/('%d_'%i+p.name);q.write_bytes(p.read_bytes());snapshots.append(q)
 data=build/'synthetic.bin';data.write_bytes(bytes((i*37+23)&255 for i in range(0x9000)));data_hash=sha(data)
 writable=build/'owned-writes.bin';writable.write_bytes(data.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)+';'+str(src)
 flags=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(inc),'/I'+str(src)]
 objects=[]
 for i,p in enumerate(snapshots):
  obj=build/('provider%d.obj'%i);result=V.cl([*flags,'/Fo'+str(obj),str(p)],env,build)
  save('compile%d.json'%i,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr))
  assert result.returncode==0,result.stdout+result.stderr
  shown=[]
  for line in result.stdout.splitlines():
   if 'including file:' in line.lower():
    q=Path(line.split(':',2)[-1].strip()).resolve();assert q.is_relative_to(build) or q.is_relative_to(V.VC/'include'),q;shown.append(q)
  assert shown
  objects.append(obj)
 # The frozen worker object must equal a separately fresh-compiled, gated one;
 # executable linking performs all named host relocations normally.
 accepted,grade=worker.built()
 coff=Coff(objects[1]);section,off=coff.symbols[worker.SYMBOL];body,rels=coff.sections[section]
 assert off==0 and len(body)==len(accepted)
 patched=bytearray(body)
 for site,(symbol,kind) in rels.items():
  assert struct.unpack_from('<i',body,site)[0]==0
  target=worker.ADDRESS[symbol];struct.pack_into('<I',patched,site,(target-(worker.ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==accepted
 exact=[]
 for index,gate in ((4,begin),(5,exit_gate),(6,shutdown),(7,finish)):
  root=gate.ROOTS[0];coff=Coff(objects[index]);section,off=coff.symbols[root['symbol']]
  body,relocations=coff.sections[section];assert off==0 and len(body)==root['size'] and len(relocations)==root['relocations']
  original=read_virtual_data(EXE.read_bytes(),root['address'],root['size']);copy=bytearray(body)
  for site,(symbol,kind) in relocations.items():
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0
   target=gate.ADDRESS[symbol]
   old=struct.unpack_from('<i' if kind==20 else '<I',original,site)[0]
   assert (root['address']+site+4+old if kind==20 else old)==target
   struct.pack_into('<I',copy,site,(target-(root['address']+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(copy)==original
  exact.append(dict(symbol=root['symbol'],address=hex(root['address']),bytes=len(body),grade='MATCH'))
 installer_roots=[]
 get_map={'?g_CFileInstallerSingleton_013CA818@@3UCCountedFileInstaller@@A':0x13ca818,'?FableFileInstallerAllocate@@YAPAXI@Z':0xbfea1a,'??0CFileInstaller@@QAE@XZ':0x9c5890,'??4CCountedFileInstaller@@QAEXPAVCFileInstaller@@@Z':0x403db0}
 profiles=[(8,'?Get@CFileInstallerSingleton@@SIAAVCFileInstaller@@XZ',0x404440,69,get_map,True),
  (9,installer_pause.SYMBOL,installer_pause.ENTRY,103,installer_pause.ADDRESS,True),
  (10,installer_unpause.SYMBOL,installer_unpause.ENTRY,89,installer_unpause.ADDRESS,False),
  (12,'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',0xa62920,None,installer_pause.ADDRESS,True),
  (13,'?Release@CCriticalSectionLock@@QAEXXZ',0xa62960,None,installer_pause.ADDRESS,True),
  (14,'??1CCriticalSectionLock@@QAE@XZ',0xa629a0,None,installer_pause.ADDRESS,True)]
 for index,symbol,address,size,targets,is_exact in profiles:
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0
  if size is not None:assert len(body)==size
  original=read_virtual_data(EXE.read_bytes(),address,len(body));copy=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0
   target=targets[name];old=struct.unpack_from('<i' if kind==20 else '<I',original,site)[0]
   assert (address+site+4+old if kind==20 else old)==target
   struct.pack_into('<I',copy,site,(target-(address+site+4 if kind==20 else 0))&0xffffffff)
  if is_exact:assert bytes(copy)==original
  else:assert compare(bytes(copy),original).near_match
  installer_roots.append(dict(symbol=symbol,address=hex(address),bytes=len(body),grade='MATCH' if is_exact else 'NEAR_MATCH'))
 clock_targets={'?g_FableTimerInitialised_013CAA10@@3_NA':0x13caa10,'?g_FableHighResolutionTimer_013CAA00@@3_NA':0x13caa00,
  '?g_FableTimerFrequency_013CAA08@@3_JA':0x13caa08,'?g_FableTimerStartTick_013CB2F0@@3_JA':0x13cb2f0,
  '__imp__QueryPerformanceCounter@4':0x143fe00,'__imp__QueryPerformanceFrequency@4':0x143fe04,'__imp__timeGetTime@0':0x14404b4,'__real@3f50624dd2f1a9fc':0x126f430}
 c=Coff(objects[15]);sec,off=c.symbols['?GFGetTime@@YINXZ'];body,rels=c.sections[sec]
 assert off==0 and len(body)==168 and len(rels)==14
 original=read_virtual_data(EXE.read_bytes(),0x9e1bc0,168);copy=bytearray(body)
 for site,(symbol,kind) in rels.items():
  assert kind==6
  addend=struct.unpack_from('<i',body,site)[0];target=clock_targets[symbol]+addend
  assert struct.unpack_from('<I',original,site)[0]==target
  struct.pack_into('<I',copy,site,target)
 assert bytes(copy)==original and read_virtual_data(EXE.read_bytes(),0x126f430,8)==struct.pack('<d',0.001)
 exact.append(dict(symbol='?GFGetTime@@YINXZ',address='0x9e1bc0',bytes=168,grade='MATCH'))
 owner_grades=[]
 for index,module,size in ((17,construction,350),(18,destruction,214)):
  c=Coff(objects[index]);sec,off=c.symbols[module.SYMBOL];body,rels=c.sections[sec];assert off==0 and len(body)==size
  _,targets=module.providers();patched=bytearray(body)
  for site,(symbol,kind) in rels.items():
   assert kind in (6,20)
   addend=struct.unpack_from('<i',body,site)[0];target=targets[symbol]+addend
   struct.pack_into('<I',patched,site,(target-(module.ENTRY+site+4 if kind==20 else 0))&0xffffffff)
  original=read_virtual_data(EXE.read_bytes(),module.ENTRY,size)
  grade_owner=compare(bytes(patched),original,tuple(rels),retail_relocations('%08x'%module.ENTRY,size)).to_dict()
  assert grade_owner['near_match']
  owner_grades.append(dict(symbol=module.SYMBOL,address=hex(module.ENTRY),bytes=size,grade=grade_owner))
 c=Coff(objects[21]);symbol='?KeepUpdating@CThreadedFileManager@@QAEXXZ';sec,off=c.symbols[symbol];body,rels=c.sections[sec]
 assert off==0 and len(body)==24
 copy=bytearray(body)
 for site,(symbol,kind) in rels.items():
  assert kind==6 and struct.unpack_from('<i',body,site)[0]==0
  struct.pack_into('<I',copy,site,update.ADDRESS[symbol])
 assert bytes(copy)==read_virtual_data(EXE.read_bytes(),0x98e190,24)
 exact.append(dict(symbol='?KeepUpdating@CThreadedFileManager@@QAEXXZ',address='0x98e190',bytes=24,grade='MATCH'))
 query_roots=[]
 for root in queries.ROOTS:
  index=25 if root['name']=='const_find' else 24
  c=Coff(objects[index]);section,off=c.symbols[root['symbol']];body,rels=c.sections[section];assert off==0
  targets=dict(queries.ADDRESS);targets.update({r['symbol']:r['address'] for r in queries.ROOTS});copy=bytearray(body)
  for site,(symbol,kind) in rels.items():
   assert kind in (6,20);add=struct.unpack_from('<i',body,site)[0]
   struct.pack_into('<I',copy,site,(targets[symbol]+add-(root['address']+site+4 if kind==20 else 0))&0xffffffff)
  original=read_virtual_data(EXE.read_bytes(),root['address'],root['original_size']);query_grade=compare(bytes(copy),original).to_dict();assert query_grade['near_match']
  if root['grade']=='MATCH':assert bytes(copy)==original
  query_roots.append(dict(name=root['name'],grade=query_grade,acceptance_grade=root['grade']))
 def link(name,objs):
  exe=build/(name+'.exe');result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib',str(WINMM)],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,result=link('probe',objects);assert result.returncode==0,result.stdout+result.stderr
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  cold=[section for section in pe.sections if section.Name.rstrip(b'\0')==b'.coldmgr']
  assert len(cold)==1 and cold[0].Misc_VirtualSize==4096 and cold[0].VirtualAddress%4096==0
  assert not cold[0].Characteristics&0x20000000
  imports={(d.dll.decode().lower(),x.name.decode()) for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports if x.name}
  for name in ('ReadFileEx','WriteFileEx','WaitForSingleObjectEx','SetEvent','InitializeCriticalSection','EnterCriticalSection','LeaveCriticalSection','DeleteCriticalSection','GetCurrentThreadId','GetThreadPriority','SetThreadPriority','QueryPerformanceCounter','QueryPerformanceFrequency','CreateEventW','DuplicateHandle','VirtualProtect'):
   assert ('kernel32.dll',name) in imports,name
  assert ('winmm.dll','timeGetTime') in imports
 def run(name,exe):
  result=subprocess.run([str(exe),str(data),str(writable)],cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return result
 result=run('positive',exe);assert result.returncode==0,result.stdout+result.stderr
 assert 'NATIVE_WORKER_OWNER_PASS cases=672 reads=336 writes=336 joins=672 shutdowns=672 callbacks=336 exit_objects=672' in result.stdout
 baseline=data.read_bytes();expected=bytearray(baseline)
 for p in range(7):
  for i in range(p*17,p*17+17+p):expected[i]=baseline[i]^0x5a
 assert writable.read_bytes()==bytes(expected),'Owned file writes differ from requested ranges'
 omissions=[]
 required=[(1,worker.SYMBOL),(2,callback.SYMBOL),(3,'?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA'),(4,begin.ROOTS[0]['symbol']),(5,exit_gate.ROOTS[0]['symbol']),(6,shutdown.ROOTS[0]['symbol']),(7,finish.ROOTS[0]['symbol'])]
 required.extend((index,symbol) for index,symbol,*_ in profiles)
 required.append((11,'?DirtyDisc@CFileInstaller@@1_NA'))
 required.extend(((15,'?GFGetTime@@YINXZ'),(16,'?g_FableTimerInitialised_013CAA10@@3_NA'),(17,construction.SYMBOL),(18,destruction.SYMBOL),(19,'??0CBaseClassNonCopyable@@QAE@XZ'),(20,next(r['symbol'] for r in finish_gate.ROOTS if r['name']=='uid_node_erase')),(21,'?KeepUpdating@CThreadedFileManager@@QAEXXZ'),(22,'?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'),(23,'?_S_maximum@_Rb_tree_node_base@_STL@@SAPAU12@PAU12@@Z')))
 required.extend(((24,'?IsObjectFinished@CThreadedFileManager@@QBE_NK@Z'),(25,queries.FIND)))
 required.append((26,'??0CFileObject@CThreadedFileManager@@QAE@XZ'))
 for i,symbol in required:
  _,result=link('omit%d'%i,[o for n,o in enumerate(objects) if n!=i]);diag=result.stdout+result.stderr
  assert result.returncode and re.search(r'LNK(?:2001|2019)',diag) and (symbol in diag or (i in (1,2,4,11) and any(alias in diag for alias in ('FixtureProcess','FixtureReadCompletion','FixtureBeginThread','FixtureInstallerDirty')))),diag
  omissions.append(str(SOURCES[i]))
 control=src/'noop.cpp';control.write_text('#include "fable_gfuninitialise.h"\nvoid __cdecl CThreadedFileManager::BeginThread(void*) {}\n')
 obj=build/'noop.obj';result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
 save('noop.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0
 noexe,result=link('noop',[*objects[:4],obj,*objects[5:]]);assert result.returncode==0
 result=run('noop',noexe);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr and 'GetTickCount()-started<5000' in result.stderr,result.stderr
 # No-op installer methods must fail independently of the entry control.
 for index,method in ((9,'Pause'),(10,'Unpause')):
  control=src/('noop-'+method+'.cpp');control.write_text('#include "fable_file_installer.h"\nvoid CFileInstaller::'+method+'() {}\n')
  obj=build/('noop-'+method+'.obj');result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
  save('noop-'+method+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0
  replacement=list(objects);replacement[index]=obj
  bad,result=link('noop-'+method,replacement);assert result.returncode==0
  result=run('noop-'+method,bad)
  assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,result.stderr
 control=src/'false-exists.cpp';text=queries.SOURCE.read_text(encoding='utf-8')
 assert text.count('return it!=end;')==1
 control.write_text(text.replace('return it!=end;','return false;'))
 obj=build/'false-exists.obj';result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
 save('false-exists.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0
 replacement=list(objects);replacement[24]=obj
 bad,result=link('false-exists',replacement);assert result.returncode==0
 result=run('false-exists',bad)
 assert result.returncode==80 and 'view.ObjectExists' in result.stderr,result.stderr
 assert writable.read_bytes()==bytes(expected)
 assert sha(data)==data_hash and all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),cases=672,real_reads=336,real_writes=336,thread_joins=672,shutdowns=672,completion_callbacks=336,exit_objects=672,
  worker_grade=grade,owner_grades=owner_grades,query_roots=query_roots,exact_roots=exact,installer_roots=installer_roots,installer_runtime_controls=2,query_false_control='DETECTED',link_omissions=omissions,runtime_noop_entry_control='DETECTED',
  input_sha256=pins,object_sha256={str(o):sha(o) for o in objects},executable_sha256=sha(exe),synthetic_sha256=data_hash,owned_writes_sha256=sha(writable),
  boundaries=['672 actual manager constructors allocate 15 distinct map headers each and start their own CRT _beginthread into actual BeginThread/ThreadMain/ProcessObjects and deliver actual read/write APCs on the initiating thread.',
   'Actual SetThreadPriority(1) observed on the IO thread, main/worker IDs differ, critical-section recursion and callback/unlock ordering checked.',
   'Main observes actual OnFinishObject status4 publication under the manager critical section, calls actual Uninitialise, wakes worker, waits for ThreadFinished and joins.',
   'Actual OnFinishObject invokes 336 virtual completion callbacks outside the manager lock, clears callbacks, publishes finished and clears current.',
   'Each retained request is CRT-new allocated and its OVERLAPPED GlobalAlloc allocated. A real one-node UID tree makes actual ThreadMain exit execute GlobalFree and operator delete on 672 owned requests.',
   'Object/OVERLAPPED state validated before shutdown; no freed pointers dereferenced afterward. Free result is ignored by original, not separately observed. UID nodes/header remain fixture-owned and dangling values are never dereferenced after join.',
   'Cold UID erase/rebalance/extrema are linked real dependencies; no removal-on-finish claim. Pending removal controlled, empties queue and frees its fixture-owned node.',
   'Actual existing-singleton Get/Pause/Unpause/scoped locks execute on the IO thread with raw installer and real owned critical section/events; allocation/constructor/assignment cold paths trap.',
   '12 pause-count/running/dirty combinations per base case cover initial 0/1/FFFFFFFF and counter wrap. Pause acknowledgment event is pre-signaled; installer worker/acknowledgment production excluded.',
   'Actual exact GFGetTime with loader-zero timer data initializes through real QueryPerformanceCounter/Frequency; initialized high-resolution state and positive frequency/time checked. WinMM fallback linked but not exercised.',
   'Actual const IsObjectFinished/ObjectExists observe pending/finished/missing UID, including recursive entry during setup. These APIs replace the raw status polling fixture.',
   'Actual KeepUpdating wakes the owner-created worker; duplicated owned thread handles are joined after actual Uninitialise. Actual deleting destructor releases UID node and 15 map headers. No raw manager owner boundary remains.',
   'File wrappers use a separate cold global-manager link page guarded PAGE_NOACCESS; they are tested in the differential gate, not executed by this native owner probe.',
   'Installer still raw with pre-signaled acknowledgment; pending insertion/removal controlled. No actual game assets, saves or installed files. Entire owned output checked.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-worker-owner.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_WORKER_OWNER_PASS cases=672 reads=336 writes=336 joins=672 shutdowns=672 callbacks=336 exit_objects=672 link_omissions=26 runtime_controls=4')
 print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
