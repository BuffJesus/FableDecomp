"""Canonical exact surface-section wrapper; allocator/data initializer boundaries."""
import hashlib,itertools,json,struct,sys,tempfile,subprocess
from pathlib import Path
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
SOURCE=R/'rebuild/src/compiled/00/9f/CGraphicFrame_InitialiseAsSurfaceSection_009fcb60.cpp'
BUILD=Path(tempfile.mkdtemp(prefix='loader-surface-section-'))
OBJ=BUILD/'root.obj';ENTRY=0x9fcb60;ALLOC=0xbfea1a;INIT=0x9fca40
A=0x2000000;STACK=0x3000000;STOP=0x4000000;SIZE=16384
SYMBOL='?InitialiseAsSurfaceSection@CGraphicFrame@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z'
def word(v):return struct.pack('<I',v&0xffffffff)
q=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(OBJ),str(SOURCE)],V.env(),BUILD)
assert not q.returncode,q.stdout+q.stderr
c=Coff(OBJ);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec]
native=read_virtual_data(EXE.read_bytes(),ENTRY,57);patched=bytearray(built)
assert off==0 and len(built)==57 and set(rel)=={6,41,53}
targets={'??2@YAPAXI@Z':ALLOC,
 '?InitialiseAsSurfaceSection@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z':INIT}
for at,(target,kind) in rel.items():
 assert kind==20 and target in targets
 assert struct.unpack_from('<I',built,at)[0]==0,(at,target,'nonzeroCOFFaddend')
 patched[at:at+4]=word(targets[target]-(ENTRY+at+4))
assert bytes(patched)==native
proof=check(OBJ,SYMBOL,ENTRY,57,3,targets)
required=R/'rebuild/tests/integration/NativeGraphicFrameInitialiseSurfaceSection_required.cpp'
required_obj=BUILD/'required.obj'
q=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(required_obj),str(required)],V.env(),BUILD)
assert not q.returncode,q.stdout+q.stderr
link=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(BUILD/'required.exe'),str(required_obj)]
q=subprocess.run(link,env=V.env(),capture_output=True,text=True)
assert q.returncode and SYMBOL in q.stdout+q.stderr and ('LNK2001' in q.stdout+q.stderr or 'LNK2019' in q.stdout+q.stderr),q.stdout+q.stderr
q=subprocess.run(link+[str(OBJ)],env=V.env(),capture_output=True,text=True)
assert not q.returncode,q.stdout+q.stderr
regs=(UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI)
active=[None];machines=[]
def dispatch(u,pc,size,data):active[0](u,pc,size,data)
for body in (native,bytes(patched)):
 u=Uc(UC_ARCH_X86,UC_MODE_32)
 for address,length in ((ENTRY&~4095,4096),(ALLOC&~4095,4096),(A,SIZE),(STACK,65536),(STOP,4096)):u.mem_map(address,length)
 u.mem_write(ENTRY,body);u.hook_add(UC_HOOK_CODE,dispatch);machines.append(u)

def run(u,initial,owner,args,allocated,result,mutation):
 u.mem_write(A,initial);u.mem_write(STACK,bytes(65536));sp=STACK+0x8000
 u.mem_write(sp,word(STOP)+b''.join(word(x) for x in args))
 u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_EFLAGS,2)
 for i,r in enumerate(regs):u.reg_write(r,0x11223300+i)
 events=[];snapshots=[]
 def hook(uc,pc,size,data):
  if pc not in (ALLOC,INIT):return
  esp=uc.reg_read(UC_X86_REG_ESP)
  if pc==ALLOC:
   size_arg=struct.unpack('<I',uc.mem_read(esp+4,4))[0];assert size_arg==27
   if mutation:uc.mem_write(owner,word(owner+0x80))
   events.append(('allocation',27));result_arg=allocated;cleanup=0
  else:
   supplied=struct.unpack('<7I',uc.mem_read(esp+4,28));receiver=uc.reg_read(UC_X86_REG_ECX)
   assert supplied==args and receiver==allocated
   events.append(('initializer',receiver,supplied));result_arg=result;cleanup=28
  snapshots.append(bytes(uc.mem_read(A,SIZE)))
  if pc==INIT:
   # Deliberate bounded side effects distinguish forwarding from callback-only
   # final-state coincidence. No child initializer behavior is claimed.
   uc.mem_write(owner+4,word(receiver));uc.mem_write(owner+8,word(result))
  destination=struct.unpack('<I',uc.mem_read(esp,4))[0]
  uc.reg_write(UC_X86_REG_EAX,result_arg);uc.reg_write(UC_X86_REG_ECX,0xabcdef01);uc.reg_write(UC_X86_REG_EDX,0x98765432)
  uc.reg_write(UC_X86_REG_ESP,esp+4+cleanup);uc.reg_write(UC_X86_REG_EIP,destination)
 active[0]=hook;u.emu_start(ENTRY,STOP,count=200)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+32
 assert u.reg_read(UC_X86_REG_EAX)==result
 assert [u.reg_read(r) for r in regs]==[0x11223300+i for i in range(4)]
 snapshots.append(bytes(u.mem_read(A,SIZE)))
 return events,snapshots

floats=((0,0x80000000),(0x3f800000,0xbf800000),(0x7f800000,0xff800000),
 (0x7fc01234,0xffc01234),(1,0x80000001),(0x7f800001,0xff800001))
integers=((0,0),(0x80000000,0xffffffff),(0x7fffffff,65535))
booleans=((0,1),(0x80,0xff),(0xffffffff,0x80000000),(0x12345600,0xabcdef01))
cases=snapshots=0
for flag,allocation,place,fill,fp,intp,bp,result,mutation in itertools.product(
 range(256),range(3),range(2),range(2),range(6),range(3),range(4),
 (0,1,0x80,0xffffffff),range(2)):
 # Eight bounded subprocess batches avoid accumulating Unicorn host resources;
 # batch defaults cover only32 flag bytes, full coordinator will run all256.
 start=int(sys.argv[1]) if len(sys.argv)>1 else 0
 if not start<=flag<start+32:continue
 base=A+place*0x1800;owner=base+0x100;block=base+0x200
 allocated=(0,block,owner)[allocation]
 initial=bytearray([0xa9 if fill else 0])*SIZE
 def put(buf,addr,value):buf[addr-A:addr-A+4]=word(value)
 put(initial,owner,base+0x400);initial[(allocated or block)-A+26]=flag
 args=(base+0x600,*floats[fp],*integers[intp],*booleans[bp])
 expected=bytearray(initial)
 if mutation:put(expected,owner,owner+0x80)
 expected_snapshots=[bytes(expected)]
 if allocated:
  put(expected,allocated,1);put(expected,allocated+8,0);put(expected,allocated+12,0)
  expected[allocated-A+26]&=0xef
 put(expected,owner,allocated);expected_snapshots.append(bytes(expected))
 put(expected,owner+4,allocated);put(expected,owner+8,result);expected_snapshots.append(bytes(expected))
 expected_events=[('allocation',27),('initializer',allocated,args)]
 for u in machines:
  actual=run(u,bytes(initial),owner,args,allocated,result,mutation)
  assert actual==(expected_events,expected_snapshots),(flag,allocation,place,fill,fp,intp,bp,result,mutation)
 cases+=1;snapshots+=3
 if cases%10000==0:print('SURFACE_SECTION_PROGRESS batch=%d cases=%d'%(start,cases),flush=True)
print(json.dumps({'status':'NATIVE_SECTION_WRAPPER_BATCH_PASS','cases':cases,'snapshots':snapshots,
 'batch_start':start,'arena_bytes':SIZE,'retail_bytes':57,'compiled_bytes':len(built),'relocations':3,
 'mandatory_source_omission_checks':1,'zero_coff_relocation_addends':3,
 'source_sha256':hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
 'provider':proof,'relocation_resolved_body_sha256':hashlib.sha256(bytes(patched)).hexdigest(),
 'scope':'Original/model/compiled fullarena snapshots; rawfloat/int/bool args, callbackreturn, allocationNULL/fresh/owneralias, constructor27B effects, oldowner overwritten without release, liveallocation callbackmutation, RET1C/calleeregisters. Data initializer9FCA40 remains boundedcallback; no allocator/childbehavior or surfaceUV/rendering claim.'},indent=2))
