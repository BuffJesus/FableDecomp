"""Full request-submission model, original/rebuilt storage, call order and ABI."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY,MANAGER,DESC,FILE,OBJECT,OVERLAP,STACK,STOP=0x98f0f0,0x2000100,0x2001000,0x2002000,0x2003000,0x2004000,0x3000000,0x4000000
def run(code,args):
    uid,device,priority,allocation,seed=args
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0x98f000,0x1000),(0x991000,0x1000),(0xbfe000,0x1000),(0x143f000,0x1000),(0x5000000,0x1000),(0x2000000,0x6000),(STACK,0x10000),(STOP,0x1000)):
        u.mem_map(address,size)
    u.mem_write(ENTRY,code);u.mem_write(0x2000000,b'\xa5'*0x6000)
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    for i,slot in enumerate((0x143fe0c,0x143fe10,0x143fdd4,0x143fde0)):
        put(slot,0x5000000+i*16);u.mem_write(0x5000000+i*16,b'\xc3')
    for address in (0xbfea1a,0x991350,0x9913d0):u.mem_write(address,b'\xc3')
    put(MANAGER+0x168,uid);put(MANAGER+0x164,0x12345678);put(FILE+4,device)
    desc=bytes((i*17+seed*31)&255 for i in range(20))+struct.pack('<I',priority)
    u.mem_write(DESC,desc);depth=0;trace=[]
    def hook(uc,a,size,user):
        nonlocal depth
        if a not in (0x5000000,0x5000010,0x5000020,0x5000030,0xbfea1a,0x991350,0x9913d0):return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);arg=get(sp+4)
        pop=0;result=0
        if a in (0x5000000,0x5000010):
            assert arg==MANAGER+0x14c
            depth+=1 if a==0x5000000 else -1;assert depth in (0,1)
            trace.append(('enter' if depth else 'leave',));pop=4
        elif a==0xbfea1a:
            assert arg==64 and depth==1;result=OBJECT;trace.append(('new',arg))
        elif a==0x5000020:
            assert arg==0x40 and get(sp+8)==20 and depth==1
            assert get(OBJECT+0x18)==uid and bytes(uc.mem_read(OBJECT,24))==desc
            assert get(OBJECT+0x24)==MANAGER and get(OBJECT+0x28)==FILE and get(OBJECT+0x1c)==0
            result=OVERLAP if allocation else 0;pop=8;trace.append(('global_alloc',result))
            if result:uc.mem_write(result,b'\0'*20)
        elif a==0x9913d0:
            assert this==MANAGER+0x13c and get(arg)==uid and depth==1
            result=0x2005000;pop=4;trace.append(('uid_slot',uid))
        elif a==0x991350:
            assert this==MANAGER+4+(device*7+priority)*16 and get(arg)==OBJECT and depth==1
            assert get(0x2005000)==OBJECT
            result=0x2005004;pop=4;trace.append(('pending_slot',device,priority))
        else:
            assert arg==0x12345678 and depth==1 and get(0x2005004)==OBJECT
            assert get(MANAGER+0x168)==(uid+1)&0xffffffff
            trace.append(('event',));pop=4;result=seed%2
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    expected=bytearray(u.mem_read(0x2000000,0x6000))
    def model_put(a,v):struct.pack_into('<I',expected,a-0x2000000,v&0xffffffff)
    model_put(MANAGER+0x168,uid+1);expected[OBJECT-0x2000000:OBJECT-0x2000000+24]=desc
    for off,value in ((0x18,uid),(0x1c,0),(0x20,OVERLAP if allocation else 0),(0x24,MANAGER),(0x28,FILE)):model_put(OBJECT+off,value)
    if allocation:expected[OVERLAP-0x2000000:OVERLAP-0x2000000+20]=b'\0'*20
    model_put(0x2005000,OBJECT);model_put(0x2005004,OBJECT)
    u.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,STOP);put(sp+4,DESC);put(sp+8,FILE)
    saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
    for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
    u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,MANAGER)
    u.emu_start(ENTRY,STOP,count=2000)
    assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+12
    assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
    assert u.reg_read(UC_X86_REG_EAX)==uid and depth==0
    state=bytes(u.mem_read(0x2000000,0x6000));assert state==bytes(expected)
    assert trace==[('enter',),('new',64),('global_alloc',OVERLAP if allocation else 0),('uid_slot',uid),('pending_slot',device,priority),('event',),('leave',)]
    return state,trace


def compare_add(candidate,native):
 cases=0
 for args in itertools.product((0,1,0xfffffffe,0xffffffff),(0,1),range(1,7),(False,True),range(3)):
  assert run(candidate,args)==run(native,args),args;cases+=1
 try:run(b'\x31\xc0\xc2\x08\x00',(1,1,6,True,2))
 except AssertionError:pass
 else:raise AssertionError('No-op body escaped independent model')
 return dict(cases=cases,model=True,full_storage_bytes=0x6000,controls=1)


def run_pending(code,device,priority,seed):
 u=Uc(UC_ARCH_X86,UC_MODE_32);entry=0x98eca0;slot=0x2005004;obj=OBJECT+seed*4
 for a,n in ((0x98e000,0x1000),(0x991000,0x1000),(0x2000000,0x6000),(STACK,0x10000),(STOP,0x1000)):u.mem_map(a,n)
 u.mem_write(entry,code);u.mem_write(0x991350,b'\xc3');u.mem_write(0x2000000,b'\xa5'*0x6000)
 def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
 def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
 put(FILE+4,device);put(obj+0x28,FILE);put(obj+0x14,priority)
 expected=bytearray(u.mem_read(0x2000000,0x6000));struct.pack_into('<I',expected,slot-0x2000000,obj);calls=[]
 def hook(uc,a,size,_):
  if a!=0x991350:return
  sp=uc.reg_read(UC_X86_REG_ESP);assert uc.reg_read(UC_X86_REG_ECX)==MANAGER+4+(device*7+priority)*16 and get(get(sp+4))==obj
  calls.append(a);uc.reg_write(UC_X86_REG_EAX,slot);uc.reg_write(UC_X86_REG_ECX,0xdeadbeef);uc.reg_write(UC_X86_REG_EDX,0x87654321);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
 u.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x8000;put(sp,STOP);put(sp+4,obj);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ECX,MANAGER)
 saved=(UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP)
 for i,reg in enumerate(saved):u.reg_write(reg,0x55660000+i)
 u.emu_start(entry,STOP,count=100)
 assert u.reg_read(UC_X86_REG_EIP)==STOP and u.reg_read(UC_X86_REG_ESP)==sp+8 and calls==[0x991350]
 assert all(u.reg_read(reg)==0x55660000+i for i,reg in enumerate(saved))
 state=bytes(u.mem_read(0x2000000,0x6000));assert state==bytes(expected);return state

def compare_pending(candidate,native):
 cases=0
 for args in itertools.product(range(2),range(1,7),range(4)):
  assert run_pending(candidate,*args)==run_pending(native,*args);cases+=1
 try:run_pending(b'\xc2\x04\x00',1,6,3)
 except AssertionError:pass
 else:raise AssertionError('No-op pending helper escaped model')
 return dict(cases=cases,controls=1,model=True,full_storage_bytes=0x6000)
