from pathlib import Path
import sys,json,re,struct,hashlib,subprocess,pefile,capstone
P=Path(__file__).resolve().parent;R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]);r=json.loads((b/'composition.json').read_text());mapping={};sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
lookup=lambda prefix:next(n for n in mapping if n.startswith(prefix))
targets={0x993ca0:'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z',0x4428f0:'?resize@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXIABJ@Z',0x9d5620:'?Sizeof@CBankFileAsyncData@@QBEKXZ',0x9fbf20:'?SetResourceSize@CResource@@QAEXK@Z',0x99b7d0:'??4CWideString@@QAEAAV0@ABV0@@Z',0x99b510:'??1CWideString@@QAE@XZ',0x996390:'?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ',0x99efb0:'??4CCharString@@QAEAAV0@ABV0@@Z',0x99eae0:'??1CCharString@@QAE@XZ',0x9cb7d0:lookup('?resize@?$vector@VCTextTag@NGameText@@V?$allocator@VCTextTag@NGameText@@@_STL@@@_STL@@QAEXI@Z'),0x9c7d90:'?Sizeof@CTextEntry@NGameText@@QBEKXZ'}
globals={0x138de48:'?g_NativeTextReadContext_0138DE48@@3KA',0x138e064:'?g_NativeTextReadContext_0138E064@@3KA'};roots=[(0x9c8470,292,lookup('?ReadFromDataStream@CGroupEntry')),(0x9c8de0,452,lookup('?ReadFromDataStream@CTextEntry'))];decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;align=lambda n,a:(n+a-1)//a*a;image=EXE.read_bytes();proof=[];baseline=json.loads((b/'run.json').read_text())['stdout'];variants=[]
for mask in range(4):
 raw=(b/'probe.exe').read_bytes()
 for index,(address,size,symbol) in enumerate(roots):
  if not (mask&(1<<index)):continue
  pe=pefile.PE(data=raw);last=pe.sections[-1];rva=align(last.VirtualAddress+max(last.Misc_VirtualSize,last.SizeOfRawData),pe.OPTIONAL_HEADER.SectionAlignment);base=pe.OPTIONAL_HEADER.ImageBase+rva;payload=bytearray(read_virtual_data(image,address,size));global_sites=0
  for ins in decoder.disasm(bytes(payload),address):
   pos=ins.address-address
   if ins.mnemonic=='call' and ins.bytes[0]==0xe8:
    target=int(ins.op_str,16);name=targets[target];struct.pack_into('<I',payload,pos+1,(mapping[name]-base-pos-5)&0xffffffff)
    if mask==3:proof.append(dict(site=hex(ins.address),target=hex(target),symbol=name))
   for op in ins.operands:
    if op.type==capstone.x86.X86_OP_MEM and op.mem.base==0 and op.mem.index==0:
     target=op.mem.disp;assert target in globals,(hex(ins.address),hex(target));struct.pack_into('<I',payload,pos+ins.disp_offset,mapping[globals[target]]);global_sites+=1
  assert global_sites==(6 if index==1 else 0)
  header=last.get_file_offset()+40;assert header+40<=pe.OPTIONAL_HEADER.SizeOfHeaders and not any(raw[header:header+40]);start=align(len(raw),pe.OPTIONAL_HEADER.FileAlignment);file_size=align(size,pe.OPTIONAL_HEADER.FileAlignment);oracle=bytearray(raw);oracle.extend(bytes(start+file_size-len(oracle)));oracle[start:start+size]=payload;struct.pack_into('<8sIIIIIIHHI',oracle,header,('.read%d'%index).encode(),size,rva,file_size,start,0,0,0,0,0x60000020);struct.pack_into('<H',oracle,pe.DOS_HEADER.e_lfanew+6,pe.FILE_HEADER.NumberOfSections+1);struct.pack_into('<I',oracle,pe.OPTIONAL_HEADER.get_field_absolute_offset('SizeOfImage'),align(rva+size,pe.OPTIONAL_HEADER.SectionAlignment));at=mapping[symbol];off=pe.get_offset_from_rva(at-pe.OPTIONAL_HEADER.ImageBase);oracle[off:off+5]=b'\xe9'+struct.pack('<I',(base-at-5)&0xffffffff);raw=bytes(oracle)
 exe=b/('original-readers-%d.exe'%mask);exe.write_bytes(raw)
 for attempt in range(2):
  run=subprocess.run([str(exe),r.get('asset',str(b/'probe.exe'))],cwd=b,capture_output=True,text=True,timeout=30);assert not run.returncode and run.stdout==baseline,(mask,attempt,run.returncode,run.stdout[-500:],run.stderr)
 variants.append(dict(mask=mask,runs=2,exe_sha256=sha(exe)));print('TEXT_PAYLOAD_ORIGINAL_VARIANT_PASS',mask)
(b/'matrix-original-receipt.json').write_text(json.dumps(dict(status='PASS',variants=variants,text=512,groups=512,calls=proof,context_global_operands=6,synthetic=True,boundary='Original/recreated text/group reader combinations usecommon recreated memory/string/vector/resource-size dependencies. Full-field rawCRC oracle andcontext guards separate. No scheduler/resource loading orrenderer claim.'),indent=2))
