"""Write-open publication, independent write sort-key allocation and string cleanup ordering."""
import hashlib,itertools,re,struct,tempfile
from pathlib import Path
from movable_pool_construct_oracle import compile_codes
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[2]
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFile_Open_0098e1e0.cpp'
ROOTS=[dict(name='open_for_write',source=SOURCE,symbol='?OpenForWrite@CThreadedFile@@QAE_NABVCWideString@@_N@Z',address=0x98e000,size=143,instructions=44,relocations=8)]
ADDRESS={'?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z':0x999110,'??BCWideString@@QBEPBGXZ':0x99af00,'??1CWideString@@QAE@XZ':0x99b510,'__imp__CreateFileW@28':0x143fe2c,'__imp__GetFileSize@8':0x143fdf0,'_s_NextPhysicalSortKey':0x13bc9e8}
ENTRY=0x98e000
ARENA,OBJ,NAME,PATH,STACK,STOP,API,GLOBAL=0x2000000,0x2000103,0x2000300,0x2000400,0x3000000,0x4000000,0x5000000,0x13bc000

def execute(codes,args):
 overwrite,handle,file_size,counter,mutation,seed=args
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x98e000,0x1000),(0x999000,0x1000),(0x99a000,0x1000),(0x99b000,0x1000),(0x143f000,0x1000),(ARENA,0x4000),(GLOBAL,0x1000),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):u.mem_map(a,n)
 for a,b in codes.items():u.mem_write(a,b)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 for a,n in ((ARENA,0x4000),(GLOBAL,0x1000)):u.mem_write(a,bytes((i*37+seed*19)&255 for i in range(n)))
 put(GLOBAL+0x9e8,counter);expected={a:bytearray(u.mem_read(a,n)) for a,n in ((ARENA,0x4000),(GLOBAL,0x1000))}
 def model(a,v):
  base=ARENA if a>=ARENA and a<ARENA+0x4000 else GLOBAL
  struct.pack_into('<I',expected[base],a-base,v&0xffffffff)
 def change(a,v):put(a,v);model(a,v)
 def model_flags():expected[ARENA][OBJ+0x18-ARENA:OBJ+0x1a-ARENA]=b'\x01\x01'
 def verify():assert all(bytes(u.mem_read(a,len(b)))==bytes(b) for a,b in expected.items())
 put(0x143fe2c,API);put(0x143fdf0,API+16)
 sp=STACK+0x8000;put(sp,STOP);put(sp+4,NAME);put(sp+8,overwrite)
 trace=[];local=sp+4
 def hook(uc,a,size,_):
  if a not in (0x999110,0x99af00,0x99b510,API,API+16):return
  esp=uc.reg_read(UC_X86_REG_ESP);ecx=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xbad00000
  if a==0x999110:
   assert not trace and ecx==local and uc.reg_read(UC_X86_REG_EDX)==NAME
   verify();trace.append(('convert',NAME));put(local,0x13579bdf);result=local
   if mutation==1:change(OBJ+0x10,0xcafe0010);change(OBJ+0x14,0xcafe0014);change(GLOBAL+0x9e8,counter^0x80000000)
   # After conversion the method clears device before converting to wchar*.
   model(OBJ+4,0)
  elif a==0x99af00:
   assert trace==[('convert',NAME)] and ecx==local and get(local)==0x13579bdf
   verify();trace.append(('wide_pointer',));result=PATH
  elif a==API:
   assert trace==[('convert',NAME),('wide_pointer',)]
   assert tuple(get(esp+4+i*4) for i in range(7))==(PATH,0x40000000,0,0,2 if overwrite else 4,0x68000000,0)
   verify();trace.append(('create',handle));pop=28;result=handle
   if mutation==2:change(OBJ+4,0x11223344);change(OBJ+0x10,0xcafe1010);change(OBJ+0x14,0xcafe1014)
   model(OBJ+8,handle)
  elif a==API+16:
   assert handle!=0xffffffff and len(trace)==3 and (get(esp+4),get(esp+8))==(handle,0)
   verify();trace.append(('size',file_size));pop=8;result=file_size
   if mutation==2:change(OBJ+8,0xeeeeeeee);change(GLOBAL+0x9e8,counter^0xffffffff)
   next_key=(get(GLOBAL+0x9e8)+1)&0xffffffff
   model(OBJ+0x10,file_size);model(GLOBAL+0x9e8,next_key);model(OBJ+0x14,next_key);model_flags()
  else:
   assert ecx==local and get(local)==0x13579bdf and len(trace)==(3 if handle==0xffffffff else 4)
   verify();trace.append(('destroy',))
   if mutation==3:
    for off in (4,8,0x10,0x14):change(OBJ+off,0xfedc0000+off)
    u.mem_write(OBJ+0x18,b'\x7e\x81');expected[ARENA][OBJ+0x18-ARENA:OBJ+0x1a-ARENA]=b'\x7e\x81'
   put(local,0)
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 u.hook_add(UC_HOOK_CODE,hook)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+seed+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(ENTRY,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
 assert all(u.reg_read(reg)==0x55660000+seed+i for i,reg in enumerate(saved))
 result=u.reg_read(UC_X86_REG_EAX)&255;assert result==int(handle!=0xffffffff)
 assert len(trace)==(4 if handle==0xffffffff else 5) and trace[-1]==('destroy',);verify()
 return result,trace,{a:bytes(b) for a,b in expected.items()}

def compare_write_open():
 inputs={Path(__file__),EXE,R/'tools/decomp_pipeline/movable_pool_construct_oracle.py'}
 def closure(p):
  if p in inputs:return
  inputs.add(p)
  for n in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):closure(next(q for q in (p.parent/n,R/'rebuild/include'/n) if q.is_file()))
 closure(SOURCE);pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 with tempfile.TemporaryDirectory(prefix='threaded-write-open-') as tmp:
  built=compile_codes(ROOTS,ADDRESS,Path(tmp));coff=Coff(Path(tmp)/'provider0.obj')
  counters=[];locations=[]
  for symbol,local,address,entry in ((ROOTS[0]['symbol'],'_s_NextPhysicalSortKey',0x13bc9e8,ENTRY),('?Open@CThreadedFile@@QAE_NABVCWideString@@_N@Z','_s_NextReadPhysicalSortKey',0x13bc9ec,0x98e1e0)):
   sec,offset=coff.symbols[local];body,rels=coff.sections[sec];metadata=coff.section_metadata[sec]
   assert body==b'' and not rels and offset+4<=metadata['raw_size'] and metadata['raw_pointer']==0 and metadata['characteristics']&0x80
   assert read_virtual_data(EXE.read_bytes(),address,4)==b'\0'*4;locations.append((sec,offset))
   code_sec,_=coff.symbols[symbol];refs=[site for site,(target,kind) in coff.sections[code_sec][1].items() if target==local]
   assert len(refs)==2
   assert all(struct.unpack('<I',read_virtual_data(EXE.read_bytes(),entry+site,4))[0]==address for site in refs)
   counters.append(dict(address=hex(address),bytes=4,zero_bss=True,local_source_name=local,user=symbol,sites=refs))
  assert locations[0]!=locations[1]
 original={ENTRY:read_virtual_data(EXE.read_bytes(),ENTRY,143)};assert built==original;cases=0
 for args in itertools.product((0,1,2,255),(0xffffffff,0,0x12345678),(0,17,0xffffffff),(0,0x7fffffff,0xffffffff),range(4),(0,7)):
  assert execute(built,args)==execute(original,args);cases+=1
 for bad in (b'\xcc',b'\x32\xc0\xc2\x08\x00'):
  detected=False
  try:execute({ENTRY:bad},(0,0xffffffff,0,0,0,0))
  except Exception:detected=True
  assert detected
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 return dict(status='PASS',cases=cases,missing_provider_checks=1,root_controls=2,input_sha256=pins,independent_counters=counters,
  boundaries=['Path conversion, wchar conversion, destruction and two Win32 APIs controlled; no actual opening or engine owner.',
  'Full20KiB storage guards, unsigned write-counter wrap/mutations, raw bool bytes, success/null/failure handles, cleanup before return and retained filename; no exception unwinding contract.'])
