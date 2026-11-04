"""Recover stack-held registered IDs in proved global GSI SetTimer calls."""
import hashlib,re
from collections import defaultdict
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from capstone.x86 import X86_OP_REG,X86_OP_MEM,X86_OP_IMM
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_timer_operands import result_stack_slot

GSI=('global',0x143e8f8)
FTOL=(0xbfea70,0x75,'5333b186c02836974c6f792303aeb2c00d856316b93ccbbe65f51def6ae661b4')

def recover(source,fn,ordered,rdata=None):
 if not ordered:return source,[]
 if not any(vt and site.get('slot')=='0x15c' and args==['DAT_0143e8f8'] for _,_,args,site,_,vt in ordered):return source,[]
 if not any(vt and site.get('slot')=='0x164' for _,_,_,site,_,vt in ordered):return source,[]
 if rdata is None:
  if not hasattr(recover,'rdata'):
   from tools.script_recovery.lift_native_lua import RData
   recover.rdata=RData()
  rdata=recover.rdata
 try:start=int(fn['address'],16);end=int(fn.get('bodyEndExclusive','0'),16)
 except (KeyError,ValueError,TypeError):return source,[]
 if end<=start:return source,[]
 raw=rdata.bytes_at(start,end-start)
 if not raw:return source,[]
 decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
 code=list(decoder.disasm(raw,start))
 if not code or code[-1].address+code[-1].size!=end:return source,[]
 first=code[0]
 if first.mnemonic!='sub' or len(first.operands)!=2 or first.operands[0].type!=X86_OP_REG or first.reg_name(first.operands[0].reg)!='esp' or first.operands[1].type!=X86_OP_IMM:return source,[]
 frame=first.operands[1].imm
 for ins in code[1:8]:
  if ins.mnemonic=='push' and ins.operands[0].type==X86_OP_REG:frame+=4
  elif ins.mnemonic=='mov' and all(o.type==X86_OP_REG for o in ins.operands):continue
  else:break
 indices={i.address:n for n,i in enumerate(code)}
 ftol_ok=None
 def origin(reg,before,budget=60):
  nonlocal ftol_ok
  if budget<=0:return None
  for n in range(before-1,max(-1,before-budget-1),-1):
   ins=code[n];ops=ins.operands
   if ins.mnemonic.startswith(('j','ret','loop')):return None
   if ins.mnemonic=='call':
    if not(reg in ('ebx','ebp','esi','edi') and len(ops)==1 and ops[0].type==X86_OP_IMM and ops[0].imm==FTOL[0]):return None
    if ftol_ok is None:
     body=rdata.bytes_at(FTOL[0],FTOL[1]);ftol_ok=body is not None and hashlib.sha256(body).hexdigest()==FTOL[2]
    if not ftol_ok:return None
    continue
   if ops and ops[0].type==X86_OP_MEM and ins.mnemonic in ('mov','inc','dec','add','sub','xor','and','or','xchg','fstp','fistp','fst','fist'):
    if ins.reg_name(ops[0].mem.base)!='esp' or ops[0].mem.index:return None
   _,writes=ins.regs_access()
   aliases={'eax':{'eax','ax','al','ah'},'ebx':{'ebx','bx','bl','bh'},'ecx':{'ecx','cx','cl','ch'},'edx':{'edx','dx','dl','dh'},'ebp':{'ebp','bp'},'esi':{'esi','si'},'edi':{'edi','di'}}
   if not any(ins.reg_name(r) in aliases.get(reg,{reg}) for r in writes):continue
   if ins.mnemonic!='mov' or len(ops)!=2 or ops[0].type!=X86_OP_REG or ops[0].size!=4 or ins.reg_name(ops[0].reg)!=reg:return None
   op=ops[1]
   if op.type==X86_OP_REG and op.size==4:return origin(ins.reg_name(op.reg),n,budget-(before-n))
   if op.type!=X86_OP_MEM or op.size!=4 or op.mem.index or op.mem.segment:return None
   if not op.mem.base:return ('global',op.mem.disp)
   base=origin(ins.reg_name(op.mem.base),n,budget-(before-n))
   return ('load',base,op.mem.disp) if base is not None else None
  return None
 candidates=defaultdict(list)
 for pos,last,args,site,key,vt in ordered:
  if not vt or site.get('slot')!='0x15c' or args!=['DAT_0143e8f8']:continue
  head=source[source.rfind('\n',0,pos)+1:pos]
  match=re.fullmatch(r'\s*(\w+) = \(\*\*\(code \*\*\)\(\*DAT_0143e8f8 \+ 0x15c\)\)\(',head)
  if not match or len(re.findall(r'^\s*'+re.escape(match[1])+r' = ',source,re.M))!=1:continue
  at=indices.get(int(site['site'],16))
  if at is None:continue
  call=code[at];op=call.operands[0]
  if op.type!=X86_OP_MEM or op.mem.index or op.mem.disp!=0x15c or not op.mem.base:continue
  if origin('ecx',at)!=GSI or origin(call.reg_name(op.mem.base),at)!=('load',GSI,0):continue
  slot=result_stack_slot(fn,site,rdata)
  if slot is not None:candidates[slot].append((int(site['site'],16),match[1]))
 edits=[];evidence=[]
 for pos,last,args,site,key,vt in ordered:
  if not vt or site.get('slot')!='0x164':continue
  printed=len(args)==3 and args[0]=='DAT_0143e8f8'
  if len(args)!=(3 if printed else 2):continue
  address=int(site['site'],16);at=indices.get(address)
  if at is None or site.get('depth')!=frame+8:continue
  call=code[at];op=call.operands[0]
  if op.type!=X86_OP_MEM or op.mem.index or op.mem.disp!=0x164 or not op.mem.base:continue
  if origin('ecx',at)!=GSI or origin(call.reg_name(op.mem.base),at)!=('load',GSI,0):continue
  setup=read_call_window(rdata,start,end-start,address,argument_count=2)
  if setup is None:continue
  first_arg=setup.stack_arguments[0]
  if len(first_arg)!=2 or first_arg[0]!='memory' or len(first_arg[1])!=2 or first_arg[1][0]!='stack':continue
  # The bounded setup must push exactly the two ABI arguments. This makes
  # its entry ESP the steady local frame, including pushes before the ID load.
  begin=at
  while begin>0 and not(code[begin-1].mnemonic=='call' or code[begin-1].mnemonic.startswith(('j','ret','loop'))):begin-=1
  if sum(i.mnemonic=='push' for i in code[begin:at])!=2:continue
  if any(i.mnemonic in ('pop','leave','enter') or (i.mnemonic in ('add','sub') and i.op_str.startswith('esp,')) for i in code[begin:at]):continue
  slot=first_arg[1][1]-frame;choices=candidates.get(slot,[])
  if len(choices)!=1 or choices[0][0]>=address:continue
  index=int(printed);old=args[index];new=choices[0][1]
  if not re.fullmatch(r'\w+',old) or old==new:continue
  replaced=list(args);replaced[index]=new
  edits.append((pos,last,','.join(replaced)))
  evidence.append({'site':site['site'],'slot':slot,'old':old,'new':new,'register_site':hex(choices[0][0])})
 for pos,last,value in reversed(edits):source=source[:pos]+value+source[last:]
 return source,evidence
