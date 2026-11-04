"""Mocked x86 shutdown execution; no host windows or subsystem teardown."""
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ENTRY=0x9a60d0
SYSTEM=0x2000000
CALLBACK=0x2200000
STACK=0x3000000
GLYPHS=SYSTEM+0x4000
ALTERNATE=SYSTEM+0x8000
DRIVE=0x13caa28
SCRATCH=0x13cb498
MEMBERS=[0x58,0x7c,0x84,0x6c,0x74,0x50,0x60,0xf8]
TARGETS={
    '?IsValid@CInitBaseClass@@QBE_NXZ':0x99a330,
    '??3@YAXPAX@Z':0xbfe9bc,
    '?PClass@?$CTBaseSingletonNoDefaultAlloc@VCDriveManager@@@@0V?$CCountedPointer@VCDriveManager@@@@A':DRIVE,
    '?FablePendingGlyphCount_0129ab30@@3KB':0x129ab30,
    '?FablePendingGlyphs_0138e074@@3PAUPendingLoadedGlyph@@A':0x138e074,
    '?FablePendingGlyphRead_013c5a74@@3KA':0x13c5a74,
    '?FablePendingGlyphWrite_013c5a78@@3KA':0x13c5a78,
    '?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z':0x9bf5a0,
    '?GScratchBuffer@@3VCScratchBuffer@@A':SCRATCH,
    '?Uninitialise@CScratchBuffer@@QAEXXZ':0x9f4950,
    '__imp__DestroyWindow@4':0x1440340,
    '__imp__DeleteCriticalSection@4':0x143fde4,
    '?NoInstances@CSystemManager@@2EA':0x13ca608,
    '?Uninitialise@CInitBaseClass@@IAEXXZ':0x99a360,
}

def execute(code, valid, pattern, window, scratch, mutate, count, instances):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(0x990000,0x80000),(0xbf0000,0x10000),(0x129a000,4096),
                (0x138e000,4096),(0x13c5000,0x7000),(0x143f000,8192),
                (SYSTEM,0x20000),(CALLBACK,4096),(STACK,0x10000)]:uc.mem_map(a,n)
    def put(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def byte(a,v):uc.mem_write(a,bytes([v&255]))
    uc.mem_write(ENTRY,bytes(code));uc.mem_write(SYSTEM,b'\xa5'*0x180)
    byte(SYSTEM+4,valid);byte(SCRATCH+4,scratch)
    byte(SYSTEM+0xec,window//2);byte(SYSTEM+0xe8,window%2)
    put(SYSTEM+0x94,0x98765432);put(0x138e074,GLYPHS);put(0x129ab30,count)
    put(0x13c5a74,231);put(0x13c5a78,427);byte(0x13ca608,instances)
    put(0x1440340,CALLBACK+0x10);put(0x143fde4,CALLBACK+0x20)
    locations={};infos={};serial=0
    def owner(a,mode):
        nonlocal serial
        serial+=1;info=SYSTEM+0xc000+serial*16;obj=SYSTEM+0x18000+serial*8
        put(a,obj if mode else 0);put(a+4,info if mode else 0)
        if mode:
            put(info,1 if mode==1 else (2 if mode==2 else 0))
            put(info+4,CALLBACK);put(info+8,obj)
            locations[obj]=a;infos[info]=obj
    for i,offset in enumerate(MEMBERS):owner(SYSTEM+offset,(pattern+i)%4)
    owner(DRIVE,pattern%4)
    for base in [GLYPHS,ALTERNATE]:
        for i in range(512):
            # Sparse ownership exercises first, middle and final entries.
            owner(base+i*16,(pattern+i)%4 if i in [0,1,255,511] else 0)
            put(base+i*16+8,0xa5a5a501);put(base+i*16+12,0x12340000+i)
    events=[];returned=False;swapped=False
    def hook(uc,a,size,data):
        nonlocal returned,swapped
        if a==CALLBACK+0x100:
            returned=True;uc.emu_stop();return
        if a not in [CALLBACK,CALLBACK+0x10,CALLBACK+0x20,0x99a330,0xbfe9bc,0x9bf5a0,0x9f4950,0x99a360]:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0
        if a==0x99a330:
            assert this in [SYSTEM,SCRATCH];result=uc.mem_read(this+4,1)[0]
            events.append(('valid',this,result))
        elif a==CALLBACK:
            owner_address=locations[this];events.append(('destroy',this,bytes(uc.mem_read(owner_address,8)).hex()))
            # Destructors can replace the Info pointer. Retail reloads it for delete.
            if mutate==1:
                replacement=SYSTEM+0x16000+(len(events)%64)*16
                put(replacement,0xabcdef01);put(owner_address+4,replacement)
            if mutate==2 and GLYPHS<=owner_address<GLYPHS+8192 and not swapped:
                put(0x138e074,ALTERNATE);swapped=True
        elif a==0xbfe9bc:
            events.append(('free',get(sp+4)))
        elif a==0x9bf5a0:
            pop=4;assert this==get(SYSTEM+0x60)
            mesh=get(SYSTEM+0x74);assert get(sp+4)==(mesh+4 if mesh else 0)
            events.append(('detach',this,get(sp+4)))
        elif a==0x9f4950:
            assert this==SCRATCH;events.append(('scratch',));byte(SCRATCH+4,0)
        elif a==CALLBACK+0x10:
            pop=4;assert get(sp+4)==0x98765432;events.append(('window',get(sp+4)));result=0
        elif a==CALLBACK+0x20:
            pop=4;assert get(sp+4)==SYSTEM+0x34;events.append(('critical',))
        else:
            assert this==SYSTEM;events.append(('base',));byte(SYSTEM+4,0)
        ret=get(sp);uc.reg_write(UC_X86_REG_EAX,result)
        uc.reg_write(UC_X86_REG_ECX,0xdead0001);uc.reg_write(UC_X86_REG_EDX,0xdead0002)
        uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    sp=STACK+0x8000;put(sp,CALLBACK+0x100)
    uc.reg_write(UC_X86_REG_ESP,sp);uc.reg_write(UC_X86_REG_ECX,SYSTEM)
    preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
    for i,r in enumerate(preserved):uc.reg_write(r,0xabcd0000+i)
    uc.emu_start(ENTRY,CALLBACK+0x101,count=100000)
    assert returned and uc.reg_read(UC_X86_REG_ESP)==sp+4
    for i,r in enumerate(preserved):assert uc.reg_read(r)==0xabcd0000+i
    return events, bytes(uc.mem_read(SYSTEM,0x20000)),bytes(uc.mem_read(0x13c5000,0x7000)),bytes(uc.mem_read(0x138e074,4))
