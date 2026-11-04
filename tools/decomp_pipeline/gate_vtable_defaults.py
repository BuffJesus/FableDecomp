"""Validate a batch of trivial virtual defaults re-landed as genuine members (vtable_defaults.py).

    python tools/decomp_pipeline/gate_vtable_defaults.py --batch 1

Reads rebuild/compile-gate/vtable-defaults-<N>.spec.json. Each provider must define its Ego_r
decorated symbol with no relocations and the retail bytes; a Unicorn run of the retail and
candidate bytes must agree and pop the arguments, preserve the callee-saved registers, write
nothing, and return the spec's constant; the compiled fixture must pass; and omitting any one
provider must break the fixture's link.
"""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
OBJ=0x2000000;STACK=0x3000000;STOP=0x4000000
def run(code,addr,argc,seed):
 u=Uc(UC_ARCH_X86,UC_MODE_32);page=addr&~0xfff
 for a,z in [(page,0x2000),(OBJ,0x1000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 u.mem_write(addr,bytes(code));u.mem_write(OBJ,bytes((i*7+seed)&0xff for i in range(0x1000)))
 sp=STACK+0x8000;args=b''.join(struct.pack('<I',(0x1000*seed+i)*0x10001&0xffffffff) for i in range(argc))+b'\x5a'*16
 u.mem_write(sp,struct.pack('<I',STOP)+args)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,OBJ);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EAX,0xdeadbeef)
 u.emu_start(addr,STOP,count=100)
 return (u.reg_read(UC_X86_REG_ESP)-sp,[u.reg_read(r) for r in regs],u.reg_read(UC_X86_REG_EAX),
         bytes(u.mem_read(OBJ,0x1000))==bytes((i*7+seed)&0xff for i in range(0x1000)),bytes(u.mem_read(sp+4,len(args)))==args)
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--batch',required=True);ap.add_argument('--spec',type=Path);ap.add_argument('--source-dir',type=Path)
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path);ap.add_argument('--output',type=Path);a=ap.parse_args()
 spec_path=a.spec or ROOT/('rebuild/compile-gate/vtable-defaults-%s.spec.json'%a.batch);spec=json.loads(spec_path.read_text())
 test=a.test or ROOT/('rebuild/tests/integration/VtableDefaults%s_test.cpp'%a.batch);out=a.output or ROOT/('rebuild/compile-gate/vtable-defaults-%s.json'%a.batch)
 w=Path(tempfile.mkdtemp(prefix='vtable-defaults-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objects=[];providers=[];entries=[];cases=0
 def compile(source,name):
  obj=w/name;r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;return obj
 with pefile.PE(str(EXE)) as pe:
  for i,s in enumerate(spec):
   source=a.source_dir/s['file'] if a.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file']
   obj=compile(source,'p%d.obj'%i);c=Coff(obj);objects.append(obj);providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
   sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];addr=int(s['address'],16)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert off==0 and not rel and len(b)==s['bytes'] and bytes(b)==raw,s['file']
   for seed in range(3):
    got=run(b,addr,s['pop']//4,seed);assert got==run(raw,addr,s['pop']//4,seed),s['file']
    pop,regs,eax,obj_ok,args_ok=got
    assert pop==4+s['pop'] and regs==[0x778800+k for k in range(4)] and obj_ok and args_ok,(s['file'],got)
    if s['kind']=='zero':assert eax==0
    elif s['kind']=='bool':assert eax&0xff==s['value']&0xff
    elif s['kind']=='const':assert eax==s['value']&0xffffffff
    else:assert eax==0xdeadbeef
    cases+=1
   entries.append(dict(s,grade='MATCH'))
 tobj=compile(test,'test.obj');tc=Coff(tobj);assert all(s['symbol'] not in tc.symbols for s in spec)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),str(tobj)]
 for omit in range(len(spec)):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('provider omission linked',spec[omit]['file'])
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);want='VTABLE_DEFAULTS_BEHAVIOR_PASS scenarios=%d'%(3*len(spec))
 assert not r.returncode and want in r.stdout,(r.returncode,r.stdout[-500:])
 report=dict(entries=entries,comparative_scenarios=cases,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(test.read_bytes()).hexdigest(),
  spec_sha256=hashlib.sha256(spec_path.read_bytes()).hexdigest(),missing_provider_checks=len(spec))
 out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n')
 print('VTABLE_DEFAULTS_%s_PASS compiled=%d comparative=%d providers=%d'%(a.batch,3*len(spec),cases,len(spec)))
if __name__=='__main__':main()
