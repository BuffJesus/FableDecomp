"""Native public bank BeginReadData through real Windows IO and counted-owner teardown."""
import hashlib,json,re,struct,subprocess,sys
from datetime import datetime,timezone
from pathlib import Path
import pefile
import bank_begin_worker_link_oracle as banklink
import bank_begin_worker_controls as bankcontrols
import bank_shared_worker_controls as sharedcontrols
import bank_pending_owner_controls as pendingcontrols
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
FIXTURE=R/'rebuild/tests/integration/NativePendingOwner_test.cpp'
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

SOURCES.extend(banklink.SOURCES)

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-bank-pending-owner/00/9d/009d6060'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 build.mkdir(parents=True,exist_ok=False)
 inc=build/'include';inc.mkdir();src=build/'sources';src.mkdir()
 inputs={Path(__file__),Path(M.__file__),Path(T.__file__),Path(A.__file__),R/"tools/decomp_pipeline/movable_pool_construct_oracle.py",EXE,ABI,WINMM,Path(worker.__file__),Path(callback.__file__),Path(begin.__file__),Path(exit_gate.__file__),Path(shutdown.__file__),Path(finish_gate.__file__),Path(installer_pause.__file__),Path(installer_unpause.__file__),Path(construction.__file__),Path(destruction.__file__),Path(update.__file__),Path(queries.__file__)}
 inputs.add(Path(statistics.__file__))
 inputs.add(Path(banklink.__file__));inputs.add(Path(bankcontrols.__file__));inputs.add(Path(sharedcontrols.__file__));inputs.add(Path(pendingcontrols.__file__))
 for module in (banklink.ctor,banklink.buffer,banklink.teardown,banklink.tree,banklink.key):inputs.add(Path(module.__file__))
 inputs.add(R/'rebuild/tests/integration/NativeAsyncPointerTreeKeyErase_test.py')
 inputs.add(R/'tools/decomp_pipeline/gate_native_bank_entry_begin_composition.py')

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
  obj=build/('provider%d.obj'%i);result=V.cl([*flags,*banklink.flags(p),'/Fo'+str(obj),str(p)],env,build)
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
 bank_grades,bank_symbols,bank_tables=banklink.audit(objects,SOURCES,add_selected)
 bank_aliases=banklink.aliases(objects,snapshots[0])
 bank_model_run=subprocess.run([sys.executable,str(R/'tools/decomp_pipeline/gate_native_bank_entry_begin_composition.py')],capture_output=True,text=True,timeout=120)
 save('bank-model.run.json',dict(returncode=bank_model_run.returncode,stdout=bank_model_run.stdout,stderr=bank_model_run.stderr))
 assert bank_model_run.returncode==0,bank_model_run.stdout+bank_model_run.stderr
 bank_model=json.loads((R/'rebuild/compile-gate/native-bank-entry-begin-composition.json').read_text())
 save('bank-model.receipt.json',bank_model)
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
  banklink.verify_bindings(exe,selected,bank_symbols,bank_tables,bank_aliases)
 def link(name,objs):
  objs=([preferred]+[p for p in objs if p!=preferred]) if preferred in objs else objs
  exe=build/(name+'.exe');result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),'/map:'+str(exe.with_suffix('.map')),*map(str,objs),'kernel32.lib',str(WINMM),*exports],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,result=link('probe',objects);assert result.returncode==0,result.stdout+result.stderr;verify(exe)
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  cold=[section for section in pe.sections if section.Name.rstrip(b'\0')==b'.livemgr']
  assert len(cold)==1 and cold[0].Misc_VirtualSize==4096 and cold[0].VirtualAddress%4096==0
  assert not cold[0].Characteristics&0x20000000
  imports={(d.dll.decode().lower(),x.name.decode()) for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports if x.name}
  for name in ('ReadFileEx','WriteFileEx','WaitForSingleObjectEx','SetEvent','InitializeCriticalSection','EnterCriticalSection','LeaveCriticalSection','DeleteCriticalSection','GetCurrentThreadId','SetThreadPriority','QueryPerformanceCounter','QueryPerformanceFrequency','CreateEventW','CreateToolhelp32Snapshot','Thread32First','Thread32Next','OpenThread','VirtualProtect'):
   assert ('kernel32.dll',name) in imports,name
  assert ('winmm.dll','timeGetTime') in imports
 def run(name,exe):
  result=subprocess.run([str(exe),str(data),str(writable)],cwd=build,capture_output=True,text=True,timeout=180)
  save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return result
 result=run('positive',exe);assert result.returncode==0,result.stdout+result.stderr
 assert 'NATIVE_PENDING_OWNER_PASS batches=24 owners=288 pending_removals=288 real_reads=0 joins=24 contexts=48 tree_checks=624 partial_deletes=240' in result.stdout
 two_child=re.search(r'two_child_deletes=(\d+) manager_checks=312 data_deletes=288 info_deletes=288 node_deletes=288',result.stdout)
 assert two_child and int(two_child[1])>=12
 baseline=data.read_bytes();assert writable.read_bytes()==baseline
 omissions=[];redundant=[];selection_controls=[]
 for i in range(1,len(objects)):
  omitted,result=link('omit%d'%i,[o for n,o in enumerate(objects) if n!=i])
  if result.returncode:omissions.append(str(SOURCES[i]));continue
  try:verify(omitted)
  except AssertionError:selection_controls.append(str(SOURCES[i]));continue
  redundant.append(str(SOURCES[i]))
 cases=pendingcontrols.controls(SOURCES)
 for name,index,text in cases:
  control=src/(name+'.cpp');control.write_text(text);obj=build/(name+'.obj')
  result=V.cl([*flags,*banklink.flags(SOURCES[index]),'/Fo'+str(obj),str(control)],env,build)
  save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,result.stdout+result.stderr
  replacement=list(objects);replacement[index]=obj;bad,result=link(name,replacement);assert result.returncode==0,result.stdout+result.stderr
  result=run(name,bad);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,result.stdout+result.stderr
 assert len(cases)==41 and writable.read_bytes()==baseline and sha(data)==data_hash
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),batches=24,owners=288,pending_removals=288,real_reads=0,apcs=0,read_callbacks=0,thread_joins=24,shared_contexts=48,shared_tree_checks=624,partial_deletes=240,two_child_deletes=int(two_child[1]),manager_checks=312,bank_data_deletes=288,bank_info_deletes=288,bank_node_deletes=288,bank_enters=576,bank_leaves=576,bank_tries=0,uid_starts=[1,0x7ffffff8,0x80000000,0xffffffe0],release_orders=[6,6,6,6],runtime_controls=len(cases),control_names=[name for name,index,text in cases],selected_bodies=selected,bank_grades=bank_grades,bank_tables=bank_tables,bank_model=bank_model,behavior=behavior,link_omissions=omissions,selection_controls=selection_controls,redundant_sources=redundant,input_sha256=pins,object_sha256={str(o):sha(o) for o in objects+list(model_build.glob('*.obj'))},executable_sha256=sha(exe),synthetic_sha256=data_hash,new_roots=0,boundaries=[
  'Actual manager constructor starts its worker. Fixture sets recovered BlockPendingObjectProcessingRefCount+16C under its real CS before admission, and observes at least two worker waits with queued requests/blocked processing/no current IO; an initial pre-admission empty wait cannot satisfy this handshake alone. All public BeginReadData admissions and owner drops are main-thread operations. No read/try/APC/callback may execute.',
  'Each last-owner drop decrements native info RefCount1to0, invokes actual deleting wrapper/data destructor, unregisters data and calls actual file/manager RemoveObject for a still-pending nonzero UID. Real pending/UID tree erasure and physical request/20B overlap release occur while bank mutex held and data alive. No retry Sleep permitted in this success domain.288 data/info/registration-node releases exactly once with guards; no retired node/object read.',
  'Four nonzero UID starts cross signed boundaries without zero wrap. Four release orders exercise shared bank6-owner trees and manager UID/pending maps with tied physical sort/file offsets and pointer ordering.312 independent manager count/order/cursor/full64B live request/statistics checks and624 bank-tree checks pass; caller buffers unchanged.',
  'Cached asynchronous descriptor preparation only; zero actual IO. Read-completion/pool/allocator/sync/packed paths remain cold. Borrowed raw bank/entry/file contexts; no full bank/game lifetime. Zero-UID owner destruction and active-reading retry are separate unproven domains. Earlier registered shared-read gate retains actual IO/completion coverage; this gate does not relabel cold methods as executed recoveries. Pointer-tree heap bridge names explicit,81 selected bodies and96 bank final named bindings audited; predecessor functional-only, no new root credit.',
 ])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-pending-owner.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_PENDING_OWNER_PASS batches=24 owners=288 pending_removals=288 reads=0 joins=24 contexts=48 tree_checks=624 partial_deletes=240 manager_checks=312 bodies=%d link_omissions=%d selection_controls=%d runtime_controls=41'%(len(selected),len(omissions),len(selection_controls)))
 print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
