"""Restore numeric zero tests whose byte predicate was emitted with Lua truthiness."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

def restore(source,fn,row,rdata):
 if row is None:return source,[]
 def require(ok,why):
  if not ok:raise ValueError(f'reviewed numeric condition {fn["address"]}: {why}')
 digest=lambda value:hashlib.sha256(value).hexdigest()
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rdata.bytes_at(start,end-start)
 require(raw is not None and digest(raw)==row['nativeSha256'],'native body changed')
 require(digest(fn['decompile'].encode())==row['sourceSha256'],'exported source changed')
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True;ins=list(decoder.disasm(raw,start));by={i.address:i for i in ins}
 require(ins and ins[-1].address+ins[-1].size==end,'native disassembly incomplete')
 for site,encoded in row['nativeInstructions'].items():require(int(site,16) in by and by[int(site,16)].bytes.hex()==encoded,'native instruction changed')
 compare=row['comparison'];call=by[int(compare['site'],16)];target=int(compare['target'],16);body=bytes.fromhex(compare['bytes'])
 require(len(body)>=16 and rdata.bytes_at(target,len(body))==body,'comparison body changed')
 require(call.mnemonic=='call' and call.operands[0].type==x86.X86_OP_IMM and call.operands[0].imm==target,'comparison dispatch changed')
 require(any(int(c['site'],16)==call.address and int(c['target'],16)==target for c in fn['calls']),'comparison call metadata changed')
 result=[];seen=set()
 for edit in row['edits']:
  flag=edit['local'];require(re.fullmatch(r'[A-Za-z_]\w*',flag) is not None and flag not in seen,'invalid or repeated local');seen.add(flag)
  scope=edit['luaScope'];require(source.count(scope)==1 and digest(scope.encode())==edit['luaScopeSha256'],'numeric condition scope changed')
  position=source.index(scope);prefix=source[:position]
  declarations=re.findall(r'(?m)^\s*local ([\w, ]+)\s*$',prefix)
  require(sum(flag in [s.strip() for s in d.split(',')] for d in declarations)==1,'predicate is not an unshadowed local')
  condition='if not '+flag+' then';require(scope.count(condition)==1 and scope.rstrip().endswith(condition),'unsupported condition')
  definition=re.search(r'(?m)^(?P<indent>[ \t]*)'+re.escape(flag)+r' = \([^\n]+\) and 1 or 0\n(?P=indent)'+re.escape(condition)+r'$',scope)
  require(definition is not None,'predicate lacks an adjacent numeric0/1 definition')
  normalization=[by[int(a,16)] for a in edit['normalization']]
  require(len(normalization)==4 and all(hex(i.address) in row['nativeInstructions'] for i in normalization),'normalization witness missing')
  require([i.mnemonic for i in normalization]==['mov','neg','sbb','inc'] and [i.op_str for i in normalization]==['ebx, eax','ebx','bl, bl','bl'],'unsupported native predicate normalization')
  require(all(a.address+a.size==b.address for a,b in zip(normalization,normalization[1:])) and call.address+call.size==normalization[0].address,'comparison normalization disconnected')
  test=by[int(edit['test'],16)];branch=by[int(edit['branch'],16)]
  require(hex(test.address) in row['nativeInstructions'] and hex(branch.address) in row['nativeInstructions'],'branch witness missing')
  require(test.mnemonic=='test' and test.op_str=='bl, bl' and test.address+test.size==branch.address,'predicate zero test changed')
  require(branch.mnemonic=='je' and branch.operands[0].type==x86.X86_OP_IMM and branch.operands[0].imm==int(edit['zeroTarget'],16),'zero branch changed')
  replacement=scope.replace(condition,'if '+flag+' == 0 then',1);source=source[:position]+replacement+source[position+len(scope):]
  result.append(dict(nativeBranch=edit['branch'],zeroTarget=edit['zeroTarget'],local=flag,classification='native numeric0/1 predicate;explicit Lua zero test',changedConditions=1))
 require(result,'empty condition review')
 return source,result
