from pathlib import Path
import sys,struct,json,itertools,hashlib
sys.path.insert(0,'tools/decomp_pipeline')
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
A=0x2000000;VECTOR=A+0x100;DATA=A+0x400;VALUE=A+0x200;STACK=0x3000000;STOP=0x4000000
SPECS=[{'name': 'runtime', 'flag': 'O2', 'symbol': '?resize@?$vector@UCRuntimeData@CBankFile@@V?$allocator@UCRuntimeData@CBankFile@@@_STL@@@_STL@@QAEXIABUCRuntimeData@CBankFile@@@Z', 'address': 10305552, 'bytes': 87, 'fixups': 2, 'targets': {'??$__copy@PAUCRuntimeData@CBankFile@@PAU12@H@_STL@@YIPAUCRuntimeData@CBankFile@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z': 10296368, '?_M_fill_insert@?$vector@UCRuntimeData@CBankFile@@V?$allocator@UCRuntimeData@CBankFile@@@_STL@@@_STL@@QAEXPAUCRuntimeData@CBankFile@@IABU34@@Z': 10304032}, 'differences': []}, {'name': 'updates', 'flag': 'O2', 'symbol': '?resize@?$vector@PAVCBankFileEntryUpdateData@@V?$allocator@PAVCBankFileEntryUpdateData@@@_STL@@@_STL@@QAEXIABQAVCBankFileEntryUpdateData@@@Z', 'address': 10305008, 'bytes': 87, 'fixups': 2, 'targets': {'_memmove': 12577510, '?_M_fill_insert@?$vector@PAVCBankFileEntryUpdateData@@V?$allocator@PAVCBankFileEntryUpdateData@@@_STL@@@_STL@@QAEXPAPAVCBankFileEntryUpdateData@@IABQAV3@@Z': 10302208}, 'differences': []}, {'name': 'checksums', 'flag': 'O1', 'symbol': '?resize@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXIABK@Z', 'address': 4606257, 'bytes': 62, 'fixups': 2, 'targets': {'?erase@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEPAKPAK0@Z': 4606319, '?_M_fill_insert@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKIABK@Z': 4606371}, 'differences': []}, {'name': 'symbols', 'flag': 'O2', 'symbol': '?resize@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEXI@Z', 'address': 4831072, 'bytes': 101, 'fixups': 5, 'targets': {'??0CCharString@@QAE@XZ': 10085552, '?erase@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEPAVCCharString@@PAV3@0@Z': 4404074, '??1CCharString@@QAE@XZ': 10087136, '?_M_fill_insert@?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEXPAVCCharString@@IABV3@@Z': 4404147}, 'differences': []}];image=EXE.read_bytes()

def execute(spec,body,args):
 old_size,new_size,mutation=args;name=spec['name'];width=12 if name=='runtime' else 4;entry=spec['address'];targets=spec['targets'];u=Uc(UC_ARCH_X86,UC_MODE_32)
 for page in {entry&~4095,STOP}|{a&~4095 for a in targets.values()}:u.mem_map(page,4096)
 for address in targets.values():u.mem_write(address,b'\xc3')
 u.mem_map(A,4096);u.mem_map(STACK,0x10000);u.mem_write(entry,body);expected=bytearray((i*37+13)&255 for i in range(4096));u.mem_write(A,bytes(expected))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 def put(a,value,track=True):
  data=struct.pack('<I',value&0xffffffff);u.mem_write(a,data)
  if track:expected[a-A:a-A+4]=data
 def expect(a,value):struct.pack_into('<I',expected,a-A,value&0xffffffff)
 put(VECTOR,DATA);put(VECTOR+4,DATA+old_size*width);put(VECTOR+8,DATA+12*width);put(VALUE,0x12345678);sp=STACK+0x8000;put(sp,STOP,False);put(sp+4,new_size,False);put(sp+8,VALUE,False);trace=[];temporary=None;effective_old=old_size+(name=='symbols' and mutation==2)
 def hook(uc,address,length,_):
  nonlocal temporary
  if address not in targets.values():assert entry<=address<entry+len(body),hex(address);return
  esp=uc.reg_read(UC_X86_REG_ESP);cx=uc.reg_read(UC_X86_REG_ECX);dx=uc.reg_read(UC_X86_REG_EDX);pop=0;result=0x87654321
  if address==0x99e4b0:
   assert name=='symbols' and not trace;temporary=cx;put(cx,0x11223344,False);result=cx;trace.append('construct')
   if mutation==2:put(VECTOR+4,DATA+effective_old*width)
  elif address==0x99eae0:
   assert name=='symbols' and cx==temporary and get(cx)==0x11223344;trace.append('destroy')
   if mutation==3:put(VECTOR+8,A+0x900)
  elif address==0x9d1c30:
   assert name=='runtime' and new_size<effective_old and cx==DATA+old_size*width and dx==cx and get(esp+4)==DATA+new_size*width and get(esp+12)==0
   result=DATA+(new_size+(mutation==4))*width;expect(VECTOR+4,result);pop=12;trace.append('copy-empty-tail')
  elif address in (0x46496f,0x43336a):
   assert cx==VECTOR and new_size<effective_old and get(esp+4)==DATA+new_size*width and get(esp+8)==DATA+effective_old*width
   result=DATA+new_size*width;put(VECTOR+4,result);pop=8;trace.append('erase-tail')
  elif address in (0x9d3a20,0x9d3300,0x4649a3,0x4333b3):
   assert cx==VECTOR and new_size>=effective_old and get(esp+4)==DATA+effective_old*width and get(esp+8)==new_size-effective_old
   assert get(esp+12)==(temporary if name=='symbols' else VALUE)
   put(VECTOR+4,DATA+(new_size+(mutation==1))*width);put(A+0x800,0x11220000+new_size);pop=12;trace.append('fill-tail')
  else:raise AssertionError(('unexpected helper',hex(address)))
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0xdeadf00d);uc.reg_write(UC_X86_REG_EIP,get(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
 if name=='updates' and new_size<old_size:expect(VECTOR+4,DATA+new_size*width)
 u.hook_add(UC_HOOK_CODE,hook);saved=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
 for n,reg in enumerate(saved):u.reg_write(reg,0x55660000+n)
 u.reg_write(UC_X86_REG_ECX,VECTOR);u.reg_write(UC_X86_REG_ESP,sp)
 try:u.emu_start(entry,STOP,count=1000)
 except Exception as exc:raise AssertionError((name,args,trace,hex(u.reg_read(UC_X86_REG_EIP)))) from exc
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+(8 if name=='symbols' else 12) and all(u.reg_read(reg)==0x55660000+n for n,reg in enumerate(saved))
 assert bytes(u.mem_read(A,4096))==expected,(name,args,trace,'state')
 if name=='symbols':assert trace[0]=='construct' and trace[-1]=='destroy'
 assert ('fill-tail' in trace)==(new_size>=effective_old)
 return dict(args=args,trace=trace,state_sha256=hashlib.sha256(expected).hexdigest())
def compare(b):
 results=[]
 for spec in SPECS:
  c=Coff(b/(spec['name']+'.obj'));s,o=c.symbols[spec['symbol']];body,rels=c.sections[s];assert not o;fixed=bytearray(body)
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(spec['targets'][name]+struct.unpack_from('<i',body,site)[0]-(spec['address']+site+4 if kind==20 else 0))&0xffffffff)
  original=read_virtual_data(image,spec['address'],spec['bytes']);assert bytes(fixed)==original
  for args in itertools.product((0,1,3,7),(0,1,3,7,9),range(5)):
   a=execute(spec,original,args);z=execute(spec,bytes(fixed),args);assert a==z;results.append(dict(name=spec['name'],**a))
 return dict(status='PASS',cases=len(results),boundary='Four original-matched resize roots, full4KiB state/ABI and helper mutation cases. Helpers controlled in differential model.')

def oracle_controls(b):
 results=[]
 for spec in SPECS:
  original=read_virtual_data(image,spec['address'],spec['bytes'])
  address={'runtime':0x9d4033,'updates':0x9d3e06,'checksums':0x464945,'symbols':0x49b781}[spec['name']]
  probes=[('branch',address-spec['address'],b'\x72',(3,1,0))]
  from gate_game_component_abi import Coff
  c=Coff(b/(spec['name']+'.obj'));s,o=c.symbols[spec['symbol']]
  fill=next(site for site,(name,kind) in c.sections[s][1].items() if name.startswith('?_M_fill_insert'))
  probes.append(('fill',fill-1,b'\x90'*5,(1,3,0)))
  if spec['name']=='symbols':probes.append(('cleanup',89,b'\x90'*5,(1,3,0)))
  if spec['name']=='runtime':probes.append(('finish-offset',0x9d404d-spec['address'],b'\x08',(3,1,0)))
  for label,offset,replacement,args in probes:
   execute(spec,original,args);changed=bytearray(original);changed[offset:offset+len(replacement)]=replacement
   try:execute(spec,bytes(changed),args)
   except AssertionError as exc:results.append(dict(name=spec['name']+'-'+label,detected=True,detail=str(exc)))
   else:raise AssertionError((spec['name'],label))
 return results
