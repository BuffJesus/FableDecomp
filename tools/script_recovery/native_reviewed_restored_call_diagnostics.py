"""Classify stale diagnostics only when their native calls are already restored."""
import hashlib
from collections import Counter
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

def classify(source,todo,fn,row,rdata):
 if row is None:return source,list(todo),[]
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed restored call diagnostics {fn["address"]}: {why}')
 digest=lambda value:hashlib.sha256(value).hexdigest()
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'restored Lua body changed')
 from tools.script_recovery.annotate_interface_slots import load_slots
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 slot,target=int(row['slot'],16),int(row['target'],16)
 require(load_slots().get(slot)==row['method'] and rdata.bytes_at(0x1260f0c+slot,4)==target.to_bytes(4,'little'),'native interface identity changed')
 callee=bytes.fromhex(row['calleeBytes']);require(len(callee)>=16 and rdata.bytes_at(target,len(callee))==callee and callee_stack_words(target)==1,'native callee changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;instructions=list(decoder.disasm(raw,start));indexed={i.address:i for i in instructions}
 require(instructions and instructions[-1].address+instructions[-1].size==end,'incomplete native body')
 for site,encoded in row['nativeInstructions'].items():require(int(site,16) in indexed and indexed[int(site,16)].bytes.hex()==encoded,'native witness changed')
 expected=Counter(call['diagnostic'] for call in row['calls']);require(expected and all(todo.count(d)==n for d,n in expected.items()),'diagnostic multiplicity changed')
 seen=set();review=[]
 for call in row['calls']:
  site=int(call['site'],16);require(site not in seen,'duplicated native site');seen.add(site)
  metadata=[c for c in fn['indirectCalls'] if int(c['site'],16)==site]
  require(len(metadata)==1 and int(metadata[0]['slot'],16)==slot,'native call inventory changed')
  ins=indexed[site];n=instructions.index(ins);setup=instructions[n-3:n]
  require(ins.mnemonic=='call' and ins.operands[0].type==x86.X86_OP_MEM and ins.operands[0].mem.disp==slot,'native dispatch changed')
  require(all(hex(i.address) in row['nativeInstructions'] for i in [*setup,ins]),'incomplete call witness')
  reg=lambda op,r:op.type==x86.X86_OP_REG and op.reg==r
  mem=lambda op,r,off:op.type==x86.X86_OP_MEM and op.size==4 and op.mem.base==r and not op.mem.index and op.mem.disp==off
  receiver,table,push=setup
  require(receiver.mnemonic=='mov' and reg(receiver.operands[0],x86.X86_REG_ECX) and mem(receiver.operands[1],x86.X86_REG_ESP,row['savedReceiverOffset']),'saved receiver changed')
  require(table.mnemonic=='mov' and reg(table.operands[0],ins.operands[0].mem.base) and mem(table.operands[1],x86.X86_REG_ECX,0),'native table load changed')
  require(push.mnemonic=='push' and push.operands[0].type==x86.X86_OP_IMM and push.operands[0].imm==0,'native false argument changed')
  anchor=call['luaRegion'];require(source.count(anchor)==1 and anchor.count('quest:'+row['method']+'(false)')==1,'restored call region changed')
  require(call['diagnostic'].startswith('unlifted: ') and call['diagnostic'][10:] in fn['decompile'],'native source diagnostic changed')
  review.append(dict(nativeSite=call['site'],method=row['method'],argument=False,classification='already restored by existing reviewed passes;stale report diagnostic',luaRegion=anchor))
 updated=list(todo)
 for diagnostic,n in expected.items():
  for _ in range(n):updated.remove(diagnostic)
 return source,updated,review

def classify_staging(source,todo,fn,row,rdata):
 """Retire witnessed vtable-load diagnostics without altering draft assignments."""
 if row is None:return source,list(todo),[]
 import re
 from collections import deque
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed restored vtable staging {fn["address"]}: {why}')
 digest=lambda b:hashlib.sha256(b).hexdigest()
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'lifted source phase changed')
 from tools.script_recovery.annotate_interface_slots import load_slots
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 slot,target=int(row['slot'],16),int(row['target'],16)
 require(row['method']=='PauseAllNonScriptedEntities' and load_slots().get(slot)==row['method'],'unsupported interface')
 require(rdata.bytes_at(0x1260f0c+slot,4)==target.to_bytes(4,'little'),'native interface identity changed')
 callee=bytes.fromhex(row['calleeBytes'])
 require(len(callee)>=16 and rdata.bytes_at(target,len(callee))==callee and callee_stack_words(target)==1,'native callee/ABI changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
 require(ins and ins[-1].address+ins[-1].size==end,'incomplete native body')
 for site,encoded in row['nativeInstructions'].items():require(int(site,16) in by and by[int(site,16)].bytes.hex()==encoded,'native witness changed')
 def witness(site):
  require(site in row['nativeInstructions'],'missing native witness');return by[int(site,16)]
 def sequence(sites):
  result=[witness(s) for s in sites]
  require(all(a.address+a.size==b.address for a,b in zip(result,result[1:])),'noncontiguous witness')
  return result
 def reg(op,name):return op.type==x86.X86_OP_REG and decoder.reg_name(op.reg)==name
 def mem(op,name,off):return op.type==x86.X86_OP_MEM and op.size==4 and decoder.reg_name(op.mem.base)==name and not op.mem.index and op.mem.disp==off
 call=witness(row['sharedCall']);require(call.mnemonic=='call' and mem(call.operands[0],'eax',slot),'shared dispatch changed')
 require(row['luaCallLabel']==f'::LAB_{call.address:08x}::','shared Lua label changed')
 metadata=[c for c in fn['indirectCalls'] if int(c['site'],16)==call.address]
 require(len(metadata)==1 and int(metadata[0]['slot'],16)==slot,'native call inventory changed')
 # Whole-body identity binds the accepted CFG/cache/alias proof; witnesses keep
 # its receiver origin and outstanding-argument offset explicit in the unit.
 proof=row['receiverProof']
 require(re.fullmatch('[0-9a-f]{64}',proof['acceptedEvidenceSha256']) is not None,'missing reviewed receiver evidence')
 require(proof['savedFrameOffset']==0x14 and proof['entryFrameOffset']==-396 and proof['ordinaryMsvcAbiAssumed'] is True,'receiver proof frame/ABI changed')
 origin,table,push,receiver,store,pause=sequence(proof['cacheInstructions'])
 require(origin.mnemonic=='mov' and reg(origin.operands[0],'ebx') and mem(origin.operands[1],'ebp',4),'script GSI origin changed')
 require(table.mnemonic=='mov' and reg(table.operands[0],'edx') and mem(table.operands[1],'ebx',0),'cache vtable changed')
 require(push.mnemonic=='push' and push.operands[0].type==x86.X86_OP_IMM and push.operands[0].imm==1,'cache argument depth changed')
 require(receiver.mnemonic=='mov' and reg(receiver.operands[0],'ecx') and reg(receiver.operands[1],'ebx'),'cache receiver changed')
 require(store.mnemonic=='mov' and mem(store.operands[0],'esp',proof['savedFrameOffset']+4) and reg(store.operands[1],'ebx'),'cache slot changed')
 require(pause.mnemonic=='call' and mem(pause.operands[0],'edx',slot),'cache initialization call changed')
 require(isinstance(row['scopeLimit'],str) and 'borrowed' in row['scopeLimit'] and 'unverified' in row['scopeLimit'],'missing engine lifetime limitation')
 paths=row['paths'];require(len(paths)==2 and {p['kind'] for p in paths}=={'saved-frame','preserved-register'},'incomplete shared-call paths')
 expected=Counter(p['diagnostic'] for p in paths);require(len(expected)==2 and all(todo.count(d)==n for d,n in expected.items()),'diagnostic multiplicity changed')
 variable=row['luaStagingVariable'];require(re.fullmatch('[A-Za-z_][A-Za-z_0-9]*',variable) is not None,'invalid staging name')
 from tools.script_recovery.readable_lua import tokens
 significant=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
 require(sum(value==variable for kind,value in significant(source))==3,'staging value has additional uses')
 updated=list(todo);out=source;reviews=[]
 for path in paths:
  region=path['luaRegion'];diagnostic=path['diagnostic'];require(source.count(region)==1,'Lua staging region changed')
  require(diagnostic.startswith('unlifted: '+variable+' = ') and path['nativeSourceFragment'] in fn['decompile'],'native staging source changed')
  label=path['luaLabel'];require(region.startswith(label+'\n') and label.startswith('::LAB_'),'missing native Lua label')
  comment='-- TODO(native): '+diagnostic[10:]
  require(region.count(comment)==1 and region.count(variable+' = nil --[[unresolved native value]]')==1,'staging placeholder changed')
  native=sequence(path['instructions'])
  require(label==f'::LAB_{native[0].address:08x}::','native/Lua entry identity changed')
  if path['kind']=='saved-frame':
   require(len(native)==3,'saved receiver setup changed');load,table,push=native
   require(load.mnemonic=='mov' and reg(load.operands[0],'ecx') and mem(load.operands[1],'esp',proof['savedFrameOffset']),'saved receiver load changed')
   require(table.mnemonic=='mov' and reg(table.operands[0],'eax') and mem(table.operands[1],'ecx',0),'saved vtable changed')
   require(push.mnemonic=='push' and push.operands[0].type==x86.X86_OP_IMM and push.operands[0].imm==0 and push.address+push.size==call.address,'literal false argument changed')
   require(region.rstrip().endswith('goto '+row['luaCallLabel'][2:-2]),'restored join changed')
  else:
   require(len(native)==4,'register receiver setup changed');table,push,receiver,jump=native
   require(table.mnemonic=='mov' and reg(table.operands[0],'eax') and mem(table.operands[1],'ebx',0),'register vtable changed')
   require(push.mnemonic=='push' and reg(push.operands[0],'edi'),'register false argument changed')
   require(receiver.mnemonic=='mov' and reg(receiver.operands[0],'ecx') and reg(receiver.operands[1],'ebx'),'register receiver changed')
   require(jump.mnemonic=='jmp' and jump.operands[0].type==x86.X86_OP_IMM and jump.operands[0].imm==call.address,'shared native join changed')
   # Conservative constant propagation over every native conditional edge.
   pending=deque([(start,None)]);seen=set();values=set()
   while pending:
    a,value=pending.popleft()
    if (a,value) in seen:continue
    seen.add((a,value));i=by[a]
    if a==push.address:values.add(value)
    if {'edi','di'} & {decoder.reg_name(r) for r in i.regs_access()[1]}:
     if i.mnemonic=='xor' and i.op_str=='edi, edi':value=0
     elif i.mnemonic=='mov' and len(i.operands)==2 and i.operands[1].type==x86.X86_OP_IMM:value=i.operands[1].imm
     else:value=None
    if i.mnemonic.startswith('ret'):continue
    if i.mnemonic.startswith('j'):
     require(i.operands[0].type==x86.X86_OP_IMM,'unresolved indirect branch')
     if i.operands[0].imm in by:pending.append((i.operands[0].imm,value))
     if i.mnemonic=='jmp':continue
    if a+i.size in by:pending.append((a+i.size,value))
   require(values=={0},'register argument is not proved false')
   require(row['luaCallLabel']+'\n' in region and region.count('quest:'+row['method']+'(false)')==1,'restored false call changed')
  replacement=re.sub(r'(?m)^[ \t]*'+re.escape(comment)+r'\n','',region).replace(' --[[unresolved native value]]','')
  out=out.replace(region,replacement,1);updated.remove(diagnostic)
  reviews.append(dict(nativeEntry=hex(native[0].address),nativeCall=row['sharedCall'],method=row['method'],argument=False,receiver='script GSI',classification='vtable staging for already restored shared call',luaAssignmentRetained=True))
 require(significant(out)==significant(source),'classification changed executable Lua')
 return out,updated,[dict(paths=reviews,executableLuaUnchanged=True,receiverProof=proof,scopeLimit=row['scopeLimit'])]

def classify_argument_copy(source,todo,fn,row,rdata):
 """Classify retained actor-copy comments without deleting executable aliases."""
 if row is None:return source,list(todo),[]
 import re
 from tools.script_recovery.annotate_interface_slots import load_slots
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.readable_lua import tokens
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed argument copy comments {fn["address"]}: {why}')
 digest=lambda b:hashlib.sha256(b).hexdigest()
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'lifted source phase changed')
 begin,limit=int(row['spanBegin'],16),int(row['spanEnd'],16)
 require(start<=begin<limit<=end and rdata.bytes_at(begin,limit-begin).hex()==row['spanBytes'],'argument instructions changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
 full=list(decoder.disasm(raw,start));indexed={i.address:i for i in full}
 require(begin in indexed and limit in indexed,'argument span boundary changed')
 ins=list(decoder.disasm(bytes.fromhex(row['spanBytes']),begin))
 require(len(ins)==15 and ins[-1].address+ins[-1].size==limit,'incomplete argument setup')
 member=row['actorMemberOffset'];receiver=row['scriptReceiverOffset'];slot=row['slot'];target=int(row['target'],16)
 require(member==8 and receiver==4 and row['method']=='SetIsPushableByHero' and load_slots().get(slot)==row['method'],'actor/interface identity changed')
 require(rdata.bytes_at(0x1260f0c+slot,4)==target.to_bytes(4,'little'),'native vtable target changed')
 # This complete instruction template proves the 12-byte POD copy, retained
 # Info, literal false, script GSI receiver and consuming call. It does not
 # infer alias liveness: every executable Lua token must remain below.
 template=[('push','0'),('sub','esp, 0xc'),('mov','eax, esp'),
  ('mov','dword ptr [eax], 0x1231710'),('mov','dword ptr [eax], 0x1238c8c'),
  ('mov',f'ecx, dword ptr [ebp + {hex(member+4)}]'),('mov','dword ptr [eax + 4], ecx'),
  ('mov',f'ecx, dword ptr [ebp + {hex(member+8)}]'),('test','ecx, ecx'),
  ('mov','dword ptr [eax + 8], ecx'),('je',hex(ins[-3].address)),('inc','dword ptr [ecx]'),
  ('mov',f'ecx, dword ptr [ebp + {receiver}]'),('mov','edx, dword ptr [ecx]'),('call',f'dword ptr [edx + {hex(slot)}]')]
 require([(i.mnemonic,i.op_str) for i in ins]==template,'actor copy/retain/receiver setup changed')
 call=ins[-1];require(call.address==int(row['callSite'],16),'argument call site changed')
 metadata=[c for c in fn['indirectCalls'] if int(c['site'],16)==call.address]
 require(len(metadata)==1 and int(metadata[0]['slot'],16)==slot and metadata[0].get('ecxValue')==receiver,'argument call metadata changed')
 callee=bytes.fromhex(row['calleeBytes']);require(len(callee)>=32 and rdata.bytes_at(target,len(callee))==callee and callee_stack_words(target)==4,'consuming callee bytes/ABI changed')
 body=list(decoder.disasm(callee,target));require(body[-1].address+body[-1].size==target+len(callee) and body[-1].mnemonic=='ret' and body[-1].op_str=='0x10','consuming return changed')
 require(sum(i.mnemonic=='dec' and i.op_str=='dword ptr [eax]' for i in body)==2,'callee copy-release paths changed')
 require(sum(i.mnemonic=='call' and i.op_str=='0x99a2e0' for i in body)==1,'callee base destruction changed')
 base=bytes.fromhex(row['baseDestructorBytes']);require(len(base)==7 and rdata.bytes_at(0x99a2e0,len(base))==base and [(i.mnemonic,i.op_str) for i in decoder.disasm(base,0x99a2e0)]==[('mov','dword ptr [ecx], 0x1231710'),('ret','')],'base destructor changed')
 region=row['luaRegion'];require(source.count(region)==1 and region.count(row['luaCall'])==1 and row['luaCall']=='quest:'+row['method']+'(me, false)','restored Lua call region changed')
 diagnostics=row['diagnostics'];fragments=row['nativeSourceFragments']
 require(len(diagnostics)==len(set(diagnostics))==len(fragments)==3 and all(d.startswith('unlifted: ') and f in fn['decompile'] and todo.count(d)==1 for d,f in zip(diagnostics,fragments)),'diagnostic/native source inventory changed')
 require(re.fullmatch('[0-9a-f]{64}',row['acceptedSdkEvidenceSha256']) is not None and 'unverified' in row['scopeLimit'] and 'empty conditional remain' in row['scopeLimit'],'missing accepted ownership evidence/limits')
 out=source;updated=list(todo)
 for diagnostic in diagnostics:
  pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n')
  require(len(list(pattern.finditer(region)))==len(list(pattern.finditer(source)))==1,'argument comment inventory changed')
  out=pattern.sub('',out);updated.remove(diagnostic)
 significant=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
 require(significant(out)==significant(source),'classification changed executable Lua')
 return out,updated,[dict(classification='retained by-value actor setup for already restored call',nativeCall=hex(call.address),method=row['method'],removedDiagnostics=len(diagnostics),executableLuaUnchanged=True,acceptedSdkEvidenceSha256=row['acceptedSdkEvidenceSha256'],scopeLimit=row['scopeLimit'])]


def classify_hidden_result(source,todo,fn,row,rdata):
 """Classify a reviewed hidden CString return-address diagnostic, comments only."""
 if row is None:return source,list(todo),[]
 import re
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed hidden return staging {fn["address"]}: {why}')
 digest=lambda b:hashlib.sha256(b).hexdigest()
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'lifted source phase changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
 require(ins and ins[-1].address+ins[-1].size==end,'incomplete native body')
 for site,encoded in row['nativeInstructions'].items():require(int(site,16) in by and by[int(site,16)].bytes.hex()==encoded,'native witness changed')
 def witness(key):
  site=row['roles'][key];require(site in row['nativeInstructions'],'missing native witness');return by[int(site,16)]
 def reg(op,name):return op.type==x86.X86_OP_REG and decoder.reg_name(op.reg)==name
 def mem(op,name,offset):return op.type==x86.X86_OP_MEM and op.size==4 and decoder.reg_name(op.mem.base)==name and not op.mem.index and op.mem.disp==offset
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 expected={'getter':(0x4aa900,1),'assign':(0x99efb0,1),'destructor':(0x99eae0,0)}
 for key,(address,words) in expected.items():
  helper=row['helpers'][key];data=bytes.fromhex(helper['bytes']);require(int(helper['address'],16)==address and len(data)>=12 and rdata.bytes_at(address,len(data))==data and callee_stack_words(address)==words,'native helper identity/ABI changed')
 require(row['method']=='GetDataString' and row['slot']==0xc and int.from_bytes(rdata.bytes_at(0x1238c8c+row['slot'],4),'little')==expected['getter'][0],'getter identity changed')
 frame=row['frameBytes'];offset=row['resultFrameOffset'];require(frame==24 and offset==20 and row['resultEntryOffset']==offset-frame,'hidden-result frame changed')
 prefix=[witness(key) for key in ['reserve','saveEbx','saveEbp','scriptReceiver','thingTable','saveEsi','saveEdi']]
 require(prefix[0].address==start and all(a.address+a.size==b.address for a,b in zip(prefix,prefix[1:])),'prologue witness changed')
 require([(i.mnemonic,i.op_str) for i in prefix]==[('sub','esp, 8'),('push','ebx'),('push','ebp'),('mov','ebp, ecx'),('mov','eax, dword ptr [ebp + 8]'),('push','esi'),('push','edi')],'script/actor frame origin changed')
 outaddr,actor,push,receiver,member,call=[witness(k) for k in ['outAddress','actorAddress','pushOut','getterReceiver','memberAddress','getterCall']]
 require(outaddr.mnemonic=='lea' and reg(outaddr.operands[0],'ecx') and mem(outaddr.operands[1],'esp',offset),'hidden result address changed')
 require(actor.mnemonic=='lea' and reg(actor.operands[0],'esi') and mem(actor.operands[1],'ebp',8),'actor receiver changed')
 require(push.mnemonic=='push' and reg(push.operands[0],'ecx') and outaddr.address+outaddr.size==actor.address and actor.address+actor.size==push.address,'hidden argument setup changed')
 require(receiver.mnemonic=='mov' and reg(receiver.operands[0],'ecx') and reg(receiver.operands[1],'esi') and push.address+push.size==receiver.address,'getter receiver changed')
 require(member.mnemonic=='lea' and reg(member.operands[0],'ebx') and mem(member.operands[1],'ebp',row['memberOffset']),'member destination changed')
 require(call.mnemonic=='call' and mem(call.operands[0],'eax',row['slot']),'getter dispatch changed')
 metadata=[c for c in fn['indirectCalls'] if int(c['site'],16)==call.address]
 require(len(metadata)==1 and int(metadata[0]['slot'],16)==row['slot'] and metadata[0]['depth']==frame+4 and metadata[0]['ecxStack']==8 and metadata[0]['pushedStack']==[offset-frame],'hidden argument metadata changed')
 tail=[witness(k) for k in ['pushReturned','assignmentReceiver','assignmentCall','destructorReceiver','destructorCall']]
 require(call.address+call.size==tail[0].address and all(a.address+a.size==b.address for a,b in zip(tail,tail[1:])),'result lifetime sequence changed')
 require(tail[0].mnemonic=='push' and reg(tail[0].operands[0],'eax') and tail[1].mnemonic=='mov' and reg(tail[1].operands[0],'ecx') and reg(tail[1].operands[1],'ebx'),'returned pointer assignment changed')
 require(tail[3].mnemonic=='lea' and reg(tail[3].operands[0],'ecx') and mem(tail[3].operands[1],'esp',offset),'destructor result frame changed')
 for i,key in [(tail[2],'assign'),(tail[4],'destructor')]:
  require(i.mnemonic=='call' and i.operands[0].type==x86.X86_OP_IMM and i.operands[0].imm==expected[key][0] and any(int(c['site'],16)==i.address and int(c['target'],16)==expected[key][0] for c in fn['calls']),'result helper dispatch changed')
 diagnostic=row['diagnostic'];require(diagnostic.startswith('unlifted: ') and todo.count(diagnostic)==1 and row['nativeSourceFragment'] in fn['decompile'],'diagnostic/source inventory changed')
 from tools.script_recovery.readable_lua import tokens
 significant=lambda s:[(t.lastgroup,t[0]) for t in tokens(s) if t.lastgroup not in ['space','comment','longcomment']]
 for alias in row['nativeOnlyAliases']:require(re.fullmatch('[A-Za-z_][A-Za-z_0-9]*',alias) is not None and not any(value==alias for kind,value in significant(source)),'native address alias is executable')
 require(len(row['nativeOnlyAliases'])==2 and diagnostic=='unlifted: '+' = '.join(row['nativeOnlyAliases'])+';','staging alias identity changed')
 require(source.count(row['luaRegion'])==1 and row['luaRegion'].count('me:GetDataString()')==1 and ':SetStateString(' in row['luaRegion'],'restored value/assignment region changed')
 require(re.fullmatch('[0-9a-f]{64}',row['acceptedSdkEvidenceSha256']) is not None and 'unverified' in row['scopeLimit'] and 'construction exceptions' in row['scopeLimit'],'missing accepted SDK evidence/limits')
 pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n');require(len(list(pattern.finditer(source)))==1,'comment inventory changed');out=pattern.sub('',source);require(significant(source)==significant(out),'classification changed executable Lua')
 updated=list(todo);updated.remove(diagnostic)
 return out,updated,[dict(classification='hidden CString result address for already restored entity getter; SDK owns result cleanup',nativeCall=hex(call.address),method=row['method'],resultEntryOffset=offset-frame,assignmentCall=hex(tail[2].address),destructorCall=hex(tail[4].address),executableLuaUnchanged=True,acceptedSdkEvidenceSha256=row['acceptedSdkEvidenceSha256'],scopeLimit=row['scopeLimit'])]
