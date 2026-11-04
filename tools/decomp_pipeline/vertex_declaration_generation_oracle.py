"""Validate declaration-generator code/table boundaries and execute native comparisons."""
from pathlib import Path
import sys,struct,json
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE
import pefile,capstone,near_match
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE,UcError
from unicorn.x86_const import *
SYMBOL='?GenerateD3DDeclaration@CVertexShaderInput@@AAEXXZ'
ENTRY=0xab3870;INSERT=0xab4260;TABLE=0x129e9cc
OBJ=0x2000000;STREAM=OBJ+0x1000;ELEMENT=OBJ+0x2000;DECL=OBJ+0x9000;ALT=OBJ+0x19000;STOP=0x2200000;STACK=0x3000000
TYPES=[0,1,2,3,4,6,7,6,-1,-1,-1,-1,-1,-1]
SIZES=[4,8,12,16,4,4,8,4,2,4,6,8,4,4]
def validate(obj):
 c=Coff(obj);sec,off=c.symbols['?GenerateD3DDeclaration@CVertexShaderInput@@AAEXXZ'];b,rel=c.sections[sec];assert off==0 and len(b)==662
 mapsec,mapoff=c.symbols['_g_VertexStreamD3DTypes_0129E9CC'];assert c.sections[mapsec][0][mapoff:mapoff+56]==struct.pack('<14l',*TYPES)
 with pefile.PE(str(EXE)) as pe:
  n=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,668)
  assert pe.get_data(TABLE-pe.OPTIONAL_HEADER.ImageBase,56)==struct.pack('<14l',*TYPES)
 md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
 bis=list(md.disasm(b[:622],ENTRY));nis=list(md.disasm(n[:612],ENTRY))
 for ins,size in [(bis,622),(nis,612)]:assert sum(i.size for i in ins)==size and ins[-1].mnemonic=='ret'
 assert b[622:624]==b'\x8b\xff'
 patched=bytearray(b);checked=[]
 for site,(sym,kind) in rel.items():
  add=int.from_bytes(b[site:site+4],'little')
  if kind==20:
   assert site==184 and add==0 and sym=='?_M_fill_insert@?$vector@U_D3DVERTEXELEMENT9@@V?$allocator@U_D3DVERTEXELEMENT9@@@_STL@@@_STL@@QAEXPAU_D3DVERTEXELEMENT9@@IABU3@@Z'
   target=INSERT
  elif sym=='_g_VertexStreamD3DTypes_0129E9CC':assert site==284 and add==0;target=TABLE
  else:
   s,o=c.symbols[sym];assert s==sec and kind==6;target=ENTRY+o
   if site>=624:assert site in range(624,648,4) and target+add in {i.address for i in bis}
   else:assert site in [367,374] and o+add in [624,648]
  struct.pack_into('<I',patched,site,(target+add-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
  checked.append([site,sym,kind,target+add])
 assert len(rel)==10 and list(b[648:])==[0,1,2,3,0,0,1,0,4,0,5,1,0,0]
 native_abs=[]
 for ins in nis:
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_MEM and op.mem.disp in [TABLE,ENTRY+612]:native_abs.append(ins.address-ENTRY+ins.disp_offset)
 assert len(native_abs)==2
 starts={i.address for i in nis}
 for i in range(14):
  target=struct.unpack_from('<I',n,612+4*i)[0];assert target in starts
  candidate_target=struct.unpack_from('<I',patched,624+4*b[648+i])[0]
  for code,address in [(n,target),(patched,candidate_target)]:
   instruction=next(md.disasm(bytes(code[address-ENTRY:]),address))
   assert instruction.mnemonic=='mov' and instruction.operands[0].reg==capstone.x86.X86_REG_EAX and instruction.operands[1].imm==SIZES[i]
 assert any(i.mnemonic=='call' and i.op_str==hex(INSERT) for i in nis)
 score=near_match.compare(b[:622],n[:612],list(rel),native_abs);assert score.near_match
 return bytes(patched),n,dict(similarity=score.to_dict(),relocations=checked,native_absolute_sites=native_abs)

def execute(code,streams,used,cap):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,size in [(0xab0000,0x10000),(0x129e000,0x1000),(OBJ,0x30000),(STOP,0x1000),(STACK,0x10000)]:u.mem_map(a,size)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return int.from_bytes(u.mem_read(a,4),'little')
 u.mem_write(ENTRY,code);u.mem_write(TABLE,struct.pack('<14l',*TYPES))
 put(OBJ+4,STREAM);put(OBJ+8,STREAM+20*len(streams));put(OBJ+12,STREAM+20*len(streams))
 for i,types in enumerate(streams):
  p=STREAM+20*i;e=ELEMENT+0x1000*i;put(p,57+i);put(p+8,e);put(p+12,e+8*len(types));put(p+16,e+8*len(types))
  for j,t in enumerate(types):put(e+8*j,0x123400fe+j);put(e+8*j+4,t)
 begin=DECL if cap else 0
 put(OBJ+16,begin);put(OBJ+20,begin+8*used);put(OBJ+24,begin+8*cap)
 u.mem_write(DECL,b'\xcd'*4096);u.mem_write(ALT,b'\xab'*4096);events=[]
 total=sum(len(s)+1 for s in streams)
 def hook(u,a,size,data):
  if a!=INSERT:return
  sp=u.reg_read(UC_X86_REG_ESP);position,count,value=get(sp+4),get(sp+8),get(sp+12)
  assert u.reg_read(UC_X86_REG_ECX)==OBJ+16 and position==begin+used*8 and count==total-used
  fill=bytes(u.mem_read(value,8));assert fill==bytes(8);events.append((count,fill.hex()))
  first=begin
  if total>cap:
   first=ALT
   if used:u.mem_write(first,bytes(u.mem_read(begin,used*8)))
   put(OBJ+16,first);put(OBJ+24,first+total*8)
  if count:u.mem_write(first+used*8,fill*count)
  put(OBJ+20,first+total*8)
  u.reg_write(UC_X86_REG_ESP,sp+16);u.reg_write(UC_X86_REG_EIP,get(sp))
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,OBJ)
 saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(saved):u.reg_write(r,0x556600+i)
 try:u.emu_start(ENTRY,STOP,count=100000)
 except UcError:
  assert not streams and not cap
  return ('null-empty-fault',events)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4
 assert all(u.reg_read(r)==0x556600+i for i,r in enumerate(saved))
 assert len(events)==int(total>=used)
 first=get(OBJ+16);valid=all(t<8 for s in streams for t in s)
 assert get(OBJ+20)==first+(total*8 if valid else 0)
 expected=bytearray(b'\xcd'*(min(used,total)*8)+bytes(max(0,total-used)*8))
 offset=0;out=0
 for i,types in enumerate(streams):
  for j,t in enumerate(types):
   if t>=8:break
   expected[out*8:(out+1)*8]=struct.pack('<HHBBBB',i,offset&65535,TYPES[t],0,5,(0xfe+j)&255)
   offset+=SIZES[t];out+=1
  else:continue
  break
 if valid:expected[out*8:(out+1)*8]=struct.pack('<HHBBBB',255,0,17,0,0,0)
 assert bytes(u.mem_read(first,len(expected)))==expected
 return events,bytes(u.mem_read(OBJ+16,12)).hex(),bytes(u.mem_read(DECL,4096)).hex(),bytes(u.mem_read(ALT,4096)).hex()

def cases():
 patterns=[[],[[]],[[0]],[[0,1,2,3,4,5,6,7]],[[7,0],[2,3]], [[],[4],[]]]
 patterns += [[[t]] for t in range(14)]
 patterns += [[[0,1,t,3],[5,6]] for t in range(8,14)]
 patterns += [[[0]*260,[7,3,1]]]
 for streams in patterns:
  total=sum(len(s)+1 for s in streams)
  for used in sorted({0,1,max(0,total-1),total,total+2}):
   for cap in sorted({used,max(used,total),max(used,total+3)}):yield streams,used,cap

