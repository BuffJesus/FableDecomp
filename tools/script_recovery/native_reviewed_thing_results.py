"""Recover reviewed named Thing-result references from native frame operands."""
import hashlib,json,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

MOVEMENT=re.compile(
 r'(?m)^(?P<indent>[ \t]*)if not \((?P<resource>\w+) ~= nil and not resources:ScriptThing\((?P=resource)\):IsNull\(\)\) then\n'
 r'(?P=indent)    (?P<position>\w+) = \{x = 0, y = 0, z = 0\}\n'
 r'(?P=indent)else\n(?P=indent)    (?P=position) = resources:ScriptThing\((?P=resource)\):GetPos\(\)\n'
 r'(?P=indent)end\n(?P=indent)me:MoveToPosition\((?P=position), 1.0, 0, false, true\)')
DISTANCE=re.compile(r'quest:IsDistanceBetweenThingsUnder\(([^()\n]+)\)')

def restore(source,fn,row,rdata,*,entity=False):
 if row is None:return source,[]
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed Thing results {fn["address"]}: {why}')
 digest=lambda value:hashlib.sha256(value).hexdigest()
 require(entity and row['actorLocal']=='me' and row['actorOffset']==8,'unsupported actor context')
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'lifted source phase changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;instructions=list(decoder.disasm(raw,start));indexed={i.address:i for i in instructions}
 require(instructions and instructions[-1].address+instructions[-1].size==end,'native disassembly incomplete')
 for site,encoded in row['nativeInstructions'].items():require(int(site,16) in indexed and indexed[int(site,16)].bytes.hex()==encoded,'native witness changed')
 def witness(site):
  require(hex(int(site,16)) in row['nativeInstructions'],'unwitnessed instruction');return indexed[int(site,16)]
 reg=lambda op,r:op.type==x86.X86_OP_REG and op.reg==r
 mem=lambda op,r,d:op.type==x86.X86_OP_MEM and op.size==4 and op.mem.base==r and not op.mem.index and op.mem.disp==d
 bind=witness(row['scriptBind']);require(bind.mnemonic=='mov' and len(bind.operands)==2,'script binding changed');script=bind.operands[0].reg
 require(reg(bind.operands[1],x86.X86_REG_ECX) and script in [x86.X86_REG_EBP,x86.X86_REG_ESI,x86.X86_REG_EDI,x86.X86_REG_EBX],'script binding changed')
 def setup(addresses,site):
  seq=[witness(a) for a in addresses];call=witness(site)
  require(seq and all(a.address+a.size==b.address for a,b in zip(seq,seq[1:]+[call])),'disconnected argument setup')
  values={script:('script',0)};pushed=[];depth=0
  for i in seq:
   op=i.operands
   if i.mnemonic=='push':
    value=('immediate',op[0].imm) if op[0].type==x86.X86_OP_IMM else values.get(op[0].reg) if op[0].type==x86.X86_OP_REG else None
    require(value is not None,'unclassified pushed operand');pushed.append(value);depth+=4;continue
   require(i.mnemonic in ['mov','lea'] and op[0].type==x86.X86_OP_REG,'unsupported setup instruction')
   value=None
   if i.mnemonic=='mov' and op[1].type==x86.X86_OP_REG:value=values.get(op[1].reg)
   elif i.mnemonic=='lea' and op[1].type==x86.X86_OP_MEM and not op[1].mem.index:
    if op[1].mem.base==x86.X86_REG_ESP:value=('frame',op[1].mem.disp-depth)
    elif values.get(op[1].mem.base)==('script',0) and op[1].mem.disp==8:value=('actor',0)
   elif i.mnemonic=='mov' and op[1].type==x86.X86_OP_MEM and not op[1].mem.index:
    if values.get(op[1].mem.base)==('script',0) and op[1].mem.disp==4:value=('gsi',0)
    elif values.get(op[1].mem.base)==('gsi',0) and op[1].mem.disp==0:value=('gsi-table',0)
   require(value is not None,'unclassified native operand');values[op[0].reg]=value
  return call,values,pushed
 calls={int(c['site'],16):c for c in fn['calls']};indirect={int(c['site'],16):c for c in fn['indirectCalls']}
 def ordered(target):return [int(s,16) for s in fn['callOrder'] if int(s,16) in calls and int(calls[int(s,16)]['target'],16)==target]
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 lookup=row['lookup'];slot,target=int(lookup['slot'],16),int(lookup['target'],16)
 require(slot==0x120 and load_slots().get(slot)=='GetThingWithScriptName' and rdata.bytes_at(0x1260f0c+slot,4)==target.to_bytes(4,'little'),'lookup identity changed')
 body=bytes.fromhex(lookup['bytes']);require(len(body)>=16 and rdata.bytes_at(target,len(body))==body and callee_stack_words(target)==2,'lookup body or hidden-result purge changed')
 require([int(s,16) for s in fn['callOrder'] if int(s,16) in indirect and int(indirect[int(s,16)].get('slot','0'),16)==slot]==[int(b['site'],16) for b in row['bindings']],'lookup source inventory changed')
 results={};lookup_proof=[]
 for binding in row['bindings']:
  name,local=binding['name'],binding['local'];require(re.fullmatch(r'[A-Za-z_]\w*',local) and re.fullmatch(r'[A-Za-z_]\w*',name),'invalid named result')
  line=local+' = quest:GetThingWithScriptName('+json.dumps(name)+')'
  require(source.count(line)==1 and len(re.findall(r'(?m)^\s*'+re.escape(local)+r'\s*=',source))==1,'lookup result is rebound or missing')
  require(any(local in [v.strip() for v in d.split(',')] for d in re.findall(r'(?m)^\s*local ([\w, ]+)\s*$',source)),'lookup result is not local')
  call,values,pushed=setup(binding['setup'],binding['site']);require(values.get(x86.X86_REG_ECX)==('gsi',0) and len(pushed)==2 and pushed[0][0]==pushed[1][0]=='frame','lookup arguments changed')
  require(call.mnemonic=='call' and call.operands[0].type==x86.X86_OP_MEM and call.operands[0].mem.disp==slot and values.get(call.operands[0].mem.base)==('gsi-table',0),'lookup dispatch changed')
  ctor,ctor_values,ctor_args=setup(binding['nameSetup'],binding['nameCtor'])
  require(ctor.mnemonic=='call' and ctor.operands[0].type==x86.X86_OP_IMM and int(calls[ctor.address]['target'],16)==ctor.operands[0].imm and callee_stack_words(ctor.operands[0].imm)==2,'name constructor changed')
  require(ctor_values.get(x86.X86_REG_ECX)==pushed[0] and len(ctor_args)==2 and ctor_args[0]==('immediate',-1) and ctor_args[1][0]=='immediate' and rdata.string_at(ctor_args[1][1])==name,'lookup name or slot changed')
  frame=pushed[1][1];require(frame not in results,'overlapping result slots');results[frame]=local;lookup_proof.append(dict(nativeSite=binding['site'],name=name,local=local,frameOffset=frame))
 distance_target=int(row['distanceTarget'],16);distance_body=bytes.fromhex(row['distanceBytes'])
 require(len(distance_body)>=16 and rdata.bytes_at(distance_target,len(distance_body))==distance_body and list(decoder.disasm(distance_body,distance_target))[-1].mnemonic=='ret' and list(decoder.disasm(distance_body,distance_target))[-1].op_str=='4','distance helper changed')
 distance_matches=list(DISTANCE.finditer(source));require(len(distance_matches)==len(row['distances']) and ordered(distance_target)==[int(c['site'],16) for c in row['distances']],'distance source inventory changed')
 def named(value):
  if value==('actor',0):return row['actorLocal']
  require(value is not None and value[0]=='frame' and value[1] in results,'unresolved Thing operand');return results[value[1]]
 edits=[];distance_proof=[]
 for match,review in zip(distance_matches,row['distances']):
  call,values,pushed=setup(review['setup'],review['site'])
  require(call.mnemonic=='call' and call.operands[0].type==x86.X86_OP_IMM and call.operands[0].imm==distance_target and pushed==[('immediate',0x40a00000)],'distance dispatch or radius changed')
  args=[named(values.get(x86.X86_REG_ECX)),named(values.get(x86.X86_REG_EDX)),'5.0'];require(match[0]==review['luaBefore'],'lifted distance call changed')
  replacement='quest:IsDistanceBetweenThingsUnder('+', '.join(args)+')';edits.append((match.start(),match.end(),replacement));distance_proof.append(dict(nativeSite=review['site'],arguments=args,changed=replacement!=match[0]))
 move_target=int(row['moveTarget'],16);move_body=bytes.fromhex(row['moveBytes']);require(rdata.bytes_at(move_target,len(move_body))==move_body and len(move_body)>=15,'movement wrapper changed')
 moves=list(MOVEMENT.finditer(source));require(len(moves)==len(row['movements']) and ordered(move_target)==[int(c['site'],16) for c in row['movements']],'movement source inventory changed')
 movement_proof=[]
 for match,review in zip(moves,row['movements']):
  data,test,jump,zero,zero_jump,table,getpos=[witness(review[k]) for k in ['dataLoad','nullTest','nonnullJump','zeroLoad','zeroJump','tableLoad','getPos']]
  require(data.mnemonic=='mov' and reg(data.operands[0],x86.X86_REG_ECX) and data.operands[1].type==x86.X86_OP_MEM and data.operands[1].mem.base==x86.X86_REG_ESP and not data.operands[1].mem.index,'marker Data load changed')
  frame=data.operands[1].mem.disp-4;require(frame in results,'marker Data does not belong to a named result');local=results[frame]
  require(test.mnemonic=='test' and test.op_str=='ecx, ecx' and data.address+data.size==test.address and test.address+test.size==jump.address,'Data null test changed')
  require(jump.mnemonic=='jne' and jump.operands[0].type==x86.X86_OP_IMM and jump.operands[0].imm==table.address,'nonnull path changed')
  require(zero.mnemonic=='mov' and reg(zero.operands[0],x86.X86_REG_EAX) and zero.operands[1].type==x86.X86_OP_IMM and zero.operands[1].imm==0x143e8e0 and jump.address+jump.size==zero.address and zero.address+zero.size==zero_jump.address,'zero vector fallback changed')
  require(table.mnemonic=='mov' and mem(table.operands[1],x86.X86_REG_ECX,0) and getpos.mnemonic=='call' and getpos.operands[0].type==x86.X86_OP_MEM and getpos.operands[0].mem.disp==0x18 and reg(table.operands[0],getpos.operands[0].mem.base) and table.address+table.size==getpos.address,'Data GetPos dispatch changed')
  arguments=[witness(a) for a in review['arguments']];call=witness(review['site'])
  require(len(arguments)==6 and getpos.address+getpos.size==arguments[0].address and all(a.address+a.size==b.address for a,b in zip(arguments,arguments[1:]+[call])),'movement argument sequence changed')
  require(zero_jump.mnemonic=='jmp' and zero_jump.operands[0].type==x86.X86_OP_IMM and zero_jump.operands[0].imm==arguments[0].address,'fallback join changed')
  require([i.mnemonic for i in arguments]==['push']*5+['lea'] and all(i.operands[0].type==x86.X86_OP_IMM and i.operands[0].imm==v for i,v in zip(arguments[:4],[1,0,0,0x3f800000])) and reg(arguments[4].operands[0],x86.X86_REG_EAX),'movement argument values changed')
  require(reg(arguments[5].operands[0],x86.X86_REG_ECX) and mem(arguments[5].operands[1],x86.X86_REG_ESP,0x28) and call.mnemonic=='call' and call.operands[0].type==x86.X86_OP_IMM and call.operands[0].imm==move_target,'movement control receiver changed')
  resource=match['resource'];replacement=match[0].replace(resource+' ~= nil',local+' ~= nil',1).replace('resources:ScriptThing('+resource+')',local)
  edits.append((match.start(),match.end(),replacement));movement_proof.append(dict(nativeSite=review['site'],getPosNativeSite=review['getPos'],local=local,frameOffset=frame))
 for begin,limit,replacement in sorted(edits,reverse=True):source=source[:begin]+replacement+source[limit:]
 return source,[dict(lookups=lookup_proof,movements=movement_proof,distances=distance_proof,note='Pinned native frame/result/Data and fastcall operands;lookup/ownership/fullMain gameplay remain separate evidence gates.')]
