"""Typed bank-data StartRead in the native callback-submitted Windows IO graph."""
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
FIXTURE=R/'rebuild/tests/integration/NativeBankDataStartRead_test.cpp'
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

START_SOURCE=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_StartRead_009d5650.cpp'
START_SYMBOL='?StartRead@CBankFileAsyncData@@AAEXXZ'
SOURCES.append(START_SOURCE)

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-bank-data-start-read/00/9d/009d5650'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
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
 add_selected(START_SYMBOL,SOURCES.index(START_SOURCE),0x9d5650,26)
 c=Coff(objects[SOURCES.index(START_SOURCE)]);sec,off=c.symbols[START_SYMBOL];body,rels=c.sections[sec]
 assert off==0 and rels=={17:('?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z',20)}
 patched=bytearray(body);assert patched[17:21]==b'\0'*4
 struct.pack_into('<i',patched,17,0x98f350-(0x9d5650+21))
 assert bytes(patched)==read_virtual_data(EXE.read_bytes(),0x9d5650,26)
 exact.append(dict(symbol=START_SYMBOL,address='0x9d5650',bytes=26,grade='MATCH'))
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
 assert 'NATIVE_BANK_START_READ_PASS batches=24 requests=960 reads=864 writes=0 joins=24 shutdowns=24 callbacks=288 immediate_deletions=576 exit_objects=144 overlap_frees=960 installer_joins=24 acknowledgments=864 pause_waits=864 resumes=864 wait_calls=1656 blocking_batches=24 global_managers=24 file_submissions=960 file_priorities=312 caller_retirements=144 protected_refusals=1440 missing_removals=264 statistics_checks=1560 statistic_calls=6240 eof_apcs=576 hard_errors=288 dvd_errors=288 pending_cancellations=96 pending_tree_checks=120 zero_submissions=24 zero_priority_moves=24 zero_immediate=12 zero_caller=12 child_submissions=288 child_bytes=6216' in result.stdout
 reuse=re.search(r'ALLOCATION_REUSE requests=(\d+) overlaps=(\d+)',result.stdout);assert reuse
 request_reuse,overlap_reuse=map(int,reuse.groups())
 baseline=data.read_bytes();expected=baseline
 assert writable.read_bytes()==expected,'EOF reads must not modify owned output'
 omissions=[];redundant=[];selection_controls=[]
 for i in range(1,len(objects)):
  omitted,result=link('omit%d'%i,[o for n,o in enumerate(objects) if n!=i])
  if result.returncode:omissions.append(str(SOURCES[i]));continue
  try:verify(omitted)
  except AssertionError:selection_controls.append(str(SOURCES[i]));continue
  redundant.append(str(SOURCES[i]))
 control=src/'noop.cpp';control.write_text('#include "fable_gfuninitialise.h"\nvoid __cdecl CThreadedFileManager::BeginThread(void*) {}\n')
 obj=build/'noop.obj';result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
 save('noop.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0
 noexe,result=link('noop',[*objects[:4],obj,*objects[5:]]);assert result.returncode==0
 result=run('noop',noexe);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,result.stderr
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
 assert result.returncode==80 and 'ObjectExists' in result.stderr,result.stderr
 new_controls=[('no-pending-removal',SOURCES.index(M.roots[-1]['source']),'#include "fable_threaded_file_object.h"\nvoid CThreadedFileManager::RemoveObjectFromPendingMap(CFileObject*) {}\n'),('wrong-submission-uid',26,A.SOURCE.read_text(encoding='utf-8').replace('    return uid;','    return uid+1;'))]
 new_controls.append(('no-priority-change',SOURCES.index(PRIORITY),'#include "fable_threaded_file_object.h"\nvoid CThreadedFileManager::SetObjectPriority(unsigned long,EThreadedFilePriority) {}\n'))
 new_controls.extend([('ignore-locked-priority',SOURCES.index(PRIORITY),PRIORITY.read_text(encoding='utf-8').replace('object->LoadingStatus==FILE_LOADING_STATUS_PENDING_LOCKED','false')),('ignore-begin-refusal',1,worker.SOURCE.read_text(encoding='utf-8').replace('        if (failed) {','        if (false) {'))])
 finish_source=SOURCES[7].read_text(encoding='utf-8')
 for name,old,new in (('retain-completed','if (!object->Desc.QueueWhenFinished)', 'if (false)'),('omit-uid-erase','Objects.erase(object->UID);',';'),('omit-request-delete','delete object;',';'),('retire-retained','if (!object->Desc.QueueWhenFinished)','if (true)')):
  assert finish_source.count(old)==1
  new_controls.append((name,7,finish_source.replace(old,new)))
 new_controls.extend([
  ('no-worker-entry',SOURCES.index(INSTALLER_ENTRY),INSTALLER_ENTRY.read_text().replace('static_cast<CFileInstaller*>(object)->ThreadMain();',';')),
  ('no-pause-ack',SOURCES.index(INSTALLER_LOOP),INSTALLER_LOOP.read_text().replace('SetEvent(*reinterpret_cast<void**>(bytes+0x8c));',';')),
  ('no-pause-wait',9,SOURCES[9].read_text().replace('WaitForSingleObjectEx(PauseStartEvent, 0xffffffffUL, 1);',';')),
 ])
 new_controls.append(('no-blocking-wait',SOURCES.index(WAIT_SOURCE),'#include "fable_threaded_file_object.h"\nvoid CThreadedFileManager::WaitForObjectToFinish(unsigned long) {}\n'))
 new_controls.extend([
  ('wrong-file-uid',SOURCES.index(R/'rebuild/src/compiled/00/98'/FILE_WRAPPERS[0][0]),(R/'rebuild/src/compiled/00/98'/FILE_WRAPPERS[0][0]).read_text().replace('return g_FableThreadedFileManager_013BC870.AddObject','return 1+g_FableThreadedFileManager_013BC870.AddObject')),
  ('no-file-wait',SOURCES.index(R/'rebuild/src/compiled/00/98'/FILE_WRAPPERS[1][0]),'#include "fable_threaded_file.h"\nvoid CThreadedFile::WaitForObjectToFinish(unsigned long) {}\n'),
  ('no-file-priority',SOURCES.index(R/'rebuild/src/compiled/00/98'/FILE_WRAPPERS[2][0]),'#include "fable_threaded_file.h"\nvoid CThreadedFile::SetObjectPriority(unsigned long,EThreadedFilePriority) {}\n'),
 ])
 remove_source=MANAGER_REMOVE['source'].read_text()
 new_controls.extend([
  ('omit-caller-delete',SOURCES.index(MANAGER_REMOVE['source']),remove_source.replace('delete object;',';')),
  ('omit-caller-uid-erase',SOURCES.index(MANAGER_REMOVE['source']),remove_source.replace('Objects.erase(it);',';')),
  ('wrong-missing-return',SOURCES.index(MANAGER_REMOVE['source']),remove_source.replace('return true;','return false;')),
  ('allow-locked-removal',SOURCES.index(MANAGER_REMOVE['source']),remove_source.replace('object->LoadingStatus==FILE_LOADING_STATUS_PENDING_LOCKED ||','false ||')),
  ('false-file-remove',SOURCES.index(FILE_REMOVE['source']),'#include "fable_threaded_file.h"\nbool CThreadedFile::RemoveObject(unsigned long) {return false;}\n'),
 ])
 for root in statistics.ROOTS:
  new_controls.append(('zero-'+root['name'],SOURCES.index(root['source']),'#include "fable_threaded_file_object.h"\nlong CThreadedFileManager::'+root['name']+'() const {return 0;}\n'))
 apc_source=callback.SOURCE.read_text(encoding='utf-8')
 for name,old,new in (
  ('no-hard-error-policy','m_harddiskErrorCallback("");',';'),
  ('no-dvd-dirty-policy','m_bDirtyDiscError = true;',';'),
  ('ignore-apc-error','if (error) {','if (false) {'),
  ('no-error-unpause','CFileInstallerSingleton::Get().Unpause();',';'),
  ('no-error-completed-state','object->LoadingStatus = FILE_LOADING_STATUS_READ_COMPLETED;','object->LoadingStatus = FILE_LOADING_STATUS_READING;'),
 ):
  assert apc_source.count(old)==1
  new_controls.append((name,2,apc_source.replace(old,new)))
 finish_source=SOURCES[7].read_text();assert finish_source.count('OnFileReadCompletion(true);')==1
 new_controls.append(('false-virtual-completion',7,finish_source.replace('OnFileReadCompletion(true);','OnFileReadCompletion(false);')))
 for name,old,new in (
  ('allow-reading-removal','object->LoadingStatus==FILE_LOADING_STATUS_READING ||','false ||'),
  ('allow-completed-removal','object->LoadingStatus==FILE_LOADING_STATUS_READ_COMPLETED','false'),
  ('skip-pending-cancel-erase','if (object->LoadingStatus==FILE_LOADING_STATUS_PENDING)','if (false)'),
  ('no-pending-cancel','if (object->LoadingStatus==FILE_LOADING_STATUS_PENDING)','if(object->LoadingStatus==FILE_LOADING_STATUS_PENDING) {LeaveCriticalSection(section);return true;} if (false)'),
 ):
  assert remove_source.count(old)==1
  new_controls.append((name,SOURCES.index(MANAGER_REMOVE['source']),remove_source.replace(old,new)))
 pending_source=M.roots[-1]['source'].read_text();assert pending_source.count('++current;')==1
 new_controls.append(('stale-pending-cursor',SOURCES.index(M.roots[-1]['source']),pending_source.replace('++current;',';')))
 zero_add=A.SOURCE.read_text();assert zero_add.count('    return uid;')==1
 new_controls.append(('reject-zero-uid',26,zero_add.replace('    return uid;','    return uid ? uid : 1;')))
 zero_priority=PRIORITY.read_text();marker='    _RTL_CRITICAL_SECTION* section='
 assert zero_priority.count(marker)==1
 new_controls.append(('ignore-zero-priority',SOURCES.index(PRIORITY),zero_priority.replace(marker,'    if(!uid) return;\n'+marker)))
 zero_queries=queries.SOURCE.read_text()
 for name,method,value in (('false-zero-exists','ObjectExists','false'),('true-zero-finished','IsObjectFinished','true')):
  marker='bool CThreadedFileManager::'+method+'(unsigned long uid) const\n{'
  assert zero_queries.count(marker)==1
  new_controls.append((name,24,zero_queries.replace(marker,marker+'\n    if(!uid) return '+value+';')))
 marker='bool CThreadedFileManager::RemoveObject(unsigned long uid)\n{'
 assert remove_source.count(marker)==1
 new_controls.append(('ignore-zero-removal',SOURCES.index(MANAGER_REMOVE['source']),remove_source.replace(marker,marker+'\n    if(!uid) return true;')))
 child_add=A.SOURCE.read_text()
 for name,old,new in (
  ('wrong-child-uid','    return uid;','    return desc.FilePos<0x9000 ? uid+1 : uid;'),
  ('fake-child-admission','    unsigned long uid=NextObjectUID++;','    unsigned long uid=NextObjectUID++; if(desc.FilePos<0x9000) {LeaveCriticalSection(section);return uid;}'),
  ('skip-child-pending','    AddObjectToPendingMap(object);','    if(desc.FilePos>=0x9000) AddObjectToPendingMap(object);'),
  ('retain-child','    object->Desc=desc;','    object->Desc=desc; if(desc.FilePos<0x9000) object->Desc.QueueWhenFinished=true;'),
  ('wrong-child-buffer','    object->Desc=desc;','    object->Desc=desc; if(desc.FilePos<0x9000) object->Desc.PDst=0;'),
 ):
  assert child_add.count(old)==1
  new_controls.append((name,26,child_add.replace(old,new)))
 child_worker=worker.SOURCE.read_text()
 old='ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size,object->Overlapped,OnFileReadCompletion);'
 assert child_worker.count(old)==1
 new_controls.append(('short-child-read',1,child_worker.replace(old,'ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size-(object->Desc.FilePos<0x9000),object->Overlapped,OnFileReadCompletion);')))
 old='object->Overlapped->Offset = object->Desc.FilePos;';assert child_worker.count(old)==2
 new_controls.append(('shift-child-read',1,child_worker.replace(old,'object->Overlapped->Offset = object->Desc.FilePos+(object->Desc.FilePos<0x9000);')))
 finish_source=SOURCES[7].read_text();old='        LeaveCriticalSection(section);';assert finish_source.count(old)==1
 new_controls.append(('callback-keeps-lock',7,finish_source.replace(old,'        ;')))
 start_source=START_SOURCE.read_text()
 for name,old,new in (
  ('start-no-call','ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);',';'),
  ('start-no-store','ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);','BankFile.ThreadedFile->AddObject(LoadDesc);'),
  ('start-wrong-uid','ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);','ThreadedFileUID = 1+BankFile.ThreadedFile->AddObject(LoadDesc);'),
  ('start-change-desc','ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);','++LoadDesc.FilePos; ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);'),
  ('start-change-bank','ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);','++BankFile.Size; ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc);'),
 ):
  assert start_source.count(old)==1
  new_controls.append((name,SOURCES.index(START_SOURCE),start_source.replace(old,new)))
 for name,index,text in new_controls:
  control=src/(name+'.cpp');control.write_text(text);obj=build/(name+'.obj')
  result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
  save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0
  replacement=list(objects);replacement[index]=obj;bad,result=link(name,replacement);assert result.returncode==0,result.stdout+result.stderr
  result=run(name,bad);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,result.stdout+result.stderr
 assert writable.read_bytes()==bytes(expected)
 assert sha(data)==data_hash and all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),batches=24,requests=960,real_reads=864,real_writes=0,thread_joins=24,shutdowns=24,completion_callbacks=288,immediate_deletions=576,exit_objects=144,overlap_frees=960,installer_joins=24,acknowledgments=864,pause_waits=864,resumes=864,installer_loop_grades=installer_loop_grades,wait_calls=1656,blocking_batches=24,wait_grade=wait_grade,global_managers=24,file_submissions=960,file_priorities=312,file_wrapper_grades=file_wrapper_grades,caller_retirements=144,protected_refusals=1440,missing_removals=264,remove_grades=remove_grades,statistics_checks=1560,statistic_calls=6240,statistics_grades=statistics_grades,eof_apcs=576,hard_errors=288,dvd_errors=288,pending_cancellations=96,pending_tree_checks=120,locked_refusals=288,reading_refusals=864,completed_refusals=288,uid_start_values=[0xfffffff0,0xfffffffe],zero_submissions=24,zero_priority_moves=24,zero_immediate=12,zero_caller=12,zero_exit=0,child_submissions=288,child_bytes=6216,successful_apcs=288,request_address_reuses=request_reuse,overlap_address_reuses=overlap_reuse,new_roots=0,worker_grade=grade,owner_grades=owner_grades,query_roots=query_roots,exact_roots=exact,installer_roots=installer_roots,selected_bodies=selected,behavior=behavior,link_omissions=omissions,redundant_sources=redundant,selection_controls=selection_controls,runtime_controls=58,input_sha256=pins,object_sha256={str(o):sha(o) for o in objects+list(model_build.glob('*.obj'))},executable_sha256=sha(exe),synthetic_sha256=data_hash,owned_writes_sha256=sha(writable),boundaries=[
 'Typed private CBankFileAsyncData::StartRead at009D5650 (26B exact) submits all288 follow-up requests through the actual file wrapper/manager. Bank/data storage is fixture-owned, guarded and initialized for this ABI probe; no bank constructor/lifetime claim. Entire storage is unchanged except ThreadedFileUID+18; LoadDesc+2c and bank ThreadedFile+110 use real shared types.',
 '24 constructed global managers initially receive28 parent requests each.96 pending requests are canceled with120 independent pending-tree validations.288 parent virtual completion callbacks submit one follow-up read through actual CThreadedFile/AddObject with the manager lock unowned on API entry/return. Fixture locks only for pre/post observation; production AddObject owns admission and returns a UID/node/request/overlap. Child pending-node membership is checked immediately after admission, before any wait or worker use of that queued request.',
 '288 child reads have independent buffers and no callback/retention. All succeed through Windows ReadFileEx/APC, transferring6216 bytes whose values and guards are checked independently from the synthetic file formula. Parent576 EOF APCs still exercise288 HDD callbacks/288 DVD dirty flags and original completion(true) policy.864 real reads/installer pause-acknowledgment-resume cycles total; output unchanged.',
 '960 allocations retire once:96 pending cancels,576 immediate completions (288 parents/288 children),144 caller-retired finished and144 exit-owned. Active ownership lookup skips already-retired generations so legitimate CRT/GlobalAlloc address reuse is accepted; actual reuse counts recorded. No request/overlap dereference after retirement. Independent live unsigned UID tree/model checked at child admission, with original exit-owned dangling nodes explicitly retained only until destructor.',
 '1440 actual protected-state refusals (288 locked/864 reading/288 completed),264 repeated/unrelated missing removals,1656 waits/24 forced blocking batches and1560 independent statistics checkpoints/6240 getters pass. Callback-time statistics model derives current parent pending/previous settled requests from single-worker event state, then includes the independently described child; getter internals are not used as expected values.',
 'Unsigned UID wrap/24 zero submissions and queued moves/12 immediate and12 caller zero retirements remain covered.312 explicit file-priority changes include288 callback changes plus24 zero moves. Children receive consecutive unique nonzero UIDs after parent allocation.',
 'Installer context/list heads and idle processing remain controlled; actual worker entry/ThreadMain/Pause/Unpause/OS events execute. Raw file descriptors/handles are fixture-owned; no production file open/close/lifetime or full startup claim. Actual manager new/delete uses guarded global-symbol storage. Predecessor remains functional-only; typed StartRead replaces its legacy facade without new root credit.',
 ])
 report['priority_changes']=312;report['begin_refusals']=288;report['begin_retries']=288;report['priority_grade']=priority_grade
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-data-start-read.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_DATA_START_READ_PASS batches=24 requests=960 reads=864 writes=0 joins=24 shutdowns=24 callbacks=288 immediate_deletions=576 exit_objects=144 overlap_frees=960 installer_joins=24 acknowledgments=864 pause_waits=864 resumes=864 wait_calls=1656 blocking_batches=24 global_managers=24 file_submissions=960 file_priorities=312 caller_retirements=144 protected_refusals=1440 missing_removals=264 statistics_checks=1560 statistic_calls=6240 eof_apcs=576 hard_errors=288 dvd_errors=288 pending_cancellations=96 pending_tree_checks=120 zero_submissions=24 zero_priority_moves=24 zero_immediate=12 zero_caller=12 child_submissions=288 child_bytes=6216 bodies=%d link_omissions=%d selection_controls=%d runtime_controls=58'%(len(selected),len(omissions),len(selection_controls)))
 print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
