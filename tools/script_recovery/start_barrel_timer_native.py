"""Whole original timer helper and actual counted-Thing destructor execution."""
import struct
from tools.script_recovery.start_barrel_timer import prove
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EDX,UC_X86_REG_EIP

def execute(cancel=99,start=(-1,0,1),spoken=2,near=(True,False),timer_ids=(-1,0,3),bar_ids=(-1,0,17),values=(-2147483648,2147483647),created=-3,count=1,populated=True):
    d=RData();w=prove(d);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdb4000,0xdb5000,0x99e000,0x99a000,0xcb7000,0xcbe000,0x4aa000,0xbfe000,0x143e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for r in w['regions']:u.mem_write(r['address'],d.bytes_at(r['address'],r['size']))
    for pc in (0x99ebf0,0x99eae0,0xcb7940,0xcbe2ff,0xbfe9bc,0x208000):u.mem_write(pc,b'\xc3')
    stack=0x21e000;owner=0x201000;game=0x202000;global_games=(0x202100,0x202200);table=0x203000;done=0x204000;info=0x205000;hero=0x206000;guard=stack-12
    def write(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def signed(v):return v if v<0x80000000 else v-0x100000000
    def floatat(a):return struct.unpack('<f',u.mem_read(a,4))[0]
    write(stack,done);write(owner+0x40,game)
    for g in (game,*global_games):write(g,table)
    write(info,count);write(info+4,0x208000);write(info+8,hero)
    spec=((0x168,'timer',1),(0x1c,'frame',0),(0x510,'add',7),(0x120,'lookup',2),(0x118,'hero',0),(0x534,'colour',3),(0x530,'update',4),(0x548,'remove',1))
    apis={0x207000+i*16:v for i,v in enumerate(spec)}
    for pc,(slot,*_) in apis.items():write(table+slot,pc);u.mem_write(pc,b'\xc3')
    events=[];texts={};state={'query':0,'timers':0,'start':0,'spoken':0,'distance':0,'bars':0,'updates':0,'live':False,'global':0}
    def event(*a):events.append(a)
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc in (0xdb4f76,0xdb4fa7,0xdb5137):
            index=state['timers'];state['global']=index%2;write(0x143e8f8,global_games[index%2]);write(owner+0x108,timer_ids[index%len(timer_ids)])
        if pc in (0xdb5128,0xdb515f,0xdb5186):
            index=state['bars'];state['bars']+=1;v=bar_ids[index%len(bar_ids)];write(owner+0x60,v);event('bar.read',v)
        if pc in (0xdb5084,0xdb516f):
            v=state['spoken']>=spoken;state['spoken']+=1;u.mem_write(owner+0x73,bytes([v]));event('spoken',v)
        if pc==0xdb5048:event('bar.store',signed(read(owner+0x60)))
        if pc==0x99ebf0:texts[this]=d.string_at(read(sp+4)) or '';assert read(sp+8)==0xffffffff;event('key.new',texts[this]);pop=8
        elif pc==0x99eae0:event('key.destroy',texts.pop(this));pop=0
        elif pc==0xcb7940:assert this==owner;state['query']+=1;result=state['query']>=cancel;event('term',result);pop=0
        elif pc==0xcbe2ff:
            assert this==hero and uc.reg_read(UC_X86_REG_EDX)==guard and read(sp+4)==0x40000000 and state['live'];i=state['distance'];state['distance']+=1;result=near[i%len(near)];event('near',result);pop=4
        elif pc==0x208000:assert this==hero and read(info)==0;event('object.destroy');pop=0
        elif pc==0xbfe9bc:assert read(sp+4)==info and read(info)==0;event('info.free');pop=0
        elif pc==0x99a2e0:assert this==guard and state['live'] and read(guard+4)==read(guard+8)==0;state['live']=False;event('destroy');return
        elif pc in apis:
            _,name,n=apis[pc];args=[read(sp+4+i*4) for i in range(n)];pop=n*4
            assert this==(global_games[state['global']] if name=='timer' else game)
            if name=='timer':
                i=state['timers'];state['timers']+=1;assert signed(args[0])==timer_ids[i%len(timer_ids)]
                if read(sp)==0xdb5156:result=values[state['updates']%len(values)];state['updates']+=1
                else:result=start[min(state['start'],len(start)-1)];state['start']+=1
                event('timer',state['global'],signed(args[0]),result)
            elif name=='frame':event('frame')
            elif name=='add':
                assert args[0:2]==[0x42340000,0] and read(args[2])==read(args[3])==0xff00ff00 and texts[args[4]]=='HUD_CLOCK_ICON' and texts[args[5]]=='' and args[6]==0x3f800000
                result=created;event('add',created)
            elif name=='lookup':
                assert args[0]==guard and texts[args[1]]=='M_WHouse_GuardPoint';write(guard,0x1238c8c);write(guard+4,hero if populated else 0);write(guard+8,info if count else 0);state['live']=True;event('lookup',populated);result=guard
            elif name=='hero':result=hero;event('hero')
            elif name=='colour':assert read(args[1])==read(args[2]);event('colour',signed(args[0]),read(args[1]))
            elif name=='update':assert args[2:]==[0xbf800000]*2;event('update',signed(args[0]),floatat(sp+8),-1.,-1.)
            elif name=='remove':event('remove',signed(args[0]))
        if pop is not None:uc.reg_write(UC_X86_REG_EAX,int(result)&0xffffffff);uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdb4f70,done,count=6000)
    assert not texts and not state['live'] and u.reg_read(UC_X86_REG_EIP)==done
    return events
if __name__=='__main__':print(execute())
