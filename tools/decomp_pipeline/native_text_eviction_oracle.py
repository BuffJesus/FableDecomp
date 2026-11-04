from pathlib import Path
import sys,json,re,struct,hashlib,subprocess,pefile,capstone,os
P=Path(__file__).resolve().parent;R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]);r=json.loads((b/'composition.json').read_text());mapping={};sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
lookup=lambda prefix:next(n for n in mapping if n.startswith(prefix))
targets={0x9fbfb0:'?ListRemove@CResource@@AAEXXZ',0x9fc170:'?UnloadNextResource@CResourceList@@QAE_NK@Z',0x9fc210:'?ShrinkResources@CResourceList@@QAEXK@Z',0x9e1bc0:'?GFGetTime@@YINXZ',0x9fc250:'?ProcessLoadingQueue@CResourceList@@QAEX_NN@Z',0xa62920:'??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z',0xa629a0:'??1CCriticalSectionLock@@QAE@XZ',0xa62ed0:'?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z',0x9d5bf0:'?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ',0x9d7cf0:lookup('?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@')}
globals={0x1396c5c:'?ResourceAbortingEnabled@CResourceBank@@0_NA'};roots=[(0x9fc150,11,lookup('?FrameEnd@CResourceList')),(0x9fc210,64,lookup('?ShrinkResources@')),(0x9fc4f0,33,lookup('?MakeFreeSpace@')),(0x9fc520,20,lookup('?SetMaximumMemory@')),(0x9fc170,149,lookup('?UnloadNextResource@')),(0x9fc250,260,lookup('?ProcessLoadingQueue@CResourceList')),(0x9fc410,26,lookup('?ProcessLoadingQueue@CResourceBank')),(0x9d5bf0,314,lookup('?HandleFailedAllocation@CBankFileAsync')),(0x9d5f10,104,lookup('?HandleFailedAllocation@CLoadingMemoryFailureHandler'))];decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;align=lambda n,a:(n+a-1)//a*a;image=EXE.read_bytes();proof=[];baseline=json.loads((b/'run.json').read_text())['stdout'];variants=[]
device_baseline=json.loads((b/'device-runs.json').read_text())[0];assert os.environ.get('FABLE_NATIVE_DEVICE_REFERENCE')==str(EXE)
for mask in range(512):
 raw=(b/'probe.exe').read_bytes()
 for index,(address,size,symbol) in enumerate(roots):
  if not (mask&(1<<index)):continue
  pe=pefile.PE(data=raw);last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(read_virtual_data(image,address,size));global_sites=0
  for ins in decoder.disasm(bytes(payload),address):
   pos=ins.address-address
   if ins.bytes[0] in (0xe8,0xe9):
    target=int(ins.op_str,16)
    if address<=target<address+size:continue
    name=targets[target];struct.pack_into('<I',payload,pos+1,(mapping[name]-base-pos-5)&0xffffffff)
    if mask==511:proof.append(dict(site=hex(ins.address),target=hex(target),symbol=name,branch_kind=ins.mnemonic))
   for op in ins.operands:
    if op.type==capstone.x86.X86_OP_MEM and op.mem.base==0 and op.mem.index==0:
     target=op.mem.disp;assert target in globals,(hex(ins.address),hex(target));struct.pack_into('<I',payload,pos+ins.disp_offset,mapping[globals[target]]);global_sites+=1
  assert global_sites==(1 if address==0x9fc250 else 0)
  header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);file_size=align(size,pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+file_size-len(oracle)));oracle[start:start+size]=payload;struct.pack_into('<8sIIIIIIHHI',oracle,header,('.read%d'%index).encode(),size,rva,file_size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+size,pe.OPTIONAL_HEADER.SectionAlignment));at=mapping[symbol];off=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[off:off+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);raw=bytes(oracle)
 exe=b/('original-readers-%d.exe'%mask);exe.write_bytes(raw)
 for attempt in range(2):
  run=subprocess.run([str(exe),r['asset']],cwd=b,capture_output=True,text=True,timeout=30);assert not run.returncode and run.stdout==baseline,(mask,attempt,run.returncode,run.stdout[-500:],run.stderr)
  device=subprocess.run([str(exe),device_baseline['asset'],'--devices'],cwd=b,capture_output=True,text=True,timeout=30);assert device.returncode==0 and device.stdout==device_baseline['stdout'],('devices',mask,attempt,device.returncode,device.stdout,device.stderr)
 variants.append(dict(mask=mask,runs=2,exe_sha256=sha(exe)));print('TEXT_EVICTION_ORIGINAL_PASS',mask) if mask in (0,511) else None
assert sha(Path(r['asset']))==r['asset_sha256'];(b/'eviction-original-receipt.json').write_text(json.dumps(dict(status='PASS',device_runs=1024,device_payload_windows=61440,device_priorities=6,variants=variants,text=65,groups=65,calls=proof,context_global_operands=1,asset_unchanged=True,pending_reads=2,boundary='Actual decoded native text/group resources: budget shrink dynamic-count limit,frame-delay andsize guards,per-frame attempt budget,external-reference cache removal andfinal release.512 original/recreated resource-list,loading-queue andfailure-handler root combinations twice. Two actual worker reads held pending under the bank critical section survive eviction and clean up after completion. Completed disposal-list ownership andcontrolled loading consumers withreal read readiness,pending anddeadline stops,then two native queue notifications. Native loading-failure handler reclaims a finished read,publishes restart work,andreal worker resubmission recovers both payload windows. Consumer callbacks remain controlled. No renderer claim.'),indent=2))
