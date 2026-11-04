"""Owned CRT thread: actual engine entry, IO loop and concurrent shutdown."""
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
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2]
FIXTURE=R/'rebuild/tests/integration/NativeThreadedWorkerThread_test.cpp'
ABI=FIXTURE.with_name('NativeThreadedReadAPC_win32.h')
SOURCES=[FIXTURE,worker.SOURCE,callback.SOURCE,callback.DATA_SOURCE,begin.F,exit_gate.F,shutdown.F]
SOURCES.append(R/'rebuild/src/compiled/00/98/CThreadedFileObject_Constructor_0098df00.cpp')

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()

def main():
 assert sys.platform=='win32'
 build=R/'rebuild/build/native-worker-thread/00/98/0098ecd0'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 build.mkdir(parents=True,exist_ok=False)
 inc=build/'include';inc.mkdir();src=build/'sources';src.mkdir()
 inputs={Path(__file__),EXE,ABI,Path(worker.__file__),Path(callback.__file__),Path(begin.__file__),Path(exit_gate.__file__),Path(shutdown.__file__)}
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
 for p in [V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll')]+[V.VC/'lib'/n for n in ('kernel32.lib','libcmt.lib','oldnames.lib')]:
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
 for index,gate in ((4,begin),(5,exit_gate),(6,shutdown)):
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
 def link(name,objs):
  exe=build/(name+'.exe');result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),*map(str,objs),'kernel32.lib'],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return exe,result
 exe,result=link('probe',objects);assert result.returncode==0,result.stdout+result.stderr
 with pefile.PE(str(exe)) as pe:
  assert pe.FILE_HEADER.Machine==0x14c and pe.OPTIONAL_HEADER.Magic==0x10b
  imports={(d.dll.decode().lower(),x.name.decode()) for d in pe.DIRECTORY_ENTRY_IMPORT for x in d.imports if x.name}
  for name in ('ReadFileEx','WriteFileEx','WaitForSingleObjectEx','SetEvent','InitializeCriticalSection','EnterCriticalSection','LeaveCriticalSection','DeleteCriticalSection','GetCurrentThreadId','GetThreadPriority','SetThreadPriority'):
   assert ('kernel32.dll',name) in imports,name
 def run(name,exe):
  result=subprocess.run([str(exe),str(data),str(writable)],cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));return result
 result=run('positive',exe);assert result.returncode==0,result.stdout+result.stderr
 assert 'NATIVE_WORKER_THREAD_PASS cases=56 reads=28 writes=28 joins=56 shutdowns=56' in result.stdout
 baseline=data.read_bytes();expected=bytearray(baseline)
 for p in range(7):
  for i in range(p*17,p*17+17+p):expected[i]=baseline[i]^0x5a
 assert writable.read_bytes()==bytes(expected),'Owned file writes differ from requested ranges'
 omissions=[]
 for i,symbol in ((1,worker.SYMBOL),(2,callback.SYMBOL),(3,'?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA'),(4,begin.ROOTS[0]['symbol']),(5,exit_gate.ROOTS[0]['symbol']),(6,shutdown.ROOTS[0]['symbol']),(7,'??0CFileObject@CThreadedFileManager@@QAE@XZ')):
  _,result=link('omit%d'%i,[o for n,o in enumerate(objects) if n!=i]);diag=result.stdout+result.stderr
  assert result.returncode and re.search(r'LNK(?:2001|2019)',diag) and (symbol in diag or ('FixtureProcess' in diag if i==1 else ('FixtureBeginThread' in diag if i==4 else 'FixtureReadCompletion' in diag))),diag
  omissions.append(str(SOURCES[i]))
 control=src/'noop.cpp';control.write_text('#include "fable_gfuninitialise.h"\nvoid __cdecl CThreadedFileManager::BeginThread(void*) {}\n')
 obj=build/'noop.obj';result=V.cl([*flags,'/Fo'+str(obj),str(control)],env,build)
 save('noop.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0
 noexe,result=link('noop',[*objects[:4],obj,*objects[5:]]);assert result.returncode==0
 result=run('noop',noexe);assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr and 'g_finishEvent' in result.stderr,result.stderr
 assert writable.read_bytes()==bytes(expected)
 assert sha(data)==data_hash and all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',build=str(build),cases=56,real_reads=28,real_writes=28,thread_joins=56,shutdowns=56,
  worker_grade=grade,exact_roots=exact,link_omissions=omissions,runtime_noop_entry_control='DETECTED',
  input_sha256=pins,object_sha256={str(o):sha(o) for o in objects},executable_sha256=sha(exe),synthetic_sha256=data_hash,owned_writes_sha256=sha(writable),
  boundaries=['56 owned _beginthreadex threads invoke actual BeginThread/ThreadMain/ProcessObjects and deliver actual read/write APCs on the initiating thread.',
   'Actual SetThreadPriority(1) observed on the IO thread, main/worker IDs differ, critical-section recursion and callback/unlock ordering checked.',
   'Main calls actual Uninitialise after a fixture finish signal, concurrently wakes the worker, waits for ThreadFinished publication, and joins the CRT thread.',
   'Pending removal and finish controlled: remove empties the one-node queue; finish checks completed state, marks finished and signals the observer. UID tree empty at worker exit.',
   'Timer fixed; installer absent with fail-only traps. No owner construction, production pending/finish helpers, real game assets, saves or installed files. Entire owned output checked.'])
 save('receipt.json',report);(R/'rebuild/compile-gate/native-threaded-worker-thread.json').write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_THREADED_WORKER_THREAD_PASS cases=56 reads=28 writes=28 joins=56 shutdowns=56 link_omissions=7 runtime_control=PASS')
 print('RECEIPT',build/'receipt.json')
if __name__=='__main__':main()
