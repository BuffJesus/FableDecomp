"""Reconstruct an entity frame from reviewed native return/static-type profiles.

No exported depth is used as an anchor. Indirect runtime types and the normal
ABIs of opaque/non-null children remain explicit caller conditions.
"""
import hashlib,json
from collections import deque
import capstone as cs
from capstone import x86 as x
from tools.script_recovery.native_parent_member_call_contracts import resolve
from tools.script_recovery.native_receiver_reaching import _cfg
from tools.script_recovery.native_external_import_contracts import prove as prove_import
from tools.script_recovery.native_hit_health_entry import prove_frame

def prove(raw,fn,image,row,*,entity_vtable,scope_start,scope_end,frame_depth,this_definition,actor_definition,actor_profile,owner_register):
 def require(ok,why):
  if not ok:raise ValueError('member frame contracts: '+why)
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16)
 require(len(raw)==end-start and hashlib.sha256(raw).hexdigest()==row['cacheProof']['nativeSha256'],'native caller bytes')
 require(row['cacheProof']['interfaceOffset']==4 and row['globalInterfaceAddresses']==[0x143e8f8],'entity interface profile')
 md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));by={i.address:k for k,i in enumerate(ins)}
 require(sum(i.size for i in ins)==len(raw),'native decode')
 conditional=row['conditionalNonNullWrappers'];require(set(conditional)=={'0x7e7390','0x7e73d0','0x7e7450','0x4aabd0','0x4aad40','0x4aad20','0x4aab30'},'conditional wrapper inventory')
 external=row.get('externalNormalABIs',{});require(set(external)<={'0xbfe9bc'},'external ABI inventory');external_reports={}
 def words(target):
  if hex(target) in external:
   evidence=prove_import(image,target,external[hex(target)]);external_reports[hex(target)]=evidence;return evidence['purgeBytes']//4
  witness=conditional.get(hex(target));require(witness is not None,'missing bounded return profile')
  data=bytes.fromhex(witness['bytes']);require(15<=len(data)<=20 and image.bytes_at(target,len(data))==data,'nullable wrapper bytes')
  code=list(md.disasm(data,target));require(sum(i.size for i in code)==len(data),'nullable decode')
  require(code[0].op_str=='ecx, dword ptr [ecx + '+str(8 if target>=0x700000 else 4)+']' and code[1].op_str=='ecx, ecx','nullable receiver')
  returns=[i for i in code if i.mnemonic=='ret'];tails=[i for i in code if i.mnemonic=='jmp'];require(len(returns)==len(tails)==1,'nullable return/tail inventory')
  ret,tail=returns[0],tails[0];require(tail.operands[0].type==x.X86_OP_MEM and tail.operands[0].mem.base==x.X86_REG_EAX and not tail.operands[0].mem.index,'nullable implementation dispatch')
  if code[2].mnemonic=='je':
   require([i.mnemonic for i in code]==['mov','test','je','mov','jmp','ret'] and code[2].operands[0].imm==ret.address,'nullable void graph')
  else:
   require([i.mnemonic for i in code]==['mov','test','jne','xor','ret','mov','jmp'] and code[2].operands[0].imm==code[5].address and code[3].op_str=='al, al','nullable bool graph')
  purge=ret.operands[0].imm if ret.operands else 0
  require(witness['purgeBytes']==purge and witness['nonNullChildABIRequired']==dict(stackPurge=purge,ordinaryThiscall=True) and witness['closedAllPathReturnProof'] is False,'nonnull ABI precondition')
  require(witness['implementationSlot']==hex(tail.operands[0].mem.disp) and purge%4==0,'nonnull slot/purge')
  return purge//4
 for name in conditional:words(int(name,16))
 contracts,reviews=resolve(image,raw,start,row,words,global_interface_addresses=tuple(row['globalInterfaceAddresses']))
 preds=_cfg(ins,x,cs,image);require(preds is not None,'native CFG');succ={k:[] for k in preds}
 for k,parents in preds.items():
  for parent in parents:succ[parent].append(k)
 reachable=set();pending=[0]
 while pending:
  k=pending.pop()
  if k in reachable:continue
  reachable.add(k);pending.extend(succ[k])
 edges={k:[] for k in reachable};call_records={int(c['site'],16):c for c in fn['calls']+fn['indirectCalls']}
 require(len(call_records)==len(fn['calls'])+len(fn['indirectCalls']),'source call inventory')
 actual_calls={i.address for i in ins if i.mnemonic=='call'};require(actual_calls==set(call_records),'native/source calls')
 unknown=row['requiredIndirectABI'];require(set(contracts).isdisjoint(int(a,16) for a in unknown) and set(contracts)|{int(a,16) for a in unknown}==actual_calls,'complete call contract inventory')
 for k in sorted(reachable):
  i=ins[k];ops=i.operands;delta=0
  if i.mnemonic=='push':require(not(ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP),'push ESP');delta=4
  elif i.mnemonic=='pop':require(ops[0].reg!=x.X86_REG_ESP,'pop ESP');delta=-4
  elif i.mnemonic in ['add','sub'] and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP:
   require(ops[1].type==x.X86_OP_IMM,'stack adjustment');delta=ops[1].imm*(1 if i.mnemonic=='sub' else -1)
  elif i.mnemonic=='lea' and ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP:
   require(ops[1].type==x.X86_OP_MEM and ops[1].mem.base==x.X86_REG_ESP and not ops[1].mem.index,'stack LEA');delta=-ops[1].mem.disp
  elif i.mnemonic=='call':
   if i.address not in contracts:continue
   delta=-contracts[i.address]['purgeBytes']
  elif i.mnemonic=='ret':require(not ops,'caller return ABI');continue
  else:require(x.X86_REG_ESP not in i.regs_access()[1],'unmodeled stack write')
  for nxt in succ[k]:
   if nxt in reachable:edges[k].append((nxt,delta));edges[nxt].append((k,-delta))
 depths={0:0};pending=[0]
 while pending:
  k=pending.pop()
  for nxt,delta in edges[k]:
   value=depths[k]+delta;require(0<=value<=4096 and value%4==0,'native stack domain')
   if nxt in depths:require(depths[nxt]==value,'conflicting native stack edges')
   else:depths[nxt]=value;pending.append(nxt)
 require(set(depths)==reachable and all(depths[k]==0 for k in reachable if ins[k].mnemonic=='ret'),'unanchored native stack component')
 for a,witness in unknown.items():
  i=ins[by[int(a,16)]];purge=depths[by[i.address]]-depths[by[i.address+i.size]]
  require(i.bytes.hex()==witness['instructionBytes'] and witness['targetProvenanceVerified'] is False and witness['requiredNormalPurge']==purge and 0<=purge<=64 and purge%4==0,'indirect ABI requirement')
 projected=dict(fn,**{key:[dict(c,depth=depths[by[int(c['site'],16)]]) for c in fn[key]] for key in ['calls','indirectCalls']})
 frame=prove_frame(raw,projected,image,entity_vtable=entity_vtable,scope_start=scope_start,scope_end=scope_end,frame_depth=frame_depth,mask_offset=0,this_definition=this_definition,actor_definition=actor_definition,actor_profile=actor_profile,owner_register=owner_register)
 require(all(frame['depths'][k]==depth for k,depth in depths.items()),'registered frame replay')
 for review in reviews:
  if review['target'] in conditional:review['nonNullChildABIRequired']=conditional[review['target']]['nonNullChildABIRequired'];review['closedAllPathReturnProof']=False
  if review['target'] in external_reports:review['externalNormalABIRequired']=external_reports[review['target']];review['closedAllPathReturnProof']=False
  if 'savedParentTable' in review:
   saved=review['savedParentTable'];store=int(saved['cacheStore'],16);reload=int(saved['cacheRead'],16);coordinate=saved['cacheCoordinate']-depths[by[store]]
   require(ins[0].mnemonic=='sub' and ins[0].operands[0].reg==x.X86_REG_ESP and ins[0].operands[1].type==x.X86_OP_IMM,'private frame allocation')
   require(-ins[0].operands[1].imm<=coordinate and coordinate+4<=0 and saved['relativeEspAfterEpoch']==depths[by[store]]-depths[by[int(saved['dispatch'],16)]],'private saved table frame bounds/ESP replay');saved['callerEntryCoordinate']=coordinate
 changes=[dict(site=c['site'],original=c['depth'],derived=depths[by[int(c['site'],16)]]) for c in fn['calls']+fn['indirectCalls'] if c['depth']!=depths[by[int(c['site'],16)]]]
 report=dict(reachableInstructions=len(reachable),callCount=len(actual_calls),knownStaticProfileCalls=len(contracts),requiredIndirectABI=unknown,conditionalNonNullWrappers=conditional,correctedExportDepths=changes,sourceCallEvidenceUnchanged=True,callReviews=reviews,limits='Native entry/RET/stack relations,reviewed static GSI/member target profiles and bounded RET operands;opaque normal-child balance and7 nonnull implementation ABIs remain conditions. Indirect target requirements,if any,remain explicit parent-join requirements. Initial parent/member constructor evidence does not establish current runtime wrapper type/lifetime.')
 if external_reports:report['externalNormalABIConditions']=external_reports;report['limits']+=' Imported operator delete current binding/normal cdecl0 is an explicit condition,not external implementation proof.'
 return frame,report
