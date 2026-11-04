"""Restore reviewed CString operands from original producer and accessor frame slots."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

def restore(source,fn,row,rdata):
 if row is None:return source,[]
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed string operand {fn["address"]}: {why}')
 digest=lambda b:hashlib.sha256(b).hexdigest()
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'lifted source phase changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
 require(ins and ins[-1].address+ins[-1].size==end,'native disassembly incomplete')
 for a,b in row['nativeInstructions'].items():require(int(a,16) in by and by[int(a,16)].bytes.hex()==b,'native instruction changed')
 helpers=row['helpers'];expected={'integer':0x99f830,'prefix':0x99f690,'literalSuffix':0x99f600,'stringSuffix':0x99f570,'accessor':0x99e4c0,'speak':0x7e7390}
 for name,target in expected.items():
  h=helpers[name];body=bytes.fromhex(h['bytes']);require(int(h['address'],16)==target and len(body)>=8 and rdata.bytes_at(target,len(body))==body,'helper identity changed')
 require(helpers['accessor']['bytes']=='8b0185c07506b8f4aa2901c38b00c3','unsupported CString accessor')
 require(helpers['speak']['bytes']=='8b490885c974058b01ff6034c21800','unsupported Speak wrapper')
 def sequence(addresses):
  require(addresses and all(hex(int(a,16)) in row['nativeInstructions'] for a in addresses),'unwitnessed sequence')
  seq=[by[int(a,16)] for a in addresses]
  require(all(a.address+a.size==b.address for a,b in zip(seq,seq[1:])),'disconnected sequence')
  return seq
 calls={int(c['site'],16):int(c['target'],16) for c in fn['calls']}
 order=[int(a,16) for a in fn['callOrder'] if calls.get(int(a,16))==expected['speak']]
 require(order==[int(a,16) for a in row['speechOrder']],'speech inventory changed')
 result=[];seen=set()
 for edit in row['edits']:
  site=int(edit['site'],16);require(site in order and site not in seen,'missing or repeated speech site');seen.add(site)
  values={x86.X86_REG_EBP:('script',0)};stack=[];depth=0;products=[]
  def value(op,lea=False):
   if op.type==x86.X86_OP_REG:return values.get(op.reg)
   if op.type==x86.X86_OP_IMM:return ('immediate',op.imm)
   if op.type==x86.X86_OP_MEM and not op.mem.index:
    if lea and op.mem.base==x86.X86_REG_ESP:return ('frame',op.mem.disp-depth)
    if op.mem.base==x86.X86_REG_EBP and not lea:return ('field',op.mem.disp)
   return None
  for i in sequence(edit['producer']):
   op=i.operands
   if i.mnemonic in ['mov','lea']:
    require(op[0].type==x86.X86_OP_REG,'unsupported producer store');v=value(op[1],i.mnemonic=='lea');require(v is not None,'unclassified producer value');values[op[0].reg]=v
   elif i.mnemonic=='push':
    v=value(op[0]);require(v is not None,'unclassified pushed producer value');stack.append(v);depth+=4
   elif i.mnemonic=='inc':
    require(value(op[0])==('field',edit['indexField']),'unexpected integer adjustment');values[op[0].reg]=('incrementedField',edit['indexField'])
   elif i.mnemonic=='call':
    require(op[0].type==x86.X86_OP_IMM and calls.get(i.address)==op[0].imm,'producer dispatch changed');target=op[0].imm
    out=values.get(x86.X86_REG_ECX);arg=values.get(x86.X86_REG_EDX);require(out is not None and out[0]=='frame','producer lacks frame result')
    if target==expected['integer']:
     require(arg in [('field',edit['lineField']),('incrementedField',edit['indexField'])],'unexpected integer source');formula=('integer',arg)
    else:
     require(stack,'missing producer argument');other=stack.pop();depth-=4
     if target==expected['prefix']:
      require(arg is not None and arg[0]=='immediate' and rdata.string_at(arg[1])==edit['prefix'] and other==('string',('integer',('incrementedField',edit['indexField']))),'prefix or index changed')
      formula=('prefix',edit['prefix'],other[1])
     elif target==expected['literalSuffix']:
      require(arg is not None and arg[0]=='string' and arg[1][0]=='prefix' and other[0]=='immediate' and rdata.string_at(other[1])==edit['suffix'],'literal suffix changed');formula=('literalSuffix',arg[1],edit['suffix'])
     else:
      require(target==expected['stringSuffix'] and arg is not None and arg[0]=='string' and arg[1][0]=='literalSuffix' and other==('string',('integer',('field',edit['lineField']))),'string suffix changed');formula=('stringSuffix',arg[1],other[1])
    products.append((out,formula));values[x86.X86_REG_EAX]=('string',formula)
   else:require(False,'unsupported producer instruction')
  require(depth==0 and not stack and len(products)==5 and products[-1][1][0]=='stringSuffix','incomplete dynamic string')
  output=products[-1][0];require(output==('frame',edit['frameOffset']),'producer frame changed')
  # Four flags are pushed before accessor; its argument is a frame-relative ECX.
  seq=sequence(edit['consumer']);require(seq[-1].address==site,'consumer does not end at Speak')
  access=next((n for n,i in enumerate(seq) if i.mnemonic=='call' and i.operands[0].type==x86.X86_OP_IMM and i.operands[0].imm==expected['accessor']),None)
  require(access is not None and access>=5,'missing accessor')
  flags=seq[access-5:access-1];require([i.mnemonic for i in flags]==['push']*4 and [i.op_str for i in flags]==['0','1','0','0'],'speech flags changed')
  pointer=seq[access-1];require(pointer.mnemonic=='lea' and pointer.op_str==f'ecx, [esp + {hex(edit["frameOffset"]+16)}]','CString accessor frame changed')
  require(seq[-1].mnemonic=='call' and seq[-1].operands[0].type==x86.X86_OP_IMM and seq[-1].operands[0].imm==expected['speak'],'speech dispatch changed')
  scope=edit['luaScope'];require(source.count(scope)==1 and digest(scope.encode())==edit['luaScopeSha256'],'Lua scope changed')
  local=edit['resultLocal'];require(re.fullmatch(r'[A-Za-z_]\w*',local) is not None,'invalid result local')
  definitions=re.findall(r'(?m)^\s*'+re.escape(local)+r' = ([^\n]+)',scope)
  require(definitions==['(pCVar9 .. pCVar7)'],'ambiguous string result definition')
  before=edit['luaBefore'];after='pvVar11 = '+local
  require(before!=after and scope.count(before)==1 and re.fullmatch(r'pvVar11 = [A-Za-z_]\w*',before),'invalid operand edit')
  require(scope.count(':Speak(')==1 and 'pvVar11' in scope.split(':Speak(',1)[1] and scope.index(before)<scope.index(':Speak('),'missing speech use')
  declarations=re.findall(r'(?m)^\s*local ([\w, ]+)\s*$',source[:source.index(scope)])
  require(sum(local in [s.strip() for s in d.split(',')] for d in declarations)==1,'result is not a unique function local')
  source=source.replace(scope,scope.replace(before,after,1),1)
  result.append(dict(nativeSite=edit['site'],frameOffset=edit['frameOffset'],resultLocal=local,changedOperand=True,classification='constructed CString and speech accessor share original frame slot'))
 require(result,'empty string operand review')
 return source,result
