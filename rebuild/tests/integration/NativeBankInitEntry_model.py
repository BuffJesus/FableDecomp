from pathlib import Path
import sys,json,struct,itertools,hashlib
sys.path.insert(0,'tools/decomp_pipeline')
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
A=0x2000000;BANK=A+0x100;SYMBOL=A+0x300;FILENAME=A+0x304;DATA=A+0x400;STACK=0x3000000;STOP=0x4000000;ENTRY=0x9ce050
TARGETS={0x42d131,0x4014a0,0x9b6300,0x9cbe10,0x99ed10,0x99eae0,0x99ec30,0x9d34e0}
def execute(body,args):
 flags,symbol_kind,file_kind,capacity,index,mode=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {ENTRY&~4095,STOP,0x129a000}|{a&~4095 for a in TARGETS}:u.mem_map(page,4096)
 for address in TARGETS:u.mem_write(address,b'\xc3')
 u.mem_map(A,4096);u.mem_map(STACK,0x10000);u.mem_write(ENTRY,body);u.mem_write(A,bytes((i*31+17)&255 for i in range(4096)))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 put(BANK+0x78,flags);put(SYMBOL,A+0x320 if symbol_kind else 0);put(FILENAME,A+0x340 if file_kind else 0)
 put(A+0x320,A+0x360);put(A+0x324,3 if symbol_kind==2 else 0);put(A+0x340,A+0x380);put(A+0x344,5 if file_kind==2 else 0)
 for off,val in ((0xd0,DATA),(0xd4,DATA+8),(0xd8,DATA+8+capacity*8)):put(BANK+off,val)
 sp=STACK+0x8000;u.mem_write(sp,struct.pack('<4I',STOP,index,SYMBOL,FILENAME));trace=[];live=set();copied=0;destroyed=0
 def hook(uc,address,length,_):
  nonlocal copied,destroyed
  if address not in TARGETS:
   assert ENTRY<=address<ENTRY+len(body),hex(address);return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);dx=uc.reg_read(UC_X86_REG_EDX);pop=0;result=0x1234abcd
  if address==0x42d131:
   assert cx==BANK+0xac and get(esp+4)==SYMBOL;trace.append(('symbol',get(SYMBOL)));result=A+0x700;pop=4
   if mode==1:put(BANK+0x78,get(BANK+0x78)^0x180)
  elif address==0x4014a0:
   assert cx==0;trace.append(('crc',dx,get(esp+4)));result=0x12345678;pop=4
   if mode==2:put(BANK+0x78,get(BANK+0x78)^0x80)
  elif address==0x9b6300:
   assert cx==BANK+0xd0 and get(esp+4)==get(BANK+0xd4) and get(esp+16)==1 and get(esp+20)==1
   value=get(esp+8);trace.append(('overflow',get(value),get(value+4)));u.mem_write(A+0x600,bytes(u.mem_read(value,8)));put(BANK+0xd0,A+0x600);put(BANK+0xd4,A+0x608);put(BANK+0xd8,A+0x620);pop=20
  elif address==0x9cbe10:
   assert cx==BANK and get(esp+8)==FILENAME;result=get(esp+4);put(result,0x11223344);live.add(result);trace.append(('convert',get(FILENAME)));pop=8
  elif address==0x99ed10:
   assert cx in live and get(cx)==0x11223344;result=get(esp+4);put(result,0x55667788);live.add(result);trace.append(('lower',get(cx)));pop=4
  elif address==0x99ec30:
   src=get(esp+4);assert src in live;put(cx,get(src));live.add(cx);copied+=1;result=cx;trace.append(('copy',get(src)));pop=4
   if mode==3:put(BANK+0x78,get(BANK+0x78)^0x40)
  elif address==0x99eae0:
   assert cx in live;trace.append(('destroy',get(cx)));live.remove(cx);put(cx,0);destroyed+=1
  elif address==0x9d34e0:
   assert cx==BANK+0xc4;out=get(esp+4);pair=get(esp+8);assert pair in live;trace.append(('filename',get(pair),get(pair+4)));put(A+0x710,get(pair));put(A+0x714,get(pair+4));put(out,A+0x710);result=out;pop=8
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadc0de);uc.reg_write(UC_X86_REG_EDX,0xdeadf00d);uc.reg_write(UC_X86_REG_ESP,esp+4+pop);uc.reg_write(UC_X86_REG_EIP,get(esp))
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 for n,r in enumerate(saved):u.reg_write(r,0x12340000+n)
 u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(ENTRY,STOP,count=10000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+16 and all(u.reg_read(r)==0x12340000+n for n,r in enumerate(saved))
 assert not live and (copied,destroyed) in ((0,0),(2,4))
 return dict(trace=trace,state_sha256=hashlib.sha256(u.mem_read(A,4096)).hexdigest(),state_hex=bytes(u.mem_read(A,4096)).hex())
def compare(b):
 b=Path(b);spec=json.loads((b/'inventory.json').read_text());c=Coff(b/'root.obj');sec,off=c.symbols[spec['symbol']];body,rels=c.sections[sec];fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(spec['targets'][name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),ENTRY,302);cases=0;inputs=bytearray();expected=bytearray();events={'symbol':1,'crc':2,'overflow':3,'convert':4,'lower':5,'copy':6,'destroy':7,'filename':8}
 for args in itertools.product(range(0,512,64),range(3),range(3),range(2),(0,1,0xffffffff),range(4)):
  a=execute(original,args);z=execute(bytes(fixed),args);assert a==z,(args,a,z);cases+=1;inputs.extend(struct.pack('<6I',*args));expected.extend(struct.pack('<I',len(a['trace'])));expected.extend(bytes.fromhex(a['state_hex']))
  for event in a['trace']:expected.extend(struct.pack('<3I',events[event[0]],*(list(event[1:])+[0]*(3-len(event)))))
 (b/'input.bin').write_bytes(inputs);(b/'expected.bin').write_bytes(expected)
 report=dict(cases=cases,bytes=302,difference_offsets=spec['differences'],boundary='Scratch exact or explicitly graded body: full4KiB state, ABI, normal string lifetime and helper mutation checks; helpers controlled, not native integration.')
 (b/'model.json').write_text(json.dumps(report,indent=2));return report

