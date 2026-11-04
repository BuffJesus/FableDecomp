"""Retail shader default state: complete output bytes, aliasing and padding."""
import hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,pe_sections,va_to_off
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='shader-bank-default-'));env=V.env();objects=[]
for i,name in enumerate(['rebuild/src/compiled/00/a6/CShaderBankStateBlock_GetDefault_00a61f20.cpp','rebuild/tests/00/a6/CShaderBankStateBlock_GetDefault_00a61f20_test.cpp']):
 obj=w/('%d.obj'%i);r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/name)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'?GetDefault@CShaderBankStateBlock@@UAEXAAVCBankStateBlock@@@Z',0xa61f20,27,0,{})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode,'provider required'
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(exe),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,r.stdout+r.stderr
u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0xa61000,0x2000);u.mem_map(0x2000000,0x10000);raw=EXE.read_bytes();offset=va_to_off(pe_sections(raw),0xa61f20);u.mem_write(0xa61f20,raw[offset:offset+27]);expected=bytearray();cases=0
for fill in range(256):
 for output in [0,28,32,64]:
  u.mem_write(0x2000000,bytes([fill])*128);u.mem_write(0x2008000,struct.pack('<II',0x200f000,0x2000000+output));u.reg_write(UC_X86_REG_ESP,0x2008000);u.reg_write(UC_X86_REG_ECX,0x2000020)
  u.emu_start(0xa61f20,0x200f000,count=100);assert u.reg_read(UC_X86_REG_EIP)==0x200f000 and u.reg_read(UC_X86_REG_ESP)==0x2008008
  result=bytes(u.mem_read(0x2000000,128));semantic=bytearray([fill]*128);semantic[output+4:output+12]=b'\0'*8;semantic[output+8]=1;assert result==semantic
  expected.extend(result);cases+=1
assert (w/'actual.bin').read_bytes()==expected
report=dict(providers=[proof],cases=cases,compared_bytes=len(expected),omission_checks=1,boundary='Actual typed public virtual method called qualified; full128-byte output across all256 fills and four separate/overlapping receiver/output placements. State padding zeroed, output vtable and unrelated bytes preserved.')
(ROOT/'rebuild/compile-gate/shader-bank-default.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
