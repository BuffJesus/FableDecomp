"""Checked linear entry initialization of a caller-owned 16-byte resource.

This proves an initial stack layout, not acquisition/current lifetime or a host
NewResource implementation. Constructor code and frame separation are checked.
"""
import hashlib
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x

def prove(image,fn,raw,profile):
 def require(ok,why):
  if not ok:raise ValueError('owned local resource origin: '+why)
 start=int(fn['address'],16);end=int(profile['prefixEnd'],16);require(start<end<=start+len(raw),'entry prefix bounds');data=raw[:end-start];require(hashlib.sha256(data).hexdigest()==profile['prefixSha256'],'entry prefix pin')
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;code=list(md.disasm(data,start));require(code and sum(i.size for i in code)==len(data),'entry decode')
 expected={0x99a380:[('mov','eax, ecx'),('mov','dword ptr [eax], 0x129a7d8'),('mov','dword ptr [eax + 4], 0'),('ret','')],0x99e4b0:[('mov','eax, ecx'),('mov','dword ptr [eax], 0'),('inc','dword ptr [0x13bd800]'),('ret','')]}
 helpers={}
 for name,address,size in [('base',0x99a380,16),('CString',0x99e4b0,15)]:
  witness=profile['constructors'][name];b=bytes.fromhex(witness['bytes']);require(int(witness['address'],16)==address and len(b)==size and image.bytes_at(address,size)==b,'constructor bytes');decoded=list(md.disasm(b,address));require([(i.mnemonic,i.op_str) for i in decoded]==expected[address],'constructor effects');helpers[address]=name
 require(set(profile['constructors'])=={'base','CString'},'constructor inventory')
 regs={r:('entry_register',r) for r in [x.X86_REG_EAX,x.X86_REG_EBX,x.X86_REG_ECX,x.X86_REG_EDX,x.X86_REG_ESI,x.X86_REG_EDI,x.X86_REG_EBP]};regs[x.X86_REG_ECX]=('entry_this',);sp=0;memory={};calls=[];allocation=None
 def value(op):
  if op.type==x.X86_OP_IMM:return op.imm&0xffffffff
  require(op.type==x.X86_OP_REG and op.reg in regs,'source operand');return regs[op.reg]
 def address(op):
  require(op.type==x.X86_OP_MEM and op.mem.base==x.X86_REG_ESP and not op.mem.index and not op.mem.segment and op.size==4,'private stack store');return sp+op.mem.disp
 for ordinal,i in enumerate(code):
  ops=i.operands
  if i.mnemonic=='sub':
   require(ordinal==0 and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP and ops[1].type==x.X86_OP_IMM and 16<=ops[1].imm<=4096 and ops[1].imm%4==0,'initial private allocation');allocation=ops[1].imm;sp-=allocation
  elif i.mnemonic=='push':sp-=4;memory[sp]=value(ops[0])
  elif i.mnemonic=='xor':
   require(all(op.type==x.X86_OP_REG and op.size==4 for op in ops) and ops[0].reg==ops[1].reg and ops[0].reg in regs,'zero definition');regs[ops[0].reg]=0
  elif i.mnemonic=='lea':
   require(ops[0].type==x.X86_OP_REG and ops[0].reg in regs,'stack address destination');regs[ops[0].reg]=('stack',address(ops[1]))
  elif i.mnemonic=='mov':
   v=value(ops[1])
   if ops[0].type==x.X86_OP_REG:
    require(ops[0].reg in regs and ops[0].size==4,'register destination');regs[ops[0].reg]=v
   else:memory[address(ops[0])]=v
  elif i.mnemonic=='call':
   require(ops[0].type==x.X86_OP_IMM and ops[0].imm in helpers,'unreviewed initialization call');target=ops[0].imm;receiver=regs[x.X86_REG_ECX];require(isinstance(receiver,tuple) and receiver[0]=='stack','constructor receiver');coordinate=receiver[1];calls.append(dict(site=hex(i.address),target=hex(target),receiverEntryOffset=coordinate))
   if target==0x99a380:memory[coordinate]=0x129a7d8;memory[coordinate+4]=0
   else:memory[coordinate]=0
   regs[x.X86_REG_EAX]=receiver;regs[x.X86_REG_ECX]=None;regs[x.X86_REG_EDX]=None
  else:require(False,'nonlinear or unsupported initialization instruction')
 require(allocation is not None and len(calls)==2 and [c['target'] for c in calls]==['0x99a380','0x99e4b0'],'initial call inventory')
 coordinate=profile['resourceEntryOffset'];require(type(coordinate)is int and profile['resourceBytes']==16 and profile['resourceVtable']=='0x127094c' and -allocation<=coordinate and coordinate+16<=0,'private resource extent');require(calls[0]['receiverEntryOffset']==coordinate and calls[1]['receiverEntryOffset']==coordinate-4,'base/string constructor coordinates')
 require([memory.get(coordinate+k) for k in [0,4,8,12]]==[0x127094c,0,0,0],'initial resource fields')
 return dict(resourceEntryOffset=coordinate,resourceBytes=16,resourceVtable='0x127094c',initialData=0,initialControl=0,privateAllocationBytes=allocation,prefixEnd=hex(end),depthAtPrefixEnd=-sp,constructorCalls=calls,normalConstructorEffectsVerified=True,currentLifetimeVerified=False,hostNewResourceImplementationVerified=False,limits='Decoded linear registered entry and original resource-base/CString constructor effects; initial 16-byte caller-owned layout only. Valid disjoint stack/global memory and ordinary instruction execution assumed. Acquisition, current runtime type/lifetime, host NewResource implementation, heap, reentry and exceptions are outside this proof.')
