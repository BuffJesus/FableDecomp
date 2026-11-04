"""Uniform NULL-path RET operand with explicit nonnull implementation ABI."""
import capstone as cs
from capstone import x86 as x

def prove(image,target,witness):
 def require(ok,why):
  if not ok:raise ValueError('nullable wrapper contract: '+why)
 data=bytes.fromhex(witness['bytes']);require(10<=len(data)<=32 and image.bytes_at(target,len(data))==data,'native bytes')
 md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);md.detail=True;code=list(md.disasm(data,target));require(sum(i.size for i in code)==len(data),'complete decode')
 def reg(op,r):return op.type==x.X86_OP_REG and op.reg==r
 def mem(op,r,offset):return op.type==x.X86_OP_MEM and op.mem.base==r and op.mem.disp==offset and not op.mem.index and not op.mem.segment
 require(len(code)>=6 and code[0].mnemonic=='mov' and reg(code[0].operands[0],x.X86_REG_ECX) and mem(code[0].operands[1],x.X86_REG_ECX,witness['receiverOffset']) and witness['receiverOffset'] in [4,8],'nullable receiver')
 require(code[1].mnemonic=='test' and all(reg(op,x.X86_REG_ECX) for op in code[1].operands),'null test')
 if code[2].mnemonic=='je':
  require([i.mnemonic for i in code]==['mov','test','je','mov','jmp','ret'],'void graph');table,tail,ret=code[3:];require(code[2].operands[0].imm==ret.address,'null exit')
 else:
  require(code[2].mnemonic=='jne' and [i.mnemonic for i in code[4:]]==['ret','mov','jmp'],'value graph');ret,table,tail=code[4:];require(code[2].operands[0].imm==table.address,'nonnull entry');null=code[3]
  require(null.mnemonic=='xor' and all(reg(op,x.X86_REG_AL) for op in null.operands) or null.mnemonic=='mov' and reg(null.operands[0],x.X86_REG_EAX) and null.operands[1].type==x.X86_OP_IMM,'null value')
 require(table.mnemonic=='mov' and reg(table.operands[0],x.X86_REG_EAX) and mem(table.operands[1],x.X86_REG_ECX,0),'implementation table load')
 slot=int(witness['implementationSlot'],16);require(0<=slot<=256 and slot%4==0 and mem(tail.operands[0],x.X86_REG_EAX,slot),'implementation slot')
 purge=ret.operands[0].imm if ret.operands else 0;require(type(witness['purgeBytes'])is int and witness['purgeBytes']==purge and 0<=purge<=64 and purge%4==0,'return operand')
 require(witness['nonNullChildABIRequired']==dict(stackPurge=purge,ordinaryThiscall=True) and witness['closedAllPathReturnProof'] is False and witness['implementationVerified'] is False,'nonnull proof scope')
 return dict(purgeBytes=purge,receiverOffset=witness['receiverOffset'],implementationSlot=hex(slot),nullReturn=hex(ret.address),nonNullChildABIRequired=witness['nonNullChildABIRequired'],implementationVerified=False,closedAllPathReturnProof=False,limits='Original NULL-path return operand and dispatch graph only; current implementation normal ABI and valid runtime type/lifetime remain caller conditions.')
