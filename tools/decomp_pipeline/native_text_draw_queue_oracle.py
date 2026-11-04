from pathlib import Path
import sys,json,re,struct,hashlib,subprocess,pefile,capstone,time
P=Path(__file__).resolve().parent;R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]);r=json.loads((b/'composition.json').read_text());mapping={};sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
lookup=lambda prefix:next(n for n in mapping if n.startswith(prefix))
targets={0x9cc210:'?GetEntryDataType@CBankFile@@QBEKK@Z',0x9d68b0:'?AsyncUpdatePending@CBankFileAsync@@QAEXXZ',0x9d54e0:'?IsFinished@CBankFileAsyncData@@QBE_NXZ',0x9c8220:'?BeginLoadingGroup@CDataBank@NGameText@@QBEXK@Z',0x9c8630:'?BeginLoadingText@CDataBank@NGameText@@QBEXK@Z',0x9c80f0:'?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z',0x9c8340:'?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z',0x9c8700:'?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ',0x9c8fb0:'?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ',0x9fc4f0:'?MakeFreeSpace@CResourceList@@QAE_NK@Z',0x9fbfb0:'?ListRemove@CResource@@AAEXXZ',0x9fbf90:'?ListInsert@CResource@@AAEXPAV1@0@Z'}
targets.update({0x99aed0:'??0CWideString@@QAE@XZ',0xa0aaa0:'?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ',0x9e1e10:'?GFVXToAX@@YIMM@Z',0x9e1e40:'?GFVYToAY@@YIMM@Z',0x99b7d0:'??4CWideString@@QAEAAV0@ABV0@@Z',0x9de890:'??0CQuickDrawTriInfo@@QAE@ABV0@@Z',0x9e1750:'?_M_insert_overflow@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXPAVCQuickDrawTriInfo@@ABV3@ABU__false_type@2@I_N@Z',0x9f9f70:'?Uninitialise@CTexture@@QAEXXZ',0x99b510:'??1CWideString@@QAE@XZ'})
globals={};roots=[(0x9c8890,247,lookup('?GetPGroupEntry@')),(0x9c9070,245,lookup('?GetPTextEntry@')),(0x9db700,266,lookup('?AddText@CRenderManager2D'))];decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;align=lambda n,a:(n+a-1)//a*a;image=EXE.read_bytes();proof=[];baseline=json.loads((b/'run.json').read_text())['stdout'];variants=[];run_records=[]
for mask in range(8):
 raw=(b/'probe.exe').read_bytes()
 for index,(address,size,symbol) in enumerate(roots):
  if not (mask&(1<<index)):continue
  pe=pefile.PE(data=raw);last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(read_virtual_data(image,address,size));global_sites=0
  for ins in decoder.disasm(bytes(payload),address):
   pos=ins.address-address
   if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
    target=int(ins.op_str,16);name=targets[target];struct.pack_into('<I',payload,pos+1,(mapping[name]-base-pos-5)&0xffffffff)
    if mask==7:proof.append(dict(site=hex(ins.address),target=hex(target),symbol=name))
   for op in ins.operands:
    if op.type==capstone.x86.X86_OP_MEM and op.mem.base==0 and op.mem.index==0:
     target=op.mem.disp;assert target in globals,(hex(ins.address),hex(target));struct.pack_into('<I',payload,pos+ins.disp_offset,mapping[globals[target]]);global_sites+=1
  assert global_sites==0
  header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);file_size=align(size,pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+file_size-len(oracle)));oracle[start:start+size]=payload;struct.pack_into('<8sIIIIIIHHI',oracle,header,('.read%d'%index).encode(),size,rva,file_size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+size,pe.OPTIONAL_HEADER.SectionAlignment));at=mapping[symbol];off=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[off:off+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);raw=bytes(oracle)
 exe=b/('original-readers-%d.exe'%mask);exe.write_bytes(raw)
 for attempt in range(2):
  started=time.perf_counter();run=subprocess.run([str(exe),r['asset']],cwd=b,capture_output=True,text=True,timeout=30);run_records.append(dict(mask=mask,attempt=attempt,elapsed_seconds=time.perf_counter()-started,code=run.returncode,stdout=run.stdout,stderr=run.stderr));(b/'draw-queue-original-progress.json').write_text(json.dumps(run_records,indent=2));assert not run.returncode and run.stdout==baseline,(mask,attempt,run.returncode,run.stdout[-500:],run.stderr)
 variants.append(dict(mask=mask,runs=2,exe_sha256=sha(exe)));print('TEXT_DRAW_QUEUE_ORIGINAL_PASS',mask,flush=True)
assert sha(Path(r['asset']))==r['asset_sha256'];(b/'draw-queue-original-receipt.json').write_text(json.dumps(dict(status='PASS',variants=variants,runs=run_records,text=128,groups=128,draw_queue_records=128,calls=proof,context_global_operands=0,asset_unchanged=True,boundary='Original text getter/group getter/AddText roots in eight combinations twice, generated schema106 metadata/payloads and native string/queue ownership after native resources release. Display/system context borrowed and font services fail-fast; no renderer lifecycle or rasterization claim.'),indent=2))
