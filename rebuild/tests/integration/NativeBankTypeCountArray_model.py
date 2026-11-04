from pathlib import Path
import sys,struct,json,itertools,hashlib
sys.path.insert(0,'tools/decomp_pipeline')
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
ENTRY=0x9d2af0;MALLOC=0xbfea0e;A=0x2000000;VECTOR=A+0x100;DATA=A+0x1000;STACK=0x3000000;STOP=0x4000000
SYMBOL='??0?$vector@VCBankFileDataTypeCount@@V?$allocator@VCBankFileDataTypeCount@@@_STL@@@_STL@@QAE@I@Z'
original=read_virtual_data(EXE.read_bytes(),ENTRY,94)
def execute(body,args):
 count,mutation=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in (ENTRY&~4095,MALLOC&~4095,STOP):u.mem_map(page,4096)
 u.mem_map(A,0x3000);u.mem_map(STACK,0x10000);u.mem_write(ENTRY,body);u.mem_write(MALLOC,b'\xc3');expected=bytearray((i*37+13)&255 for i in range(0x3000));u.mem_write(A,bytes(expected));sp=STACK+0x8000;calls=[];data=DATA+(64 if mutation==2 else 0)
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,value):u.mem_write(a,struct.pack('<I',value&0xffffffff))
 put(sp,STOP);put(sp+4,count)
 def hook(uc,address,length,_):
  if address!=MALLOC:assert ENTRY<=address<ENTRY+len(body),hex(address);return
  esp=uc.reg_read(UC_X86_REG_ESP);assert count and not calls and get(esp+4)==count*8;assert [get(VECTOR+i*4) for i in range(3)]==[0,0,0];calls.append(count*8)
  if mutation==1:
   for i in range(3):put(VECTOR+4*i,0xdeadbeef+i)
  if mutation==3:put(sp+4,0x12345678)
  if mutation==4:
   put(A+0x200,0xabcdef12);struct.pack_into('<I',expected,0x200,0xabcdef12)
  uc.reg_write(UC_X86_REG_EAX,data);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0xdeadf00d);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4)
 start=data if count else 0;struct.pack_into('<III',expected,0x100,start,start+count*8,start+count*8)
 if count:expected[data-A:data-A+count*8]=b'\0'*(count*8)
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 for n,reg in enumerate(saved):u.reg_write(reg,0x55660000+n)
 u.reg_write(UC_X86_REG_ECX,VECTOR);u.reg_write(UC_X86_REG_ESP,sp)
 try:u.emu_start(ENTRY,STOP,count=5000)
 except Exception as exc:raise AssertionError((args,calls,hex(u.reg_read(UC_X86_REG_EIP)))) from exc
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and u.reg_read(UC_X86_REG_EAX)==VECTOR and all(u.reg_read(reg)==0x55660000+n for n,reg in enumerate(saved))
 assert len(calls)==bool(count) and bytes(u.mem_read(A,0x3000))==expected,(args,'state')
 return dict(args=args,calls=calls,state_sha256=hashlib.sha256(expected).hexdigest())
def compare(obj):
 c=Coff(obj);s,o=c.symbols[SYMBOL];body,rels=c.sections[s];assert not o and len(body)==94 and len(rels)==1;fixed=bytearray(body)
 for site,(name,kind) in rels.items():assert name in ('_malloc','_FixtureTypeCountMalloc') and kind==20;struct.pack_into('<I',fixed,site,(MALLOC+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4))&0xffffffff)
 assert bytes(fixed)==original;results=[]
 for args in itertools.product((0,1,2,7,16,31,256),range(5)):
  a=execute(original,args);z=execute(bytes(fixed),args);assert a==z;results.append(a)
 controls=[('allocation-width',0x9d2b12,b'\xbd'),('first-word',0x9d2b39,b'\x90'*2),('second-word',0x9d2b3b,b'\x90'*3),('stride',0x9d2b40,b'\x04'),('finish',0x9d2b44,b'\x90'*3)]
 for name,address,replacement in controls:
  args=(7,0);execute(original,args);changed=bytearray(original);offset=address-ENTRY;changed[offset:offset+len(replacement)]=replacement
  try:execute(bytes(changed),args)
  except AssertionError:pass
  else:raise AssertionError(name)
 return dict(status='PASS',grade='MATCH',cases=len(results),bytes=94,fixups=1,controls=[c[0]for c in controls],object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),boundary='Full12KiB state, zero/nonzero counts, return/register/stack ABI and mutating allocator callbacks. Normal allocation only; no failure/unwinding claim.')
if __name__=='__main__':
 b=Path(sys.argv[1]).resolve();report=compare(b/'root.obj');p=b/'model.json';assert not p.exists();p.write_text(json.dumps(report,indent=2)+'\n');print('TYPE_COUNT_ARRAY_MODEL_PASS',report['cases'])
