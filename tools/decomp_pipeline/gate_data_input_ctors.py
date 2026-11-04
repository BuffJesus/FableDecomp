"""Validate the DataInputStream unit's memory-stream members (CDataInputStream(), OpenMemorySource,
CMemoryDataInputStream(data, size), CMemoryDataInputStream(const CCharString&)), which share one unit
so VC7.1 expands the base constructor and OpenMemorySource into the constructors as retail does."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from near_match import compare, retail_relocations
from memory_stream_copy_oracle import compare_streams
from gate_game_component_abi import Coff
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
UNIT='00/99/CDataInputStream_CDataInputStream_00993b20.cpp'
SPECS=[('00993b20','??0CDataInputStream@@QAE@XZ',35),('00994570','?OpenMemorySource@CMemoryDataInputStream@@QAEXPBXK@Z',0),
       ('00994930','??0CMemoryDataInputStream@@QAE@PBXK@Z',48),('00995f50','??0CMemoryDataInputStream@@QAE@ABVCCharString@@@Z',85),
       ('00995ee0','??0CMemoryDataInputStream@@QAE@ABVCMemoryDataOutputStream@@@Z',110),
       ('00995eb0','?Write@CDataOutputStream@@QAEXABVCMemoryDataOutputStream@@@Z',42),
       ('00994b40','?GetLength@CMemoryDataOutputStream@@QBEKXZ',7),
       ('00995e70','?PeekData@CMemoryDataOutputStream@@QBEPBXXZ',15),
       ('00996cc0','?Write@CMemoryDataOutputStream@@UAEXPBXJ@Z',129)]
EXTERNAL={'?resize@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXIABD@Z':0x4122d0,'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CDataInputStream@@6B@':0x129a6b0,'??_7CMemoryDataInputStream@@6B@':0x123675c,
          '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4}
OBJ=0x2000000;DATA=0x2001000;STR=0x2002000;STRDATA=0x2002100;STACK=0x3000000;STOP=0x4000000;BASE=0x99a2f0
def run(code,addr,args,ecx=OBJ,setup=None):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,z in [(addr&~0xfff,0x2000),(0x99a000,0x1000),(OBJ,0x4000),(STACK,0x10000),(STOP,0x1000)]:u.mem_map(a,z)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 u.mem_write(addr,bytes(code));u.mem_write(BASE,b'\xc3');u.mem_write(OBJ,b'\xa5'*0x40)
 if setup:setup(put)
 trace=[]
 def hook(uc,a,s,user):
  if a!=BASE:return
  sp=uc.reg_read(UC_X86_REG_ESP);trace.append(('base',uc.reg_read(UC_X86_REG_ECX)));uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP)
 for i,v in enumerate(args):put(sp+4+4*i,v)
 regs=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
 for i,r in enumerate(regs):u.reg_write(r,0x778800+i)
 u.reg_write(UC_X86_REG_ECX,ecx);u.reg_write(UC_X86_REG_ESP,sp)
 u.emu_start(addr,STOP,count=500)
 assert u.reg_read(UC_X86_REG_ESP)==sp+4+4*len(args) and all(u.reg_read(r)==0x778800+i for i,r in enumerate(regs))
 return u.reg_read(UC_X86_REG_EAX),trace,[get(OBJ+4*k) for k in range(8)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled'/UNIT);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/99/CDataInputStream_CDataInputStream_00993b20_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/data-input-ctors.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='data-input-ctors-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE']
 obj=w/'unit.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,w);assert not r.returncode,r.stdout;c=Coff(obj)
 targets={s:int(addr,16) for addr,s,_ in SPECS};targets.update(EXTERNAL);code={};entries=[];cases=0
 with pefile.PE(str(EXE)) as pe:
  for addr,sym,size in SPECS:
   sec,off=c.symbols[sym];b,rel=c.sections[sec];assert off==0 and (size==0 or len(b)==size),(sym,len(b));patched=bytearray(b);a0=int(addr,16)
   for pos,(name,kind) in rel.items():
    assert kind in (6,20);t=targets[name];struct.pack_into('<I',patched,pos,(t-a0-pos-4 if kind==20 else t)&0xffffffff)
   raw=pe.get_data(a0-0x400000,len(b));score=compare(bytes(patched),raw,tuple(rel),retail_relocations(addr,len(raw)));assert patched==raw or (addr=='00995ee0' and score.near_match),(sym,score);code[addr]=(patched,raw)
   entries.append(dict(address=addr,symbol=sym,bytes=len(b),grade=('RELOCATION_MATCH' if rel else 'MATCH') if patched==raw else 'NEAR_MATCH',similarity=score.to_dict()))
 Z=0xa5a5a5a5
 for data,size in [(DATA,0),(DATA,9),(DATA+3,0x7fffffff),(0,5)]:
  cand=run(code['00994570'][0],0x994570,[data,size]);assert cand==run(code['00994570'][1],0x994570,[data,size])
  _,trace,f=cand;assert not trace and f==[Z,0,size,data,0,size,data,Z];cases+=1
  cand=run(code['00994930'][0],0x994930,[data,size]);assert cand==run(code['00994930'][1],0x994930,[data,size])
  eax,trace,f=cand;assert eax==OBJ and trace==[('base',OBJ)] and f==[0x123675c,0,size,data,0,size,data,Z];cases+=1
 cand=run(code['00993b20'][0],0x993b20,[]);assert cand==run(code['00993b20'][1],0x993b20,[])
 eax,trace,f=cand;assert eax==OBJ and trace==[('base',OBJ)] and f==[0x129a6b0,0,0,0,0,0,Z,Z];cases+=1
 for storage,length in [(STRDATA,8),(STRDATA,0),(0,0)]:
  def setup(put,storage=storage,length=length):
   put(STR,storage);put(STRDATA,DATA+0x40);put(STRDATA+4,length)
  args=[STR]
  cand=run(code['00995f50'][0],0x995f50,args,setup=setup);assert cand==run(code['00995f50'][1],0x995f50,args,setup=setup)
  eax,trace,f=cand;text=DATA+0x40 if storage else 0x129aaf4;n=length if storage else 0
  assert eax==OBJ and trace==[('base',OBJ)] and f==[0x123675c,0,n,text,0,n,text,Z],(f,);cases+=1
 cases+=compare_streams(code)
 tobj=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(tobj),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout
 exe=w/'f.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),str(tobj)]
 r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode,'fixture linked without the unit'
 r=subprocess.run(cmd+[str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'FABLETLC_DATAINPUTSTREAM_CTORS_00993B20_PASS' in r.stdout,(r.returncode,r.stdout)
 report=dict(entries=entries,comparative_scenarios=cases,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest())
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n')
 print('DATA_INPUT_CTORS_PASS entries=%d comparative=%d'%(len(entries),cases))
if __name__=='__main__':main()
