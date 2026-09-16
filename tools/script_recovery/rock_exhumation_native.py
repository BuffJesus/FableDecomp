"""Execute original map/movie sequence; thread-service calls are engine doubles."""
import struct
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ESI,UC_X86_REG_EBX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_exhumation import recover


def execute(empty_hero=False,empty_troll=False,empty_movie=False,allocation_fails=False):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x200000,0x20000)
    actor=0x201000;parent=0x202000;game=0x203000;table=0x204000;thread=0x205000;start=0x206000;pause=0x206010
    stack=0x21ef00;actor_map=stack+0x24;self_resource=stack+0x3c;hero_resource=stack+0x4c;movie=stack+0x5c
    for page in (0xec4000,0xcd0000,0xcdb000,0xcdd000,0xcd3000,0x8ab000,0x99e000,0x99f000,0x6e7000,0xcbf000,0xbfe000,0x4ab000,0x4aa000,0xcb7000,0x7e7000):u.mem_map(page,4096)
    u.mem_write(0xec4cfb,data.bytes_at(0xec4cfb,499))
    for entry in (0xcdbf70,0x99ebf0,0x99eae0,0xcd3d2e,0x8abd10,0x6e7b60,
                  start,pause,0xcbfb7d,0xbfea1a,0x4abe90,0x4aa840,0x99f570,
                  0xcdd450,0xcb7e50,0x6e7b80,0xcdbfb0,0x7e74d0):u.mem_write(entry,b'\xc3')
    def write(at,value):u.mem_write(at,struct.pack('<I',value))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(actor+4,game);write(actor+0x14,parent);write(game,table);write(table+0x5c8,start);write(table+0x5ec,pause)
    write(actor+8,0x1238c8c);write(actor+12,0x207000);write(actor+16,0) # Thing Info ownership dispatch tested separately
    write(self_resource+8,0 if empty_troll else 0x20a000)
    write(hero_resource+8,0 if empty_hero else 0x20b000)
    u.reg_write(UC_X86_REG_ESI,actor);u.reg_write(UC_X86_REG_EBX,0);u.reg_write(UC_X86_REG_ESP,stack)
    strings={};entries={};events=[];state={'paused':False,'movie':False,'hero':True,'self':True};last_entry=None
    def hook(uc,pc,size,user):
        nonlocal last_entry
        esp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0xcdbf70:assert this==actor_map;events.append(('map.new',));pop=0
        elif pc==0x99ebf0:
            address=read(esp+4);name='' if data.bytes_at(address,1)==b'\0' else data.string_at(address)
            strings[this]=name;result=this;pop=8
        elif pc==0x99eae0:strings.pop(this);pop=0
        elif pc==0xcd3d2e:
            assert this==actor_map;key=strings[read(esp+4)];result=0x208000+len(entries)*32;entries[result]=key;last_entry=result;pop=4
        elif pc==0x8abd10:
            assert this==last_entry;source=read(esp+4);name='hero' if source==hero_resource else 'self'
            assert source in (hero_resource,self_resource);uc.mem_write(this,bytes(uc.mem_read(source,16)))
            events.append(('map.actor',entries[this],name));pop=4
        elif pc==0x6e7b60:assert this==movie;events.append(('movie.new',));pop=0
        elif pc==start:
            assert this==game and read(esp+8)==movie and strings[read(esp+4)]==''
            write(movie+8,0 if empty_movie else 0x20c000)
            state['movie']=True;events.append(('movie.start','',empty_movie));pop=8
        elif pc==pause:
            assert this==game;state['paused']=bool(read(esp+4));events.append(('pause',state['paused']));pop=4
        elif pc==0xcbfb7d:
            assert this in strings and uc.reg_read(UC_X86_REG_EDX)==actor_map
            assert [read(esp+4+4*i) for i in range(4)]==[0,0,0,1]
            assert state['movie'] and state['paused'] and state['hero'] and state['self']
            actual={key:read(entry+8) for entry,key in entries.items()}
            assert actual=={'HERO':0 if empty_hero else 0x20b000,'TROLL':0 if empty_troll else 0x20a000}
            events.append(('macro',strings[this],False,True,empty_hero,empty_troll));pop=16
        elif pc==0xbfea1a:assert read(esp+4)==0x48;result=0 if allocation_fails else thread;pop=0
        elif pc==0x4abe90:uc.mem_write(this,bytes(uc.mem_read(read(esp+4),12)));pop=4
        elif pc==0x4aa840:pop=0
        elif pc==0x99f570:
            prefix=strings[uc.reg_read(UC_X86_REG_EDX)];suffix=strings[read(esp+4)]
            strings[this]=prefix+suffix;result=this;pop=4
        elif pc==0xcdd450:
            assert this==thread and strings[read(esp+4)]=='ParentClass.WatchForRockTrollHit' and read(esp+8)==0;pop=8
        elif pc==0xcb7e50:
            assert this==parent and read(esp+4)==(0 if allocation_fails else thread) and strings[read(esp+8)]==''
            if not allocation_fails:assert read(thread+0x34)==0xec5140 and read(thread+0x38)==parent and read(thread+0x40)==0x207000
            assert state['paused'] and state['movie'];events.append(('thread','WatchForRockTrollHit',False,''));pop=8
        elif pc==0xec4ec0:assert not strings;events.append(('state',True))
        elif pc==0x6e7b80:assert this==movie and not state['paused'];state['movie']=False;events.append(('movie.destroy',empty_movie));pop=0
        elif pc==0xcdbfb0:assert this==actor_map and not state['movie'];events.append(('map.destroy',));pop=0
        elif pc==0x7e74d0:assert this==hero_resource;state['hero']=False;events.append(('hero.destroy',empty_hero));pop=0
        if pop is not None:
            uc.reg_write(UC_X86_REG_EAX,int(result));uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xec4cfb,0xec4eee,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==0xec4eee and not strings and not state['movie'] and not state['hero'] and state['self']
    return events
