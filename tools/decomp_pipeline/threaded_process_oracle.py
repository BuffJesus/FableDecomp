"""Worker differential oracle; native helper/API boundaries remain explicit."""
import sys,struct,itertools,json,math,re,tempfile,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY,SIZE=0x98ecd0,0x312
SOURCE=R/'rebuild/src/compiled/00/98/CThreadedFileManager_ProcessObjects_0098ecd0.cpp'
SYMBOL='?ProcessObjects@CThreadedFileManager@@AAEXXZ'
ARENA,LENGTH,M=0x2000000,0x8000,0x2000100
OBJ,FILE,OVER,OLD=ARENA+0x2000,ARENA+0x3000,ARENA+0x4000,ARENA+0x4800
API,STACK,STOP=0x5000000,0x3000000,0x4000000
ADDRESS={
 '__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10,
 '__imp__WaitForSingleObjectEx@12':0x143fe08,'__imp__WriteFileEx@20':0x143fdd8,
 '__imp__ReadFileEx@20':0x143fdf4,'__imp__SetEvent@4':0x143fde0,'__imp__SleepEx@8':0x143fddc,
 '?OnFinishObject@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z':0x98ebc0,
 '?RemoveObjectFromPendingMap@CThreadedFileManager@@AAEXPAVCFileObject@1@@Z':0x98e770,
 '?GFGetTime@@YINXZ':0x9e1bc0,
 '?g_CFileInstallerSingleton_013CA818@@3UCCountedFileInstaller@@A':0x13ca818,
 '?Get@CFileInstallerSingleton@@SIAAVCFileInstaller@@XZ':0x404440,
 '?Pause@CFileInstaller@@QAEXXZ':0x9c31e0,'?Unpause@CFileInstaller@@QAEXXZ':0x9c3250,
 '?OnFileReadCompletion@CThreadedFileManager@@CGXKKPAU_OVERLAPPED@@@Z':0x98e320,
 '?m_harddiskErrorCallback@CThreadedFileManager@@0P6IXPBD@ZA':0x13bc864,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
def built(source=SOURCE):
 import check_one as V
 from near_match import compare,retail_relocations
 from native_exact_roots import import_names
 with tempfile.TemporaryDirectory(prefix='threaded-process-') as tmp:
  obj=Path(tmp)/'root.obj'
  result=V.cl(['/nologo','/c','/O2','/MT','/W3','/Fo'+str(obj),str(source)],V.env(),Path(tmp))
  assert result.returncode==0,result.stdout+result.stderr
  c=Coff(obj);section,offset=c.symbols[SYMBOL];assert offset==0
  body,rels=c.sections[section]
  raw=EXE.read_bytes();original=read_virtual_data(raw,ENTRY,SIZE)
  assert len(body)==792 and len(rels)==28
  grade=compare(body,original,tuple(rels),retail_relocations('0098ecd0',SIZE)).to_dict()
  assert grade['near_match'] and grade['opcode_similarity']>=.90
  imports=import_names(raw);b=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20)
   target=ADDRESS[name]
   add=struct.unpack_from('<i',body,site)[0];assert add==0
   if name.startswith('__imp__'):assert imports[target]==name[7:].split('@')[0]
   if name.startswith('??_C@'):
    sec,off=c.symbols[name];assert c.sections[sec][0][off:]==b'\0'
    assert read_virtual_data(raw,target,1)==b'\0'
   struct.pack_into('<I',b,site,(target-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
  return bytes(b),grade

def execute(code,case):
 priority,device,cursor,expired,mode,callback,current,blocked,installer,submit,error,seed=case
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for a,n in ((0x980000,0x70000),(0x404000,0x1000),(0x122d000,0x1000),(0x13bc000,0x1000),(0x13ca000,0x1000),(0x143f000,0x1000),(ARENA,LENGTH),(STACK,0x10000),(STOP,0x1000),(API,0x1000)):
  u.mem_map(a,n)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 initial=bytes((i*37+seed*19)&255 for i in range(LENGTH));u.mem_write(ARENA,initial)
 u.mem_write(ENTRY,code)
 u.mem_write(0x122d70e,b'\0')
 put(0x13ca818,ARENA+0x5000 if installer else 0);put(0x13bc864,API+0x800 if error else 0)
 # One-node non-owning queue fixtures. Parent never traverses or mutates RB links.
 populated={}
 if priority>=0:
  populated[device,priority]=OBJ
  if cursor==2:populated[1-device,priority]=OBJ+0x80
 for d in range(2):
  for p in range(7):
   q=M+4+(d*7+p)*16;h=ARENA+0x1000+(d*7+p)*64;n=h+32
   put(q,h);put(q+4,int((d,p) in populated));put(q+8,0);put(h+4,n if (d,p) in populated else 0);put(h+8,n if (d,p) in populated else h);put(h+12,n if (d,p) in populated else h)
   put(q+12,n if cursor==1 and (d,p) in populated else h)
   if (d,p) in populated:
    put(n,1);put(n+4,h);put(n+8,0);put(n+12,0);put(n+16,populated[d,p]);put(n+20,populated[d,p])
 for p in range(7):
  put(M+0xe4+p*4,device if not expired else 1-device)
  u.mem_write(M+0x100+p*8,struct.pack('<d',(11.0,9.0,10.0,float('nan'))[expired]))
 put(M+0x138,OLD if current else 0);put(OLD+0x1c,current)
 put(M+0x16c,blocked);put(M+0x164,0x11223344)
 u.mem_write(M+0x170,struct.pack('<ff',0.25,0.5))
 for index in range(2):
  o=OBJ+index*0x80;f=FILE+index*0x40;ov=OVER+index*0x40
  for off,value in ((0,ARENA+0x6000+index*0x100),(4,0x12345678+index),(8,17+index),(12,ARENA+0x5800 if callback else 0),(20,priority),(28,0),(32,ov),(36,M),(40,f)):put(o+off,value)
  put(f+8,0x88776655+index);u.mem_write(f+0x18,bytes([mode]));put(ARENA+0x5800,ARENA+0x5900);put(ARENA+0x5904,API+0x700)
 u.mem_write(ARENA+0x7000,struct.pack('<d',10.0))
 # Genuine x87 return shape for a controlled timestamp boundary.
 u.mem_write(0x9e1bc0,b'\xdd\x05'+struct.pack('<I',ARENA+0x7000)+b'\xc3')
 apis={}
 for i,(name,slot) in enumerate((('enter',0x143fe0c),('leave',0x143fe10),('wait',0x143fe08),('write',0x143fdd8),('read',0x143fdf4),('event',0x143fde0),('sleep',0x143fddc))):
  apis[API+i*0x20]=name;put(slot,API+i*0x20)
 apis.update({0x98ebc0:'finish',0x98e770:'remove',0x404440:'get',0x9c31e0:'pause',0x9c3250:'unpause',API+0x700:'begin',API+0x800:'error'})
 for a in apis:u.mem_write(a,b'\xc3')
 before=bytes(u.mem_read(ARENA,LENGTH));trace=[];lock=0
 def hook(uc,a,size,_):
  nonlocal lock
  if a not in apis:return
  name=apis[a];sp=uc.reg_read(UC_X86_REG_ESP);args=lambda n:[get(sp+4+4*i) for i in range(n)]
  cleanup=0;result=0xbadc0de
  if name in ('enter','leave'):
   assert args(1)==[M+0x14c];lock+=1 if name=='enter' else -1;assert lock in (0,1);cleanup=4;trace.append((name,))
  elif name=='finish':
   assert lock==1 and uc.reg_read(UC_X86_REG_ECX)==M and args(1)==[OLD];cleanup=4;put(M+0x138,0);trace.append((name,))
  elif name=='remove':
   assert lock==1 and uc.reg_read(UC_X86_REG_ECX)==M;cleanup=4;trace.append((name,*args(1)))
  elif name=='get':result=ARENA+0x5000;trace.append((name,))
  elif name in ('pause','unpause'):
   assert lock==0 and uc.reg_read(UC_X86_REG_ECX)==ARENA+0x5000;trace.append((name,))
  elif name=='begin':
   assert lock==0 and uc.reg_read(UC_X86_REG_ECX)==ARENA+0x5800
   dst,p=args(2);assert get(dst+28)==1;trace.append((name,dst,p));cleanup=8;result=callback!=2
   if callback==3:put(dst,ARENA+0x6200);put(dst+20,4)
  elif name in ('read','write'):
   assert lock==0;v=args(5);assert v[-1]==0x98e320;assert get(v[3]+16)==get(M+0x138);assert get(get(M+0x138)+28)==2
   trace.append((name,*v));cleanup=20;result=submit
  elif name=='error':
   assert lock==0 and uc.reg_read(UC_X86_REG_ECX)==0x122d70e;trace.append((name,))
  elif name=='wait':assert lock==0 and args(3)==[0x11223344,0xffffffff,1];trace.append((name,));cleanup=12;result=0xc0
  elif name=='event':assert lock==0 and args(1)==[0x11223344];trace.append((name,));cleanup=4;result=0
  elif name=='sleep':assert lock==0 and args(2)==[1,1];trace.append((name,));cleanup=8;result=0xc0
  uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+cleanup)
 u.hook_add(UC_HOOK_CODE,hook)
 sp=STACK+0x8000;put(sp,STOP);saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,M)
 u.emu_start(ENTRY,STOP,count=5000)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+4 and lock==0
 assert all(u.reg_read(r)==0x55660000+i for i,r in enumerate(saved))
 # Independent scheduling expectations: priority wins, a live slice keeps its
 # device, expiration rotates first. Failed begin yields without submitting.
 names=[t[0] for t in trace]
 selected=populated.get((device,priority))
 if current==2 or blocked or priority<0:
  assert 'remove' not in names and names[-1]=='wait'
 else:
  assert selected is not None
  if not mode and callback==2:
   assert names[-3:]==['leave','event','sleep'] and 'remove' not in names and get(selected+28)==0
  else:
   assert ('write' if mode else 'read') in names and names[-1]=='wait' and get(M+0x138)==selected
   assert get(selected+28)==2
   assert bytes(u.mem_read(get(selected+32),20))==struct.pack('<IIIII',0,0,get(selected+4),0,selected)
   assert names.count('error')==int(bool(mode and not submit and error))
   assert names.count('unpause')==int(bool(mode and not submit and installer))
 assert names.count('finish')==int(current not in (0,2))
 # Predict every byte written by this parent from the frozen input. Helpers in
 # this gate are deliberately no-ops except finish's documented current reset.
 expected=bytearray(before)
 def eg(a):return struct.unpack_from('<I',expected,a-ARENA)[0]
 def ep(a,v):struct.pack_into('<I',expected,a-ARENA,v&0xffffffff)
 if current not in (0,2):ep(M+0x138,0)
 chosen=None
 if current!=2 and not blocked:
  for p in reversed(range(7)):
   d=eg(M+0xe4+p*4)
   def head_and_value(d):
    q=M+4+(d*7+p)*16;h=eg(q)
    if eg(q+12)==h:ep(q+12,eg(h+8))
    return None if eg(q+12)==h else eg(eg(q+12)+0x14)
   if not expired:chosen=head_and_value(d)
   if chosen is None:
    d=(d+1)%2;ep(M+0xe4+p*4,d)
    for attempt in range(2):
     chosen=head_and_value(d)
     if chosen is not None:
      struct.pack_into('<d',expected,M+0x100+p*8-ARENA,10.0+(0.25,0.5)[d]);break
     d=(d+1)%2;ep(M+0xe4+p*4,d)
   if chosen is not None:break
  if chosen is not None:
   if not mode and callback==3:ep(chosen,ARENA+0x6200);ep(chosen+20,4)
   if not mode and callback==2:ep(chosen+28,0)
   else:
    ov=eg(chosen+32)
    expected[ov-ARENA:ov-ARENA+20]=struct.pack('<IIIII',0,0,eg(chosen+4),0,chosen)
    ep(chosen+28,2);ep(M+0x138,chosen)
 assert bytes(u.mem_read(ARENA,LENGTH))==bytes(expected),('storage model',case)
 return bytes(u.mem_read(ARENA,LENGTH)),bytes(u.mem_read(0x13ca818,4)),bytes(u.mem_read(0x13bc864,4)),trace

def compare_process(source=SOURCE):
 inputs={Path(__file__),Path(source),EXE}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','native_exact_roots.py'):
  inputs.add(R/'tools/decomp_pipeline'/name)
 def closure(path):
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',path.read_text(encoding='utf-8'),re.M):
   q=R/'rebuild/include'/name;assert q.is_file(),name
   if q not in inputs:inputs.add(q);closure(q)
 closure(Path(source))
 pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
 original=read_virtual_data(EXE.read_bytes(),ENTRY,SIZE);candidate,grade=built(source);count=0
 # Pairwise-rich bounded Cartesian slices, rather than multiplying irrelevant
 # empty/blocked dimensions by every I/O case.
 cases=[]
 for p,d,c,e in itertools.product(range(-1,7),range(2),range(3),range(4)):
  cases.append((p,d,c,e,0,0,0,0,0,1,0,0))
 for mode,cb,cur,block,inst,submit,err,seed in itertools.product(range(2),range(4),(0,2,3),range(2),range(2),range(2),range(2),(0,7)):
  cases.append((6,1,0,1,mode,cb,cur,block,inst,submit,err,seed))
 for case in cases:
  a=execute(original,case);b=execute(candidate,case);assert a==b,case;count+=1
 report=dict(status='PASS',cases=count,qualification='Original/rebuilt complete arena and ABI equality; independent scheduling and submission assertions. Helpers, APIs, timestamp and callbacks controlled. No actual IO in this scratch gate.')
 # Removing/replacing the actual root must be detected by the independent model.
 for bad in (b'\xc3',b'\xcc'):
  detected=False
  try:execute(bad,(6,1,0,1,0,0,0,0,0,1,0,0))
  except Exception:detected=True
  assert detected
 assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
 report.update(grade=grade,acceptance_grade='NEAR_MATCH',input_sha256=pins,root_controls=2)
 return report
if __name__=='__main__':print(compare_process())
