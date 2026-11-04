"""Counted character append, always-reallocating reserve and word fill contracts."""
import hashlib,itertools,re,struct,tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from native_exact_roots import import_names
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,CS_OP_IMM
from unicorn import Uc,UcError,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
APPEND,RESERVE,FILL=0x99b9a0,0x99c990,0x5123a0
SPECS=[('append',R/'rebuild/src/compiled/00/99/CWideString_AppendCharacter_0099b9a0.cpp','??YCWideString@@QAEABV0@G@Z',APPEND,203,205),
 ('reserve',R/'rebuild/src/compiled/00/99/STLWideString_Reserve_0099c990.cpp','?reserve@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEXI@Z',RESERVE,152,152),
 ('fill',R/'rebuild/src/compiled/00/51/STLWideString_UninitializedFillN_005123a0.cpp','??$uninitialized_fill_n@G@_STL@@YIPAGPAGIABG@Z',FILL,32,32)]
ADDRESS={SPECS[1][2]:RESERVE,SPECS[2][2]:FILL,'?MakeOwnCopyOfData@CWideString@@AAEXXZ':0x99b560,
 '?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z':0x99b3c0,
 '?_M_throw_length_error@?$_String_base@GV?$allocator@G@_STL@@@_STL@@QBEXXZ':0x99c550,
 '_malloc':0xbfea0e,'_memmove':0xbfeae6,'_free':0xbfea14}
ARENA,LENGTH,OBJ,DATA,COPY_DATA,ALLOC_DATA=0x2000000,0x8000,0x2000100,0x2000200,0x2000240,0x2000280
OLD_TEXT,COPY_TEXT,NEW_TEXT,ALLOC_TEXT,ALT_TEXT=0x2001000,0x2002000,0x2003000,0x2004000,0x2005000
STACK,STOP=0x3000000,0x4000000

def audit_bodies(directory,sources=None,include=None):
 reports=[];codes={};raw=EXE.read_bytes();imports=import_names(raw);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
 for name,canonical,symbol,entry,size,built_size in SPECS:
  source=(sources or {}).get(name,canonical);obj=directory/(name+'.obj');env=V.env()
  if include:env['INCLUDE']=str(include)+';'+str(V.VC/'include')
  flags=['/nologo','/c','/O2','/MT','/W3',*(['/I'+str(include)] if include else []),'/Fo'+str(obj),str(source)]
  result=V.cl(flags,env,directory);assert result.returncode==0,result.stdout+result.stderr
  c=Coff(obj);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert off==0 and len(body)==built_size
  original=read_virtual_data(raw,entry,size);grade=compare(body,original,tuple(rels),retail_relocations('%08x'%entry,size)).to_dict();assert grade['near_match']
  patched=bytearray(body);audit=[]
  original_calls=[i.operands[0].imm for i in md.disasm(original,entry) if i.mnemonic=='call' and i.operands[0].type==CS_OP_IMM]
  assert original_calls==[ADDRESS[target] for site,(target,kind) in sorted(rels.items()) if kind==20]
  for site,(target,kind) in sorted(rels.items()):
   assert kind==20 and struct.unpack_from('<i',body,site)[0]==0;address=ADDRESS[target]
   if name!='append':assert (entry+site+4+struct.unpack_from('<i',original,site)[0])&0xffffffff==address
   if target in ('_malloc','_memmove','_free'):
    thunk=read_virtual_data(raw,address,6);assert thunk[:2]==b'\xff\x25' and imports[struct.unpack_from('<I',thunk,2)[0]]==target[1:]
   struct.pack_into('<I',patched,site,(address-entry-site-4)&0xffffffff);audit.append(dict(site=site,symbol=target,kind=kind,target=hex(address)))
  if name!='append':assert bytes(patched)==original
  codes[entry]=bytes(patched);reports.append(dict(name=name,symbol=symbol,address=hex(entry),bytes=len(body),original_bytes=size,grade=grade,acceptance_grade='NEAR_MATCH' if name=='append' else 'MATCH',relocations=audit))
 return codes,reports

class Machine:
 def __init__(self,codes,seed):
  self.u=Uc(UC_ARCH_X86,UC_MODE_32)
  for a,n in ((0x99b000,0x2000),(0x512000,0x1000),(0xbfe000,0x1000),(ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000)):
   self.u.mem_map(a,n);self.u.mem_write(a,b'\xcc'*n)
  for a,b in codes.items():self.u.mem_write(a,b)
  self.u.mem_write(ARENA,bytes((i*37+seed*19)&255 for i in range(LENGTH)));self.trace=[];self.seed=seed
 def put(self,a,v):self.u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(self,a):return struct.unpack('<I',self.u.mem_read(a,4))[0]
 def seal(self):self.expected=bytearray(self.u.mem_read(ARENA,LENGTH))
 def model(self,a,v):struct.pack_into('<I',self.expected,a-ARENA,v&0xffffffff)
 def bytes_model(self,a,b):self.expected[a-ARENA:a-ARENA+len(b)]=b
 def bytes_both(self,a,b):self.u.mem_write(a,b);self.bytes_model(a,b)
 def run(self,entry,this,args,hook,expected='normal',edx=0,returned=None):
  sp=STACK+0x8000;self.put(sp,STOP)
  for i,value in enumerate(args):self.put(sp+4+i*4,value)
  saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
  for i,reg in enumerate(saved):self.u.reg_write(reg,0x55660000+self.seed+i)
  self.u.reg_write(UC_X86_REG_ECX,this);self.u.reg_write(UC_X86_REG_EDX,edx);self.u.reg_write(UC_X86_REG_ESP,sp)
  def dispatch(u,a,n,_):hook(self,a)
  self.u.hook_add(UC_HOOK_CODE,dispatch)
  fault=False
  try:self.u.emu_start(entry,STOP,count=10000)
  except UcError:
   if expected!='fault':raise
   fault=True
  assert fault==(expected=='fault')
  actual=bytes(self.u.mem_read(ARENA,LENGTH))
  assert actual==bytes(self.expected),[(hex(ARENA+i),x,y) for i,(x,y) in enumerate(zip(actual,self.expected)) if x!=y][:16]
  if expected=='normal':
   assert self.u.reg_read(UC_X86_REG_EIP)==STOP and self.u.reg_read(UC_X86_REG_ESP)==sp+4+len(args)*4
   assert all(self.u.reg_read(reg)==0x55660000+self.seed+i for i,reg in enumerate(saved))
   if returned is not None:assert self.u.reg_read(UC_X86_REG_EAX)==returned
  elif expected=='fatal':assert self.trace[-1][0]=='length_error' and self.u.reg_read(UC_X86_REG_EIP)==0x99c550
  return expected,tuple(self.trace),bytes(self.expected)
 def ret(self,result,pop=0):
  sp=self.u.reg_read(UC_X86_REG_ESP);self.u.reg_write(UC_X86_REG_EAX,result&0xffffffff);self.u.reg_write(UC_X86_REG_ECX,0xdeadbeef);self.u.reg_write(UC_X86_REG_EDX,0x87654321)
  self.u.reg_write(UC_X86_REG_EIP,self.get(sp));self.u.reg_write(UC_X86_REG_ESP,sp+4+pop)

def content(length,seed,base=0x410):return b''.join(struct.pack('<H',base+i+seed) for i in range(length))+b'\0\0'
def initialize(m,data,text,length,capacity,owners=4):
 m.u.mem_write(text,content(length,m.seed))
 for off,value in ((0,text),(4,text+length*2),(8,text+(capacity+1)*2),(12,owners)):m.put(data+off,value)

def reserve_model(m,data,length,request,source,mutation,fail):
 if fail:return
 first=ALT_TEXT if mutation==1 else source;payload=bytes(m.u.mem_read(first,length*2))
 m.bytes_model(NEW_TEXT,payload+b'\0\0')
 if first:m.bytes_model(first,b'\xee'*128)
 for off,value in ((0,NEW_TEXT),(4,NEW_TEXT+length*2),(8,NEW_TEXT+((max(request,length)+1)*2)&0xffffffff)):m.model(data+off,value)
 if mutation==2 and first:m.model(data+12,0xabcdef12)

def reserve_hook(m,a,data,length,request,source,mutation,fail):
 if a==0xbfea0e:
  sp=m.u.reg_read(UC_X86_REG_ESP);assert m.get(sp+4)==((max(request,length)+1)*2)&0xffffffff
  m.trace.append(('malloc',m.get(sp+4)))
  if mutation==1:
   for off,v in ((0,ALT_TEXT),(4,ALT_TEXT+length*2),(8,ALT_TEXT+128)):
    m.put(data+off,v)
    if fail:m.model(data+off,v)
  m.ret(0 if fail else NEW_TEXT)
 elif a==0xbfeae6:
  sp=m.u.reg_read(UC_X86_REG_ESP);first=ALT_TEXT if mutation==1 else source
  assert tuple(m.get(sp+4+i*4) for i in range(3))==(0 if fail else NEW_TEXT,first,length*2)
  m.trace.append(('memmove',length*2));m.u.mem_write(0 if fail else NEW_TEXT,bytes(m.u.mem_read(first,length*2)));m.ret(0 if fail else NEW_TEXT)
 elif a==0xbfea14:
  sp=m.u.reg_read(UC_X86_REG_ESP);first=ALT_TEXT if mutation==1 else source;assert not fail and first and m.get(sp+4)==first
  m.trace.append(('free',first));m.u.mem_write(first,b'\xee'*128)
  if mutation==2:
   for off,v in ((0,0xeeeeeeee),(4,0xdddddddd),(8,0xcccccccc),(12,0xabcdef12)):m.put(data+off,v)
  m.ret(0xbad00000)

def execute_reserve(codes,args):
 length,spare,request,mutation,fail,seed,has_buffer=args;m=Machine(codes,seed)
 initialize(m,DATA,OLD_TEXT,length,length+spare);m.u.mem_write(ALT_TEXT,content(length,seed,0x510))
 source=OLD_TEXT if has_buffer else 0
 if not has_buffer:
  assert length==0
  for off in (0,4,8):m.put(DATA+off,0)
 m.seal();fatal=request>0x7ffffffe
 if not fatal:reserve_model(m,DATA,length,request,source,mutation,fail)
 def hook(m,a):
  if a==0x99c550:
   assert fatal and not m.trace and m.u.reg_read(UC_X86_REG_ECX)==DATA;m.trace.append(('length_error',));m.u.emu_stop()
  else:reserve_hook(m,a,DATA,length,request,source,mutation,fail)
 result=m.run(RESERVE,DATA,[request],hook,'fatal' if fatal else 'fault' if fail else 'normal')
 expected=[('length_error',)] if fatal else [('malloc',((max(request,length)+1)*2)&0xffffffff)]+([('memmove',length*2)] if length else [])+([('free',ALT_TEXT if mutation==1 else source)] if not fail and (source or mutation==1) else [])
 assert m.trace==expected
 return result

def execute_append(codes,args):
 present,length,spare,copy,character,alloc_fail,seed,bad_length,reserve_fail=args;m=Machine(codes,seed)
 initialize(m,DATA,OLD_TEXT,length,length+spare);initialize(m,COPY_DATA,COPY_TEXT,length,length+spare,1)
 if bad_length:m.put(DATA+4,OLD_TEXT-2)
 m.put(OBJ,DATA if present else 0);m.seal();word=character&0xffff;fatal=present and bad_length and not copy
 current=COPY_DATA if copy else DATA;text=COPY_TEXT if copy else OLD_TEXT;grow=present and spare==0 and not fatal
 if present and copy:
  m.model(OBJ,COPY_DATA);m.model(DATA+12,3)
 if present and not fatal:
  if grow:reserve_model(m,current,length,length+max(length,1),text,0,reserve_fail)
  if not(grow and reserve_fail):
   target=(NEW_TEXT if grow else text)+length*2;m.bytes_model(target,struct.pack('<HH',word,0));m.model(current+4,target+2)
 elif not present and word:
  if not alloc_fail:
   for off,v in ((0,ALLOC_TEXT),(4,ALLOC_TEXT+2),(8,ALLOC_TEXT+4),(12,1)):m.model(ALLOC_DATA+off,v)
   m.bytes_model(ALLOC_TEXT,struct.pack('<HH',word,0))
  m.model(OBJ,0 if alloc_fail else ALLOC_DATA)
 def hook(m,a):
  sp=m.u.reg_read(UC_X86_REG_ESP)
  if a==0x99b560:
   assert present and not m.trace and m.u.reg_read(UC_X86_REG_ECX)==OBJ;m.trace.append(('own_copy',))
   if copy:m.put(OBJ,COPY_DATA);m.put(DATA+12,3)
   m.ret(0xbad00000)
  elif a==0x99b3c0:
   assert not present and word and not m.trace and m.u.reg_read(UC_X86_REG_ECX)==OBJ
   pointer=m.get(sp+4);assert m.get(sp+8)==0xffffffff and bytes(m.u.mem_read(pointer,4))==struct.pack('<HH',word,0)
   m.trace.append(('allocate_data',word))
   if not alloc_fail:
    for off,v in ((0,ALLOC_TEXT),(4,ALLOC_TEXT+2),(8,ALLOC_TEXT+4),(12,1)):m.put(ALLOC_DATA+off,v)
    m.u.mem_write(ALLOC_TEXT,struct.pack('<HH',word,0))
   m.put(OBJ,0xeeeeeeee);m.ret(0 if alloc_fail else ALLOC_DATA,8)
  elif a==0x99c550:
   assert fatal and m.u.reg_read(UC_X86_REG_ECX)==DATA;m.trace.append(('length_error',));m.u.emu_stop()
  elif a==FILL:
   assert present and not fatal and not(grow and reserve_fail)
   assert m.u.reg_read(UC_X86_REG_ECX)==(NEW_TEXT if grow else text)+length*2+2 and m.u.reg_read(UC_X86_REG_EDX)==0
   assert bytes(m.u.mem_read(m.get(sp+4),2))==struct.pack('<H',word);m.trace.append(('fill_zero',))
  else:reserve_hook(m,a,current,length,length+max(length,1),text,0,reserve_fail)
 expected='fatal' if fatal else 'fault' if grow and reserve_fail else 'normal'
 result=m.run(APPEND,OBJ,[character],hook,expected,returned=OBJ)
 trace=[]
 if present:
  trace=[('own_copy',)]
  if fatal:trace.append(('length_error',))
  elif grow:
   trace+=[('malloc',(length+max(length,1)+1)*2)]+([('memmove',length*2)] if length else [])
   if not reserve_fail:trace += [('free',text),('fill_zero',)]
  else:trace.append(('fill_zero',))
 elif word:trace=[('allocate_data',word)]
 assert m.trace==trace
 return result

def execute_fill(codes,args):
 count,word,alias,seed,pointer_case=args;m=Machine(codes,seed);dest=OLD_TEXT;value=ARENA+0x6000
 if alias: value=dest+{1:0,2:-1,3:1}[alias]
 else:m.u.mem_write(value,struct.pack('<H',word))
 if pointer_case==1:dest=0
 elif pointer_case==2:value=0
 m.seal();fault=count>0 and (not dest or not value)
 if not fault:
  for i in range(count):
   data=bytes(m.expected[value-ARENA:value-ARENA+2]);m.bytes_model(dest+i*2,data)
 return m.run(FILL,dest,[value],lambda m,a:None,'fault' if fault else 'normal',edx=count,returned=(dest+count*2)&0xffffffff)

def compare_append_char(sources=None,include=None):
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/check_one.py',R/'tools/decomp_pipeline/near_match.py',R/'tools/decomp_pipeline/native_exact_roots.py',R/'tools/decomp_pipeline/gate_game_component_abi.py',R/'tools/decomp_pipeline/pe_oracle.py'}
 visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   closure(next(q for q in (p.parent/n,*([Path(include)/n] if include else []),R/'rebuild/include'/n) if q.is_file()))
 for name,p,*_ in SPECS:closure((sources or {}).get(name,p))
 pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='wide-character-append-') as tmp:built,grades=audit_bodies(Path(tmp),sources,include)
 original={entry:read_virtual_data(EXE.read_bytes(),entry,size) for _,_,_,entry,size,_ in SPECS};partitions={}
 cases=list(itertools.product((False,True),(0,1,3,7),(0,2),(False,True),(0,0x41,0xffff,0xd800,0x12340000,0x12340041),(False,True),(0,7)))
 cases=[(*args,False,False) for args in cases]
 cases += [(True,1,0,copy,c,False,seed,True,False) for copy,c,seed in itertools.product((False,True),(0,0x41),(0,7))]
 cases += [(True,n,0,copy,0x41,False,seed,False,True) for n,copy,seed in itertools.product((0,1),(False,True),(0,7))]
 for args in cases:assert execute_append(built,args)==execute_append(original,args),args
 partitions['append']=len(cases)
 cases=[(*args,True) for args in itertools.product((0,1,3,7),(0,8),(0,1,2,7,16,0x7ffffffe,0x7fffffff,0xffffffff),range(3),(False,True),(0,7))]
 cases += [(0,0,n,mutation,fail,seed,False) for n,mutation,fail,seed in itertools.product((0,1,7),range(3),(False,True),(0,7))]
 for args in cases:
  try:assert execute_reserve(built,args)==execute_reserve(original,args)
  except Exception as error:raise AssertionError(('reserve',args)) from error
 partitions['reserve']=len(cases)
 cases=[(*args,0) for args in itertools.product((0,1,2,7,16),(0,0x41,0xffff,0xd800),range(4),(0,7))]
 cases += [(n,0x41,0,seed,p) for n,seed,p in itertools.product((0,1),(0,7),(1,2))]
 for args in cases:assert execute_fill(built,args)==execute_fill(original,args),args
 partitions['fill']=len(cases)
 controls=0
 for entry,execute,args,noop in ((APPEND,execute_append,(True,1,2,False,0x41,False,0,False,False),b'\x8b\xc1\xc2\x04\x00'),(RESERVE,execute_reserve,(1,8,0,0,False,0,True),b'\xc2\x04\x00'),(FILL,execute_fill,(1,0x41,0,0,0),b'\x8b\xc1\xc2\x04\x00')):
  for bad in (b'\xcc',noop):
   detected=False
   try:execute({**built,entry:bad},args)
   except Exception:detected=True
   assert detected;controls+=1
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=sum(partitions.values()),partitions=partitions,roots=grades,missing_provider_checks=3,root_controls=controls,input_sha256=pins,
  boundaries=['Append executes actual original/rebuilt reserve and fill instructions; copy-on-write and fresh CWideStringData allocation controlled, including live replacement and nullable allocation result.',
  'Full32KiB independent storage/ABI model; embedded NUL/code-unit/dirty upper-bit args, resize/shrink/copy/free, allocator-side live storage mutations, ignored return values, original length-error and null-allocation faults.',
  'Huge reserve requests use bounded synthetic allocations and pointer metadata only; no real huge allocation. Raw null/unaligned/overlap fill probes are bounded ABI stress, not valid C++ reference guarantees.',
  'Fill uses the existing reduced-model template family with an explicit wchar specialization; exact original template-parameter spelling is not claimed.'])
