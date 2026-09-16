"""Execute original Guard lecture branches and x87 checks with API doubles."""
import struct
from tools.script_recovery.guard_health_native import fixture
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ESI,UC_X86_REG_EBP,UC_X86_REG_EBX,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EIP

def execute(repeated=False, crimes=0, health=1.0, busy=0, cancel=999):
    data,w=fixture();u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdac000,0xdad000,0x7e7000,0x4aa000,0xf35000,0x122d000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(w['mainAddress'],data.bytes_at(w['mainAddress'],w['mainSize']))
    u.mem_write(w['thresholdAddress'],bytes.fromhex(w['thresholdHex']))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;healthapi=0x204000;heroapi=0x204020;frameapi=0x204040;parent=0x206000;number=0x205000
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+0x14,parent);write(game,table)
    write(table+0x420,healthapi);write(table+0x118,heroapi);write(table+0x1c,frameapi)
    u.mem_write(parent+0x92,bytes([int(repeated)]))
    for i in range(5):u.mem_write(parent+0xfc+i,bytes([int(bool(crimes&(1<<i)))]))
    u.mem_write(number,struct.pack('<f',health));u.mem_write(healthapi,b'\xd9\x05'+struct.pack('<I',number)+b'\xc2\x04\x00')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ESI,owner);u.reg_write(UC_X86_REG_EBP,game);u.reg_write(UC_X86_REG_EBX,0)
    events=[];state={'queries':0,'busy':0,'success':False};getters={row['getter']:row for row in w['healthSites']}
    reads={0xdacb33:'GuardsSpokenOnce',0xdacc9d:'WhichBadDeedsPerformed_0',0xdacd66:'WhichBadDeedsPerformed_1',0xdace2b:'WhichBadDeedsPerformed_2',0xdaceea:'WhichBadDeedsPerformed_3',0xdacfb6:'WhichBadDeedsPerformed_4',0xdad3dc:'WhichBadDeedsPerformed_0',0xdad4a6:'WhichBadDeedsPerformed_1',0xdad56b:'WhichBadDeedsPerformed_2',0xdad636:'WhichBadDeedsPerformed_3',0xdad6fb:'WhichBadDeedsPerformed_4'}
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdadd90,0xdadda5,0xdaddb8,0xdad868):
            state['success']=pc==0xdad868;uc.emu_stop();return
        if pc in reads:events.append(('get',reads[pc]))
        if pc==0xdad31b:events.append(('set','GuardsSpokenOnce',True))
        if pc==0x7e7490:
            assert this==stack+16;state['output']=read(esp+4);events.append(('thing.new',));result=state['output'];pop=4
        elif pc==healthapi:assert read(esp+4)==state['output'];events.append(('health',))
        elif pc==0x4aa840:assert this==state['output'];events.append(('thing.destroy',));pop=0
        elif pc==heroapi:events.append(('hero',));result=0x207000;pop=0
        elif pc==0x7e7390:
            assert this==stack+16 and read(esp+4)==0x207000
            key=data.string_at(read(esp+8));assert [read(esp+i) for i in (12,16,20,24)]==[0,0,1,0]
            events.append(('speak',key));state['busy']=busy;pop=24
        elif pc==0x7e7450:
            assert this==stack+16;result=state['busy']>0;state['busy']-=1;events.append(('busy',bool(result)));pop=0
        elif pc==frameapi:events.append(('frame',));pop=0
        elif pc==0xf35b30:
            state['queries']+=1;result=state['queries']>=cancel;events.append(('term',bool(result)));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdacb30,0xdade4b,count=10000)
    return state['success'],events
