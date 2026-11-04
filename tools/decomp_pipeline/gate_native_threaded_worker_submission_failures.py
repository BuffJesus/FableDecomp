"""Actual manager construction/startup/destruction around owned native IO."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86_const import X86_OP_MEM,X86_OP_IMM
import check_one as V
import threaded_pending_removal_oracle as M
import threaded_submission_tree_oracle as T
import gate_native_threaded_manager_add as A
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
import gate_native_threaded_manager_statistics as statistics
from types import SimpleNamespace
finish=SimpleNamespace(ROOTS=[r for r in finish_gate.ROOTS if r["name"]=="on_finish_object"],ADDRESS=finish_gate.ADDRESS)
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2]
WINMM=Path('C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x86/winmm.lib')
FIXTURE=R/'rebuild/tests/integration/NativeThreadedWorkerSubmissionFailures_test.cpp'
ABI=FIXTURE.with_name('NativeThreadedReadAPC_win32.h')
SOURCES=[FIXTURE,worker.SOURCE,callback.SOURCE,callback.DATA_SOURCE,begin.F,exit_gate.F,shutdown.F,finish.ROOTS[0]["source"]]
SOURCES.extend(R/"rebuild/src/compiled"/p for p in ("00/40/CFileInstallerSingleton_Get_00404440.cpp","00/9c/CFileInstaller_Pause_009c31e0.cpp","00/9c/CFileInstaller_Unpause_009c3250.cpp","01/3c/CFileInstaller_DirtyDisc_013ca814.cpp","00/a6/CCriticalSectionLock_Constructor_00a62920.cpp","00/a6/CCriticalSectionLock_Release_00a62960.cpp","00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp"))
SOURCES.extend(R/'rebuild/src/compiled'/p for p in ('00/9e/global_GFGetTime_009e1bc0.cpp','01/3c/GFGetTimeData_013caa00.cpp'))
SOURCES.extend(R/'rebuild/src/compiled'/p for p in ('00/98/CThreadedFileManager_Constructor_0098f3a0.cpp','00/98/CThreadedFileManager_Destructor_0098e690.cpp','00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/STLThreadedUIDTree_KeyErase_00990d60.cpp','00/98/CThreadedFileManager_SetDeviceTimeSlice_0098e170.cpp','00/42/STLRbTree_RebalanceForErase_0042a34e.cpp','00/41/STLRbTree_Extrema_0041c7e0.cpp'))
SOURCES.extend((queries.SOURCE,queries.FIND_SOURCE))
SOURCES.append(A.SOURCE)
SOURCES.extend(p for p in T.SOURCES+[r['source'] for r in M.roots] if p not in SOURCES)
PRIORITY=R/'rebuild/src/compiled/00/98/CThreadedFileManager_SetObjectPriority_0098eff0.cpp'
UID_FIND=next(r for r in finish_gate.ROOTS if r['name']=='uid_find')
SOURCES.extend((PRIORITY,UID_FIND['source']))

INSTALLER_LOOP=R/'rebuild/src/compiled/00/9c/CFileInstaller_ThreadMain_009c5690.cpp'
INSTALLER_ENTRY=R/'rebuild/src/compiled/00/9c/CFileInstaller_Workers_009c5870.cpp'
INSTALLER_LOCK=R/'rebuild/src/compiled/00/a6/CCriticalSectionLock_Lock_00a62980.cpp'
SOURCES.extend((INSTALLER_LOOP,INSTALLER_ENTRY,INSTALLER_LOCK))

WAIT_SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_WaitForObjectToFinish_0098f1e0.cpp'
SOURCES.append(WAIT_SOURCE)

FILE_WRAPPERS=[('CThreadedFile_AddObject_0098f350.cpp','?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z',0x98f350,19,'?AddObject@CThreadedFileManager@@QAEKABVCThreadedFileObjectDesc@@AAVCThreadedFile@@@Z',0x98f0f0),('CThreadedFile_WaitForObjectToFinish_0098f370.cpp','?WaitForObjectToFinish@CThreadedFile@@QAEXK@Z',0x98f370,10,'?WaitForObjectToFinish@CThreadedFileManager@@QAEXK@Z',0x98f1e0),('CThreadedFile_SetObjectPriority_0098f0e0.cpp','?SetObjectPriority@CThreadedFile@@QAEXKW4EThreadedFilePriority@@@Z',0x98f0e0,10,'?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z',0x98eff0)]
SOURCES.extend(R/'rebuild/src/compiled/00/98'/row[0] for row in FILE_WRAPPERS)

MANAGER_REMOVE=next(r for r in finish_gate.ROOTS if r['name']=='remove_object')
UID_ITERATOR_ERASE=dict(source=R/'rebuild/src/compiled/00/99/STLThreadedUIDTree_IteratorErase_00990430.cpp',symbol=finish_gate.ERASE,address=0x990430,size=50)
FILE_REMOVE=dict(source=R/'rebuild/src/compiled/00/98/CThreadedFile_RemoveObject_0098ec90.cpp',symbol='?RemoveObject@CThreadedFile@@QAE_NK@Z',address=0x98ec90,size=10)
REMOVE_ROOTS=[MANAGER_REMOVE,UID_ITERATOR_ERASE,FILE_REMOVE]
SOURCES.extend(r['source'] for r in REMOVE_ROOTS if r['source'] not in SOURCES)

SOURCES.extend(root['source'] for root in statistics.ROOTS)

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-worker-submission-failures/00/98/0098ecd0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 build.mkdir(parents=True,exist_ok=False)
 inc=build/'include';inc.mkdir();src=build/'sources';src.mkdir()
 inputs={Path(__file__),Path(M.__file__),Path(T.__file__),Path(A.__file__),R/"tools/decomp_pipeline/movable_pool_construct_oracle.py",EXE,ABI,WINMM,Path(worker.__file__),Path(callback.__file__),Path(begin.__file__),Path(exit_gate.__file__),Path(shutdown.__file__),Path(finish_gate.__file__),Path(installer_pause.__file__),Path(installer_unpause.__file__),Path(construction.__file__),Path(destruction.__file__),Path(update.__file__),Path(queries.__file__)}
 inputs.add(Path(statistics.__file__))
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
 model_build=build/'tree-model';model_build.mkdir()
 original,rebuilt,tree_audits,tree_objects,addresses=M.build_codes(model_build)
 behavior=M.compare_removal(original,rebuilt)
 assert (behavior['cases'],behavior['removals'],len(behavior['controls']))==(360,2016,13)
 selected=list(tree_audits)
 def add_selected(symbol,index,address=None,size=None):
  if any(a['symbol']==symbol for a in selected):return
  c=Coff(objects[index]);sec,off=c.symbols[symbol];assert off==0;body,rels=c.sections[sec]
  if size is not None:assert len(body)==size
  selected.append(dict(symbol=symbol,address=address,bytes=len(body),body=body.hex(),relocations=[dict(site=site,symbol=n,kind=k) for site,(n,k) in rels.items()]))
 for root in M.roots:
  index=SOURCES.index(root['source']);add_selected(root['symbol'],index,root['address'],len(rebuilt[root['address']]))
  c=Coff(objects[index]);sec,off=c.symbols[root['symbol']];body,rels=c.sections[sec];patched=bytearray(body)
  for site,(name,kind) in rels.items():
   addend=struct.unpack_from('<i',body,site)[0];target=addresses[name]+addend
   struct.pack_into('<I',patched,site,(target-(root['address']+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(patched)==rebuilt[root['address']]
 add_selected(A.SYMBOL,26,0x98f0f0,244)
 c=Coff(objects[26]);sec,off=c.symbols[A.SYMBOL];body,rels=c.sections[sec]
 assert compare(body,read_virtual_data(EXE.read_bytes(),0x98f0f0,240),tuple(rels),retail_relocations('0098f0f0',240)).near_match
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
 original_calls=[(i.operands[0].imm if i.operands[0].type==X86_OP_IMM else i.operands[0].mem.disp)&0xffffffff for i in md.disasm(read_virtual_data(EXE.read_bytes(),0x98f0f0,240),0x98f0f0) if i.mnemonic=='call']
 assert sorted(original_calls)==sorted(A.ADDRESS[name] for name,kind in rels.values())
 add_selected(worker.SYMBOL,1,worker.ENTRY)
 add_selected(callback.SYMBOL,2)
 for index,gate in ((4,begin),(5,exit_gate),(6,shutdown),(7,finish)):add_selected(gate.ROOTS[0]['symbol'],index)
 for index,symbol,*_ in profiles:add_selected(symbol,index)
 for index,symbol in ((15,'?GFGetTime@@YINXZ'),(17,construction.SYMBOL),(18,destruction.SYMBOL),(21,'?KeepUpdating@CThreadedFileManager@@QAEXXZ')):add_selected(symbol,index)
 for root in queries.ROOTS:add_selected(root['symbol'],25 if root['name']=='const_find' else 24)
 priority_root=finish_gate.NEAR_ROOTS[0]
 for source,symbol,address,size,targets in ((PRIORITY,priority_root['symbol'],0x98eff0,151,finish_gate.NEAR_ADDRESS),(UID_FIND['source'],UID_FIND['symbol'],UID_FIND['address'],UID_FIND['size'],finish_gate.ADDRESS)):
  targets=dict(finish_gate.ADDRESS,**targets)
  index=SOURCES.index(source);add_selected(symbol,index,address,size)
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];patched=bytearray(body)
  for site,(name,kind) in rels.items():
   addend=struct.unpack_from('<i',body,site)[0];target=targets[name]+addend
   struct.pack_into('<I',patched,site,(target-(address+site+4 if kind==20 else 0))&0xffffffff)
  original=read_virtual_data(EXE.read_bytes(),address,size)
  for site,(name,kind) in rels.items():
   target=targets[name]+struct.unpack_from('<i',body,site)[0]
   assert (address+site+4+struct.unpack_from('<i',original,site)[0])&0xffffffff==target if kind==20 else struct.unpack_from('<I',original,site)[0]==target
  if source==PRIORITY:
   priority_grade=compare(body,original,tuple(rels),retail_relocations('0098eff0',151)).to_dict();assert priority_grade['near_match']
  else:assert bytes(patched)==original,symbol
 installer_targets=dict(installer_pause.ADDRESS)
 installer_targets.update({'?Lock@CCriticalSectionLock@@QAEXXZ':0xa62980,'?PostFileReadCompletion@CFileInstaller@@QAEXPAVCActiveRead@1@@Z':0x9c4120,'?ProcessActiveFileForWrite@CFileInstaller@@QAEXAAVCActiveFile@1@@Z':0x9c3fb0,'?ProcessReads@CFileInstaller@@QAEXXZ':0x9c53a0,'?ProcessAlways@CFileInstaller@@QAEXXZ':0x9c4f20,'__imp__GetCurrentThread@0':0x143fdcc,'__imp__SetThreadPriority@8':0x143fdd0,'?ThreadMain@CFileInstaller@@QAEXXZ':0x9c5690})
 installer_loop_grades=[]
 for source,symbol,address,original_size,exact_loop in ((INSTALLER_LOOP,'?ThreadMain@CFileInstaller@@QAEXXZ',0x9c5690,469,False),(INSTALLER_ENTRY,'?FableInstallerWorker_009C5870@@YAXPAX@Z',0x9c5870,24,True),(INSTALLER_LOCK,'?Lock@CCriticalSectionLock@@QAEXXZ',0xa62980,25,True)):
  index=SOURCES.index(source);add_selected(symbol,index,address)
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0
  original=read_virtual_data(EXE.read_bytes(),address,original_size);patched=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);addend=struct.unpack_from('<i',body,site)[0];target=installer_targets[name]+addend
   struct.pack_into('<I',patched,site,(target-(address+site+4 if kind==20 else 0))&0xffffffff)
  if exact_loop:assert bytes(patched)==original,(symbol,len(body),original_size)
  profile=compare(bytes(patched),original).to_dict();assert profile['near_match']
  installer_loop_grades.append(dict(symbol=symbol,address=hex(address),grade=profile,acceptance_grade='MATCH' if exact_loop else 'NEAR_MATCH'))
 wait_symbol='?WaitForObjectToFinish@CThreadedFileManager@@QAEXK@Z'
 wait_index=SOURCES.index(WAIT_SOURCE);add_selected(wait_symbol,wait_index,0x98f1e0,187)
 c=Coff(objects[wait_index]);sec,off=c.symbols[wait_symbol];body,rels=c.sections[sec];assert off==0
 wait_targets=dict(finish_gate.ADDRESS,**finish_gate.NEAR_ADDRESS)
 wait_targets.update({'?SetObjectPriority@CThreadedFileManager@@QAEXKW4EThreadedFilePriority@@@Z':0x98eff0,'__imp__SetEvent@4':0x143fde0,'__imp__SleepEx@8':0x143fddc})
 patched=bytearray(body)
 for site,(name,kind) in rels.items():
  assert kind in (6,20);target=wait_targets[name]+struct.unpack_from('<i',body,site)[0]
  struct.pack_into('<I',patched,site,(target-(0x98f1e0+site+4 if kind==20 else 0))&0xffffffff)
 wait_grade=compare(bytes(patched),read_virtual_data(EXE.read_bytes(),0x98f1e0,181)).to_dict();assert wait_grade['near_match']
 file_wrapper_grades=[]
 for filename,symbol,address,size,child,target in FILE_WRAPPERS:
  source=R/'rebuild/src/compiled/00/98'/filename;index=SOURCES.index(source);add_selected(symbol,index,address,size)
  c=Coff(objects[index]);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(rels)==2
  original=read_virtual_data(EXE.read_bytes(),address,size);patched=bytearray(body)
  targets={'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':0x13bc870,child:target}
  for site,(name,kind) in rels.items():
   assert kind in (6,20) and struct.unpack_from('<i',body,site)[0]==0
   expected=targets[name];old=struct.unpack_from('<i' if kind==20 else '<I',original,site)[0]
   assert (address+site+4+old if kind==20 else old)==expected
   struct.pack_into('<I',patched,site,(expected-(address+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(patched)==original
  file_wrapper_grades.append(dict(symbol=symbol,address=hex(address),bytes=size,grade='MATCH'))
 remove_targets=dict(finish_gate.ADDRESS)
 remove_targets.update({r['symbol']:r['address'] for r in REMOVE_ROOTS})
 remove_targets.update({'?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z':0x98e770,'?g_FableThreadedFileManager_013BC870@@3VCThreadedFileManager@@A':0x13bc870})
 remove_grades=[]
 for root in REMOVE_ROOTS:
  index=SOURCES.index(root['source']);add_selected(root['symbol'],index,root['address'],root['size'])
  c=Coff(objects[index]);sec,off=c.symbols[root['symbol']];body,rels=c.sections[sec];assert off==0
  patched=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);target=remove_targets[name]+struct.unpack_from('<i',body,site)[0]
   struct.pack_into('<I',patched,site,(target-(root['address']+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(patched)==read_virtual_data(EXE.read_bytes(),root['address'],root['size']),root['symbol']
  remove_grades.append(dict(symbol=root['symbol'],address=hex(root['address']),bytes=root['size'],grade='MATCH'))
 statistics_grades=[]
 for root in statistics.ROOTS:
  index=SOURCES.index(root['source']);add_selected(root['symbol'],index,root['address'],root['size'])
  c=Coff(objects[index]);sec,off=c.symbols[root['symbol']];body,rels=c.sections[sec];assert off==0 and len(rels)==2
  patched=bytearray(body);original=read_virtual_data(EXE.read_bytes(),root['address'],root['size'])
  for site,(name,kind) in rels.items():
   assert kind==6 and struct.unpack_from('<i',body,site)[0]==0
   target=statistics.ADDRESS[name];assert struct.unpack_from('<I',original,site)[0]==target
   struct.pack_into('<I',patched,site,target)
  assert bytes(patched)==original
  statistics_grades.append(dict(symbol=root['symbol'],address=hex(root['address']),bytes=root['size'],grade='MATCH'))
 # Canonical /Os insertion rebalance must precede inline /O2 COMDAT alternatives.
 preferred=objects[SOURCES.index(T.SOURCES[0])]
 exports=['/export:Root%d=%s'%(i,a['symbol']) for i,a in enumerate(selected)]+['/export:LiveManager=FixtureGlobalManagerStorage,DATA']
 def verify(exe):
  with pefile.PE(str(exe)) as pe:
   names={x.name.decode():x.address for x in pe.DIRECTORY_ENTRY_EXPORT.symbols if x.name}
   live=[section for section in pe.sections if section.Name.rstrip(b'\0')==b'.livemgr'];assert len(live)==1 and names['LiveManager']==live[0].VirtualAddress
   for i,a in enumerate(selected):
    body=bytearray(pe.get_data(names['Root%d'%i],a['bytes']));expected_body=bytearray.fromhex(a['body'])
    for rel in a['relocations']:
     body[rel['site']:rel['site']+4]=b'\0'*4
     expected_body[rel['site']:rel['site']+4]=b'\0'*4
    assert body==expected_body,a['symbol']
 def link(name,objs):
  objs=([preferred]+[p for p in objs if p!=preferred]) if preferred in objs else objs
  exe=build/(name+'.exe');result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib',str(WINMM),*exports],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,result=link('probe',objects);assert result.returncode==0,result.stdout+result.stderr;verify(exe)
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  cold=[section for section in pe.sections if section.Name.rstrip(b'\0')==b'.livemgr']
  assert len(cold)==1 and cold[0].Misc_VirtualSize==4096 and cold[0].VirtualAddress%4096==0
  assert not cold[0].Characteristics&0x20000000
  imports={(d.dll.decode().lower(),x.name.decode()) for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports if x.name}
  for name in ('ReadFileEx','WriteFileEx','WaitForSingleObjectEx','SetEvent','InitializeCriticalSection','EnterCriticalSection','LeaveCriticalSection','DeleteCriticalSection','GetCurrentThreadId','GetThreadPriority','SetThreadPriority','QueryPerformanceCounter','QueryPerformanceFrequency','CreateEventW','CreateToolhelp32Snapshot','Thread32First','Thread32Next','OpenThread','VirtualProtect'):
   assert ('kernel32.dll',name) in imports,name
  assert ('winmm.dll','timeGetTime') in imports
 def run(name,exe):
  result=subprocess.run([str(exe),str(data),str(writable)],cwd=build,capture_output=True,text=True,timeout=180)
  save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return result
 result=run('positive',exe);assert result.returncode==0,result.stdout+result.stderr
 assert 'NATIVE_WORKER_SUBMISSION_FAILURES_PASS batches=24 requests=672 failed_reads=12 failed_writes=12 no_completions=24 shutdowns=24 exit_objects=672 overlap_frees=672 installer_joins=24 acknowledgments=24 write_error_callbacks=12 read_cleanup_unpauses=12 statistics_checks=720' in result.stdout
 baseline=data.read_bytes();assert writable.read_bytes()==baseline,'Failed submissions must not write any bytes'
 omissions=[];redundant=[];selection_controls=[]
 for i in range(1,len(objects)):
  omitted,result=link('omit%d'%i,[o for n,o in enumerate(objects) if n!=i])
  if result.returncode:omissions.append(str(SOURCES[i]));continue
  try:verify(omitted)
  except AssertionError:selection_controls.append(str(SOURCES[i]));continue
  redundant.append(str(SOURCES[i]))
 def changed(source,old,new):
  text=source.read_text(encoding='utf-8');assert text.count(old)==1,(source,old);return text.replace(old,new)
 controls=[
  ('no-worker-entry',4,'#include "fable_gfuninitialise.h"\nvoid __cdecl CThreadedFileManager::BeginThread(void*) {}\n'),
  ('no-Pause',9,'#include "fable_file_installer.h"\nvoid CFileInstaller::Pause() {}\n'),
  ('no-Unpause',10,'#include "fable_file_installer.h"\nvoid CFileInstaller::Unpause() {}\n'),
  ('false-exists',24,changed(queries.SOURCE,'return it!=end;','return false;')),
  ('no-pending-removal',SOURCES.index(M.roots[-1]['source']),'#include "fable_threaded_file_object.h"\nvoid CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject*) {}\n'),
  ('wrong-submission-uid',26,changed(A.SOURCE,'    return uid;','    return uid+1;')),
  ('no-idle-worker',SOURCES.index(INSTALLER_ENTRY),changed(INSTALLER_ENTRY,'static_cast<CFileInstaller*>(object)->ThreadMain();',';')),
  ('no-pause-ack',SOURCES.index(INSTALLER_LOOP),changed(INSTALLER_LOOP,'SetEvent(*reinterpret_cast<void**>(bytes+0x8c));',';')),
  ('no-pause-wait',9,changed(SOURCES[9],'WaitForSingleObjectEx(PauseStartEvent, 0xffffffffUL, 1);',';')),
  ('no-write-error-callback',1,changed(worker.SOURCE,'if (m_harddiskErrorCallback) m_harddiskErrorCallback("");',';')),
  ('no-write-unpause',1,changed(worker.SOURCE,'if (g_CFileInstallerSingleton_013CA818.object) CFileInstallerSingleton::Get().Unpause();',';')),
  ('unpause-failed-read',1,changed(worker.SOURCE,'ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size,object->Overlapped,OnFileReadCompletion);','ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size,object->Overlapped,OnFileReadCompletion); if (g_CFileInstallerSingleton_013CA818.object) CFileInstallerSingleton::Get().Unpause();')),
  ('publish-failed-completion',1,changed(worker.SOURCE,'    WaitForSingleObjectEx(Event,0xffffffffUL,1);','    if(CurrentlyReadingObject) CurrentlyReadingObject->LoadingStatus=FILE_LOADING_STATUS_READ_COMPLETED; WaitForSingleObjectEx(Event,0xffffffffUL,1);')),
  ('no-exit-delete',5,changed(exit_gate.F,'delete object;',';')),
  ('lose-exit-overlap',5,changed(exit_gate.F,'delete object;','object->Overlapped=0;delete object;')),
  ('wrong-file-uid',SOURCES.index(R/'rebuild/src/compiled/00/98'/FILE_WRAPPERS[0][0]),changed(R/'rebuild/src/compiled/00/98'/FILE_WRAPPERS[0][0],'return g_FableThreadedFileManager_013BC870.AddObject','return 1+g_FableThreadedFileManager_013BC870.AddObject')),
 ]
 for root in statistics.ROOTS:
  if root['finished']:continue
  controls.append(('zero-'+root['name'],SOURCES.index(root['source']),'#include "fable_threaded_file_object.h"\nlong CThreadedFileManager::'+root['name']+'() const {return 0;}\n'))
 # Reading status must survive a failed OS submission; no synthetic completion.
 controls.append(('drop-current-object',1,worker.SOURCE.read_text().replace('CurrentlyReadingObject = object;','CurrentlyReadingObject = 0;')))
 assert len(controls)==19
 outcomes=[]
 for name,index,text in controls:
  control=src/(name+'.cpp');control.write_text(text);obj=build/(name+'.obj')
  result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
  save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
  replacement=list(objects);replacement[index]=obj;bad,result=link(name,replacement);assert result.returncode==0,result.stdout+result.stderr
  result=run(name,bad);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,(name,result.returncode,result.stdout,result.stderr)
  outcomes.append(dict(name=name,returncode=result.returncode,detected=True))
 assert writable.read_bytes()==baseline
 assert sha(data)==data_hash and all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),batches=24,requests=672,failed_reads=12,failed_writes=12,thread_joins=24,shutdowns=24,exit_objects=672,overlap_frees=672,installer_joins=24,acknowledgments=24,write_error_callbacks=12,read_cleanup_unpauses=12,no_completions=24,statistics_checks=720,new_roots=0,worker_grade=grade,owner_grades=owner_grades,query_roots=query_roots,exact_roots=exact,installer_roots=installer_roots,installer_loop_grades=installer_loop_grades,selected_bodies=selected,behavior=behavior,link_omissions=omissions,redundant_sources=redundant,selection_controls=selection_controls,runtime_controls=19,control_outcomes=outcomes,input_sha256=pins,object_sha256={str(o):sha(o) for o in objects+list(model_build.glob('*.obj'))},executable_sha256=sha(exe),synthetic_sha256=data_hash,owned_writes_sha256=sha(writable),boundaries=[
 '24 constructed global managers/24 real installer threads each receive28 actual file-wrapper submissions:672 allocated requests. Controlled file descriptors contain invalid handle values; forwarding ReadFileEx/WriteFileEx observers verify real Windows ERROR_INVALID_HANDLE failures, not injected returns. Only24 OS submissions attempted:12 reads/12 writes.',
 'After failure the selected request stays reading and remains the manager current object/UID; the other27 remain pending. No OS or virtual completion callback runs, no UID retires, pending count/bytes include all28. Failed reads leave one installer pause outstanding; failed writes call the actual hard-disk error policy callback with empty text and execute actual installer Unpause.720 independent statistics checks confirm empty/submission/failure state.',
 'Actual Uninitialise/ThreadMain deletes all672 requests/overlaps once; manager destruction releases all live UID and pending nodes/headers. Only after the read worker is joined does fixture cleanup explicitly invoke installer Unpause; this is not claimed as production failure recovery.Only12 reads require this fixture cleanup action.',
 'Installer context/list heads and idle ProcessReads/ProcessAlways remain controlled; actual entry/ThreadMain/Pause/Unpause and OS events execute. No installer filesystem or decompression work. File descriptors/invalid handles are fixture-owned; no file open/close/lifetime claim. Predecessor remains functional-only; no new root credit.',
 'All64 linked bodies are preserved from the successful composition and selected-byte audited; wait, priority, caller-retirement and finished-statistics roots are cold in this failure fixture and are not newly claimed as executed. Existing success receipts retain those claims. Actual manager current pointer remains dangling after ThreadMain exit by original design; fixture never dereferences it after retirement.',
 ])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-worker-submission-failures.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_WORKER_SUBMISSION_FAILURES_PASS batches=24 requests=672 failed_reads=12 failed_writes=12 no_completions=24 shutdowns=24 exit_objects=672 overlap_frees=672 installer_joins=24 acknowledgments=24 write_error_callbacks=12 read_cleanup_unpauses=12 statistics_checks=720 bodies=%d link_omissions=%d selection_controls=%d runtime_controls=19'%(len(selected),len(omissions),len(selection_controls)))
 print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
