"""Original Init/GivenTeddy instructions with API and quest-helper boundaries."""
import struct
from functools import lru_cache
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP
from tools.script_recovery.teddy_girl_lifecycle import prove
from tools.script_recovery.lift_native_lua import RData

@lru_cache(maxsize=1)
def fixture():
    data=RData();return data,prove(data)

def execute(kind,initial=(True,True,True,True)):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdaf000,0xdb0000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for row in w['functions']:u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    for address in (0x99ebf0,0x99eae0,0x99efe0,0xdb0660):u.mem_write(address,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;done=0x204000;parent=0x205000;master=0x206000
    def write(a,v):u.mem_write(a,struct.pack('<I',v))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    write(stack,done);write(owner+4,game);write(owner+0x14,parent);write(owner+0x18,master);write(game,table)
    u.mem_write(owner+0x1c,bytes(initial));apis={0x207000:('damage',1),0x207010:('kill',2),0x207020:('combo',1),0x207030:('information',3),0x207040:('take',1),0x207050:('clear',0)}
    for slot,pc in zip((0x810,0x814,0x838,0x5a0,0x1f4,0x5a4),apis):write(table+slot,pc)
    events=[];live={};names=('DoneIntro','FoundTeddy','SpokeAboutFindingTeddy','HeroHitMe')
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None
        if pc in (0xdaf00b,0xdaf00e,0xdaf011,0xdaf014):events.append(('set',names[(pc-0xdaf00b)//3],False))
        elif pc==0xdb0630:events.append(('set','FoundTeddy',True))
        elif pc==0x99ebf0:
            assert read(sp+8)==0xffffffff;live[this]=data.string_at(read(sp+4));events.append(('text.new',live[this]));pop=8
        elif pc==0x99eae0:events.append(('text.destroy',live.pop(this)));pop=0
        elif pc==0xdb0660:assert this==parent;events.append(('goodDeed',));pop=0
        elif pc==0x99efe0:
            assert this==master+0x54;events.append(('master','TeddySolution',data.string_at(read(sp+4))));pop=4
        elif pc in apis:
            assert this==game;name,count=apis[pc]
            if name=='take':events.append(('take',live[read(sp+4)]));pop=4
            else:
                assert read(sp+4)==owner+8
                events.append((name,*[bool(read(sp+8+4*i)) for i in range(count)]));pop=4*(1+count)
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,0);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook)
    try:u.emu_start(0xdaf000 if kind=='Init' else 0xdb0600,done,count=1000)
    except Exception as error:raise AssertionError((hex(u.reg_read(UC_X86_REG_EIP)),events)) from error
    assert u.reg_read(UC_X86_REG_EIP)==done and not live
    return events,tuple(bool(v) for v in u.mem_read(owner+0x1c,4))
