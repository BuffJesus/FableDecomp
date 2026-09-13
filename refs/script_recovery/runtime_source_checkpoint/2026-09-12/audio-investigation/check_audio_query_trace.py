"""Compile and execute the actual diagnostic hook against native branch behavior."""
import hashlib,json,struct,subprocess,sys,shutil
from pathlib import Path
sys.path.insert(0,str(Path('.').resolve()))
sys.path.insert(0,str(Path('work/runtime_re_tools').resolve()))
import pefile,capstone
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.run_meet_sister_runtime_checks import compiler_environment,find_vcvars

out=Path(sys.argv[1]).resolve();out.mkdir(exist_ok=False)
header=Path(__file__).with_name('NoviAudioQueryTrace.inl').resolve()
src=out/'fixture.cpp'
src.write_text('#include <windows.h>\n#include <cstdio>\n#include <cstring>\ntemplate<class T> T ASLR(DWORD p){return T(p);}\nvoid LogToFile(const char*){}\n#include "'+header.as_posix()+'"\nint main(){printf("%lu %lu %lu %lu",(DWORD)&NoviAudioQueryTrace,(DWORD)&NoviRecordAudioQuery,(DWORD)&g_noviAudioQuerySuccess,(DWORD)&g_noviAudioQueryFailure);}\n')
env=compiler_environment(find_vcvars());compiler=shutil.which('cl.exe',path=env['PATH'])
binary=out/'fixture.exe'
subprocess.run([compiler,'/nologo','/EHsc',str(src),'/Fe:'+str(binary),'/Fo:'+str(out/'fixture.obj'),'/link','/BASE:0x3000000','/DYNAMICBASE:NO','/FIXED'],cwd=out,env=env,check=True)
hook,logger,success,failure=map(int,subprocess.check_output([str(binary)],text=True).split())
if len(sys.argv)>2:
 binary=Path(sys.argv[2]).resolve()
 actual=pefile.PE(str(binary));base=actual.OPTIONAL_HEADER.ImageBase
 exports={s.name.decode():base+s.address for s in actual.DIRECTORY_ENTRY_EXPORT.symbols if s.name}
 hook=exports['NoviAudioQueryTrace'];logger=exports['NoviRecordAudioQuery']
 dis=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);dis.detail=True
 jumps=[]
 for ins in dis.disasm(actual.get_memory_mapped_image()[hook-base:hook-base+160],hook):
  if ins.mnemonic=='jmp' and ins.operands[0].type==capstone.CS_OP_MEM:
   jumps.append(ins.operands[0].mem.disp)
   if len(jumps)==2:break
 assert len(jumps)==2
 success,failure=jumps
exe=Path('C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe')
digest=hashlib.sha256(exe.read_bytes()).hexdigest()
assert digest=='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10'
native=pefile.PE(str(exe)).get_memory_mapped_image();fixture=pefile.PE(str(binary)).get_memory_mapped_image()
regs=[UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP,UC_X86_REG_ESP,UC_X86_REG_EFLAGS,UC_X86_REG_XMM0]
cases=[]
for hr in [0,1,0x80004005,0xffffffff]:
 for flags in [0x202,0x646]:
  states=[];calls=[]
  for use_hook in [False,True]:
   u=Uc(UC_ARCH_X86,UC_MODE_32)
   for base,im in [(0x400000,native),(pefile.PE(str(binary)).OPTIONAL_HEADER.ImageBase,fixture)]:
    u.mem_map(base,(len(im)+4095)&~4095);u.mem_write(base,im)
   u.mem_map(0x4000000,0x10000)
   def put(a,n):u.mem_write(a,struct.pack('<I',n))
   def word(a):return struct.unpack('<I',u.mem_read(a,4))[0]
   put(success,0xC53371);put(failure,0xC53359)
   for i,r in enumerate(regs[:-1]):u.reg_write(r,0x100+i)
   u.reg_write(UC_X86_REG_EAX,hr);u.reg_write(UC_X86_REG_ESI,0x4001000)
   u.reg_write(UC_X86_REG_ESP,0x4008000);u.reg_write(UC_X86_REG_EFLAGS,flags)
   u.reg_write(UC_X86_REG_XMM0,0x123456789abcdef)
   put(0x4001008,0x11223344);put(0x4008004,0x140004)
   # Initialize x87 and put one value on its stack; hook logger destroys it.
   u.mem_write(0x4009000,b'\xdb\xe3\xd9\xe8');u.emu_start(0x4009000,0x4009004)
   def observe(machine,address,size,data):
    if address==logger:
     sp=machine.reg_read(UC_X86_REG_ESP)
     calls.append([word(sp+4),word(sp+8),word(sp+12)])
     for r in regs[:7]:machine.reg_write(r,0xdeadbeef)
     machine.reg_write(UC_X86_REG_XMM0,0)
     # Replace callback with FNINIT; RET, preserving its cdecl stack contract.
   u.mem_write(logger,b'\xdb\xe3\xc3');u.hook_add(UC_HOOK_CODE,observe)
   stop=0xC53359 if hr&0x80000000 else 0xC53371
   u.emu_start(hook if use_hook else 0xC53352,stop,count=1000)
   state=[u.reg_read(r) for r in regs]
   # Snapshot x87/SIMD result without changing it.
   u.mem_write(0x4009000,b'\x0f\xae\x05'+struct.pack('<I',0x400a000));u.emu_start(0x4009000,0x4009007)
   fx=bytearray(u.mem_read(0x400a000,512))
   # Unicorn does not restore the saved x87 instruction-pointer metadata after
   # this FNINIT/FXRSTOR sequence. Compare control/status, stack and SIMD data;
   # do not call this a proof of preserved FIP metadata on hardware.
   fx[8:16]=b'\0'*8
   state.append(bytes(fx))
   states.append(state)
  if states[0]!=states[1]:
   print('registers',states[0][:-1],states[1][:-1])
   print('fx differences',[(i,a,b) for i,(a,b) in enumerate(zip(states[0][-1],states[1][-1])) if a!=b])
  assert states[0]==states[1],(hr,flags,'machine state mismatch')
  assert calls==[[hr,0x140004,0x4001000]],calls
  cases.append(dict(hr=hr,flags=flags,passed=True))
(out/'proof.json').write_text(json.dumps(dict(exe_sha256=digest,tested_binary=str(binary),tested_binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),header_sha256=hashlib.sha256(header.read_bytes()).hexdigest(),cases=cases,passed=True,limits='Logger body doubled; real logger compiled. FXSAVE instruction-pointer metadata bytes8..15 excluded due emulator behavior. No gameplay execution or audible fix.'),indent=2))
print(len(cases),'native hook machine-state cases passed')
