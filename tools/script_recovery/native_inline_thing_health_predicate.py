"""Ordered x87 flag across an inlined, normal-ABI Thing release graph."""
import hashlib,math,struct
from capstone import x86 as x
from tools.script_recovery.native_receiver_reaching import _reaching
from tools.script_recovery.native_reviewed_saved_health_predicates import PROFILE
def prove(frame,query,image,*,positive_exit,false_exit,zero_register='ebp',thing_vtable=0x1238c8c,delete_target=0xbfe9bc,base_cleanup=0x99a2e0):
 def require(ok,why):
  if not ok:raise ValueError('inline Thing health predicate: '+why)
 ins,by,preds=frame['ins'],frame['by'],frame['preds'];require(query in by and ins[by[query]].mnemonic=='call','query site');start=ins[by[query]].address+ins[by[query]].size;require(start in by and positive_exit in by and false_exit in by and positive_exit!=false_exit,'endpoints')
 successors={k:[] for k in preds}
 for k,parents in preds.items():
  for parent in parents:successors[parent].append(k)
 nodes=set();exits=set();pending=[by[start]]
 while pending:
  k=pending.pop();require(k in frame['reachable'],'unreachable predicate')
  if ins[k].address in [positive_exit,false_exit]:exits.add(ins[k].address);continue
  if k in nodes:continue
  nodes.add(k);require(len(nodes)<=27 and successors[k],'predicate extent/termination');pending.extend(successors[k])
 require(len(nodes)==27 and exits=={positive_exit,false_exit},'27-node graph/exits');code=[ins[k] for k in sorted(nodes)]
 require([i.mnemonic for i in code]==['fcomp','fnstsw','test','jne','mov','jmp','xor','mov','cmp','mov','je','dec','mov','cmp','jne','mov','call','mov','push','call','add','lea','mov','mov','call','test','je'],'native instruction inventory')
 compare,status,mask,negative,yes,join,no,ctrl,empty,set_type,skip,dec,reload,count,keep,data,callback,free_ptr,push,delete,restore,address,clear_data,clear_ctrl,base,test,branch=code
 require(zero_register in ['ebp','edi','esi'] and thing_vtable==0x1238c8c and delete_target==0xbfe9bc and base_cleanup==0x99a2e0,'supported release profile');zero=getattr(x,'X86_REG_'+zero_register.upper())
 def reg(op,r):return op.type==x.X86_OP_REG and op.reg==r
 def imm(op,v):return op.type==x.X86_OP_IMM and op.imm==v
 def memory(op,base,disp,size=4):return op.type==x.X86_OP_MEM and op.size==size and op.mem.base==base and op.mem.disp==disp and not op.mem.index and not op.mem.segment
 op=compare.operands[0];require(op.type==x.X86_OP_MEM and op.size==4 and not op.mem.base and not op.mem.index and not op.mem.segment,'threshold operand');constant=image.bytes_at(op.mem.disp,4);require(constant is not None,'threshold bytes');threshold=struct.unpack('<f',constant)[0];require(math.isfinite(threshold) and threshold==0,'zero threshold')
 require(reg(status.operands[0],x.X86_REG_AX) and reg(mask.operands[0],x.X86_REG_AH) and imm(mask.operands[1],0x41),'ordered x87 status mask');require(imm(negative.operands[0],no.address) and reg(yes.operands[0],x.X86_REG_BL) and imm(yes.operands[1],1) and imm(join.operands[0],ctrl.address) and all(reg(o,x.X86_REG_BL) for o in no.operands),'saved BL arms')
 require(reg(ctrl.operands[0],x.X86_REG_EAX) and ctrl.operands[1].type==x.X86_OP_MEM,'control load');control_offset=ctrl.operands[1].mem.disp;object_offset=control_offset-8;require(object_offset>=0 and memory(ctrl.operands[1],x.X86_REG_ESP,control_offset),'control field offset')
 require(reg(empty.operands[0],x.X86_REG_EAX) and reg(empty.operands[1],zero) and memory(set_type.operands[0],x.X86_REG_ESP,object_offset) and imm(set_type.operands[1],thing_vtable) and imm(skip.operands[0],address.address),'empty control/header path')
 require(memory(dec.operands[0],x.X86_REG_EAX,0) and reg(reload.operands[0],x.X86_REG_EAX) and memory(reload.operands[1],x.X86_REG_ESP,control_offset) and memory(count.operands[0],x.X86_REG_EAX,0) and reg(count.operands[1],zero) and imm(keep.operands[0],address.address),'reference decrement/shared path')
 require(reg(data.operands[0],x.X86_REG_ECX) and memory(data.operands[1],x.X86_REG_EAX,8) and memory(callback.operands[0],x.X86_REG_EAX,4),'control callback/data roles')
 require(reg(free_ptr.operands[0],x.X86_REG_ECX) and memory(free_ptr.operands[1],x.X86_REG_ESP,control_offset) and reg(push.operands[0],x.X86_REG_ECX) and imm(delete.operands[0],delete_target) and reg(restore.operands[0],x.X86_REG_ESP) and imm(restore.operands[1],4),'control delete cdecl setup')
 require(reg(address.operands[0],x.X86_REG_ECX) and memory(address.operands[1],x.X86_REG_ESP,object_offset) and memory(clear_data.operands[0],x.X86_REG_ESP,object_offset+4) and reg(clear_data.operands[1],zero) and memory(clear_ctrl.operands[0],x.X86_REG_ESP,control_offset) and reg(clear_ctrl.operands[1],zero) and imm(base.operands[0],base_cleanup),'clear fields/base cleanup')
 require(all(reg(o,x.X86_REG_BL) for o in test.operands) and imm(branch.operands[0],false_exit) and branch.address+branch.size==positive_exit,'saved flag consumer/exits')
 definitions=_reaching(ins,preds,by[start],zero,x);require(definitions and -1 not in definitions,'zero-register origin')
 for k in definitions:require(ins[k].mnemonic=='xor' and len(ins[k].operands)==2 and all(reg(o,zero) for o in ins[k].operands),'zero register reaching definition')
 incoming=[(ins[p].address,ins[k].address) for k in sorted(nodes) for p in preds[k] if p not in nodes];require(incoming==[(query,start)],'sole predicate entry')
 expected={i.address:[code[n+1].address] for n,i in enumerate(code[:-1])};expected[negative.address]=[yes.address,no.address];expected[join.address]=[ctrl.address];expected[skip.address]=[dec.address,address.address];expected[keep.address]=[data.address,address.address];expected[branch.address]=[positive_exit,false_exit]
 for i in code:require({ins[k].address for k in successors[by[i.address]]}==set(expected[i.address]),'exact inline cleanup CFG')
 depth=frame['depths'][by[start]];require(object_offset+12<=depth,'caller Thing bounds')
 for i in code:
  expected_depth=depth+4 if i.address in [delete.address,restore.address] else depth;require(frame['depths'][by[i.address]]==expected_depth,'predicate stack coordinates')
 for call in [callback,delete,base]:require(frame['contracts'].get(hex(call.address),{}).get('derivedNormalPurge')==0,'normal child purge')
 part=PROFILE[-1];require(part[0]==base_cleanup and hashlib.sha256(image.bytes_at(part[0],part[1])).hexdigest()==part[2],'original base cleanup bytes')
 return dict(start=hex(start),positiveExit=hex(positive_exit),falseExit=hex(false_exit),nodes=[hex(i.address) for i in code],nativeNodeSha256=hashlib.sha256(b''.join(bytes(i.bytes) for i in code)).hexdigest(),thresholdAddress=hex(op.mem.disp),thresholdBytes=constant.hex(),threshold=0.0,relation='ordered-greater',unordered=False,flagRegister='BL',zeroRegister=zero_register,zeroDefinitions=[hex(ins[k].address) for k in sorted(definitions)],cleanupKind='inline-Thing-release',objectOffset=object_offset,controlOffset=control_offset,frameDepth=depth,callback=hex(callback.address),delete=hex(delete.address),baseCleanup=hex(base.address),incoming=[dict(source=hex(a),target=hex(b)) for a,b in incoming],limits='Original27-node BL predicate/Thing release graph,zero origins,caller bounds andnormal purge relations only. Dynamic control deleter andimported delete normal ABI,callee-saved EBX/zero-register preservation,current valid types/disjoint heap,ownership,lifetime,reentry/exceptions remain separate conditions.')
