"""Instruction-derived roles for a member-resource GetThing/GetHealth predicate.

The caller supplies a registered, checked frame under explicit normal child ABI
contracts. The resource wrapper must retain its expected concrete type/lifetime.
Initial constructor evidence does not establish its current runtime type.
"""
from dataclasses import asdict
from capstone import x86 as x
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_receiver_reaching import _reaching
from tools.script_recovery.native_saved_x87_predicate import prove as prove_predicate

def prove_roles(frame,query_site,predicate,image,*,this_definition,member_definition,member_offset,member_vtable,owner_register='ebp',member_register='ebx',health_slot=0x420,getter_slot=0x30,getter_target=0x7e7490):
 def require(ok,why):
  if not ok:raise ValueError('member resource health roles: '+why)
 require(owner_register in ['esi','ebp'] and member_register in ['ebx','edi','esi','ebp'] and member_register!=owner_register,'register profile')
 require(type(member_offset)is int and 0<member_offset<0x1000 and member_offset%4==0,'member offset')
 ins,by,preds,depths=frame['ins'],frame['by'],frame['preds'],frame['depths']
 start,end=frame['start'],frame['end'];require(query_site in by,'query site')
 query=ins[by[query_site]]
 require(query.mnemonic=='call' and query.operands[0].type==x.X86_OP_MEM and not query.operands[0].mem.index and not query.operands[0].mem.segment and query.operands[0].mem.disp==health_slot,'query instruction')
 lo,hi=int(predicate['start'],16),int(predicate['endExclusive'],16)
 require(start<lo<hi<end and lo==query.address+query.size,'predicate adjacency')
 raw=image.bytes_at(lo,hi-lo);require(raw is not None,'predicate bytes')
 require(prove_predicate(raw,lo,image.bytes_at,0x4aa840)==predicate,'predicate replay')
 require(predicate['relation']=='ordered-greater' and predicate['threshold']==0,'predicate relation')
 previous=[i for i in ins[:by[query_site]] if i.mnemonic=='call'];require(bool(previous),'getter missing')
 getter=previous[-1];op=getter.operands[0]
 require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment and op.mem.disp==getter_slot,'getter dispatch')
 require([i.mnemonic for i in ins[by[getter.address]+1:by[query_site]]]==['mov','push'],'query setup extent')
 require(this_definition in by and member_definition in by,'origin sites')
 require(ins[by[this_definition]].mnemonic=='mov' and ins[by[this_definition]].op_str==owner_register+', ecx','owner definition')
 member=ins[by[member_definition]];require(member.mnemonic=='lea' and len(member.operands)==2,'member definition')
 dest,operand=member.operands
 owner_id=getattr(x,'X86_REG_'+owner_register.upper());member_id=getattr(x,'X86_REG_'+member_register.upper())
 require(dest.type==x.X86_OP_REG and dest.reg==member_id and operand.type==x.X86_OP_MEM and operand.mem.base==owner_id and not operand.mem.index and not operand.mem.segment and operand.mem.disp==member_offset,'member address')
 for site,register,definition in [(getter.address,member_id,member_definition),(getter.address,owner_id,this_definition),(query_site,owner_id,this_definition)]:
  definitions=_reaching(ins,preds,by[site],register,x)
  require(-1 not in definitions and {ins[k].address for k in definitions}=={definition},'reaching owner/member origin')
 creation=read_call_window(image,start,end-start,getter.address,argument_count=1)
 resource=('register',member_register);receiver=('memory',('address',('register',owner_register),4))
 getter_dispatch=('memory',('address',('memory',('address',resource,0)),getter_slot))
 health_dispatch=('memory',('address',('memory',('address',receiver,0)),health_slot))
 require(creation is not None and creation.ecx==resource and creation.target==getter_dispatch,'member getter receiver/table')
 require(creation.stack_arguments==(('stack',predicate['cleanupOffset']),),'getter hidden output')
 setup=read_call_window(image,start,end-start,query_site,{getter.address:(4,'health_actor')},argument_count=1)
 require(setup is not None and setup.ecx==receiver and setup.target==health_dispatch and setup.stack_arguments==(('result',getter.address,'health_actor'),),'query receiver/actor')
 target=image.bytes_at(member_vtable+getter_slot,4)
 require(target is not None and target==getter_target.to_bytes(4,'little'),'expected member getter target')
 nodes={k for k in frame['reachable'] if lo<=ins[k].address<hi}
 require(nodes and nodes=={by[i.address] for i in ins if lo<=i.address<hi},'predicate reachability')
 incoming=[(ins[p].address,ins[k].address) for k in sorted(nodes) for p in preds[k] if p not in nodes]
 require(incoming==[(query_site,lo)],'predicate incoming edges')
 depth=depths[by[lo]];require(all(depths[k]==depth for k in nodes),'predicate depth')
 obj,flag=predicate['cleanupOffset'],predicate['flagOffset']
 require(0<=obj and obj+12<=depth and 0<=flag<depth and not obj<=flag<obj+12,'caller local bounds/overlap')
 cleanups=[ins[k] for k in sorted(nodes) if ins[k].mnemonic=='call'];require(len(cleanups)==1,'cleanup inventory')
 cleanup=cleanups[0]
 for call,purge in [(getter,4),(query,4),(cleanup,0)]:
  contract=frame['contracts'].get(hex(call.address));require(contract is not None and contract['derivedNormalPurge']==purge,'normal purge contract')
  require(call.address+call.size in by and depths[by[call.address]]-depths[by[call.address+call.size]]==purge,'normal purge stack relation')
 require(depths[by[getter.address]]==depth+4,'getter frame relation')
 return dict(query=hex(query_site),getter=hex(getter.address),cleanup=hex(cleanup.address),resourceKind='entity-member',memberOffset=member_offset,memberRegister=member_register,ownerRegister=owner_register,expectedMemberVtable=hex(member_vtable),expectedGetterTarget=hex(getter_target),frameDepth=depth,objectOffset=obj,objectEntryOffset=obj-depth,flagOffset=flag,flagEntryOffset=flag-depth,getterSetup=asdict(creation),querySetup=asdict(setup),incoming=[dict(source=hex(a),target=hex(b)) for a,b in incoming],purges=dict(getter=4,query=4,cleanup=0),limits='Instruction roles only under checked registered frame,normal child ABI/preserved disjoint caller locals and expected current GSI/resource wrapper types. Positive exported ecxStack fields are not interpreted as caller-stack resources. Constructor/current dynamic type,lifetime,heap,reentry,exceptions and source lowering remain separate obligations.')
