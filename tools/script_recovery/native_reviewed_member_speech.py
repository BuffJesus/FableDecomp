"""Replay reviewed member speech windows whose pending arguments cross getters."""
import hashlib,json,re

def restore(text,fn,row,rdata,spec):
 if row is None:return text
 def require(ok,reason):
  if not ok:raise ValueError('reviewed member speech '+fn['address']+': '+reason)
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
 from tools.script_recovery.native_receiver_reaching import _cfg,_reaching
 from tools.script_recovery.convert_quest_unit import callee_stack_words,_split_top
 start=int(fn['address'],16);raw=rdata.bytes_at(start,int(fn['bodyEndExclusive'],16)-start)
 require(raw is not None and hashlib.sha256(raw).hexdigest()==row['nativeSha256'],'native changed')
 require(hashlib.sha256(fn['decompile'].encode()).hexdigest()==row['sourceSha256'],'source changed')
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;ins=list(md.disasm(raw,start));indexed={i.address:n for n,i in enumerate(ins)}
 require(ins and ins[-1].address+ins[-1].size==int(fn['bodyEndExclusive'],16),'incomplete native body')
 import capstone
 preds=_cfg(ins,x,capstone,rdata);require(preds is not None,'unknown native control flow')
 owners=[i for i in ins[:12] if i.mnemonic=='mov' and i.operands[0].type==x.X86_OP_REG and i.operands[1].type==x.X86_OP_REG and i.operands[1].reg==x.X86_REG_ECX and i.operands[0].reg in (x.X86_REG_ESI,x.X86_REG_EDI,x.X86_REG_EBX,x.X86_REG_EBP)]
 require(len(owners)==1,'ambiguous owner');owner=owners[0].operands[0].reg
 require(all(owner not in i.regs_access()[1] or i==owners[0] or i.mnemonic=='pop' for i in ins),'owner overwritten')
 require(rdata.bytes_at(0x99e4c0,15)==bytes.fromhex('8b0185c07506b8f4aa2901c38b00c3'),'string buffer getter changed')
 require(rdata.bytes_at(0x7e7390,15)==bytes.fromhex('8b490885c974058b01ff6034c21800') and callee_stack_words(0x7e7390)==6,'resource speech wrapper ABI changed')
 seen=set()
 for call in row['calls']:
  begin,site=int(call['start'],16),int(call['site'],16);require(site not in seen and begin in indexed and site in indexed,'bad call inventory');seen.add(site)
  window=ins[indexed[begin]:indexed[site]+1];require(hashlib.sha256(rdata.bytes_at(begin,site+window[-1].size-begin)).hexdigest()==call['windowSha256'],'window changed')
  regs={owner:('self',0)};stack={};pushes=[];depth=0;result=None
  def address(op):
   if op.type!=x.X86_OP_MEM or op.mem.index:return None
   if op.mem.base==x.X86_REG_ESP:return ('stack',op.mem.disp-depth)
   base=regs.get(op.mem.base)
   return (base[0],base[1]+op.mem.disp) if base and base[0] in ('self','parent','interface') else None
  for i in window:
   ops=i.operands
   if i.mnemonic=='push':
    value=('imm',ops[0].imm) if ops[0].type==x.X86_OP_IMM else regs.get(ops[0].reg)
    if value is None and ops[0].type==x.X86_OP_REG:
     definitions=_reaching(ins,preds,indexed[i.address],ops[0].reg,x)
     require(definitions and -1 not in definitions and all(ins[n].mnemonic=='xor' and len(ins[n].operands)==2 and all(o.type==x.X86_OP_REG and o.reg==ops[0].reg for o in ins[n].operands) for n in definitions),'flag is not proven zero')
     value=('imm',0)
    require(value is not None,'unknown push');pushes.append(value);depth+=4;continue
   if i.mnemonic=='call':
    recv=regs.get(x.X86_REG_ECX)
    if ops[0].type==x.X86_OP_IMM:
     require(ops[0].imm==0x99e4c0 and recv and recv[0]=='stack','unknown nested call');value=('string',recv[1])
    else:
     require(ops[0].type==x.X86_OP_MEM and not ops[0].mem.index,'unknown virtual call');table=regs.get(ops[0].mem.base);slot=ops[0].mem.disp
     require(table==('table',*recv) if recv else False,'receiver/table mismatch')
     if slot==0x118:
      require(recv==('interface',0),'GetHero receiver changed');require(callee_stack_words(int.from_bytes(rdata.bytes_at(0x1260f0c+slot,4),'little'))==0,'GetHero purge changed');value=('hero',i.address)
     else:
      require(i.address==site and slot==0x34 and recv==('parent',call['offset']),'speech resource changed')
      result=list(reversed(pushes));break
    for reg in (x.X86_REG_EAX,x.X86_REG_ECX,x.X86_REG_EDX):regs.pop(reg,None)
    regs[x.X86_REG_EAX]=value;continue
   require(i.mnemonic in ('mov','lea','add'),'unsupported setup instruction')
   if i.mnemonic=='mov' and ops[0].type==x.X86_OP_MEM:
    a=address(ops[0]);require(a and a[0]=='stack' and a[1]>=0 and ops[1].type==x.X86_OP_REG,'unknown spill');stack[a[1]]=regs.get(ops[1].reg);continue
   require(ops[0].type==x.X86_OP_REG,'bad destination');dest=ops[0].reg;value=None
   if i.mnemonic=='add':
    old=regs.get(dest);require(old and ops[1].type==x.X86_OP_IMM,'unknown addition');value=(old[0],old[1]+ops[1].imm)
   elif i.mnemonic=='lea':value=address(ops[1])
   elif ops[1].type==x.X86_OP_REG:value=regs.get(ops[1].reg)
   elif ops[1].type==x.X86_OP_IMM:value=('imm',ops[1].imm)
   else:
    a=address(ops[1])
    if a==('self',spec.parent_off):value=('parent',0)
    elif a==('self',4):value=('interface',0)
    elif a and a[0]=='stack':value=stack.get(a[1])
    elif a:value=('table',*a)
   require(value is not None,'unknown setup value');regs[dest]=value
  expected=[tuple(v) for v in call['operands']];require(result==expected and len(result)==6 and result[0][0]=='hero','operand replay changed')
  require(call['offset'] in spec.parent_resources and spec.parent_resources[call['offset']]==call['resource'],'resource classification changed')
  require(text.count(call['old'])==1,'printed call changed')
  printed=_split_top(call['old'][call['old'].rfind(')(')+2:-2]);require(printed and printed[0]==call['listener'] and re.fullmatch(r'\w+',call['listener']),'listener source changed')
  prior=text[:text.index(call['old'])];definitions=re.findall(r'\b'+re.escape(call['listener'])+r'\s*=\s*([^;]+);',prior)
  require(definitions and '0x118' in definitions[-1],'listener is not the source GetHero result')
  if result[1][0]=='imm':
   require(rdata.string_at(result[1][1])==call['literal'],'literal changed');key=json.dumps(call['literal'])
   definitions=re.findall(r'\b'+re.escape(printed[1])+r'\s*=\s*([^;]+);',prior)
   require(definitions and definitions[-1].strip()==key,'literal source changed')
  else:
   require(result[1][0]=='string' and re.fullmatch(r'\w+',call['textToken']) and text.count(call['textDefinition'])==call['definitionCount'],'string source changed');key=call['textToken']
   require(printed[1]==key and re.fullmatch(re.escape(key)+r' = CCharString::operator_char_const_\(\w+\);',call['textDefinition']),'borrowed string source mismatch')
  require(all(v[0]=='imm' for v in result[2:]) and all(v[1] in (0,1) for v in result[3:]),'speech flags changed')
  parameters=[call['listener'],key,str(result[2][1]),*['true' if v[1] else 'false' for v in result[3:]]]
  replacement='CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase((CScriptGameResourceObjectScriptedThingBase *)RESOURCE_MemberResource('+json.dumps(call['resource'])+'),'+','.join(parameters)+');'
  text=text.replace(call['old'],replacement)
 dead=row.get('deadTableLoads')
 if dead is not None:
  require(isinstance(dead,dict) and isinstance(dead.get('old'),str) and isinstance(dead.get('sites'),list) and dead['sites'],'invalid dead table loads')
  old=dead['old'];sites=dead['sites'];require(text.count(old)==len(sites),'dead table load source changed')
  seen_loads=set();seen_calls=set()
  for entry in sites:
   begin,load,site=(int(entry[key],16) for key in ('start','load','call'))
   require(begin in indexed and load in indexed and site in indexed and begin<=load<site and load not in seen_loads and site not in seen_calls,'bad dead table load sites')
   seen_loads.add(load);seen_calls.add(site)
   window=ins[indexed[begin]:indexed[site]+1]
   require(hashlib.sha256(rdata.bytes_at(begin,site+window[-1].size-begin)).hexdigest()==entry['windowSha256'],'dead table load window changed')
   mov=ins[indexed[load]];call=ins[indexed[site]]
   require(mov.mnemonic=='mov' and len(mov.operands)==2 and mov.operands[0].type==x.X86_OP_REG and mov.operands[1].type==x.X86_OP_MEM and mov.operands[1].mem.disp==dead['offset'] and not mov.operands[1].mem.index,'resource table load changed')
   require(any(i.mnemonic=='mov' and len(i.operands)==2 and i.operands[0].type==x.X86_OP_REG and i.operands[0].reg==mov.operands[1].mem.base and i.operands[1].type==x.X86_OP_MEM and i.operands[1].mem.base==x.X86_REG_ESI and i.operands[1].mem.disp==0x14 and not i.operands[1].mem.index for i in window[:indexed[load]-indexed[begin]]),'resource parent path changed')
   require(call.mnemonic=='call' and call.operands[0].type==x.X86_OP_MEM and call.operands[0].mem.base==mov.operands[0].reg and call.operands[0].mem.disp==0x34 and not call.operands[0].mem.index,'speech table call changed')
   require(any(i.mnemonic=='call' and i.operands[0].type==x.X86_OP_MEM and i.operands[0].mem.disp==0x118 for i in window) and any(i.mnemonic=='add' and i.operands[0].type==x.X86_OP_REG and i.operands[0].reg==x.X86_REG_ECX and i.operands[1].type==x.X86_OP_IMM and i.operands[1].imm==dead['offset'] for i in window),'speech receiver path changed')
  require(all(spec.parent_resources.get(dead['offset'])==entry['resource'] for entry in sites),'resource classification changed')
  text=text.replace(old,'')
 return text
