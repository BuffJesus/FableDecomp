"""Recover reviewed literal string-map keys lost through stale hidden-result aliases."""
import hashlib,re
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86

def restore(source,todo,fn,unit,rd):
 row=unit.get('reviewedLiteralMapKeys',{}).get(str(fn['address']).lower())
 if row is None:return source,todo
 def require(ok,why):
  if not ok:raise ValueError('Reviewed literal map keys: '+why)
 source=source.replace('\r\n','\n').replace('\r','\n')
 start,end=int(fn['address'],16),int(fn['bodyEndExclusive'],16);raw=rd.bytes_at(start,end-start)
 for key,data in [('nativeSha256',raw),('sourceSha256',fn['decompile'].encode()),('luaSha256',source.encode())]:require(data is not None and hashlib.sha256(data).hexdigest()==row[key],key+' changed')
 cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True;ins=list(cs.disasm(raw,start));index={i.address:i for i in ins}
 require(ins and ins[-1].address+ins[-1].size==end,'incomplete native body')
 def at(a):
  require(a in index,'instruction address missing');return index[a]
 def reg(op,name):return op.type==x86.X86_OP_REG and op.reg==getattr(x86,'X86_REG_'+name.upper())
 def mem(op,base,disp):return op.type==x86.X86_OP_MEM and op.mem.base==getattr(x86,'X86_REG_'+base.upper()) and not op.mem.index and op.mem.disp==disp
 def imm(op,value):return op.type==x86.X86_OP_IMM and op.imm==value
 def call(a,target):
  i=at(a);require(i.mnemonic=='call' and imm(i.operands[0],target),'call target changed')
 def lea(a,dst,offset):
  i=at(a);require(i.mnemonic=='lea' and reg(i.operands[0],dst) and mem(i.operands[1],'esp',offset),'stack receiver changed')
 def push(a,value):
  i=at(a);require(i.mnemonic=='push' and imm(i.operands[0],value),'literal push changed')
 def literal(a,s):require(rd.bytes_at(a,len(s.encode('ascii'))+1)==s.encode('ascii')+b'\0','literal bytes changed')
 require(len(row['sites'])==3,'site inventory changed')
 for site in row['sites']:
  a,b=site['start'],site['end'];data=rd.bytes_at(a,b-a)
  require(data is not None and hashlib.sha256(data).hexdigest()==site['sha256'],'call window changed')
  slot=site['stackOffset'];require(slot>=0 and slot%4==0,'invalid stack slot')
  push(site['keyPush'],site['keyAddress']);literal(site['keyAddress'],site['key'])
  lea(site['constructorLea'],'ecx',slot+8);call(site['constructorCall'],0x99ebf0)
  push(site['valuePush'],site['valueAddress']);literal(site['valueAddress'],site['value'])
  lea(site['mapLea'],site['mapRegister'],slot+4)
  i=at(site['mapPush']);require(i.mnemonic=='push' and reg(i.operands[0],site['mapRegister']),'map key push changed')
  i=at(site['mapOwner']);require(i.mnemonic=='mov' and reg(i.operands[0],'ecx') and mem(i.operands[1],'esi',0x14),'map owner changed')
  i=at(site['mapOffset']);require(i.mnemonic=='add' and reg(i.operands[0],'ecx') and imm(i.operands[1],site['memberOffset']),'map member changed')
  call(site['mapCall'],0x9ac700);call(site['assignCall'],0x99efe0)
  i=at(site['assignReceiver']);require(i.mnemonic=='mov' and reg(i.operands[0],'ecx') and reg(i.operands[1],'eax'),'map result receiver changed')
  lea(site['destructorLea'],'ecx',slot);call(site['destructorCall'],0x99eae0)
  require(site['keyPush']<site['constructorCall']<site['mapCall']<site['assignCall']<site['destructorLea']<site['destructorCall'],'call ordering changed')
  value,key,map_name,old=site['value'],site['key'],row['mapName'],row['staleAlias'];local=row['valueLocal']
  require(all(re.fullmatch(r'[A-Za-z_]\w*',s) for s in [map_name,old,local]),'invalid source name')
  require('"' not in value and '"' not in key,'invalid literal')
  pattern=re.compile(r'(?m)^(?P<i>[ \t]*)'+re.escape(f'{local} = "{value}"')+r'\n(?P=i)'+re.escape(f'resources:SetString(resources:MemberStringMap("{map_name}"), {old}, {local})')+r'$')
  matches=list(pattern.finditer(source));require(len(matches)==1,'ambiguous map write');m=matches[0]
  source=source[:m.start()]+m[0].replace(f', {old}, {local})',f', "{key}", {local})')+source[m.end():]
 return source,todo
