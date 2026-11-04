"""Opt-in parent-member health lifetimes with native/source/copy/liveness gates."""
import hashlib,json,re
from tools.script_recovery.native_parent_member_frame_contracts import prove as prove_frame
from tools.script_recovery.native_parent_member_health_roles import prove_roles
from tools.script_recovery.native_saved_register_x87_predicate import prove as prove_predicate
from tools.script_recovery.native_health_copy_correspondence import prove as prove_copy
from tools.script_recovery.native_reviewed_saved_health_predicates import PROFILE
from tools.script_recovery.lua_staging_liveness import build_cfg,prove_unused_staging
serial=lambda value:json.dumps(value,separators=(',',':')).encode()
digest=lambda data:hashlib.sha256(data).hexdigest()
METHOD='ResourceThingHealthAbove'
def recover(source,todo,calls,fn,row,unit,owner,image,manifest,before,after):
 if row is None:return source,list(todo),list(calls),[]
 def require(ok,why):
  if not ok:raise ValueError('parent member health lifetimes: '+why)
 require(owner==row['owner'] and row['method']==METHOD and row['resourceKind']=='parent-member','owner/method/resource kind')
 require(type(row['minimumHealth']) in [int,float] and row['minimumHealth']==0,'minimum')
 a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(a,b-a)
 for key,data in [('nativeSha256',raw),('fnSha256',serial(fn)),('phaseSha256',source.encode()),('callsSha256',serial(calls)),('todoSha256',serial(todo)),('beforeSha256',serial(before)),('afterSha256',serial(after))]:require(data is not None and digest(data)==row[key],key)
 entity=unit['entities'][owner];profile=row['frameProfile'];p=profile['parentProfile']
 require(entity['functions']['Main']['address'].lower()==fn['address'].lower() and int(entity['vtable'],16)==int(p['entityVtable'],16) and int(unit['vtable'],16)==int(p['parentVtable'],16),'registered parent/entity/Main')
 require(int(entity['functions']['GetParentScript']['address'],16)==int(p['witnesses']['parent_getter']['address'],16),'registered parent getter')
 require(row['parentOffset']==p['parentPointerOffset'] and row['memberOffset'] in p['resourceOffsets'] and unit['quest']['resourceFields'].get(hex(row['memberOffset']))==row['memberName'],'registered quest resource member')
 require(manifest['GetHealth']==row['hostContract'] and row['hostContract']['scope']=='Quest' and row['hostContract']['returnType']=='float' and row['hostContract']['parameters']==[dict(name='pThing',type='CScriptThing*',optional=False)],'host GetHealth ABI')
 require(row['healthSlot']==0x420 and row['getterSlot']==0x30 and row['ownerRegister']=='esi' and row['actorProfile'] in ['entry-ebp','entry-edi'],'native ABI profile')
 for address,size,pin in PROFILE:
  data=image.bytes_at(address,size);require(data is not None and digest(data)==pin,'native helper dependency')
 frame,frame_report=prove_frame(raw,fn,image,profile,entity_vtable=int(p['entityVtable'],16),scope_start=int(row['scopeStart'],16),scope_end=b,frame_depth=row['frameDepth'],this_definition=int(row['thisDefinition'],16),actor_definition=int(row['actorDefinition'],16),actor_profile=row['actorProfile'],owner_register=row['ownerRegister'])
 inline_profiles=row.get('inlineCleanupProfiles',{});allowed_callbacks={hex(int(p['callback'],16)) for p in inline_profiles.values()};require(set(frame_report['requiredIndirectABI'])==allowed_callbacks,'unresolved parent call target ABI inventory')
 for requirement in frame_report['requiredIndirectABI'].values():require(requirement['requiredNormalPurge']==0 and requirement['targetProvenanceVerified'] is False,'normal inline control callback condition')
 copy_options={}
 if inline_profiles:copy_options=dict(inline_cleanup_profiles=inline_profiles,allow_zero_staging=row.get('allowZeroStaging',False))
 correspondence=prove_copy(fn,before,after,source,getter_slot=row['getterSlot'],inline_member_name=row['memberName'],**copy_options);scopes=correspondence['scopes']
 require(correspondence['nativeSites']==correspondence['emittedUses']==len(scopes)==row['nativeSites'],'complete original/native/emitted query inventory')
 profiles={int(s['site'],16):s for s in row['nativePredicates']};require(len(profiles)==len(row['nativePredicates'])==len(scopes),'native predicate inventory')
 inline_break=re.compile(r'(?m)^(?P<i> *)if (?P<c>(?:not )?[A-Za-z_]\w*) then break end$');require(len(list(inline_break.finditer(source)))==row.get('analysisInlineBreaks',0),'analysis inline break inventory')
 expand=lambda text:inline_break.sub(lambda m:m['i']+'if '+m['c']+' then\n'+m['i']+'    break\n'+m['i']+'end',text)
 analysis=expand(source);cfg=build_cfg(analysis);predecessors={k:set() for k in cfg['nodes']}
 for n,(_,edges) in cfg['nodes'].items():
  for edge in edges:
   if edge is not None:predecessors[edge].add(n)
 require('__native_parent_member_health_decision' not in source,'analysis name collision')
 reviews=[]
 for s in scopes:
  site=hex(int(s['native']['call']['site'],16));require(int(site,16) in profiles,'native site profile');native=profiles[int(site,16)]
  inline=native.get('cleanupKind')=='inline-Thing-release';require(native.get('cleanupKind','direct-call') in ['direct-call','inline-Thing-release'],'cleanup profile kind')
  if inline:
   from tools.script_recovery.native_inline_thing_health_predicate import prove as prove_inline
   predicate=prove_inline(frame,int(site,16),image,positive_exit=int(native['positiveExit'],16),false_exit=int(native['falseExit'],16))
  else:predicate=prove_predicate(frame,int(site,16),image.bytes_at,0x4aa840,positive_exit=int(native['positiveExit'],16),false_exit=int(native['falseExit'],16))
  require(predicate['threshold']==0,'native threshold');role_options=dict(inline_cleanup=True) if inline else {}
  roles=prove_roles(frame,int(site,16),predicate,image,this_definition=int(row['thisDefinition'],16),parent_offset=row['parentOffset'],member_offset=row['memberOffset'],member_vtable=int(p['resourceVtable'],16),owner_register=row['ownerRegister'],**role_options)
  require(int(s['native']['getter']['site'],16)==int(roles['getter'],16),'source/native getter identity')
  if inline:
   declared=inline_profiles.get(site);require(declared is not None and declared==dict(kind='inline-Thing-release',callback=predicate['callback'],delete=predicate['delete'],base=predicate['baseCleanup']) and int(s['native']['cleanup']['site'],16)==int(predicate['callback'],16),'inline source/native cleanup identities')
  else:require(int(s['native']['cleanup']['site'],16)==int(roles['cleanup'],16),'source/native cleanup identity')
  require(s['resource']=='resources:MemberResource("'+row['memberName']+'")' and s['output']==s['actor'] and s['condition']==s['threshold']+' < '+s['result'],'Lua resource/actor/ordered comparison')
  at=len(expand(source[:s['start']]));end=len(expand(source[:s['end']]));require(analysis[at:end]==s['prefix'],'analysis prefix correspondence');first=analysis[:at].count('\n');last=analysis[:end].count('\n');nodes=set(range(first,last+1));require(nodes<=set(cfg['nodes']),'prefix nodes')
  incoming=[(a,b) for b in nodes for a in predecessors[b] if a not in nodes];require(incoming and all(b==first for a,b in incoming),'external prefix entry')
  indent=re.match(' *',s['prefix'])[0];staging=s.get('retainedStaging','');require(not staging or inline,'retained staging cleanup kind');decision=staging+indent+'if __native_parent_member_health_decision then';dead=[]
  for alias in [s['actor'],s['result'],s['threshold']]:
   marker=indent+alias+' = nil\n';hypothetical=analysis[:at]+marker+decision+analysis[end:];dead.append(dict(alias=alias,proof=prove_unused_staging(hypothetical,at+len(indent),alias)))
  s['indent']=indent;reviews.append(dict(site=site,native=predicate,roles=roles,clearedThingAliases=[s['actor']],deadValues=dead,incoming=incoming))
  if staging:reviews[-1]['retainedStaging']=dict(text=staging,booleanDecisionAlias=s['result'],queryBeforeLocalClears=True)
 combined=source
 for s in reversed(scopes):combined=combined[:s['start']]+s.get('retainedStaging','')+s['indent']+'if resources:'+METHOD+'('+s['resource']+', 0.0) then'+combined[s['end']:]
 helpers=list(re.finditer(r'(?m)^(?P<i> *)if resources:'+METHOD+r'\('+re.escape('resources:MemberResource("'+row['memberName']+'")')+r', 0\.0\) then',combined));require(len(helpers)==len(scopes),'composed helper inventory')
 for s,m,review in zip(scopes,helpers,reviews):
  proofs=[]
  for alias in [s['actor'],s['result'],s['threshold']]:
   marker=s['indent']+alias+' = nil\n';hypothetical=combined[:m.start()]+marker+combined[m.start():];pos=m.start()+len(s['indent']);proofs.append(prove_unused_staging(expand(hypothetical),len(expand(hypothetical[:pos])),alias))
  review['composedDeadValues']=proofs
 out=source
 for s in reversed(scopes):
  clear=s['indent']+s['actor']+' = nil\n';call='resources:'+METHOD+'('+s['resource']+', 0.0)';staging=s.get('retainedStaging','')
  replacement=clear+s['indent']+s['result']+' = '+call+'\n'+staging+s['indent']+'if '+s['result']+' then' if staging else clear+s['indent']+'if '+call+' then'
  out=out[:s['start']]+replacement+out[s['end']:]
 indices=[n for n in range(len(calls)-1) if list(calls[n:n+2])==['RESOURCE_ScriptThing','GetHealth']];require(len(indices)==len(scopes),'call-pair accounting');updated=list(calls)
 for n in reversed(indices):updated[n:n+2]=['RESOURCE_'+METHOD]
 reviews[0]['frame']=frame_report;reviews[0]['sourceCopyCorrespondence']=correspondence
 return out,list(todo),updated,reviews
