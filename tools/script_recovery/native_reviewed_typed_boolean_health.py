"""Opt-in comparison repair; preserves all calls, staging and resource lifetimes.

Native cleanup must return normally, preserve disjoint caller locals and avoid
reentry. The expected GSI and masked x87 environment remain caller contracts.
This proves the result predicate, not member-resource acquisition or ownership.
"""
import hashlib,json,re
import capstone as cs
from capstone import x86 as x
from tools.script_recovery.native_saved_x87_predicate import prove
from tools.script_recovery.native_receiver_reaching import _cfg
from tools.script_recovery.native_goto_scopes import duplicate_sibling_tails

serial=lambda value:json.dumps(value,separators=(',',':')).encode()
digest=lambda data:hashlib.sha256(data).hexdigest()
IDENT=r'[A-Za-z_]\w*'
AST=re.compile(r'^\s*('+IDENT+r') = \(float10\)GSI->GetHealth\(('+IDENT+r')\);$')
LUA=re.compile(r'(?m)^(?P<i> *)(?P<result>'+IDENT+r') = quest:GetHealth\((?P<actor>'+IDENT+r')\)\n')

def recover(source,todo,calls,fn,row,unit,owner,image,manifest,before,after):
 if row is None:return source,list(todo),list(calls),[]
 def require(ok,why):
  if not ok:raise ValueError('typed boolean health: '+why)
 require(row['owner']==owner and row['healthSlot']==0x420,'owner/slot')
 entity=unit['entities'][owner]
 require(entity['functions']['Main']['address'].lower()==fn['address'].lower() and entity['vtable'].lower()==row['entityVtable'].lower(),'registered Main')
 require(manifest['GetHealth']==row['hostContract'] and row['hostContract']['returnType']=='float','host ABI')
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=image.bytes_at(start,end-start)
 for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('phaseSha256',source.encode()),('callsSha256',serial(calls)),('todoSha256',serial(todo)),('beforeSha256',serial(before)),('afterSha256',serial(after))]:
  require(data is not None and digest(data)==row[key],key)
 require(image.bytes_at(int(row['entityVtable'],16)+4,4)==start.to_bytes(4,'little'),'native registered entry')
 from tools.script_recovery.convert_quest_unit import _text_order_sites
 ordered=_text_order_sites(fn['decompile'],fn);require(ordered is not None,'original call alignment')
 health=[s for s in ordered if s[3].get('slot')==hex(row['healthSlot'])]
 originals=[(n,AST.fullmatch(line)) for n,line in enumerate(before) if 'GetHealth(' in line]
 emitted=[(n,AST.fullmatch(line)) for n,line in enumerate(after) if 'GetHealth(' in line]
 require(all(m for n,m in originals+emitted),'AST health expression')
 require(duplicate_sibling_tails(before)==after,'actual copy replay')
 require([(m[1],m[2]) for n,m in originals]==[(m[1],m[2]) for n,m in emitted],'health-copy inventory changed')
 lua=list(LUA.finditer(source));scopes=row['scopes']
 require(len(health)==len(originals)==len(lua)==len(scopes)==calls.count('GetHealth')==source.count('quest:GetHealth('),'complete health inventory')
 md=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));by={i.address:k for k,i in enumerate(ins)}
 require(sum(i.size for i in ins)==len(raw),'native decode')
 preds=_cfg(ins,x,cs,image);require(preds is not None,'native CFG')
 edits=[];reviews=[]
 for native,ast,lua_match,scope in zip(health,emitted,lua,scopes):
  text_start,text_end,args,metadata,_,_=native;n,match=ast;site=int(metadata['site'],16)
  require(scope['site'].lower()==metadata['site'].lower(),'scope/native identity')
  line=fn['decompile'][fn['decompile'].rfind('\n',0,text_start)+1:text_start]
  assignment=re.fullmatch(r'\s*('+IDENT+r') = \(float10\)\(\*\*\(code \*\*\)\(('+IDENT+r') \+ 0x420\)\)\(',line)
  require(assignment is not None and assignment[1]==match[1]==lua_match['result'],'original/AST/Lua result')
  require(match[2]==lua_match['actor'],'AST/Lua actor')
  require(re.search(r'(?m)^\s*float10 '+re.escape(match[1])+r';',fn['decompile']) is not None,'original float type')
  query=ins[by[site]]
  require(query.mnemonic=='call' and query.operands[0].type==x.X86_OP_MEM and query.operands[0].mem.disp==row['healthSlot'],'query instruction')
  a,b=int(scope['native']['start'],16),int(scope['native']['endExclusive'],16)
  require(a==site+query.size and start<a<b<end,'query/predicate adjacency')
  actual=prove(raw[a-start:b-start],a,image.bytes_at,0x4aa840)
  require(actual==scope['native'] and actual['threshold']==0,'native predicate')
  nodes={k for k,i in enumerate(ins) if a<=i.address<b}
  incoming=[(ins[p].address,ins[k].address) for k in sorted(nodes) for p in preds[k] if p not in nodes]
  require(incoming==[(site,a)],'external predicate entry')
  tail=source[lua_match.end():];indent=lua_match['i'];result=lua_match['result']
  ast_tail=after[n+1:n+5]
  if scope['kind']=='direct':
   direct=re.match(re.escape(indent)+r'('+IDENT+r') = 0\.0 < '+re.escape(result)+r'\n',tail)
   require(direct is not None and ast_tail[0].strip()==direct[1]+' = (float10)0.0 < '+result+';','direct boolean correspondence')
   repair=False
  else:
   require(scope['kind']=='inverse','unknown predicate kind')
   require(args==[match[2]],'original explicit actor')
   flag=scope['flag'];require(re.fullmatch(IDENT,flag) is not None,'boolean alias')
   expected=[flag+' = true;','if ('+result+' <= (float10)0.0) {',flag+' = false;','}']
   require([s.strip() for s in ast_tail]==expected,'inverse AST boolean correspondence')
   fragment=indent+flag+' = true\n'+indent+'if '+result+' <= 0.0 then\n'+indent+'    '+flag+' = false\n'+indent+'end\n'
   require(tail.startswith(fragment),'inverse Lua boolean correspondence')
   source_tail=fn['decompile'][text_end+1:text_end+650]
   require(re.search(r'if \('+re.escape(result)+r' <= \(float10\)_DAT_'+format(int(actual['thresholdAddress'],16),'08x')+r'\)',source_tail) is not None,'original inverse comparison')
   old=result+' <= 0.0';pos=lua_match.end()+fragment.index(old)
   replacement='not ('+result+' > 0.0)';edits.append((pos,pos+len(old),replacement));repair=True
  reviews.append(dict(site=hex(site),native=actual,incoming=incoming,kind=scope['kind'],changed=repair,result=result,actor=match[2],limits='Only comparison text changes;original calls,cleanup and all staging retained. Member-resource ABI/frame/lifetime not accepted by this proof.'))
 require(len(edits)==row['repairCount']>0,'repair inventory')
 require(all(a[1]<=b[0] for a,b in zip(edits,edits[1:])),'overlapping edits')
 out=source
 for a,b,replacement in reversed(edits):out=out[:a]+replacement+out[b:]
 return out,list(todo),list(calls),reviews
