from pathlib import Path
import sys,json,re,struct,subprocess,hashlib,pefile,capstone
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from native_exact_roots import import_names
from pe_oracle import EXE,read_virtual_data
from threaded_process_oracle import ADDRESS as WORKER_ADDRESS,ENTRY as WORKER,SIZE as WORKER_SIZE,SYMBOL as WORKER_SYMBOL
from near_match import compare
def run(build,stdout,mode="--pending-write",source_asset=None,read_forwarder=False,child_env=None):
 B=Path(build);base=B;composition=json.loads((B/'composition.json').read_text());sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();mapping={}
 for line in (B/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 source,obj=next((Path(source),Path(obj)) for source,obj in composition['source_objects'].items() if source.endswith('CThreadedFileManager_CloseAllStreamsForFile_0098e7d0.cpp'));coff=Coff(obj);symbol='?CloseAllStreamsForFile@CThreadedFileManager@@QAEXPAVCThreadedFile@@@Z';sec,off=coff.symbols[symbol];body,rels=coff.sections[sec];assert off==0 and len(body)==840 and len(rels)==19
 image=EXE.read_bytes();imports=import_names(image);reverse={n:a for a,n in imports.items()};targets={'_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc};original=bytearray(read_virtual_data(image,0x98e7d0,840));patched=bytearray(body);close_original=bytearray(original);close_proof=[]
 for site,(name,kind) in rels.items():
  if name.startswith('__imp__'):target=reverse[name[7:].split('@')[0]]
  elif name.startswith('?_Rebalance_for_erase@'):target=0x42a34e
  elif name.startswith('?_M_insert_overflow@?$vector@K'):target=0x464ace
  elif '_M_find' in name:target=0x98fe70
  elif name.startswith('?find@'):target=0x9902d0
  elif '_M_lower_bound' in name:target=0x9901d0
  elif '_M_upper_bound' in name:target=0x98fca0
  elif '?erase@' in name:target=0x9905d0
  else:target=targets[name]
  addend=struct.unpack_from('<i',body,site)[0];assert addend==0 and kind in (6,20);actual=struct.unpack_from('<I',original,site)[0];assert (actual+0x98e7d0+site+4)&0xffffffff==target if kind==20 else actual==target
  struct.pack_into('<I',patched,site,(target-(0x98e7d0+site+4 if kind==20 else 0))&0xffffffff);struct.pack_into('<I',close_original,site,(mapping[name]-(mapping[symbol]+site+4 if kind==20 else 0))&0xffffffff);close_proof.append(dict(site=site,symbol=name,target=hex(target),kind=kind))
 assert [i for i,(a,b) in enumerate(zip(patched,original)) if a!=b]==list(range(493,501));grade=compare(body,bytes(original),tuple(rels),()).to_dict();assert grade['near_match']
 worker=bytearray(read_virtual_data(image,WORKER,WORKER_SIZE));md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;by_address={v:k for k,v in WORKER_ADDRESS.items()};worker_proof=[]
 for ins in md.disasm(bytes(worker),WORKER):
  pos=ins.address-WORKER
  if ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not WORKER<=target<WORKER+WORKER_SIZE:
    name=by_address[target];struct.pack_into('<I',worker,pos+1,(mapping[name]-mapping[WORKER_SYMBOL]-pos-5)&0xffffffff);worker_proof.append(dict(site=pos+1,symbol=name,kind='relative'))
  for operand in ins.operands:
   if operand.type==capstone.x86.X86_OP_MEM and not operand.mem.base and not operand.mem.index:
    name=by_address[operand.mem.disp];target=mapping['_FixtureReadFileExSlot'] if read_forwarder and name=='__imp__ReadFileEx@20' else mapping['_FixtureWriteFileExSlot'] if name=='__imp__WriteFileEx@20' else mapping[name];struct.pack_into('<I',worker,pos+ins.disp_offset,target);worker_proof.append(dict(site=pos+ins.disp_offset,symbol=name,kind='write_forwarder' if name=='__imp__WriteFileEx@20' else 'memory'))
   if operand.type==capstone.x86.X86_OP_IMM and ins.bytes[0] not in (0xe8,0xe9) and operand.imm in by_address:
    name=by_address[operand.imm];assert ins.imm_size==4;struct.pack_into('<I',worker,pos+ins.imm_offset,mapping[name]);worker_proof.append(dict(site=pos+ins.imm_offset,symbol=name,kind='immediate'))
 assert len(worker_proof)==27 and sum(ins.size for ins in md.disasm(read_virtual_data(image,WORKER,WORKER_SIZE),WORKER))==WORKER_SIZE
 raw=(B/'probe.exe').read_bytes();pe=pefile.PE(data=raw);runs=[];baseline=stdout
 for mask in range(4):
  output=bytearray(raw)
  for bit,name,payload in [(1,WORKER_SYMBOL,worker),(2,symbol,close_original)]:
   if mask&bit:
    at=pe.get_offset_from_rva(mapping[name]-pe.OPTIONAL_HEADER.ImageBase);output[at:at+len(payload)]=payload
  exe=B/('original-close-worker-%d.exe'%mask);exe.write_bytes(output)
  for attempt in range(2):
   run=subprocess.run([str(exe),str(source_asset or B/('original-%d-%d-owned.bin'%(mask,attempt))),mode],cwd=B,env=child_env,capture_output=True,text=True,timeout=30);assert run.returncode==0 and run.stdout==baseline,(mask,attempt,run.returncode,run.stdout,run.stderr)
  runs.append(dict(mask=mask,runs=2,exe_sha256=sha(exe)))
 result=dict(status='PASS',baseline_sha256=hashlib.sha256(stdout.encode()).hexdigest(),close_grade=grade,close_original_bytes=840,close_relocations=close_proof,worker_original_bytes=WORKER_SIZE,worker_relocations=worker_proof,runs=runs,boundary='Work-only four original/recreated worker/CloseAllStreamsForFile combinations twice with actual submitted write held by explicit forwarding boundary. Native file wrapper and query providers; typed completion leaves. Queued write on the closing file cancels without submission; active write completes and Close joins ownership. A peer writer retains its handle and queued write. The false completion callback creates/joins a bounded independent native critical-section probe and submits a further peer write through native AddObject at block count1. Native KeepUpdating explicitly wakes both peer requests before completion/UID joins, then independent CRT bytes match 12KiB across both files. One peer uses QueueWhenFinished=true: global receiver-independent queries report active/pending as unfinished, retained completion as present/finished, then native Close removes it; retired/missing UIDs remain finished. All five query bodies separately match original bytes. Four native statistics getters are snapshotted under an outer native lock: active/pending3/12288, callback cancellation/admission3/12288 until return removes the cancellation descriptor, retained finished1/4096, and all zero after Close. Totals follow current global-map status and descriptor sizes. Callback behavior and lock-probe thread remain controlled. No automatic wake, latency or physical cancellation claim. Only owned files written.')
 if read_forwarder:result['boundary']='Four original/recreated worker/Close combinations twice for one cached read through native queued0/begin1/submitted2/completed3/retained4/removal phases. Original read import targets the explicit begin/API/native-completion forwarding fixture. Native queries/statistics, independent32-byte payload/guards and joined ownership; no arbitrary concurrency, physical-media or completion-latency claim.'
 if mode=='--close-read':result['boundary']='Four original/recreated worker/Close combinations twice: Close starts at child-selected read hold1,2or3, stays blocked with open handle, and joins existing read/user completion before UID/file removal. Explicit read forwarder and phase barriers; independent32-byte payload/guards; no arbitrary timing or OS cancellation claim.'
 if mode=='--eof':result['boundary']='Four original/recreated worker/Close combinations twice, three actual cached EOF reads on a generated archive. OS completion error/transfer captured before native callback; partial13-byte payload and untouched guards, EOF error38 with user success, retained requested32-byte statistics and logical DVD dirty flag. Metadata closes and dirty flag restores under native manager lock; no failed-submission recovery claim.'
 if mode=='--eof-harddisk':result['boundary']='Four original/recreated worker/Close combinations twice with a child-only generated C: archive source. Native logical harddisk device0: OS partial13/error0, at/beyond EOF0/error38; typed harddisk error callback receives empty string once at READING2 for each error. User success, requested32-byte finished totals, unchanged dirty flag and guarded payload checked. Native metadata closes and callback/flag restore under lock; no failed-submission or physical-media claim.'
 if mode=='--eof-callback-peer':result['boundary']='Four original/recreated worker/Close combinations twice; actual cached EOF error callback (error38) admits a native peer32-byte read while active status remains READING2. Native UID/status/statistics1/32 then2/64 and active removal/priority refusals checked under lock. Both user completions true, peer auto-removes, retained original1/32 closes. Independent peer payload/guards and unchanged EOF buffer; callback/flags/forwarder restore after ownership joins. No arbitrary concurrency, failed-submission recovery or physical-media claim.'
 if mode=='--eof-public-policy':result['boundary']='Four original/recreated worker/Close combinations twice across nine cached partial/at/beyond EOF reads on a generated C: archive. Existing native public setters control unset/installed/callback-self-clearing policies, expected callback counts0/2/1 and dirty flag transitions. Native user success, requested32-byte finished totals, callback pointer mutation and independent guarded payloads checked. Original state restores via native setters under manager lock after ownership joins; no failed-submission or physical-media claim.'
 if mode=='--bank-caller-eof':result['boundary']='Four original/recreated worker/Close combinations twice, three actual cached native bank caller-buffer reads at explicit offsets crossing/at/beyond intact archive EOF. Native bank callback publishes Finished/GetData and retains requestedLength32/descriptorSize32, without internal allocation/restart/disposal/decode. Captured UID removal joins before caller storage release; independent partial payload/guards, dirty flag and native bank/file cleanup checked. No entry-bounds guard or failed-submission recovery claim.'
 if mode=='--bank-allocated-eof-disposal':result['boundary']='Four original/recreated worker/Close combinations twice for six cached native autoallocated partial/at/beyond EOF reads (three heap/three pool). Typed API boundary seeds exactly32 owned bytes before real submission and holds worker nonalertably afterward. Native discard transfers sole ownership to DisposalList, pending abort refuses; bank publication and captured UID removal join before independent partial/seeded-tail checks and drain. Native heap/pool disposal and collections clear, original pool lock accounting preserved. No error-byte decode or failed-submission recovery claim.'
 if mode=='--descriptor-meta':result['boundary']='Four original/recreated worker/Close combinations twice for caller-selected registered descriptor sizes. Native32-bit current-map byte accounting; held begin refuses and Close cancels both requests. Typed read import guard rejects submission before any Windows read API. Small caller buffers unchanged; no huge allocation/read/exhaustion claim.'
 (B/('read-phase-original-receipt.json' if read_forwarder else 'close-write-original-receipt.json')).write_text(json.dumps(result,indent=2));return result
