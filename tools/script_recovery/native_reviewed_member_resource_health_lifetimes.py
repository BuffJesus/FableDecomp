"""Opt-in member-resource health lifetime folding with explicit prior replay.

The existing SDK helper supplies the temporary Thing/query/cleanup sequence.
Only reviewed normal ABI paths are covered; VM/native object lifetime,heap,
reentry,exceptions and gameplay behavior are separate runtime obligations.
"""
import hashlib,json,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.native_member_frame_contracts import prove as prove_frame
from tools.script_recovery.native_member_resource_health_roles import prove_roles
from tools.script_recovery.native_reviewed_typed_boolean_health import recover as replay_typed
from tools.script_recovery.native_reviewed_saved_health_predicates import PROFILE
from tools.script_recovery.lua_staging_liveness import build_cfg,prove_unused_staging

METHOD='ResourceThingHealthAbove';IDENT=r'[A-Za-z_]\w*'
serial=lambda value:json.dumps(value,separators=(',',':')).encode()
digest=lambda data:hashlib.sha256(data).hexdigest()
PREFIX=re.compile(r'(?m)^(?P<i> *)(?P<output>'+IDENT+r') = resources:ScriptThing\((?P<resource>'+IDENT+r')\)\n(?P=i)(?P<actor>'+IDENT+r') = (?P=output)\n(?P=i)(?P<result>'+IDENT+r') = quest:GetHealth\((?P=actor)\)\n')

def recover(source,todo,calls,fn,row,unit,owner,image,manifest,before,after,*,prior_typed_input=None):
 if row is None:return source,list(todo),list(calls),[]
 def require(ok,why):
  if not ok:raise ValueError('member resource health: '+why)
 require(owner==row['owner'] and row['method']==METHOD and row['resourceKind']=='entity-member','owner/method/kind')
 require(type(row['minimumHealth']) in [int,float] and row['minimumHealth']==0,'minimum')
 entity=unit['entities'][owner];start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
 for key,data in [('nativeSha256',raw),('fnSha256',serial(fn)),('phaseSha256',source.encode()),('callsSha256',serial(calls)),('todoSha256',serial(todo)),('beforeSha256',serial(before)),('afterSha256',serial(after))]:
  require(data is not None and digest(data)==row[key],key)
 require(entity['functions']['Main']['address'].lower()==fn['address'].lower() and entity['vtable'].lower()==row['entityVtable'].lower(),'registered Main')
 require(manifest['GetHealth']==row['hostContract'] and row['hostContract']['scope']=='Quest' and row['hostContract']['returnType']=='float' and row['hostContract']['parameters']==[dict(name='pThing',type='CScriptThing*',optional=False)],'host GetHealth ABI')
 require(row['memberOffset']>0 and entity['resourceFields'].get(hex(row['memberOffset']))==row['memberName'],'entity resource member')
 require(row['memberVtable']==0x127094c and row['healthSlot']==0x420 and row['getterSlot']==0x30,'resource ABI profile')
 for address,size,pin in PROFILE:
  data=image.bytes_at(address,size);require(data is not None and digest(data)==pin,'native helper dependency')
 prior_row=unit.get('reviewedTypedBooleanHealth',{}).get(fn['address'].lower())
 require(prior_typed_input is not None and prior_row is not None,'prior typed comparison replay required')
 for key in ['source','todo','calls']:require(key in prior_typed_input,'prior input schema')
 require(digest(prior_typed_input['source'].encode())==row['priorTypedInputSha256'],'prior typed input')
 replayed,newtodo,newcalls,prior_reviews=replay_typed(prior_typed_input['source'],prior_typed_input['todo'],prior_typed_input['calls'],fn,prior_row,unit,owner,image,manifest,before,after)
 require(replayed==source and newtodo==list(todo) and newcalls==list(calls),'prior exact source/call/diagnostic output')
 require(digest(serial(prior_reviews))==row['priorTypedReviewsSha256'],'prior review output')
 profile=row['frameProfile'];require(profile['memberProfile']['offset']==row['memberOffset'] and int(profile['memberProfile']['vtable'],16)==row['memberVtable'],'member/frame profile')
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;witnesses=profile['memberProfile']['witnesses']
 def witness(name):
  p=witnesses[name];data=bytes.fromhex(p['bytes']);a=int(p['address'],16)
  require(image.bytes_at(a,len(data))==data,'constructor '+name);code=list(md.disasm(data,a));require(sum(i.size for i in code)==len(data),'constructor decode');return code
 factory,constructor,base=witness('factory'),witness('constructor'),witness('base_constructor')
 require(factory[0].op_str=='ecx, [esi + '+hex(row['memberOffset'])+']' and factory[-1].mnemonic=='call' and factory[-1].operands[0].type==x.X86_OP_IMM and factory[-1].operands[0].imm==int(witnesses['constructor']['address'],16),'member factory/constructor link')
 require(any(i.mnemonic=='mov' and i.op_str=='dword ptr [esi], '+hex(int(row['entityVtable'],16)) for i in factory),'factory entity vtable')
 require(not any(x.X86_REG_ECX in i.regs_access()[1] for i in factory[1:-1]),'factory receiver clobber')
 require([i.mnemonic for i in constructor]==['push','mov','call','xor','mov','mov','mov','mov','pop','ret'],'member constructor shape')
 require(constructor[0].op_str==constructor[8].op_str=='esi' and not constructor[9].operands,'constructor saved register/return ABI')
 require(not base[-1].operands,'base constructor return ABI')
 require(constructor[1].op_str=='esi, ecx' and constructor[2].operands[0].type==x.X86_OP_IMM and constructor[2].operands[0].imm==int(witnesses['base_constructor']['address'],16),'base constructor link')
 require([i.op_str for i in constructor[3:8]]==['eax, eax','dword ptr [esi], '+hex(row['memberVtable']),'dword ptr [esi + 8], eax','dword ptr [esi + 0xc], eax','eax, esi'],'initial wrapper layout')
 require([i.mnemonic for i in base]==['mov','mov','mov','ret'] and base[0].op_str=='eax, ecx' and base[2].op_str=='dword ptr [eax + 4], 0','initial wrapper header')
 frame,frame_report=prove_frame(raw,fn,image,profile,entity_vtable=int(row['entityVtable'],16),scope_start=int(row['scopeStart'],16),scope_end=end,frame_depth=row['frameDepth'],this_definition=int(row['thisDefinition'],16),actor_definition=int(row['actorDefinition'],16),actor_profile=row['actorProfile'],owner_register=row['ownerRegister'])
 bindings=list(re.finditer(r'(?m)^ *(?P<alias>'+IDENT+r') = resources:MemberResource\("(?P<name>[^"]+)", me\)\n',source))
 require(len(bindings)==1 and bindings[0]['name']==row['memberName'],'Lua member binding')
 matches=list(PREFIX.finditer(source));require(len(matches)==source.count('quest:GetHealth(')==list(calls).count('GetHealth')==len(prior_reviews)==row['nativeSites'],'complete native/Lua health inventory')
 from tools.script_recovery.convert_quest_unit import _text_order_sites
 ordered=_text_order_sites(fn['decompile'],fn);require(ordered is not None,'native/source call alignment')
 original_health=[s for s in ordered if s[3].get('slot')==hex(row['healthSlot'])];require(len(original_health)==len(matches),'original native health inventory')
 scopes=[]
 for m,prior,original in zip(matches,prior_reviews,original_health):
  require(int(prior['site'],16)==int(original[3]['site'],16) and m['result']==prior['result'] and m['actor']==prior['actor'],'prior/native/Lua operation identity')
  require(m['resource']==bindings[0]['alias'] and len({m['resource'],m['output'],m['actor'],m['result']})==4,'Lua member/alias identity')
  roles=prove_roles(frame,int(prior['site'],16),prior['native'],image,this_definition=int(row['thisDefinition'],16),member_definition=int(row['memberDefinition'],16),member_offset=row['memberOffset'],member_vtable=row['memberVtable'],owner_register=row['ownerRegister'],member_register=row['memberRegister'],health_slot=row['healthSlot'],getter_slot=row['getterSlot'])
  index=ordered.index(original);require(0<index<len(ordered)-1 and int(ordered[index-1][3]['site'],16)==int(roles['getter'],16) and int(ordered[index+1][3]['site'],16)==int(roles['cleanup'],16),'original getter/query/cleanup correspondence')
  indent,result=m['i'],m['result'];tail=source[m.end():]
  if prior['kind']=='direct':flag=re.match(re.escape(indent)+r'(?P<flag>'+IDENT+r') = 0\.0 < '+re.escape(result)+r'\n',tail)
  else:
   require(prior['kind']=='inverse','prior boolean shape')
   flag=re.match(re.escape(indent)+r'(?P<flag>'+IDENT+r') = true\n'+re.escape(indent)+r'if not \('+re.escape(result)+r' > 0\.0\) then\n'+re.escape(indent)+r'    (?P=flag) = false\n'+re.escape(indent)+r'end\n',tail)
  require(flag is not None and flag['flag'] not in {m['resource'],m['output'],m['actor'],m['result']},'retained boolean flag')
  scopes.append(dict(start=m.start(),healthEnd=m.end(),end=m.end()+flag.end(),indent=indent,output=m['output'],actor=m['actor'],result=result,resource=m['resource'],flag=flag['flag'],native=roles,site=prior['site']))
 cfg=build_cfg(source);predecessors={k:set() for k in cfg['nodes']};lines=source.splitlines()
 for n,(_,edges) in cfg['nodes'].items():
  for edge in edges:
   if edge is not None:predecessors[edge].add(n)
 require('__native_member_health_decision' not in source,'analysis decision name collision')
 reviews=[]
 for s in scopes:
  first=source[:s['start']].count('\n');last=source[:s['end']].count('\n')-1;nodes=set(range(first,last+1));missing=nodes-set(cfg['nodes'])
  require(all(lines[k].strip()=='end' for k in missing),'prefix executable nodes');nodes&=set(cfg['nodes'])
  incoming=[(a,b) for b in nodes for a in predecessors[b] if a not in nodes];require(incoming and all(b==first for a,b in incoming),'external prefix entry')
  dead=[]
  for alias in [s['output'],s['actor']]:
   marker=s['indent']+alias+' = nil\n';hypothetical=source[:s['healthEnd']]+marker+source[s['healthEnd']:]
   dead.append(dict(role='Thing',alias=alias,proof=prove_unused_staging(hypothetical,s['healthEnd']+len(s['indent']),alias)))
  marker=s['indent']+s['result']+' = nil\n';independent=source[:s['healthEnd']]+marker+s['indent']+s['flag']+' = __native_member_health_decision\n'+source[s['end']:]
  dead.append(dict(role='float',alias=s['result'],proof=prove_unused_staging(independent,s['healthEnd']+len(s['indent']),s['result'])))
  reviews.append(dict(site=s['site'],native=s['native'],retainedFlag=s['flag'],clearedThingAliases=[s['output'],s['actor']],deadValues=dead,incoming=incoming))
 combined=source
 for s in reversed(scopes):combined=combined[:s['start']]+s['indent']+s['flag']+' = resources:'+METHOD+'('+s['resource']+', 0.0)\n'+combined[s['end']:]
 helpers=list(re.finditer(r'(?m)^(?P<i> *)(?P<flag>'+IDENT+r') = resources:'+METHOD+r'\((?P<resource>'+IDENT+r'), 0\.0\)\n',combined));require(len(helpers)==len(scopes),'composed helper inventory')
 for s,m,review in zip(scopes,helpers,reviews):
  require(m['flag']==s['flag'] and m['resource']==s['resource'],'composed flag/member identity');proofs=[]
  for alias in [s['output'],s['actor'],s['result']]:
   marker=s['indent']+alias+' = nil\n';hypothetical=combined[:m.start()]+marker+combined[m.start():]
   proofs.append(prove_unused_staging(hypothetical,m.start()+len(s['indent']),alias))
  review['composedDeadValues']=proofs
 out=source
 for s in reversed(scopes):
  clear=s['indent']+s['output']+', '+s['actor']+' = nil, nil\n';decision=s['indent']+s['flag']+' = resources:'+METHOD+'('+s['resource']+', 0.0)\n'
  out=out[:s['start']]+clear+decision+out[s['end']:]
 indices=[n for n in range(len(calls)-1) if list(calls[n:n+2])==['RESOURCE_ScriptThing','GetHealth']];require(len(indices)==len(scopes),'call-pair accounting')
 updated=list(calls)
 for n in reversed(indices):updated[n:n+2]=['RESOURCE_'+METHOD]
 reviews[0]['frame']=frame_report;reviews[0]['priorTypedComparisonReplay']=dict(inputSha256=digest(prior_typed_input['source'].encode()),outputSha256=digest(source.encode()),reviews=prior_reviews,originalHealthQueries=len(original_health));reviews[0]['constructorEvidenceScope']='Initial entity member wrapper layout/type only;current expected type/lifetime remains a host condition.'
 return out,list(todo),updated,reviews
