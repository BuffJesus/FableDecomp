from pathlib import Path
import json,sys,re,struct,subprocess,hashlib,pefile,capstone
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from threaded_process_oracle import ADDRESS,ENTRY,SIZE,SYMBOL
from threaded_file_resize_oracle import ENTRY as RESIZE,SIZE as RESIZE_SIZE,SYMBOL as RESIZE_SYMBOL,ADDRESS as RESIZE_ADDRESS
from pe_oracle import EXE,read_virtual_data
def run(build,baseline,child_env=None,mode="--writes"):
 B=Path(build);receipt=dict(stdout=baseline);mapping={};sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 for line in (B/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 by_address={v:k for k,v in ADDRESS.items()};image=EXE.read_bytes();raw=(B/'probe.exe').read_bytes();pe=pefile.PE(data=raw);start=mapping[SYMBOL];body=bytearray(read_virtual_data(image,ENTRY,SIZE));md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True;proof=[];absolute=set()
 for ins in md.disasm(bytes(body),ENTRY):
  pos=ins.address-ENTRY
  if ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16)
   if not ENTRY<=target<ENTRY+SIZE:
    name=by_address[target];struct.pack_into('<I',body,pos+1,(mapping[name]-start-pos-5)&0xffffffff);proof.append(dict(site=hex(ins.address),kind='relative',symbol=name))
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_MEM and not op.mem.base and not op.mem.index:
    name=by_address[op.mem.disp];site=pos+ins.disp_offset;struct.pack_into('<I',body,site,mapping[name]);absolute.add(site);proof.append(dict(site=hex(ins.address),kind='memory',symbol=name))
   if op.type==capstone.x86.X86_OP_IMM and ins.bytes[0] not in (0xe8,0xe9) and op.imm in by_address:
    name=by_address[op.imm];site=pos+ins.imm_offset;assert ins.imm_size==4;struct.pack_into('<I',body,site,mapping[name]);absolute.add(site);proof.append(dict(site=hex(ins.address),kind='immediate',symbol=name))
 assert len(proof)==27 and len(absolute)==17,(len(proof),len(absolute))
 assert sum(ins.size for ins in md.disasm(read_virtual_data(image,ENTRY,SIZE),ENTRY))==SIZE
 runs=[]
 for mask in (0,1,2,3):
  output=bytearray(raw)
  if mask&1:
   at=pe.get_offset_from_rva(start-pe.OPTIONAL_HEADER.ImageBase);output[at:at+SIZE]=body
  if mask&2:
   original=bytearray(read_virtual_data(image,RESIZE,RESIZE_SIZE));expected=set(RESIZE_ADDRESS.values());seen=set()
   for ins in md.disasm(bytes(original),RESIZE):
    for op in ins.operands:
     if op.type==capstone.x86.X86_OP_MEM and not op.mem.base and not op.mem.index:
      name=next(n for n,a in RESIZE_ADDRESS.items() if a==op.mem.disp);site=ins.address-RESIZE+ins.disp_offset;struct.pack_into('<I',original,site,mapping[name]);seen.add(op.mem.disp)
   assert seen==expected;at=pe.get_offset_from_rva(mapping[RESIZE_SYMBOL]-pe.OPTIONAL_HEADER.ImageBase);output[at:at+RESIZE_SIZE]=original
  exe=B/('worker-resize-original-%d.exe'%mask);exe.write_bytes(output)
  for attempt in range(2):
   run=subprocess.run([str(exe),str(B/('worker-resize-%d-%d-owned.bin'%(mask,attempt))),mode],cwd=B,env=child_env,capture_output=True,text=True,timeout=30);assert run.returncode==0 and run.stdout==receipt['stdout'],(mask,attempt,run.returncode,run.stdout,run.stderr)
  runs.append(dict(mask=mask,runs=2,exe_sha256=sha(exe)))
 result=dict(status='PASS',runs=runs,baseline_sha256=hashlib.sha256(baseline.encode()).hexdigest(),worker_original_bytes=SIZE,resize_original_bytes=RESIZE_SIZE,worker_relocations=proof,absolute_sites=sorted(absolute),boundary='Four actual original/recreated worker/resize combinations, each twice, aligned WriteFileEx, typed callback leaves, native joins and exclusively owned output files. Original worker import calls remap to actual Windows IAT cells; installer remains absent. No read-barrier or error-path claim.')
 (B/'write-original-receipt.json').write_text(json.dumps(result,indent=2));return result
