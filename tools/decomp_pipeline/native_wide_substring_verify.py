from pathlib import Path
import sys,json,re,struct,itertools,hashlib
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x99bdb0;SIZE=179;ARENA=0x2000000;STACK=0x3000000;STOP=0x4000000;COUNTER=0x13bca20;EMPTY=0x129a8e0
SRC=ARENA+0x80;OUT=ARENA+0x100;DATA=ARENA+0x200;NEW=ARENA+0x300;TEXT=ARENA+0x1000;NEWTEXT=ARENA+0x1800
TARGETS={'?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z':0x99b3c0,'?UnassignString@CWideString@@AAEXXZ':0x99b4d0,'_FixtureVectorFree':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc,'?g_CWideStringInstanceCount_013BCA20@@3HA':COUNTER,'?g_FableEmptyWideString_0129A8E0@@3PAGA':EMPTY}
def execute(code,args,bad_allocation=False):
 text,start,end,owners,counter,seed=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,size in [(0x99b000,0x2000),(0xbfe000,0x2000),(0x129a000,0x1000),(0x13bc000,0x1000),(ARENA,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(address,size)
 u.mem_write(ENTRY,code);u.mem_write(EMPTY,b'\0\0');u.mem_write(COUNTER,struct.pack('<I',counter))
 memory=bytearray((i*37+seed*19)&255 for i in range(0x4000))
 def put(a,v):struct.pack_into('<I',memory,a-ARENA,v&0xffffffff)
 put(SRC,DATA if text is not None else 0)
 if text is not None:
  for offset,value in [(0,TEXT),(4,TEXT+2*len(text)),(8,TEXT+0x100),(12,owners)]:put(DATA+offset,value)
  memory[TEXT-ARENA:TEXT-ARENA+2*(len(text)+1)]=struct.pack('<%dH'%(len(text)+1),*(text+[0]))
 u.mem_write(ARENA,bytes(memory));expected=bytearray(memory);allocated=text is not None and start<len(text) and text[start]!=0
 def predict(a,v):struct.pack_into('<I',expected,a-ARENA,v&0xffffffff)
 predict(OUT,NEW if allocated else 0)
 if allocated:
  length=end-start
  for offset,value in [(0,NEWTEXT),(4,NEWTEXT+2*length),(8,NEWTEXT+2*(length+1)),(12,1)]:predict(NEW+offset,value)
  expected[NEWTEXT-ARENA:NEWTEXT-ARENA+2*(length+1)]=struct.pack('<%dH'%(length+1),*(text[start:end]+[0]))
 stack=STACK+0x8000;u.mem_write(stack,struct.pack('<4I',STOP,OUT,start,end));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,SRC)
 saved={UC_X86_REG_EBX:0x11223344,UC_X86_REG_ESI:0x22334455,UC_X86_REG_EDI:0x33445566,UC_X86_REG_EBP:0x44556677}
 for register,value in saved.items():u.reg_write(register,value)
 calls=[]
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def hook(uc,address,size,_):
  if address==STOP:uc.emu_stop();return
  if address not in (0x99b3c0,0x99b4d0,0xbfea14,0xbfe9bc):return
  assert address==0x99b3c0 and allocated and not calls,('unexpected child',hex(address),args)
  sp=uc.reg_read(UC_X86_REG_ESP);assert get(sp+4)==TEXT+start*2 and get(sp+8)==end-start
  assert STACK<=uc.reg_read(UC_X86_REG_ECX)<STACK+0x10000
  assert get(COUNTER)==(counter+1)&0xffffffff
  calls.append((start,end-start))
  if not bad_allocation:
   for offset,value in [(0,NEWTEXT),(4,NEWTEXT+2*(end-start)),(8,NEWTEXT+2*(end-start+1)),(12,1)]:uc.mem_write(NEW+offset,struct.pack('<I',value))
   uc.mem_write(NEWTEXT,struct.pack('<%dH'%(end-start+1),*(text[start:end]+[0])))
  uc.reg_write(UC_X86_REG_EAX,0 if bad_allocation else NEW);uc.reg_write(UC_X86_REG_ESP,sp+12);uc.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);u.emu_start(ENTRY,STOP,count=10000)
 assert bytes(u.mem_read(ARENA,0x4000))==bytes(expected),('arena',args)
 assert get(COUNTER)==(counter+1)&0xffffffff and u.reg_read(UC_X86_REG_EAX)==OUT
 assert u.reg_read(UC_X86_REG_ESP)==stack+16 and u.reg_read(UC_X86_REG_EIP)==STOP
 for register,value in saved.items():assert u.reg_read(register)==value
 assert len(calls)==int(allocated)
 return calls
b=Path(sys.argv[1]).resolve();name='?GetSubString@CWideString@@QBE?AV1@KK@Z';obj=Path(sys.argv[2]).resolve();c=Coff(obj);section,offset=c.symbols[name];body,rels=c.sections[section];assert not offset and len(body)==SIZE;original=read_virtual_data(EXE.read_bytes(),ENTRY,SIZE)
score=compare(body,original,tuple(rels),retail_relocations('0099bdb0',SIZE)).to_dict();assert score['near_match'];fixed=bytearray(body)
for site,(dependency,kind) in rels.items():struct.pack_into('<I',fixed,site,(TARGETS[dependency]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
cases=0
for text in [None,[],[65],[65,66,67],[65,0,67],[0,66]]:
 length=len(text) if text is not None else 0
 for start in range(length+1):
  for end in range(start,length+1):
   for owners,counter,seed in itertools.product((1,2,0x80000000,0xffffffff),(0,1,0x7fffffff,0xffffffff),(0,7)):
    args=(text,start,end,owners,counter,seed);assert execute(bytes(fixed),args)==execute(original,args);cases+=1
controls=0
for code,bad in [(b'\xc2\x0c\x00',False),(bytes(fixed),True)]:
 try:execute(code,([65,66,67],1,2,2,0xffffffff,7),bad)
 except Exception:controls+=1
 else:raise AssertionError('undetected substring fault')
(b/'substring-contract-verification.json').write_text(json.dumps(dict(status='PASS_SCRATCH',cases=cases,original_cases=cases,controls=controls,score=score,object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),boundary='Typed allocation child controlled;full16KiB arena,counter wrap,stack/nonvolatile ABI independently predicted. Null/empty/interior-NUL strings,in-bounds start/end,zero-length middle slices,source signed owner patterns. Output slot distinct from source. No out-of-range,unsigned-underflow,allocation-failure/unwind claim;actual children separately exercised in native dot-path chain.'),indent=2));print('WIDE_SUBSTRING_CONTRACT_PASS cases=%d original=%d controls=%d'%(cases,cases,controls))
