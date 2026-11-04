"""Fresh native providers and controlled last-owner release during Windows reading."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re,struct,subprocess,sys
import pefile
import check_one as V
import gate_native_bank_shared_worker as G
import bank_active_owner_controls as defects
from gate_game_component_abi import Coff
from pe_oracle import read_virtual_data

R=Path(__file__).resolve().parents[2]
FIXTURE=R/'rebuild/tests/integration/NativeActiveOwner_test.cpp'
SOURCES=[FIXTURE,*G.SOURCES[1:]]
BASE=R/'rebuild/build/native-bank-shared-worker/00/9d/009d6060/20261019T155340426945Z/receipt.json'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()

def main(phase='reading'):
 assert phase in ('reading','completed','locked','refused','retried','priority','disposal','update','consumer','queued_consumer','factory','packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory')
 lane={'reading':'active','completed':'completed','locked':'locked','refused':'refused','retried':'retried','priority':'priority','disposal':'disposal','update':'update','consumer':'consumer','queued_consumer':'queued_consumer','factory':'factory','packed_factory':'packed_factory','initialized_factory':'initialized_factory','lifetime_factory':'lifetime_factory','file_lifetime_factory':'file_lifetime_factory'}[phase]
 fixture={'reading':FIXTURE,'completed':R/'rebuild/tests/integration/NativeCompletedOwner_test.cpp','locked':R/'rebuild/tests/integration/NativeLockedOwner_test.cpp','refused':R/'rebuild/tests/integration/NativeRefusedOwner_test.cpp','retried':R/'rebuild/tests/integration/NativeRetriedOwner_test.cpp','priority':R/'rebuild/tests/integration/NativePriorityOwner_test.cpp','disposal':R/'rebuild/tests/integration/NativeDisposalOwner_test.cpp','update':R/'rebuild/tests/integration/NativeUpdateOwner_test.cpp','consumer':R/'rebuild/tests/integration/NativeConsumerOwner_test.cpp','queued_consumer':R/'rebuild/tests/integration/NativeQueuedConsumerOwner_test.cpp','factory':R/'rebuild/tests/integration/NativeFactoryOwner_test.cpp','packed_factory':R/'rebuild/tests/integration/NativePackedFactoryOwner_test.cpp','initialized_factory':R/'rebuild/tests/integration/NativeInitializedFactoryOwner_test.cpp','lifetime_factory':R/'rebuild/tests/integration/NativeLifetimeFactoryOwner_test.cpp','file_lifetime_factory':R/'rebuild/tests/integration/NativeFileBankLifetimeOwner_test.cpp'}[phase]
 sources=[fixture,*G.SOURCES[1:]]
 marker={'reading':'ACTIVE_OWNER_PASS','completed':'COMPLETED_OWNER_PASS state=3 before_bank_callback=1','locked':'LOCKED_OWNER_PASS state=1 before_begin_lock=1','refused':'REFUSED_OWNER_PASS state=0 begin_refused=1','retried':'RETRIED_OWNER_PASS begin_refusals=1 worker_retry=1','priority':'PRIORITY_OWNER_PASS bank_priority_moves=2 begin_refusals=1 worker_retry=1','disposal':'DISPOSAL_OWNER_PASS list_transfers=1 disposal_nodes_freed=1','update':'UPDATE_OWNER_PASS native_updates=2 list_transfers=1','consumer':'CONSUMER_OWNER_PASS bank_waits=2 finished_queries=2 data_queries=1','queued_consumer':'QUEUED_CONSUMER_OWNER_PASS blocking_queue_moves=1 bank_waits=2','factory':'FACTORY_OWNER_PASS native_entries=1 entry_releases=1 blocking_queue_moves=1','packed_factory':'PACKED_FACTORY_OWNER_PASS native_entries=1 entry_releases=1 blocking_queue_moves=1','initialized_factory':'INITIALIZED_FACTORY_OWNER_PASS native_banks=1 native_entries=1','lifetime_factory':'LIFETIME_FACTORY_OWNER_PASS native_banks=1 native_bank_destructions=1 metadata_frees=3','file_lifetime_factory':'FILE_BANK_LIFETIME_PASS native_files=1 native_file_destructions=1 native_banks=1'}[phase]
 if phase=='priority':sources.append(R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_ChangeLoadingPriority_009d5630.cpp')
 disposal=None
 if phase in ('disposal','update'):
  import bank_disposal_worker_evidence as disposal
  sources.extend(disposal.SOURCES)
  if phase=='update':sources.extend(disposal.UPDATE_SOURCES)
 consumer=None
 if phase in ('consumer','queued_consumer','factory','packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory'):
  if phase=='file_lifetime_factory':import bank_file_lifetime_factory_worker_evidence as consumer
  elif phase=='lifetime_factory':import bank_lifetime_factory_worker_evidence as consumer
  elif phase=='initialized_factory':import bank_initialized_factory_worker_evidence as consumer
  elif phase=='packed_factory':import bank_packed_factory_worker_evidence as consumer
  elif phase=='factory':import bank_factory_worker_evidence as consumer
  else:import bank_consumer_worker_evidence as consumer
  sources.extend(consumer.SOURCES)
 driver=Path(__file__) if phase=='reading' else R/('tools/decomp_pipeline/gate_native_bank_'+lane+'_owner.py')
 build=R/('rebuild/build/native-bank-'+lane+'-owner/00/9d/009d6060')/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ')
 build.mkdir(parents=True,exist_ok=False);inc=build/'include';inc.mkdir();src=build/'sources';src.mkdir()
 def save(name,data):
  p=build/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 baseline=json.loads(BASE.read_text());assert baseline['status']=='PASS'
 prior_objects=[BASE.parent/('provider%d.obj'%i) for i in range(1,len(G.SOURCES))]
 for p in prior_objects:assert sha(p)==baseline['object_sha256'][str(p)]
 visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,(p,name);closure(q)
 for p in sources:closure(p)
 inputs=visited|set(prior_objects)|{Path(__file__),driver,Path(defects.__file__),Path(defects.C.__file__),Path(G.__file__),Path(G.banklink.__file__),BASE,G.EXE,G.WINMM}
 for module in (G.banklink.ctor,G.banklink.buffer,G.banklink.teardown,G.banklink.tree,G.banklink.key):inputs.add(Path(module.__file__))
 inputs.update((V.VC/'include').rglob('*.h'))
 inputs.update(V.VC/'bin'/n for n in ('cl.exe','link.exe','c1xx.dll','c2.dll'))
 inputs.update(V.VC/'lib'/n for n in ('kernel32.lib','libcmt.lib','oldnames.lib'))
 inputs.update(R/'tools/decomp_pipeline'/n for n in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py'))
 inputs.add(R/'rebuild/tests/integration/NativeAsyncPointerTreeKeyErase_test.py')
 for module in tuple(sys.modules.values()):
  filename=getattr(module,'__file__',None)
  if filename:
   p=Path(filename).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p):sha(p) for p in sorted(inputs)};save('input-pins.json',pins)
 provenance={}
 for p in sorted(inputs):
  if p.suffix=='.py' and p.is_relative_to(R):
   q=build/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes());provenance[str(q)]=sha(q)
 save('python-provenance.json',provenance)
 for p in visited-set(sources):
  q=inc/p.relative_to(R/'rebuild/include') if p.is_relative_to(R/'rebuild/include') else src/p.name
  q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 snapshots=[]
 for i,p in enumerate(sources):
  q=src/('%d_'%i+p.name);q.write_bytes(p.read_bytes());snapshots.append(q)
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)+';'+str(src)
 flags=['/nologo','/c','/O2','/MT','/W3','/showIncludes','/I'+str(inc),'/I'+str(src)]
 objects=[]
 def compile(source,obj,name,original):
  r=V.cl([*flags,*G.banklink.flags(original),*(disposal.flags(original) if disposal else []),*(consumer.flags(original) if consumer and hasattr(consumer,'flags') else []),'/Fo'+str(obj),str(source)],env,build)
  save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,r.stdout+r.stderr
  for line in r.stdout.splitlines():
   if 'including file:' in line.lower():
    p=Path(line.split(':',2)[-1].strip()).resolve();assert p.is_relative_to(build) or p.is_relative_to(V.VC/'include'),p
 for i,p in enumerate(snapshots):
  obj=build/('provider%d.obj'%i);compile(p,obj,'provider%d'%i,sources[i]);objects.append(obj)
 # Reuse the accepted original-evidence classifications only after every selected
 # provider body and named relocation is reproduced by this fresh compilation.
 coffs=[Coff(p) for p in objects]
 selected=baseline['selected_bodies'];assert len(selected)==81
 prior_coffs=[Coff(p) for p in prior_objects]
 for record in selected:
  prior=[]
  for c in prior_coffs:
   if record['symbol'] not in c.symbols:continue
   section,offset=c.symbols[record['symbol']];body,rels=c.sections[section]
   if offset==0 and body.hex()==record['body'] and sorted((s,n) for s,(n,k) in rels.items())==sorted((r['site'],r['symbol']) for r in record['relocations']):
    prior.append([dict(site=s,symbol=n,kind=k) for s,(n,k) in rels.items()])
  assert prior and all(r==prior[0] for r in prior),record['symbol']
  # Older tree evidence records target addresses instead of COFF kinds. Recover
  # kinds from its hash-pinned accepted object, never from the new candidate.
  record['relocations']=prior[0]
 for record in selected:
  matches=[]
  for i,c in enumerate(coffs[1:],1):
   if record['symbol'] in c.symbols:
    section,offset=c.symbols[record['symbol']];body,rels=c.sections[section]
    if offset==0:
     matches.append((body.hex(),[dict(site=site,symbol=n,kind=k) for site,(n,k) in rels.items()]))
  assert (record['body'],record['relocations']) in matches,record['symbol']
 bank_selected=[]
 def add_selected(symbol,index,address=None,size=None):
  c=coffs[index];section,offset=c.symbols[symbol];body,rels=c.sections[section];assert offset==0
  if size is not None:assert len(body)==size
  bank_selected.append(dict(symbol=symbol,address=address,body=body.hex(),bytes=len(body),relocations=[dict(site=site,symbol=n,kind=k) for site,(n,k) in rels.items()]))
 bank_grades,bank_symbols,tables=G.banklink.audit(objects,sources,add_selected)
 disposal_records=disposal.audit(objects,sources,selected) if disposal else []
 consumer_records=consumer.audit(objects,sources,selected) if consumer else []
 if phase=='priority':
  symbol='?ChangeLoadingPriority@CBankFileAsyncData@@QAEXW4EThreadedFilePriority@@@Z'
  c=coffs[-1];section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==30 and len(rels)==1
  patched=bytearray(body)
  for site,(name,kind) in rels.items():
   assert name=='?SetObjectPriority@CThreadedFile@@QAEXKW4EThreadedFilePriority@@@Z' and kind==20
   struct.pack_into('<I',patched,site,(0x98f0e0+struct.unpack_from('<i',body,site)[0]-(0x9d5630+site+4))&0xffffffff)
  assert bytes(patched)==read_virtual_data(G.EXE.read_bytes(),0x9d5630,30)
  selected.append(dict(symbol=symbol,address=0x9d5630,body=body.hex(),relocations=[dict(site=site,symbol=name,kind=kind) for site,(name,kind) in rels.items()]))
 aliases=G.banklink.aliases(objects,fixture)
 if phase in ('lifetime_factory','file_lifetime_factory'):aliases.update(G.banklink.aliases([],R/'rebuild/tests/integration/NativeBankLifetimeColdAliases.h'))
 if phase=='file_lifetime_factory':aliases.update(consumer.ALIASES)
 preferred=objects[sources.index(G.T.SOURCES[0])]
 def link(name,providers,entry_override=None):
  ordered=([preferred]+[p for p in providers if p!=preferred]) if preferred in providers else providers
  if phase in ('factory','packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory'):
   entry=entry_override or objects[sources.index(consumer.EXTRAS[-1])]
   if entry in ordered:ordered=[entry]+[p for p in ordered if p!=entry]
  if phase=='file_lifetime_factory':
   file_close=objects[sources.index(consumer.FILE_SOURCES[1])]
   if file_close in ordered:ordered=[file_close]+[p for p in ordered if p!=file_close]
  exe=build/(name+'.exe')
  r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,ordered),'kernel32.lib',str(G.WINMM)],env=env,cwd=build,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,r=link('active-owner',objects);assert r.returncode==0,r.stdout+r.stderr
 mapping={}
 for line in exe.with_suffix('.map').read_text(errors='replace').splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 def resolve(name,visited=()):
  if name in mapping:return mapping[name]
  assert name not in visited and name in aliases,('missing final symbol',name)
  return resolve(aliases[name],visited+(name,))
 linked=[];fixups=0
 with pefile.PE(str(exe)) as pe:
  for record in selected+tables:
   # Some formerly exercised helpers become unreferenced COMDATs in this domain.
   if record['symbol'] not in mapping and record['symbol'] not in aliases:continue
   address=resolve(record['symbol']);expected=bytearray.fromhex(record['body'])
   for rel in record['relocations']:
    site,name,kind=rel['site'],rel['symbol'],rel['kind'];assert kind in (6,20)
    addend=struct.unpack_from('<i',expected,site)[0]
    value=resolve(name)+addend-(address+site+4 if kind==20 else 0)
    struct.pack_into('<I',expected,site,value&0xffffffff);fixups+=1
   assert pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,len(expected))==bytes(expected),record['symbol']
   linked.append(record['symbol'])
 assert set(bank_symbols).issubset(linked)
 assert all(record['symbol'] in linked for record in disposal_records)
 assert all(record['symbol'] in linked for record in consumer_records)
 bank_fixups=G.banklink.verify_bindings(exe,selected,bank_symbols,tables,aliases);assert bank_fixups==96
 data=build/'synthetic.bin';data.write_bytes(bytes((i*37+23)&255 for i in range(0x9000)));data_hash=sha(data)
 unused=build/'owned-writes.bin';unused.write_bytes(data.read_bytes())
 def run(name,program,mode=None):
  try:r=subprocess.run([str(program),str(data),str(unused),*([mode] if mode else [])],cwd=build,capture_output=True,text=True,timeout=15)
  except subprocess.TimeoutExpired as error:
   save(name+'.run.json',dict(timeout=True,stdout=str(error.stdout),stderr=str(error.stderr)));raise
  save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return r
 for i in range(24):
  r=run('positive-%02d'%i,exe,'zero' if phase=='factory' and i%2==0 else None);assert r.returncode==0 and marker in r.stdout,r.stdout+r.stderr
 print('Fresh native providers and 24 '+lane+'-owner processes passed',flush=True)
 omissions=[];redundant=[]
 for i in range(1,len(objects)):
  _,r=link('omit-%d'%i,objects[:i]+objects[i+1:]);(omissions if r.returncode else redundant).append(str(sources[i]))
 for source in G.banklink.SOURCES:
  # Inline support providers can be redundant, but the actual new route must bind.
  if source.name.startswith(('CBankFileAsyncEntry_BeginReadData_','CBankFileAsyncEntry_SetupRead_','CBankFileAsyncData_Lifetime_','CBankFileAsyncData_OnFileReadCompletion_','CBankFileAsyncData_OnFileReadBegin_')):assert str(source) in omissions
 runtime_omissions=[]
 if phase in ('lifetime_factory','file_lifetime_factory'):
  for source in consumer.REQUIRED_SOURCES:
   if str(source) in omissions:continue
   index=sources.index(source);result=run('omit-required-%d'%index,build/('omit-%d.exe'%index))
   assert result.returncode==80 and 'NATIVE_WORKER_FAILURE' in result.stderr,(source,result.returncode,result.stdout,result.stderr)
   runtime_omissions.append(str(source))
 controls=defects.controls(sources)
 if disposal:
  assert all(str(source) in omissions for source in disposal.SOURCES)
  if phase=='update':assert all(str(source) in omissions for source in disposal.UPDATE_SOURCES)
  controls=disposal.controls(sources)
 if consumer:
  assert all(str(source) in set(omissions)|set(runtime_omissions) for source in (consumer.REQUIRED_SOURCES if phase in ('packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory') else consumer.SOURCES[:-1] if phase=='factory' else consumer.SOURCES))
  controls=consumer.controls(sources)
 if phase=='refused':
  exercised={'bank-wrong-buffer','bank-wrong-length','bank-missing-info','bank-wrong-refcount','bank-missing-delete-func','bank-skip-request','bank-wrong-uid','bank-missing-callback','bank-omit-unregister','bank-no-node-free','bank-wrong-node-value','bank-no-registration','bank-dtor-try-lock','active-skip-cancellation','active-wrong-cancel-uid'}
  controls=[row for row in controls if row[0] in exercised]
  assert len(controls)==len(exercised)
  def add(name,file,old,new):
   index=next(i for i,p in enumerate(sources) if p.name==file)
   text=sources[index].read_text();assert text.count(old)==1
   controls.append((name,index,text.replace(old,new)))
  add('refused-begin-ignores-lock','CBankFileAsyncData_OnFileReadBegin_009d5440.cpp','lock.IsLocked() && SetupBuffer(false)','SetupBuffer(false)')
  add('refused-no-pending-restore','CThreadedFileManager_ProcessObjects_0098ecd0.cpp','object->LoadingStatus = FILE_LOADING_STATUS_PENDING;',';')
  add('refused-no-pending-erase','CThreadedFileManager_RemoveObject_0098eb20.cpp','            RemoveObjectFromPendingMap(object);','            ;')
  add('refused-wrong-delay','CThreadedFileManager_ProcessObjects_0098ecd0.cpp','SleepEx(1,1);','SleepEx(0,1);')
 if phase=='priority':
  index=len(sources)-1;text=sources[index].read_text()
  for name,old,new in [('priority-skip-call','if (ThreadedFileUID)','if (false)'),('priority-wrong-uid','SetObjectPriority(ThreadedFileUID, priority)','SetObjectPriority(ThreadedFileUID+1, priority)'),('priority-ignore-requested','SetObjectPriority(ThreadedFileUID, priority)','SetObjectPriority(ThreadedFileUID, THREADED_FILE_PRIORITY_NORMAL)')]:
   assert text.count(old)==1;controls.append((name,index,text.replace(old,new)))
 if phase in ('retried','priority'):
  index=next(i for i,p in enumerate(sources) if p.name=='CThreadedFileManager_ProcessObjects_0098ecd0.cpp')
  text=sources[index].read_text()
  for name,old,new in [('retried-no-pending-restore','object->LoadingStatus = FILE_LOADING_STATUS_PENDING;',';'),('retried-wrong-delay','SleepEx(1,1);','SleepEx(0,1);')]:
   assert text.count(old)==1;controls.append((name,index,text.replace(old,new)))
 if phase in ('completed','locked','retried','priority'):
  index=next(i for i,p in enumerate(sources) if p.name=='CThreadedFileManager_RemoveObject_0098eb20.cpp')
  text=sources[index].read_text()
  old='object->LoadingStatus==FILE_LOADING_STATUS_READ_COMPLETED' if phase=='completed' else 'object->LoadingStatus==FILE_LOADING_STATUS_PENDING_LOCKED ||'
  assert text.count(old)==1
  controls.append((phase+'-allow-premature-removal',index,text.replace(old,'false' if phase=='completed' else 'false ||')))
 for name,index,text in controls:
  source=src/(name+'.cpp');source.write_text(text);obj=build/(name+'.obj');compile(source,obj,name,sources[index])
  replacement=list(objects);replacement[index]=obj;bad,r=link(name,replacement,replacement[sources.index(consumer.EXTRAS[-1])] if phase in ('factory','packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory') else None);assert r.returncode==0,r.stdout+r.stderr
  r=run(name,bad);assert r.returncode==80 and 'NATIVE_WORKER_FAILURE' in r.stderr,(name,r.returncode,r.stdout,r.stderr)
  print(name+' detected',flush=True)
 assert sha(data)==data_hash and sha(unused)==data_hash
 assert all(sha(Path(p))==h for p,h in pins.items())
 report=dict(status='PASS',phase=phase,build=str(build),baseline=str(BASE),baseline_sha256=sha(BASE),fresh_providers=len(objects),reproduced_bodies=81,linked_bodies=len(linked),linked_symbols=linked,linked_fixups=fixups,bank_fixups=bank_fixups,bank_grades=bank_grades,bank_tables=tables,positive_processes=24,real_reads=24,real_bytes=888,native_retry_sleeps=48,bank_data_deletes=24,bank_info_deletes=24,bank_node_deletes=24,manager_joins=24,installer_joins=24,runtime_controls=[name for name,index,text in controls],link_omissions=omissions,redundant_sources=redundant,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(exe),new_roots=0,boundaries=[
  'One nonzero UID per owned process. Native BeginReadData constructs counted data with a caller buffer and cached metadata. Real Windows ReadFileEx/APC and actual installer pause acknowledgment execute.',
  'Worker waits nonalertably after issuing ReadFileEx. Last-owner destruction unregisters its bank node and attempts protected READING removal twice. Both native Sleep(0) observations require the bank lock released, request and overlap live, and data/info unreclaimed. An owned event permits APC dispatch only on the second retry.',
  'Native completion acquires the bank lock with the manager lock released, then publishes Finished and UID0. Its full 72-byte state is checked against the admitted snapshot with those two expected changes while still locked and alive. Main physical deletion requires recorded publication. Subsequent request retirement observes ledgers only and never dereferences reclaimed data.',
  'Request retirement is separately signaled; main acquires manager lock and verifies empty UID/current state before shutdown, preventing a fixture shutdown race. Actual native worker and installer are joined. Data/info/node guards and independent synthetic payload/guards pass; 5 bank enters,7 leaves,2 recursive tries per process.',
  'All 81 previously selected native bodies/relocations are reproduced from fresh source compilation. Final linked instructions and targets are checked; bank roots also compared directly with original evidence. Inactive helper classifications inherited from the pinned accepted gate are not new coverage credit.',
  'No pool allocation/failure, packed metadata, UID0 owner safety, shared-bank active concurrency, complete bank startup or installed-game execution claim. No canonical implementation change; predecessor remains functional-only and SetObjectPriority remains near151B.'
 ])
 if phase=='completed':
  report['boundaries'][1]='Actual Windows APC has set request READ_COMPLETED3, while bank Finished remains false, UID remains nonzero and callback points to live data. Worker waits nonalertably before returning from the APC observer, before bank callback dispatch. Two native destructor retries must release the bank lock while request/overlap/data/info remain live; only the second retry permits worker continuation.'
 if phase=='locked':
  report['boundaries'][1]='Native worker sets PENDING_LOCKED1 and releases manager mutex before bank begin callback. First bank TryEnter is held nonalertably before acquiring the mutex. Two native destructor retries must release bank mutex with request/overlap/data/info live. Native begin then acquires its real recursive bank locks, publishes buffer and advances through actual Windows IO/APC/bank completion before physical reclamation. Begin lock leaves are distinguished from final callback publication by live request state3.'
 if phase in ('retried','priority'):
  report.update(begin_refusals=24,successful_worker_retries=24)
  report['boundaries'][1]='First native bank TryEnter fails against a main-thread-owned bank mutex. Native ProcessObjects restores pending and reaches SleepEx(1,true); full bank state remains unchanged. Releasing the mutex and worker event permits native queue reselection and a second PENDING_LOCKED callback. Last-owner teardown then reaches two protected-removal retries before actual begin/read/APC/completion progress. One failed try plus two successful recursive tries; six enters/eight leaves.'
  report['boundaries'][3]=report['boundaries'][3].replace('5 bank enters,7 leaves,2 recursive tries','6 bank enters,8 leaves,1 failed try and2 recursive tries')
 if phase=='priority':
  report.update(reproduced_bodies=82,bank_priority_moves=48)
  report['boundaries'].append('Typed ChangeLoadingPriority directly matches original30B, final linked call targets actual file wrapper and manager. After first refused begin, actual bank calls move priority3 to5 to1. Independent checks inspect all14 pending trees, queue cursors, UID identity and unchanged72-byte bank state before worker retry. Last owner then waits through actual read/APC/completion.')
 if disposal:
  report.update(reproduced_bodies=len(selected),native_retry_sleeps=0,disposal_transfers=24,disposal_node_frees=24,disposal_original_records=disposal_records)
  report['boundaries'][:4]=[
   'One nonzero UID per process. Actual bank BeginReadData, worker ReadFileEx/APC and installer pause/resume execute using caller buffer and cached metadata.',
   'Worker is held after actual read submission. Native static DiscardAsyncData and bank method call actual Abort, which refuses protected READING removal. Native list push copies counted ownership; original caller reset leaves one list-owned reference. Initial native disposal processing retries Abort and leaves node/data/info/registered bank node live.',
   'Real APC and bank completion publish Finished and UID0 while list retains data. Request/overlap retire on worker before main drains. Actual ProcessDisposalList invokes Abort on finished data, pops the list node, and destroys data/info/bank node exactly once; list node allocation/free guards observed through instruction-preserving allocator import renames.',
   'All list links, refcounts, full72-byte bank state, request retirement, independent payload/guards and final empty list/tree checked. Ten bank enters/twelve leaves/two recursive successful tries; no destructor retry. Original180B bank discard,16B static wrapper,59B list push,164B disposal processor and196B Abort revalidated directly, then final linked bodies/targets checked.'
  ]
  if phase=='update':
   report.update(native_bank_updates=48,empty_restart_passes=48)
   report['boundaries'][3]='Actual AsyncUpdatePending drives ProcessDisposalList then RestartAbortedReads twice: while READING and after completion. Restart tree is empty; no aborted resubmission claim. Bank recursive-depth trace proves disposal-before-restart ordering in both calls, including final destructor recursion. Fourteen bank enters/sixteen leaves/two successful tries; native updates16B and restart200B directly revalidated and final linked targets verified alongside disposal roots.'
 if phase=='refused':
  report.update(real_reads=0,real_bytes=0,native_retry_sleeps=0,begin_refusals=24,pending_cancellations=24)
  report['boundaries'][:4]=[
   'One nonzero UID per owned process. Native BeginReadData constructs counted data with caller buffer and cached metadata. No Windows read/APC or installer pause executes.',
   'Worker reaches bank TryEnter while PENDING_LOCKED. Last-owner destructor owns the bank mutex; tree-node release observer permits the real TryEnter to fail and waits until native ProcessObjects restores PENDING and reaches SleepEx(1,true). Data/info/request/overlap remain live throughout callback return.',
   'Native destructor cancels PENDING through actual RemoveObject, destroying request and overlap on main thread, then bank data/info/node. No completion or Finished/UID0 publication is expected. Buffer remains entirely unchanged.',
   'Owned event prevents worker re-entry until request retirement. Actual manager/installer workers join; all release ledgers are checked once and bank lock observations require two enters/two leaves/one failed try.'
  ]
 report['python_provenance_sha256']=provenance
 if consumer:
  report.update(reproduced_bodies=len(selected),native_retry_sleeps=0,bank_wait_calls=48,bank_finished_queries=48,bank_data_queries=24,manager_wait_sleeps=24,consumer_original_records=consumer_records)
  report['boundaries'][:4]=[
   'One nonzero UID per owned process; caller buffer/cached metadata. Actual Windows read, APC, bank completion, installer pause/resume and manager retirement execute.',
   'Native IsFinished first returns false. Native WaitUntilFinished recursively checks existing buffer, visits empty restart tree, releases bank lock and calls actual file/global-manager wait. One controlled native SleepEx(0,true) requires both mutexes released and active request preserved. READING priority stays normal despite manager blocking-priority request.',
   'Worker completion publishes Finished/UID0 before request/overlap retirement. Native wait returns; actual IsFinished and GetData provide completed flag and caller-buffer pointer. Second WaitUntilFinished takes finished fast path without another sleep. Whole72-byte bank state changes only UID/Finished; native last owner then reclaims data/info/node once.',
   'Ten bank enters/twelve leaves/two recursive tries. Wait130B/restart200B/GetData77B/IsFinished65B/poolLock30B directly match original and final links are audited. Pool-lock provider is linked but cold: no pool allocation or lock-path runtime claim. Payload/guards and manager/installer joins checked.'
  ]
  if phase in ('queued_consumer','factory','packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory'):
   report.update(blocking_queue_moves=24)
   report['boundaries'][1]='Worker is held before first bank TryEnter at PENDING_LOCKED. Actual bank wait checks existing buffer, empty restart tree and releases bank lock. Actual manager wait promotes pending queue from NORMAL3 to BLOCKING6. Its controlled native SleepEx requires both mutexes released; all14 pending trees/cursors/UID identity/unchanged callback are independently checked before permitting actual begin/Windows IO/APC/completion. Subsequent finished wait is cold and GetData returns caller buffer.'
 if phase=='factory':
  report.update(native_entries=24,entry_releases=24,indexed_entries=12,zero_index_entries=12)
  report['boundaries'].append('Native GetAsyncEntry constructs guarded16-byte entry and12-byte counted info using cached size37 and alignment8. Twelve processes use index0/offset0; twelve use index2/offset0x313, with relative read offset independently adjusted to the same expected file bytes. All controls use index2. Actual entry drives BeginReadData and survives data owner teardown. Native counted destructor releases entry/info exactly once. Factory152B,Align24B,DeleteData11B,scalar destructor36B,entry-size37B,counted destructor53B and vtable4B directly match original, with final linked bodies and targets audited. Counted-entry destructor provider is redundant with the factory COMDAT; final selected native body and explicit mutation replacement are verified. Controlled bank initialization remains; no wholebank startup claim.')
 if phase in ('packed_factory','initialized_factory','lifetime_factory','file_lifetime_factory'):
  report.update(native_entries=24,entry_releases=24,packed_entries=24,cross_word_metadata_reads=48)
  report['boundaries'].append('Actual GetAsyncEntry uses entry index3 and native packed size/offset accessors. Both15-bit values straddle the32-bit boundary at shift30; nonzero bias yields size37 and offset0x313. Independent file-position/size/payload and guarded counted lifetimes checked. Native packed getter101B and mask table132B match original and final linked bytes. Runtime metadata fallback is intentionally zero. Controlled bank initialization remains; no metadata parser or wholebank startup claim.')
  report['boundaries'][5]=report['boundaries'][5].replace('packed metadata, ','')
 if phase in ('initialized_factory','lifetime_factory','file_lifetime_factory'):
  report.update(native_bank_constructions=24,initialization_allocations=168)
  report['boundaries'].append('Actual bank/base and five helper constructors allocate guarded empty map/tree/list headers and initialize the real Windows critical section before packed metadata, factory, queued wait and read. Allocation observers require original late restart/flag stores. All constructor roots match original and final linked targets. Fixture then supplies owned file and packed metadata; after native entry/data owners and reads finish, fixture manually frees seven empty headers and deletes the critical section. Native whole-bank destructor and file/header parsing remain outside this proof.')
 if phase in ('lifetime_factory','file_lifetime_factory'):
  report.update(native_bank_destructions=24,initialization_header_frees=168,metadata_frees=72,runtime_omissions=runtime_omissions)
  report['boundaries'].append('Native bank destructor runs after real read and entry/data owners retire. Owned guarded runtime vector and two packed arrays are reclaimed by actual CloseBankFileImpl/packed cleanup; native async/base destructors free seven headers, delete the released Windows critical section and restore string counters. Eleven bank enters/thirteen leaves/two tries. File handle and metadata construction remain fixture-controlled; no file/header parser, populated map erase or active-owner whole-bank teardown claim. All constructor/destructor/helper bodies are directly original/final-link verified. Cold aliases reject unexpected inactive callbacks.')
 if phase=='file_lifetime_factory':
  report.update(native_file_constructions=24,native_file_destructions=24,native_file_closes=24,file_close_streams_grade='NEAR_MATCH',file_close_streams_differing_offsets=list(range(493,501)))
  report['boundaries']=[b.replace('Ten bank enters/twelve leaves/two recursive tries.','Eleven bank enters/thirteen leaves/two recursive tries.') for b in report['boundaries'] if 'Fixture then supplies owned file and packed metadata;' not in b and 'Controlled bank initialization remains;' not in b]
  report['boundaries'].append('Actual typed file construction precedes controlled handle/metadata setup and native bank factory/read. Actual Close invokes reconstructed CloseAllStreams with empty retired request map and closes the Windows handle; repeated Close takes the invalid-handle fast path. Native file destructor then releases filename storage before manager Uninitialise deletes its critical section. Native bank destructor then frees all seven headers and three metadata blocks. CloseAllStreams is explicitly near-match840B with eight cursor-comparison scheduling bytes different; its accepted independent1000 differential/104 host cases are separate evidence. File constructor, ordinary/scalar destructors, Close and vector helper match original and final-linked bytes; file vtable weak fallback audited. Native Open/header parsing and active-stream file destruction through production worker remain outside this gate.')
 save('receipt.json',report);(R/('rebuild/compile-gate/native-bank-'+lane+'-owner.json')).write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_'+lane.upper()+'_OWNER_PASS processes=24 reads='+str(report['real_reads'])+' bytes='+str(report['real_bytes'])+' retries='+str(report['native_retry_sleeps'])+' runtime_controls='+str(len(controls)),flush=True);print(build/'receipt.json',flush=True)

if __name__=='__main__':main()
