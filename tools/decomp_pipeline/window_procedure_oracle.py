"""Retail/candidate execution boundary model for the native window callback.

Calls are recording stand-ins, with deliberate global-manager changes at
callback boundaries. Full dispatch-table domains and ABI state are checked.
"""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ENTRY=0x9a5b60
TARGETS={
'?FableWindowSystemManager_013ca798@@3PAUCSystemManager@@A':0x13ca798,
'?FableWindowGamma_0138e0b8@@3MA':0x138e0b8,
'?FableSoundDeactivated_013ca610@@3_NA':0x13ca610,
'?FableSavedSoundVolume_013ca60c@@3MA':0x13ca60c,
'__imp__DefWindowProcW@16':0x144037c,
'?ActivateWindow@CSystemManager@@CIXPAU1@_N@Z':0x9a5a50,
'?SetGamma@CDisplayManager@@QAEXM_N@Z':0x9beaa0,
'?SetHasInputFocus@CSystemManager@@AAEX_N@Z':0x9a5a30,
'?ClearEvents@CInputManager@@QAEXXZ':0x9f5540,
'?UnknownSetEnabled_009915e0@CASoundManager@@QAEX_N@Z':0x9915e0,
'?SwapScreens@CDisplayManager@@QAE_NXZ':0x9beeb0,
'?IsApplicationActive@CSystemManager@@QAE_NXZ':0x9a57b0,
'?GInkeyBuffer@@3VCInkeyBuffer@@A':0x13cb2f8,
'?AddChar@CInkeyBuffer@@QAEXG_N@Z':0xa5fa70,
'?SetAsActive@CSystemManager@@AAEX_N@Z':0x9a5270}

# PDB class spelling; retain the old struct spelling for before/after audits.
TARGETS['?FableWindowSystemManager_013ca798@@3PAVCSystemManager@@A']=0x13ca798
TARGETS['?ActivateWindow@CSystemManager@@CIXPAV1@_N@Z']=0x9a5a50

def cases():
    # Exhaust the surrounding message domain, then cover special branch inputs.
    result=[(m,0x12345678,0,0x3f,0) for m in range(0x221)]
    values={1:[0],2:[0],5:[0,1,2,3],6:[0,1,0x10000,0x10001],7:[0],8:[0],15:[0],
            0x1c:[0,1],0x102:[0x61,0xffff,0x10041],0x104:[0x12,0x79,0x41],0x105:[0x12,0x79,0x41],
            0x112:[0xf140,0xf170,0xf141,0],0x218:[0,4,1]}
    for m in [0x201,0x202,0x204,0x205,0x207,0x208]: values[m]=[0]
    for m in [0x20b,0x20c]: values[m]=[0,0x10000,0x20000,0xffff0000]
    values[0x20a]=[120<<16,0xff880000,0x7fff0000,0x80000000]
    for m,ws in values.items():
        for w in ws:
            for flags in [0,1,2,4,7,8,15,16,31,32,63,127,255]:
                for ime in [0,1,2,3]:
                    for neg in ([0,1] if m==0x102 else [0]): result.append((m,w,neg,flags,ime))
    result.extend((0x220,0x12345678,0,64,ime) for ime in range(4))
    return result

def execute(code,case):
    message,wp,negative,flags,ime=case
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for page,size in [(0x9a5000,8192),(0x9be000,4096),(0x9f5000,4096),(0x991000,4096),
                      (0xa5f000,4096),(0x13ca000,8192),(0x138e000,4096),(0x1440000,4096),
                      (0x2000000,0x4000),(0x2100000,4096),(0x3000000,65536)]:uc.mem_map(page,size)
    first,second=0x2000000,0x2000200
    inp,sound,display,table,create=0x2000800,0x2000900,0x2001000,0x2002000,0x2003000
    stop,default,imefunc,setvol,getvol,offset=0x2100000,0x2100010,0x2100020,0x2100030,0x2100040,0x2100050
    def put(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    def byte(a,v):uc.mem_write(a,bytes([int(v)]))
    def current():return get(0x13ca798)
    def recv(p):return {first:1,second:2,inp:3,sound:4,display:5,0x13cb2f8:6}.get(p,0)
    def mutate():
        if flags&128:put(0x13ca798,second if current()==first else first)
    uc.mem_write(ENTRY,bytes(code))
    callbacks={a for k,a in TARGETS.items() if k.startswith('?') and '@C' in k and a!=0x13cb2f8}
    for a in callbacks|{default,imefunc,setvol,offset}:uc.mem_write(a,b'\xc3')
    uc.mem_write(getvol,b'\xd9\x05'+struct.pack('<I',0x2100080)+b'\xc2\x04\x00')
    uc.mem_write(0x2100080,struct.pack('<f',-0.375))
    for base in [first,second]:
        uc.mem_write(base,b'\xa5'*0x180)
        byte(base+9,bool(flags&16));byte(base+0xdc,False)
        put(base+0x58,inp if flags&1 else 0);put(base+0x60,display if flags&4 else 0);put(base+0x7c,sound if flags&2 else 0)
        put(base+0x178,imefunc if ime else 0);put(base+0xe4,0x7fffffc0)
        uc.mem_write(base+0x170,struct.pack('<f',-17.5))
    uc.mem_write(display+0x2c4,struct.pack('<f',1.75))
    put(sound,table);put(table+46*4,setvol);put(table+47*4,getvol);put(table+49*4,offset)
    initially_null=bool(flags&64) and message in (1,0x220)
    put(0x13ca798,0 if initially_null else first);put(0x144037c,default);put(create,second)
    uc.mem_write(0x13ca60c,struct.pack('<f',0.625));byte(0x13ca610,bool(flags&8));uc.mem_write(0x138e0b8,struct.pack('<f',1.25))
    lp=create if message==1 else (0x80001234 if negative else 0x1234)
    events=[];done=False
    def event(kind,receiver=0,a=0,b=0,c=0,d=0):events.append((kind,receiver,a,b,c,d))
    def hook(uc,address,size,data):
        nonlocal done
        if address==stop:done=True;uc.emu_stop();return
        if address not in callbacks|{default,imefunc,setvol,getvol,offset}:return
        sp=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX);argc=0;result=0
        if address==imefunc:
            assert receiver==0x123456 and uc.reg_read(UC_X86_REG_EDX)==message
            assert get(sp+4)==wp and get(sp+8)==lp and get(get(sp+12))==0
            event(1,0,message,wp,int(negative),ime);argc=3
            put(get(sp+12),0x87654321);result=int(ime==2)
            if ime==3:put(0x13ca798,second)
        elif address==default:
            assert [get(sp+4*i) for i in range(1,5)]==[0x123456,message,wp,lp]
            event(2,0,message,wp,int(negative));argc=4;result=0x76543210
        elif address==0x9a5a50:
            assert receiver==current();event(3,recv(receiver),uc.reg_read(UC_X86_REG_EDX)&255);mutate()
        elif address==0x9beaa0:
            assert receiver==display and get(sp+8)&255==0
            event(4,5,get(sp+4),get(sp+8)&255);argc=2;mutate()
        elif address==0x9a5a30:
            assert receiver==current();event(5,recv(receiver),get(sp+4)&255);byte(receiver+10,get(sp+4)&255);argc=1;mutate()
        elif address==0x9f5540:
            assert receiver==inp;event(6,3);mutate()
        elif address==0x9915e0:
            assert receiver==sound;event(7,4,get(sp+4)&255);argc=1;mutate()
        elif address==0x9beeb0:
            assert receiver==display;event(8,5);mutate()
        elif address==0x9a57b0:
            assert receiver==current();event(9,recv(receiver));result=int(bool(flags&16));mutate()
        elif address==0xa5fa70:
            assert receiver==0x13cb2f8;event(10,6,get(sp+4)&65535,get(sp+8)&255);argc=2;mutate()
        elif address==0x9a5270:
            assert receiver==current();event(11,recv(receiver),get(sp+4)&255);argc=1;mutate()
        elif address==setvol:
            assert receiver==sound;event(12,4,get(sp+4),get(sp+8),get(sp+12));argc=3;mutate()
        elif address==getvol:
            assert receiver==sound;event(13,4,get(sp+4));mutate();return
        else:
            assert receiver==sound and get(sp+4)==current()+0x170
            event(14,4,recv(current()));uc.mem_write(get(sp+4),struct.pack('<f',42.25));argc=1;result=int(bool(flags&32));mutate()
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_ESP,sp+4+4*argc);uc.reg_write(UC_X86_REG_EIP,get(sp))
    uc.hook_add(UC_HOOK_CODE,hook)
    sp=0x3008000
    for n,v in enumerate([stop,0x123456,message,wp,lp]):put(sp+4*n,v)
    saved={UC_X86_REG_EBX:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EDI:0x33333333,UC_X86_REG_EBP:0x44444444}
    for r,v in saved.items():uc.reg_write(r,v)
    uc.reg_write(UC_X86_REG_ESP,sp);uc.emu_start(ENTRY,stop+1,count=1000)
    assert done and uc.reg_read(UC_X86_REG_ESP)==sp+20
    assert all(uc.reg_read(r)==v for r,v in saved.items())
    state=bytearray()
    for base in [first,second]:
        data=bytearray(uc.mem_read(base,0x180))
        for o in [0x58,0x60,0x7c,0x178]:data[o:o+4]=b'\0'*4
        state.extend(data)
    state.extend(struct.pack('<II',uc.reg_read(UC_X86_REG_EAX),recv(current())))
    state.extend(uc.mem_read(0x13ca60c,5));state.extend(uc.mem_read(0x138e0b8,4))
    for e in events:state.extend(struct.pack('<6I',*e))
    h=2166136261
    for b in state:h=((h^b)*16777619)&0xffffffff
    return h,events,state.hex()
