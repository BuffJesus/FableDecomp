"""Bounded original/recreated item reader; dependent cursor/string calls controlled."""
from pathlib import Path
import sys,struct,json,itertools
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9ba110;DATA=0x2000000;STACK=0x3000000;STOP=0x4000000
BANK=DATA+0x100;ITEM=DATA+0x200;TEXT=DATA+0x400;NAME=DATA+0x800;SLICE=DATA+0xc00
TARGETS={'?SkipUntilString@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9a20,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,'??1CCharString@@QAE@XZ':0x99eae0}
def execute(code,text,pos,delimiter,quote,mutation):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,size in ((0x990000,0x30000),(DATA,0x10000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(address,size)
 u.mem_write(ENTRY,bytes(code));u.mem_write(DATA,bytes([0x63])*0x10000);u.mem_write(TEXT,text+b'\0');trace=[]
 put=lambda a,v:u.mem_write(a,struct.pack('<I',v&0xffffffff))
 get=lambda a:struct.unpack('<I',u.mem_read(a,4))[0]
 put(BANK+4,TEXT);put(BANK+0x20,pos);put(BANK+0x1c,quote);put(ITEM,0x12345678);put(ITEM+4,0x77aabbcc);put(ITEM+8,0x88aabbcc)
 oldbank=bytes(u.mem_read(BANK,40));olditem=bytes(u.mem_read(ITEM,12));expectedbank=bytearray(oldbank);expecteditem=bytearray(olditem)
 found=text.find(delimiter,pos) if pos<len(text) else -1;ok=found>=0
 newpos=found if ok else len(text)+1 if pos<len(text) else pos
 struct.pack_into('<I',expectedbank,0x20,newpos)
 # Explicitly controlled dependent cursor may mutate another parser field.
 if mutation:struct.pack_into('<I',expectedbank,0x24,0xfedcba98)
 if ok:struct.pack_into('<I',expecteditem,0,57 if mutation else 2);struct.pack_into('<I',expecteditem,8,SLICE)
 def hook(uc,address,size,_):
  if address not in TARGETS.values():return
  sp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;rv=0xdead0000
  if address==0x9b9a20:
   assert not trace and cx==BANK and get(sp+4)==NAME
   put(BANK+0x20,newpos)
   if mutation:put(BANK+0x24,0xfedcba98)
   trace.append('cursor');rv=0xbeef0000|ok;pop=4
  elif address==0x99ebf0:
   assert ok and trace==['cursor'] and get(sp+4)==TEXT+pos and get(sp+8)==found-pos
   put(cx,SLICE);trace.append('slice');rv=cx;pop=8
  elif address==0x99efb0:
   assert trace==['cursor','slice'] and cx==ITEM+8 and get(get(sp+4))==SLICE
   put(cx,SLICE);trace.append('assign');rv=cx;pop=4
  else:
   assert trace==['cursor','slice','assign'] and get(cx)==SLICE and get(ITEM)==2
   put(cx,0);trace.append('destroy')
   if mutation:put(ITEM,57)
  ret=get(sp);uc.reg_write(UC_X86_REG_EAX,rv);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,NAME);put(sp+8,ITEM)
 regs={UC_X86_REG_EBX:0x11223344,UC_X86_REG_ESI:0x22334455,UC_X86_REG_EDI:0x33445566,UC_X86_REG_EBP:0x44556677}
 for reg,value in regs.items():u.reg_write(reg,value)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,BANK);u.emu_start(ENTRY,STOP,count=1000)
 assert bool(u.reg_read(UC_X86_REG_EAX)&255)==ok and u.reg_read(UC_X86_REG_ESP)==sp+12
 assert all(u.reg_read(reg)==value for reg,value in regs.items())
 assert bytes(u.mem_read(BANK,40))==bytes(expectedbank) and bytes(u.mem_read(ITEM,12))==bytes(expecteditem)
 assert trace==(['cursor','slice','assign','destroy'] if ok else ['cursor'])
 return trace

def run(obj,out):
 c=Coff(obj);sec,off=c.symbols['?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z'];body,rels=c.sections[sec];fixed=bytearray(body);assert not off
 for site,(name,kind) in rels.items():assert kind==20;struct.pack_into('<I',fixed,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),ENTRY,93);cases=0
 for text in (b'',b'alpha',b'one\\two',b'\\x\\',b'"a\\b"\\tail',b'..\\parent\\file',b'one;;two'):
  for pos,delimiter,quote,mutation in itertools.product(sorted({0,len(text)//2,len(text)}),(b'\\',b';',b'xx',b''),(0,1),(0,1)):
   assert execute(original,text,pos,delimiter,quote,mutation)==execute(fixed,text,pos,delimiter,quote,mutation);cases+=1
 report=dict(status='PASS',cases=cases,bytes=len(body),boundary='Original and recreated full parser/item memory, low return byte, ordered call arguments and callee/stack ABI. Cursor and string construction/assignment/destruction controlled, including callback mutations. No dependent helper or native unwind claim.');Path(out).write_text(json.dumps(report,indent=2));print('PARSER_ITEM_MODEL_PASS',cases)
if __name__=='__main__':run(Path(sys.argv[1]),Path(sys.argv[2]))
