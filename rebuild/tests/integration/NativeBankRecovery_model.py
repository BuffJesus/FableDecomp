from pathlib import Path
import sys,struct,itertools,json,hashlib
sys.path.insert(0,'tools/decomp_pipeline')
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
ENTRY=0x9d0380;A=0x2000000;BANK=A+0x100;DATA=A+0x1000;STACK=0x3000000;STOP=0x4000000
TARGETS={'?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ':0x9cbfe0,'?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??0CWideString@@QAE@XZ':0x99aed0,'??_7CDiskFileWin32@@6B@':0x122d06c,'?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z':0x99a6a0,'??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z':0x994700,'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'??1CFileDataInputStream@@UAE@XZ':0x994780,'?Close@CDiskFileWin32@@UAEXXZ':0x99a920,'??1CWideString@@QAE@XZ':0x99b510,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z':0x9cfbc0,'@__security_check_cookie@4':0xbfe9f9,'___security_cookie':0x139c8a8,'??_C@_04NHOPJHAP@BREC?$AA@':0x129b59c}
image=EXE.read_bytes();original=read_virtual_data(image,ENTRY,679)

def execute(code,args):
 exists,magic,valid,chunk,loaded,mutation=args;u=Uc(UC_ARCH_X86,UC_MODE_32)
 pages={v&~4095 for v in TARGETS.values()}|{ENTRY&~4095,STOP}
 for page in pages:u.mem_map(page,4096)
 for address in TARGETS.values():
  if address<0x1000000:u.mem_write(address,b"\xc3")
 u.mem_map(A,0x2000);u.mem_map(STACK,0x10000);u.mem_write(ENTRY,code);u.mem_write(0x129b59c,b'BREC\0');u.mem_write(A,bytes((i*37+13)&255 for i in range(0x2000)));expected=bytearray(u.mem_read(A,0x2000));u.mem_write(DATA,struct.pack('<4I',0x43455242 if magic else 0,7 if valid else 8,123,456));expected[0x1000:0x1010]=u.mem_read(DATA,16)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def change(v):put(BANK+0x90,v);struct.pack_into('<I',expected,0x190,v)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,7);filename=file=stream=None;trace=[]
 def hook(uc,a,size,_):
  nonlocal filename,file,stream
  if a not in TARGETS.values():
   assert ENTRY<=a<ENTRY+len(code),hex(a);return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0x12345678
  if a==0x9cbfe0:
   assert cx==BANK and not trace;filename=get(esp+4);put(filename,0x10203040);result=filename;pop=4;trace.append('path')
  elif a==0x999230:
   assert cx==filename and get(cx)==0x10203040;result=exists;trace.append('exists')
   if mutation==1:change(0x11111111)
  elif a==0x99a2f0:
   assert exists;file=cx;put(file,0x129a7c4);result=file;trace.append('base-ctor')
  elif a==0x99aed0:
   assert cx==file+24 and get(file)==0x122d06c;put(cx,0);result=cx;trace.append('wide-ctor')
  elif a==0x99a6a0:
   assert cx==file and (get(esp+4),get(esp+8),get(esp+12))==(filename,1,4);assert u.mem_read(file+20,1)==b'\0' and get(file+24)==0;put(file+16,0x55667788);u.mem_write(file+20,b'\1');pop=12;trace.append('disk-open')
  elif a==0x994700:
   assert get(esp+4)==file and get(esp+8)==0x4000;stream=cx;u.mem_write(cx,b'\0'*36);put(cx+8,16);put(cx+12,DATA);put(cx+20,chunk);pop=8;result=cx;trace.append('stream-ctor')
  elif a==0x993ca0:
   assert cx==stream and get(esp+8)==4;p=get(cx+4);u.mem_write(get(esp+4),bytes(u.mem_read(DATA+p,4)));put(cx+4,p+4);put(cx+12,DATA+p+4);put(cx+20,0);pop=8;trace.append(('overflow',p))
  elif a==0x9cfbc0:
   assert cx==BANK and (get(esp+4),get(esp+8),get(esp+12))==(stream,123,456) and get(stream+4)==16;result=loaded;pop=12;trace.append('entries')
   if mutation==2:change(0x22222222)
  elif a==0x994780:assert cx==stream;trace.append('stream-dtor')
  elif a==0x99a920:
   assert cx==file and get(file)==0x122d06c and get(file+16)==0x55667788;u.mem_write(file+20,b'\0');trace.append('disk-close')
   if mutation==3:change(0x33333333)
  elif a==0x99b510:
   assert cx in (filename,file+24 if file else None);trace.append('filename-dtor' if cx==filename else 'file-wide-dtor');put(cx,0)
  elif a==0x99a300:assert cx==file;trace.append('base-dtor')
  elif a==0xbfe9f9:assert cx==0;result=uc.reg_read(UC_X86_REG_EAX);trace.append('cookie')
  else:raise AssertionError(hex(a))
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55667700+i)
 u.reg_write(UC_X86_REG_ECX,BANK);u.reg_write(UC_X86_REG_ESP,sp)
 try:u.emu_start(ENTRY,STOP,count=1000)
 except Exception as exc:raise AssertionError((args,trace,hex(u.reg_read(UC_X86_REG_EIP)),hex(u.reg_read(UC_X86_REG_ESP)))) from exc
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and all(u.reg_read(reg)==0x55667700+i for i,reg in enumerate(saved));assert bytes(u.mem_read(A,0x2000))==expected
 result=u.reg_read(UC_X86_REG_EAX)&255;assert result==int(bool(exists) and bool(magic) and bool(valid)),(args,result,trace)
 if exists:assert trace[-6:]==['stream-dtor','disk-close','file-wide-dtor','base-dtor','filename-dtor','cookie']
 else:assert trace==['path','exists','filename-dtor','cookie']
 assert ('entries' in trace)==bool(exists and magic and valid)
 return dict(args=args,result=result,trace=trace,state=bytes(expected).hex())

def compare(obj):
 c=Coff(obj);s,o=c.symbols['?ReadRecoveryFile@CBankFile@@AAE_NK@Z'];body,rels=c.sections[s];assert not o;patched=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<i',body,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(patched)==original and len(body)==679
 results=[]
 for args in itertools.product((0,1,255),(0,1),(0,1),(0,4,16),(0,1,255),range(4)):
  a=execute(original,args);z=execute(bytes(patched),args);assert a==z;results.append(a)
 controls=[('invert-validation',0x9d04c4,b"\x75",(1,1,1,16,0,0)),('skip-file-close',0x9d04d7,b"\x90"*5,(1,0,1,16,0,0)),('skip-absent-path-dtor',0x9d0610,b"\x90"*5,(0,1,1,16,0,0)),('wrong-success',0x9d05fb,b"\0",(1,1,1,16,0,0))]
 for name,address,replacement,args in controls:
  execute(original,args);changed=bytearray(original);offset=address-ENTRY;changed[offset:offset+len(replacement)]=replacement
  try:execute(bytes(changed),args)
  except AssertionError:pass
  else:raise AssertionError(name)
 return dict(status='PASS',grade='MATCH',bytes=len(body),fixups=len(rels),cases=len(results),controls=[c[0] for c in controls],object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),boundary='Full8KiB state, normal cleanup order, argument/register/stack ABI and helper mutations. File/stream/string helpers controlled; no exception unwinding claim.')
