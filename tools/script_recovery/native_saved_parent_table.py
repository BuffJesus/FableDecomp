"""Saved parent resource-table bits across a bounded normal caller epoch."""
import hashlib
import capstone as cs
from capstone import x86 as x
from tools.script_recovery.native_receiver_reaching import _cfg
def prove(image,raw,start,profile,states,call_records):
 def require(ok,why):
  if not ok:raise ValueError('saved parent table: '+why)
 expected=dict(ordinaryMsvcCalleeSaved=True,normalChildStackBalanceAssumed=True,callerFramePreserved=True,currentExpectedTypesRequired=True,engineReentryVerified=False,exceptionsVerified=False,heapVerified=False)
 require(profile['abiContract']==expected and all(type(v)is bool for v in profile['abiContract'].values()),'normal caller ABI conditions')
 require(hashlib.sha256(raw).hexdigest()==profile['nativeSha256'],'native body')
 md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));by={i.address:k for k,i in enumerate(ins)};require(sum(i.size for i in ins)==len(raw),'native decode')
 load,store,reload,dispatch=(int(profile[k],16) for k in ['tableLoad','cacheStore','cacheRead','dispatch']);require(all(a in by for a in [load,store,reload,dispatch]) and load<store<reload<dispatch,'epoch endpoints')
 register=profile['tableRegister'];require(register in ['eax','ebx','ecx','edx','esi','edi','ebp'],'table register');register_id=getattr(x,'X86_REG_'+register.upper());parent,member=profile['parentOffset'],profile['memberOffset']
 origin=states.get(store,{}).get(register);require(origin is not None and origin[0]=='parent_member_table_snapshot' and origin[1]==parent and origin[2]==member and origin[4]==load,'original nested table snapshot')
 s,r=ins[by[store]],ins[by[reload]]
 def stack(op):return op.type==x.X86_OP_MEM and op.size==4 and op.mem.base==x.X86_REG_ESP and not op.mem.index and not op.mem.segment
 require(s.mnemonic==r.mnemonic=='mov' and stack(s.operands[0]) and s.operands[1].type==x.X86_OP_REG and s.operands[1].reg==register_id and r.operands[0].type==x.X86_OP_REG and r.operands[0].reg==register_id and stack(r.operands[1]),'saved table write/read')
 cache=s.operands[0].mem.disp;require(0<cache<4096 and cache%4==0,'relative cache slot');preds=_cfg(ins,x,cs,image);require(preds is not None,'native CFG');epoch=ins[by[store]+1:by[dispatch]];require(epoch and reload in {i.address for i in epoch},'cache epoch extent')
 for i in epoch+[ins[by[dispatch]]]:require(preds[by[i.address]]==[by[i.address]-1] and not i.group(cs.CS_GRP_JUMP) and not i.group(cs.CS_GRP_RET),'linear sole-entry epoch')
 records={int(r['site'],16):r for r in call_records};esp=0;implicit=[];calls=[];read_coordinate=None
 def overlap(a,n):return a<cache+4 and a+n>cache
 for i in epoch:
  ops=i.operands
  if i.address==reload:read_coordinate=esp+r.operands[1].mem.disp
  if i.mnemonic=='push':
   require(not (ops[0].type==x.X86_OP_REG and ops[0].reg==x.X86_REG_ESP),'ESP escape');implicit.append((i.address,esp-4,4));esp-=4
  elif i.mnemonic=='call':
   require(i.address in records and records[i.address]['family'] in ['direct','GSI_profile'],'epoch call contract');purge=records[i.address]['purgeBytes'];require(type(purge)is int and purge==0,'zero-argument/reader normal purge');implicit.append((i.address,esp-4,4));esp+=purge;calls.append(records[i.address])
  else:require(x.X86_REG_ESP not in i.regs_access()[1],'unsupported stack effect')
  for op in ops:
   if op.type==x.X86_OP_MEM and op.access&cs.CS_AC_WRITE:
    require(op.mem.base==x.X86_REG_ESP and not op.mem.index and not op.mem.segment and not overlap(esp+op.mem.disp,op.size),'explicit cache overwrite')
  require(not (i.mnemonic=='lea' and len(ops)==2 and ops[1].type==x.X86_OP_MEM and ops[1].mem.base==x.X86_REG_ESP),'unreviewed epoch stack-address escape')
 require(read_coordinate==cache and not any(overlap(a,n) for _,a,n in implicit),'cache/implicit write coordinates')
 require(len(calls)==2 and [r['family'] for r in calls]==['direct','GSI_profile'] and int(calls[0]['target'],16)==0x99e4c0 and calls[1]['slot']=='0x118','reader/GetHero epoch inventory')
 witness=profile['readOnlyBuffer'];body=bytes.fromhex(witness['bytes']);address=int(witness['address'],16);require(address==0x99e4c0 and len(body)==15 and image.bytes_at(address,len(body))==body,'readonly buffer witness');code=list(md.disasm(body,address))
 require([(i.mnemonic,i.op_str) for i in code]==[('mov','eax, dword ptr [ecx]'),('test','eax, eax'),('jne',hex(address+12)),('mov','eax, 0x129aaf4'),('ret',''),('mov','eax, dword ptr [eax]'),('ret','')],'readonly CString buffer graph')
 for i in ins[by[reload]+1:by[dispatch]]:require(register_id not in i.regs_access()[1],'cached table register clobber')
 call=ins[by[dispatch]];op=call.operands[0];require(call.mnemonic=='call' and op.type==x.X86_OP_MEM and op.mem.base==register_id and not op.mem.index and not op.mem.segment and op.mem.disp==int(profile['slot'],16),'cached dispatch')
 receiver=states.get(dispatch,{}).get('ecx');require(receiver is not None and receiver[:3]==('parent_member_address',parent,member),'current receiver type profile');equivalent=receiver==('parent_member_address',parent,member,origin[3]);table=int(profile['vtable'],16);entry=image.bytes_at(table+op.mem.disp,4);require(entry is not None,'static table entry');target=int.from_bytes(entry,'little')
 return dict(tableLoad=hex(load),cacheStore=hex(store),cacheRead=hex(reload),dispatch=hex(dispatch),cacheCoordinate=cache,readCoordinate=read_coordinate,relativeEspAfterEpoch=esp,expectedStaticVtable=hex(table),expectedStaticTarget=hex(target),receiverValueEquivalenceProved=equivalent,epochInstructions=len(epoch),normalCalls=[dict(site=r['site'],target=r['target'],purgeBytes=r['purgeBytes']) for r in calls],abiContract=profile['abiContract'],limits='Native table register origin,linear sole entry,relative ESP equations,no overlapping writes andreadonly CString reader checked. GetHero/private caller frame preservation,current expected types,ordinary child balance andno reentry/exceptions/heap proof remain conditions. Caller must independently validate all call return profiles andprivate frame bounds.')
