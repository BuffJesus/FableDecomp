"""Typed pixel reference-count getter, including aliased receiver/data storage."""
import itertools,json,struct,subprocess,tempfile
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
from pe_oracle import EXE,pe_sections,va_to_off
import check_one as V
from variable_allocator_parity import check
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-pixel-references-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/98/CPixelShader_CountReferences_00987ee0.cpp';fixture=ROOT/'rebuild/tests/00/98/CPixelShader_CountReferences_00987ee0_test.cpp'
objects=[]
for i,path in enumerate([source,fixture]):
 obj=w/(str(i)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
proof=check(objects[0],'?CountReferences@CPixelShader@@EAEKXZ',0x987ee0,7,0,{})
exe=w/'fixture.exe';cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x987000,0x1000);u.mem_map(0x24000000,0x10000);raw=EXE.read_bytes();o=va_to_off(pe_sections(raw),0x987ee0);u.mem_write(0x987ee0,raw[o:o+7])
inputs=bytearray();expected=bytearray();cases=0
for fill,refs,offset in itertools.product(range(256),[0,1,0xffffffff,0x12345678],[0,32]):
 arena=bytearray([fill])*64;struct.pack_into('<I',arena,4,0x24000000+offset);struct.pack_into('<I',arena,offset+8,refs);inputs.extend(arena);u.mem_write(0x24000000,bytes(arena));u.mem_write(0x2400e000,struct.pack('<I',0x2400f000));u.reg_write(UC_X86_REG_ESP,0x2400e000);u.reg_write(UC_X86_REG_ECX,0x24000000);u.emu_start(0x987ee0,0x2400f000,count=10)
 assert u.reg_read(UC_X86_REG_ESP)==0x2400e004;expected.extend(struct.pack('<I',u.reg_read(UC_X86_REG_EAX)));expected.extend(u.mem_read(0x24000000,64));cases+=1
(w/'input.bin').write_bytes(inputs);r=subprocess.run([str(exe),str(w/'input.bin'),str(w/'actual.bin')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(r.stdout,r.stderr);assert (w/'actual.bin').read_bytes()==expected
report=dict(cases=cases,snapshots=cases,omission_checks=1,providers=[proof],boundary='Actual seven-byte retail getter versus compiled typed provider, full64-byte state unchanged, all byte fills, four reference values and aliased receiver/data storage. Private virtual slot is separately audited by the shader-manager constructor gate; no null-data fallback invented.')
(ROOT/'rebuild/compile-gate/native-pixel-references.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip())
