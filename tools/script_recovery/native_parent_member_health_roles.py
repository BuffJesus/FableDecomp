"""Nested parent resource roles under checked frame andcurrent-type contracts."""
from dataclasses import asdict
from capstone import x86 as x
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_receiver_reaching import _reaching
from tools.script_recovery.native_saved_register_x87_predicate import prove as prove_predicate

def prove_roles(frame,query,predicate,image,*,this_definition,parent_offset,member_offset,member_vtable,owner_register='esi',health_slot=0x420,getter_slot=0x30,getter_target=0x7e7490,inline_cleanup=False):
 def require(ok,why):
  if not ok:raise ValueError('parent member health roles: '+why)
 require(owner_register in ['esi','ebp'] and type(parent_offset)is int and type(member_offset)is int and 0<parent_offset<4096 and 0<member_offset<4096 and parent_offset%4==member_offset%4==0,'register/field profile')
 ins,by=frame['ins'],frame['by'];require(query in by and this_definition in by,'query/owner sites')
 require(ins[by[this_definition]].mnemonic=='mov' and ins[by[this_definition]].op_str==owner_register+', ecx','owner entry definition')
 call=ins[by[query]];require(call.mnemonic=='call' and call.operands[0].type==x.X86_OP_MEM and not call.operands[0].mem.index and not call.operands[0].mem.segment and call.operands[0].mem.disp==health_slot,'query dispatch')
 earlier=[i for i in ins[:by[query]] if i.mnemonic=='call'];require(bool(earlier),'getter missing');getter=earlier[-1].address
 require([i.mnemonic for i in ins[by[getter]+1:by[query]]]==['mov','push'],'query setup extent')
 require(type(inline_cleanup)is bool,'inline cleanup option')
 if inline_cleanup:
  from tools.script_recovery.native_inline_thing_health_predicate import prove as prove_inline
  actual=prove_inline(frame,query,image,positive_exit=int(predicate['positiveExit'],16),false_exit=int(predicate['falseExit'],16));object_offset=predicate['objectOffset'];cleanup=int(predicate['baseCleanup'],16)
 else:
  actual=prove_predicate(frame,query,image.bytes_at,0x4aa840,positive_exit=int(predicate['positiveExit'],16),false_exit=int(predicate['falseExit'],16));object_offset=predicate['cleanupOffset'];cleanup=int(predicate['cleanup'],16)
 require(actual==predicate and predicate['threshold']==0 and predicate['relation']=='ordered-greater','original BL predicate')
 parent=('memory',('address',('register',owner_register),parent_offset));resource=('address',parent,member_offset);resource_table=('memory',resource);receiver=('memory',('address',('register',owner_register),4));health_table=('memory',('address',receiver,0))
 creation=read_call_window(image,frame['start'],frame['end']-frame['start'],getter,argument_count=1)
 require(creation is not None and creation.ecx==resource and creation.target==('memory',('address',resource_table,getter_slot)) and creation.stack_arguments==(('stack',object_offset),),'nested member getter receiver/table/hidden output')
 setup=read_call_window(image,frame['start'],frame['end']-frame['start'],query,{getter:(4,'health_actor')},argument_count=1)
 require(setup is not None and setup.ecx==receiver and setup.target==('memory',('address',health_table,health_slot)) and setup.stack_arguments==(('result',getter,'health_actor'),),'query receiver/actor result')
 require(image.bytes_at(member_vtable+getter_slot,4)==getter_target.to_bytes(4,'little'),'expected resource getter target')
 owner_id=getattr(x,'X86_REG_'+owner_register.upper())
 for site in [getter,query]:
  definitions=_reaching(ins,frame['preds'],by[site],owner_id,x);require(-1 not in definitions and {ins[k].address for k in definitions}=={this_definition},'reaching owner origin')
 depths=frame['depths']
 for site,purge in [(getter,4),(query,4),(cleanup,0)]:
  require(frame['contracts'].get(hex(site),{}).get('derivedNormalPurge')==purge and site+ins[by[site]].size in by and depths[by[site]]-depths[by[site+ins[by[site]].size]]==purge,'normal purge contract')
 require(depths[by[getter]]==predicate['frameDepth']+4 and object_offset+12<=predicate['frameDepth'],'caller object bounds/depth')
 result=dict(query=hex(query),getter=hex(getter),cleanup=hex(cleanup),resourceKind='parent-member',parentPointerOffset=parent_offset,memberOffset=member_offset,ownerRegister=owner_register,expectedMemberVtable=hex(member_vtable),expectedGetterTarget=hex(getter_target),frameDepth=predicate['frameDepth'],objectOffset=object_offset,getterSetup=asdict(creation),querySetup=asdict(setup),purges=dict(getter=4,query=4,cleanup=0),limits='Native roles only under checked normal frame,valid expected current parent/GSI/resource types,disjoint locals andordinary child ABI;initial constructor evidence does not prove current lifetime,heap,reentry/exceptions.')

 if inline_cleanup:result['cleanupKind']='inline-Thing-release';result['inlineControlCallback']=predicate['callback'];result['inlineDelete']=predicate['delete']
 return result
