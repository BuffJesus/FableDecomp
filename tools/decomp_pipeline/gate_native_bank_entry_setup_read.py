"""Canonical original/rebuilt SetupRead parity, independent model and defect controls."""
import hashlib,itertools,json,struct,sys,subprocess
from datetime import datetime,timezone
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
import check_one as V
from near_match import compare
import re,shutil
OUT=R/'rebuild/build/native-bank-entry-setup-read/00/9d/009d5970'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');OUT.mkdir(parents=True)
SOURCE=R/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_SetupRead_009d5970.cpp'
TEST=R/'rebuild/tests/00/9d/CBankFileAsyncEntry_SetupRead_009d5970_test.cpp'
inputs={Path(__file__),EXE}
def closure(p):
 p=p.resolve()
 if p in inputs:return
 inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(),re.M):
  q=next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file());closure(q)
closure(SOURCE);closure(TEST)
for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py'):inputs.add(R/'tools/decomp_pipeline'/name)
for name in ('egor_pdb_names.tsv','fablewin_pdb_names.tsv'):
 p=R/'ghidra_out'/name
 assert '?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z' in p.read_text(encoding='utf-8')
 inputs.add(p)
for folder in ('bin','include','lib'):inputs.update(p for p in (V.VC/folder).rglob('*') if p.is_file())
pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in inputs}
(OUT/'input-pins.json').write_text(json.dumps(pins,indent=2))
inc=OUT/'include';inc.mkdir()
for p in inputs:
 if p.is_relative_to(R/'rebuild/include'):shutil.copy2(p,inc/p.name)
source=OUT/SOURCE.name;shutil.copy2(SOURCE,source)
env=V.env()
def compile_source(source,obj,opt='/O2'):
 result=V.cl(['/nologo','/c',opt,'/MT','/W3','/I'+str(inc),'/Fo'+str(obj),str(source)],env,OUT)
 (OUT/(obj.stem+'.compile.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2))
 assert result.returncode==0,result.stdout+result.stderr
compile_source(source,OUT/'root.obj')
ROOT=0x9d5970
TARGETS={'setup':0x9d52d0,'offset':0x9cc270,'read':0x9cbf60,'align':0x9cbb90,'add':0x98f350}
SYMBOLS={'?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z':'setup','?ReadData@CBankFile@@IBEXKPAXK@Z':'read','?GetEntryDataOffset@CBankFile@@QBEKK@Z':'offset','?Align@CBankFile@@QBEKK@Z':'align','?AddObject@CThreadedFile@@QAEKABVCThreadedFileObjectDesc@@@Z':'add'}
SYMBOL='?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z'
c=Coff(OUT/'root.obj');sec,off=c.symbols[SYMBOL];assert off==0
body,rels=c.sections[sec];patched=bytearray(body)
for site,(name,kind) in rels.items():
 assert kind==20 and struct.unpack_from('<I',body,site)[0]==0
 struct.pack_into('<i',patched,site,TARGETS[SYMBOLS[name]]-(ROOT+site+4))
raw=EXE.read_bytes();original=read_virtual_data(raw,ROOT,191)
grade=compare(bytes(patched),original).to_dict();assert grade['near_match'] and len(body)==189 and len(rels)==7
A,N,E,D,B1,B2,B3,F1,F2,F3=0x2000000,0x8000,0x2000100,0x2000800,0x2001000,0x2001800,0x2002000,0x2003000,0x2003100,0x2003200
STACK,STOP=0x3000000,0x4000000
class Memory:
 def __init__(self,b=None,u=None):self.b=b;self.u=u
 def read(self,a,n):
  assert A<=a and a+n<=A+N
  return bytes(self.b[a-A:a-A+n]) if self.b is not None else bytes(self.u.mem_read(a,n))
 def write(self,a,b):
  assert A<=a and a+len(b)<=A+N
  if self.b is not None:self.b[a-A:a-A+len(b)]=b
  else:self.u.mem_write(a,b)
 def get(self,a):return struct.unpack('<I',self.read(a,4))[0]
 def put(self,a,v):self.write(a,struct.pack('<I',v&0xffffffff))
 def snap(self):return self.read(A,N)
def child(m,c,role,this,args,events,snaps):
 events.append((role,this,tuple(args)));snaps.append(m.snap());mode=c['mutation']
 if role=='setup':
  if mode==1:
   m.put(D+12,A+0x5100);m.put(E+4,B2);m.put(E+8,0x87654321)
  if mode==2:m.put(D+0x18,0xfedcba98);m.write(D+0x44,b'\0')
  return 0xffffff00 if c['seed'] else 1
 if role=='offset':
  if mode==1:m.put(E+4,B3);m.put(D+12,A+0x5200)
  if mode==2:m.put(D+0x28,B3);m.put(B3+0x110,F2)
  if mode==3:
   m.write(D+0x2c,bytes((i*11+17)&255 for i in range(24)))
   m.write(B1+0x148,bytes([1-c['noncached']]))
  return (0xfffffff7+args[0]*17)&0xffffffff
 if role=='align':
  if mode==4:m.put(D+0x30,0x87654321);m.put(D+0x28,B1);m.put(B1+0x110,F3)
  return (args[0]+31)&0xffffffe0
 if role=='read':
  if mode==5:
   m.write(D+0x44,b'\0');m.put(D+0x18,0x87654321);m.write(D+0x2c,b'CHILD-READ-SIDE-EFFECT!!!')
  return 0xfedcba98
 if role=='add':
  if mode==5:
   m.write(D+0x2c,b'ADD-CHILD-SIDE-EFFECT!!!!');m.put(D+0x18,0x87654321);m.put(D+0x28,B1)
  return c['uid']
 raise AssertionError(role)
def initial(c):
 m=Memory(bytearray((i*37+c['seed']*19)&255 for i in range(N)))
 m.put(E+4,B1);m.put(E+8,0x80000001);m.put(D+0x28,B2);m.put(D+12,A+0x5000)
 m.write(B1+0x148,bytes([c['noncached'],c['sync']]))
 for b,f in ((B1,F1),(B2,F2),(B3,F3)):m.put(b+0x110,f)
 return m.snap()
def model(c):
 m=Memory(bytearray(initial(c)));events=[];snaps=[]
 def call(role,this,args):return child(m,c,role,this,args,events,snaps)
 if c['sync']:
  call('setup',D,[1])
  buffer=m.get(D+12);index=m.get(E+8);bank=m.get(E+4)
  offset=call('offset',bank,[index])
  call('read',m.get(E+4),[(offset+c['offset'])&0xffffffff,buffer,c['size']])
  m.write(D+0x44,b'\1')
 else:
  noncached=m.read(m.get(E+4)+0x148,1)[0]
  m.put(D+0x2c,m.get(D+12))
  offset=call('offset',m.get(E+4),[m.get(E+8)])
  m.put(D+0x30,offset+c['offset'])
  size=call('align',m.get(E+4),[c['size']]) if noncached else c['size']
  m.put(D+0x34,size);m.put(D+0x38,D);m.put(D+0x40,c['priority']);m.write(D+0x3c,b'\0')
  uid=call('add',m.get(m.get(D+0x28)+0x110),[D+0x2c]);m.put(D+0x18,uid)
 snaps.append(m.snap());return events,snaps
u=Uc(UC_ARCH_X86,UC_MODE_32)
for page in {a&~4095 for a in [ROOT,*TARGETS.values()]}:u.mem_map(page,4096);u.mem_write(page,b'\xcc'*4096)
u.mem_map(A,N);u.mem_map(STACK,65536);u.mem_map(STOP,4096)
active=[None];u.hook_add(UC_HOOK_CODE,lambda uc,a,n,_:active[0](uc,a));context=u.context_save()
def execute(code,c):
 u.context_restore(context);u.mem_write(ROOT,b'\xcc'*384);u.mem_write(ROOT,code);u.ctl_remove_cache(ROOT,ROOT+384)
 u.mem_write(A,initial(c));u.mem_write(STACK,bytes(65536));m=Memory(u=u);events=[];snaps=[]
 sp=STACK+0x8000;u.mem_write(sp,struct.pack('<7I',STOP,c['offset'],D,c['size'],c['ignored'],c['ignored']^0xffffffff,c['priority']))
 u.reg_write(UC_X86_REG_ECX,E);u.reg_write(UC_X86_REG_ESP,sp)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,r in enumerate(saved):u.reg_write(r,0x55660000+i)
 def hook(uc,a):
  role=next((k for k,v in TARGETS.items() if a==v),None)
  if role is None:return
  esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);n=3 if role=='read' else 1
  args=struct.unpack('<'+'I'*n,uc.mem_read(esp+4,n*4));ret=struct.unpack('<I',uc.mem_read(esp,4))[0]
  result=child(m,c,role,this,args,events,snaps)
  uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
  uc.reg_write(UC_X86_REG_EIP,ret);uc.reg_write(UC_X86_REG_ESP,esp+4+n*4)
 active[0]=hook;u.emu_start(ROOT,STOP,count=512)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+28
 assert all(u.reg_read(r)==0x55660000+i for i,r in enumerate(saved))
 snaps.append(m.snap());return events,snaps
cases=points=0;roles=set();witnesses=[]
for sync,noncached,uid,mutation,size,offset,seed,ignored in itertools.product(range(2),range(2),(0,1,0x80000000,0xffffffff),range(6),(0,17,0xffffffff),(0,0xfffffff0),(0,7),(0,0xdeadbeef)):
 c=dict(sync=sync,noncached=noncached,uid=uid,mutation=mutation,size=size,offset=offset,seed=seed,ignored=ignored,priority=(0xffffffff if seed else 3))
 expected=model(c)
 if size==17 and offset==0xfffffff0 and seed==7 and ignored==0xdeadbeef and uid in (0,0xffffffff):witnesses.append(c)
 for label,code in (('original',original),('compiled',bytes(patched))):
  actual=execute(code,c)
  if actual!=expected:
   (OUT/'failure.json').write_text(json.dumps(dict(label=label,case=c,actual=actual[0],expected=expected[0]),indent=2))
   raise AssertionError((label,c,actual[0],expected[0]))
 cases+=1;points+=len(expected[1]);roles.update(x[0] for x in expected[0])
 if cases%512==0:print('setup-read progress',cases,flush=True)
controls={}
source_text=source.read_text()
mutants=[
 ('wrong-buffer','desc.PDst=data.Buffer;','desc.PDst=data.Data;'),
 ('omit-setup','data.SetupBuffer(true);',';'),
 ('respect-setup-failure','data.SetupBuffer(true);','if(!data.SetupBuffer(true)) return;'),
 ('omit-read','BankFile.ReadData(BankFile.GetEntryDataOffset(Index)+offset, data.Buffer, size);',';'),
 ('force-async','if (BankFile.DisableAsync)','if (false)'),
 ('force-sync','if (BankFile.DisableAsync)','if (true)'),
 ('always-align','if(BankFile.NonCached)','if(true)'),
 ('never-align','if(BankFile.NonCached)','if(false)'),
 ('wrong-file-bank','data.BankFile.ThreadedFile->AddObject(desc)','BankFile.ThreadedFile->AddObject(desc)'),
 ('missing-callback','desc.PCallback=&data;','desc.PCallback=0;'),
 ('retain-request','desc.QueueWhenFinished=false;','desc.QueueWhenFinished=true;'),
 ('wrong-priority','desc.Priority=priority;','desc.Priority=THREADED_FILE_PRIORITY_NORMAL;'),
 ('wrong-uid','data.ThreadedFileUID=data.BankFile.ThreadedFile->AddObject(desc);','data.ThreadedFileUID=1+data.BankFile.ThreadedFile->AddObject(desc);'),
 ('missing-finished','data.Finished=true;','data.Finished=false;'),
]
for name,old,new in mutants:
 assert source_text.count(old)==1
 mutant=OUT/(name+'.cpp');mutant.write_text(source_text.replace(old,new));obj=OUT/(name+'.obj');compile_source(mutant,obj)
 mc=Coff(obj);sec,off=mc.symbols[SYMBOL];assert off==0
 mb,mr=mc.sections[sec];code=bytearray(mb);assert len(code)<=384
 for site,(target,kind) in mr.items():
  assert kind==20 and struct.unpack_from('<I',mb,site)[0]==0
  struct.pack_into('<i',code,site,TARGETS[SYMBOLS[target]]-(ROOT+site+4))
 for case in witnesses:
  try:actual=execute(bytes(code),case);assert actual==model(case)
  except AssertionError:controls[name]=case;break
 else:raise AssertionError('undetected '+name)
assert len(controls)==14
test=OUT/TEST.name;test.write_text(TEST.read_text().replace('../../../include/',''))
compile_source(test,OUT/'typed-test.obj','/Od')
def link_and_run(name,objects,expected_link,expected_run=None):
 exe=OUT/(name+'.exe')
 result=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'/include:'+SYMBOL,*map(str,objects)],env=env,cwd=OUT,capture_output=True,text=True,timeout=30)
 (OUT/(name+'.link.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2))
 assert (result.returncode==0)==expected_link,result.stdout+result.stderr
 if not expected_link:
  assert any(SYMBOL in line and ('LNK2001' in line or 'LNK2019' in line) for line in (result.stdout+result.stderr).splitlines())
  return
 result=subprocess.run([str(exe)],cwd=OUT,capture_output=True,text=True,timeout=15)
 (OUT/(name+'.run.json')).write_text(json.dumps(dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr),indent=2))
 assert result.returncode==expected_run,result.stdout+result.stderr
 if expected_run==0:assert 'SETUPREAD_TYPED_PASS cases=24' in result.stdout
link_and_run('typed-required',[OUT/'typed-test.obj'],False)
link_and_run('typed-positive',[OUT/'typed-test.obj',OUT/'root.obj'],True,0)
link_and_run('typed-wrong-uid',[OUT/'typed-test.obj',OUT/'wrong-uid.obj'],True,1)
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in pins.items())
report=dict(status='PASS',build=str(OUT),cases=cases,snapshots_per_side=points,roles=sorted(roles),compiled_bytes=len(patched),original_bytes=191,grade=grade,controls=controls,relocations=rels,input_sha256=pins,object_sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in OUT.glob('*.obj')},typed_cases=24,strict_link_omission=True,typed_mutation_control=True,boundary='All five child functions independently controlled and mutating; no allocation, disk IO or lifetime claim. Passing first ordinary near candidate; no tuning.')
(OUT/'receipt.json').write_text(json.dumps(report,indent=2))
(R/'rebuild/compile-gate/native-bank-entry-setup-read.json').write_text(json.dumps(report,indent=2))
print('NATIVE_BANK_ENTRY_SETUP_READ_PASS cases=2304 snapshots=8640 controls=14 typed_cases=24 compiled_bytes=189 original_bytes=191')
print('RECEIPT',OUT/'receipt.json')
