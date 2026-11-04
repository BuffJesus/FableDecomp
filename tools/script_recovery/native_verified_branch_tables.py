"""Validate bounded immutable x86 switch tables before exposing CFG edges."""
import hashlib
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x

def verify(raw,start,image,rows):
 def require(ok,why):
  if not ok:raise ValueError('native branch table: '+why)
 require(isinstance(rows,list) and 0<len(rows)<=32,'invalid table inventory')
 require(image.bytes_at(start,len(raw))==raw,'code image changed')
 d=Cs(CS_ARCH_X86,CS_MODE_32);d.detail=True;ins=list(d.disasm(raw,start));by={i.address:n for n,i in enumerate(ins)}
 require(ins and sum(i.size for i in ins)==len(raw),'incomplete code decode')
 tables=[];targets={};protected=[]
 for row in rows:
  require(isinstance(row,dict) and set(row)=={'site','tableBytes'},'invalid table descriptor')
  try:site=int(row['site'],16);data=bytes.fromhex(row['tableBytes'])
  except (KeyError,TypeError,ValueError):raise ValueError('native branch table: invalid descriptor encoding')
  require(site in by and site not in targets,'missing or duplicate dispatch')
  n=by[site];require(n>=2,'missing range guard')
  compare,guard,jump=ins[n-2:n+1]
  require(compare.address+compare.size==guard.address and guard.address+guard.size==jump.address,'noncontiguous range guard')
  require(jump.mnemonic=='jmp' and len(jump.operands)==1 and jump.operands[0].type==x.X86_OP_MEM,'not a memory switch')
  operand=jump.operands[0];mem=operand.mem
  require(operand.size==4 and mem.base==0 and mem.index!=0 and mem.scale==4 and not mem.segment,'unsupported table addressing')
  require(compare.mnemonic=='cmp' and len(compare.operands)==2 and compare.operands[0].type==x.X86_OP_REG
   and compare.operands[0].size==4 and compare.operands[0].reg==mem.index and compare.operands[1].type==x.X86_OP_IMM,'wrong index range compare')
  maximum=compare.operands[1].imm;require(0<=maximum<4096,'unsupported table bound')
  require(guard.mnemonic=='ja' and len(guard.operands)==1 and guard.operands[0].type==x.X86_OP_IMM and guard.operands[0].imm in by,'missing unsigned default branch')
  require(len(data)==4*(maximum+1) and image.bytes_at(mem.disp,len(data))==data,'table extent or bytes changed')
  case_targets=tuple(int.from_bytes(data[k:k+4],'little') for k in range(0,len(data),4))
  require(all(a in by for a in case_targets),'case target outside code or instruction boundary')
  protected.append((compare.address,jump.address+jump.size))
  targets[site]=case_targets
  tables.append(dict(site=hex(site),compare=hex(compare.address),rangeBranch=hex(guard.address),default=hex(guard.operands[0].imm),index=d.reg_name(mem.index),maximum=maximum,table=hex(mem.disp),tableBytes=data.hex(),targets=[hex(a) for a in case_targets]))
 # No explicit branch may enter after the compare and bypass its flags/index check.
 for i in ins:
  if i.group(x.X86_GRP_JUMP):
   if i.operands[0].type==x.X86_OP_IMM:destinations=[i.operands[0].imm]
   elif i.address in targets:destinations=targets[i.address]
   else:raise ValueError('native branch table: unreviewed indirect branch')
   require(all(a in by for a in destinations),'explicit branch outside code or instruction boundary')
  elif i.mnemonic=='call' and len(i.operands)==1 and i.operands[0].type==x.X86_OP_IMM:
   destinations=[i.operands[0].imm]
   require(all(not start<=a<start+len(raw) or a in by for a in destinations),'internal call is not at an instruction boundary')
  else:continue
  for a in destinations:
   require(not any(first<a<last for first,last in protected),'control transfer bypasses range compare')
 return dict(targets=targets,tables=tables,codeSha256=hashlib.sha256(raw).hexdigest(),limits='Original immutable table bytes, unsigned index guard and aligned internal targets only; no runtime mutation, engine, reentry or exception proof.')
