"""Classify native CString construction masks already owned by SDK wrappers.

Only stale comment/report diagnostics change. The report retains native/SDK
scope timing and deeper engine lifetime limits; no heap trace equivalence is
asserted by this classification.
"""
import hashlib,re
from collections import Counter
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

def classify(source,todo,fn,row,rdata):
 if row is None:return source,list(todo),[]
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed temporary mask diagnostics {fn["address"]}: {why}')
 digest=lambda b:hashlib.sha256(b).hexdigest()
 a,b=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(a,b-a)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 require(digest(source.encode())==row['luaBodySha256'],'lifted source phase changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(raw,a));by={i.address:i for i in ins}
 require(ins and ins[-1].address+ins[-1].size==b,'incomplete native disassembly')
 for site,encoded in row['nativeInstructions'].items():require(int(site,16) in by and by[int(site,16)].bytes.hex()==encoded,'native witness changed')
 def witness(site):
  require(hex(int(site,16)) in row['nativeInstructions'],'missing native witness');return by[int(site,16)]
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 targets={}
 for key,words in [('constructor',2),('destructor',0)]:
  helper=row[key];target=int(helper['address'],16);body=bytes.fromhex(helper['bytes'])
  require(len(body)>=16 and rdata.bytes_at(target,len(body))==body and callee_stack_words(target)==words,'CString helper identity/ABI changed');targets[key]=target
 require(targets=={'constructor':0x99ebf0,'destructor':0x99eae0},'unsupported CString helpers')
 require(row['literal']=='SCRIPT_NAME_HERO' and rdata.string_at(int(row['literalAddress'],16))==row['literal'],'string argument changed')
 require(isinstance(row.get('scopeLimit'),str) and 'timing' in row['scopeLimit'] and 'unverified' in row['scopeLimit'],'missing lifetime limitation')
 calls={int(c['site'],16):int(c['target'],16) for c in fn['calls']}
 frame=row['maskFrameOffset'];seen=set();groups=[]
 def reg(op,name):return op.type==x86.X86_OP_REG and decoder.reg_name(op.reg)==name
 def memory(op,offset,size=4):return op.type==x86.X86_OP_MEM and op.size==size and op.mem.base==x86.X86_REG_ESP and not op.mem.index and op.mem.disp==offset
 for group in row['groups']:
  bit=group['bit'];require(bit in [1,2,4,8] and bit not in seen,'invalid or duplicate bit');seen.add(bit)
  ctor,dtor=witness(group['constructor']),witness(group['destructor'])
  for call,key in [(ctor,'constructor'),(dtor,'destructor')]:require(call.mnemonic=='call' and call.operands[0].type==x86.X86_OP_IMM and call.operands[0].imm==targets[key] and calls.get(call.address)==targets[key],'cleanup dispatch changed')
  n=ins.index(ctor);setup=ins[n-3:n];require(all(hex(i.address) in row['nativeInstructions'] for i in setup),'constructor setup missing')
  require([i.mnemonic for i in setup]==['push','push','lea'] and setup[0].operands[0].type==x86.X86_OP_IMM and setup[0].operands[0].imm==-1 and setup[1].operands[0].type==x86.X86_OP_IMM and setup[1].operands[0].imm==int(row['literalAddress'],16),'constructor literal setup changed')
  local=group['stringFrameOffset'];require(reg(setup[2].operands[0],'ecx') and memory(setup[2].operands[1],local+8),'constructor frame changed')
  n=ins.index(dtor);receiver=ins[n-2] if ins[n-1].mnemonic=='mov' else ins[n-1]
  # The cleanup is LEA object; store cleared mask; CALL destructor.
  require(receiver.address==int(group['destructorReceiver'],16) and hex(receiver.address) in row['nativeInstructions'] and receiver.mnemonic=='lea' and reg(receiver.operands[0],'ecx') and memory(receiver.operands[1],local),'destructor frame changed')
  setbit,test,clear=[witness(group[key]) for key in ['set','test','clear']]
  require(setbit.mnemonic=='or' and setbit.operands[0].type==x86.X86_OP_REG and setbit.operands[1].type==x86.X86_OP_IMM and setbit.operands[1].imm==bit,'construction bit changed')
  require(clear.mnemonic=='and' and clear.operands[0].type==x86.X86_OP_REG and clear.operands[1].type==x86.X86_OP_IMM and clear.operands[1].imm&0xffffffff==0xffffffff^bit,'clear mask changed')
  require(test.mnemonic=='test' and test.operands[1].type==x86.X86_OP_IMM and test.operands[1].imm==bit,'cleanup test changed')
  if bit==1:require(memory(test.operands[0],frame,1),'cleanup flag frame changed')
  else:require(reg(test.operands[0],'bl') and reg(setbit.operands[0],'ebx') and reg(clear.operands[0],'ebx'),'cleanup register changed')
  jump=witness(group['skip']);require(test.address+test.size==jump.address and jump.mnemonic=='je' and jump.operands[0].type==x86.X86_OP_IMM and jump.operands[0].imm==dtor.address+dtor.size,'guard does not skip only cleanup')
  for key,operation,depth in [('setStore',setbit,group['setArgumentDepth']),('clearStore',clear,0)]:
   store=witness(group[key]);require(store.mnemonic=='mov' and memory(store.operands[0],frame+depth) and store.operands[1].type==x86.X86_OP_REG and store.operands[1].reg==operation.operands[0].reg,'mask store changed')
  require(ctor.address<setbit.address<test.address<clear.address<dtor.address,'construction/cleanup order changed')
  groups.append(dict(bit=bit,stringFrameOffset=local,constructor=group['constructor'],destructor=group['destructor']))
 require(seen=={1,2,4,8},'incomplete expression ownership mask')
 for fragment in row['nativeSourceFragments']:require(fragment in fn['decompile'].replace('\r\n','\n'),'native cleanup source changed')
 for predicate,count in row['luaPredicates'].items():require(source.count(predicate)==count,'restored predicate inventory changed')
 expected=Counter()
 for item in row['diagnostics']:
  diagnostic,count=item['text'],item['count'];require(diagnostic.startswith('unlifted: ') and isinstance(count,int) and not isinstance(count,bool) and count>0,'invalid diagnostic review');expected[diagnostic]+=count
 require(expected and all(todo.count(d)==n for d,n in expected.items()),'diagnostic multiplicity changed')
 updated=list(todo);out=source
 for diagnostic,count in expected.items():
  pattern=re.compile(r'(?m)^[ \t]*-- TODO\(native\): '+re.escape(diagnostic[10:])+r'\n')
  require(len(list(pattern.finditer(out)))==count,'comment inventory changed');out=pattern.sub('',out)
  for _ in range(count):updated.remove(diagnostic)
 from tools.script_recovery.readable_lua import tokens
 significant=lambda text:[(t.lastgroup,t[0]) for t in tokens(text) if t.lastgroup not in ['space','comment','longcomment']]
 require(significant(out)==significant(source),'classification changed executable Lua')
 return out,updated,[dict(classification='native CString construction/destruction bookkeeping; SDK methods own scoped FableString',maskFrameOffset=frame,groups=groups,retiredDiagnostics=dict(expected),executableLuaUnchanged=True,scopeLimit=row['scopeLimit'])]
