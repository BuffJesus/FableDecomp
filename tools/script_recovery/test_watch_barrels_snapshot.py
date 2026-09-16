"""Native snapshot refresh/count and actual vector destructor; loop is separately compared."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EBX
from tools.script_recovery.lift_native_lua import RData


def native(data,pending,total,allocated,cancel,helper_cleanup):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in ((0xdbe000,0x2000),(0xcb7000,0x1000),(0x99e000,0x1000),(0x8ac000,0x1000),(0xbfe000,0x1000),(0x100000,0x10000),(0x200000,0x6000)):uc.mem_map(a,n)
    uc.mem_write(0xdbe890,data.bytes_at(0xdbe890,647));uc.mem_write(0x8ac970,data.bytes_at(0x8ac970,50))
    def put(a,v):uc.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    stack,quest,game,table,array=0x108000,0x200000,0x201000,0x202000,0x204000
    put(quest+0x40,game);put(game,table);put(table+0x12c,0x203000);put(table+0x1c,0x203010)
    put(stack+64,0x203ff0);put(0x205000,0x203020)
    for i in range(total):put(array+12*i,0x205000)
    calls={0x99ebf0:'key.new',0x99eae0:'key.close',0x203000:'refresh',0x203010:'frame',0xcb7940:'term',0x203020:'element.close',0xbfea14:'free'}
    for a in (*calls,0x203ff0):uc.mem_write(a,b'\xc3')
    events=[];counts=dict(refresh=0,term=0);ended=[];key=False
    def hook(machine,a,n,user):
        nonlocal key
        if a==0x203ff0:ended.append(True);machine.emu_stop();return
        if a==0xdbe960:
            value=machine.reg_read(UC_X86_REG_EBX);assert value==total
            events.extend([('count',value),('loop',value)])
            machine.reg_write(UC_X86_REG_EIP,0xdbeb07 if helper_cleanup else 0xdbead4);return
        if a not in calls:return
        name=calls[a];esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        args=lambda n:[get(esp+4+i*4) for i in range(n)]
        pop,result=0,0;event=(name,)
        if name=='key.new':
            assert receiver==stack+12 and args(2)==[0x12d82ac,0xffffffff] and not key;key=True;pop=2
        elif name=='key.close':assert receiver==stack+12 and key;key=False
        elif name=='refresh':
            assert receiver==game and args(2)==[stack+12,stack+28] and key
            counts['refresh']+=1;result=1 if counts['refresh']>pending else 0
            start=array if allocated or total else 0
            put(stack+28,start);put(stack+32,start+12*total);put(stack+36,start+12*total)
            event=('refresh',result);pop=2
        elif name=='frame':assert receiver==game
        elif name=='term':
            assert receiver==quest;counts['term']+=1;result=int(counts['term']==cancel);event=('term',bool(result))
        elif name=='element.close':
            assert array<=receiver<array+12*total and (receiver-array)%12==0 and args(1)==[0]
            event=(name,(receiver-array)//12);pop=1
        elif name=='free':assert args(1)==[array];pop=0 # cdecl: original caller removes the argument
        events.append(event);machine.reg_write(UC_X86_REG_EAX,result)
        machine.reg_write(UC_X86_REG_ESP,esp+4+4*pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.reg_write(UC_X86_REG_ESP,stack+64);uc.reg_write(UC_X86_REG_ECX,quest)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdbe890,0xdbeb17,count=3000)
    assert ended==[True] and not key and uc.reg_read(UC_X86_REG_ESP)==stack+68
    return events


def readable(pending,total,allocated,cancel,helper_cleanup):
    lua=LuaRuntime();events=[];counts=dict(refresh=0,term=0);snapshot=lua.table();q,r=lua.table(),lua.table()
    def refresh(*args):
        counts['refresh']+=1;result=1 if counts['refresh']>pending else 0
        events.extend([('key.new',),('refresh',result),('key.close',)]);return result
    def term(*args):counts['term']+=1;v=counts['term']==cancel;events.append(('term',v));return v
    def close(*args):
        events.extend(('element.close',i) for i in range(total))
        if allocated or total:events.append(('free',))
    snapshot.Refresh=refresh;snapshot.Count=lambda *args:events.append(('count',total)) or total;snapshot.Close=close
    r.NewBarrelWatchSnapshot=lambda *args:snapshot;q.NewScriptFrame=lambda *args:events.append(('frame',));q.IsActiveThreadTerminating=term
    lua.globals().processBarrelBreaks=lambda quest,resources,count,deed:events.append(('loop',count))
    body=lua.execute(Path(__file__).with_name('watch_barrels_body.lua').read_text());body(q,r,lambda *args:None)
    return events


class WatchBarrelsSnapshotTests(unittest.TestCase):
    def test_original_refresh_count_and_both_cleanup_implementations(self):
        data=RData()
        self.assertEqual(hashlib.sha256(data.bytes_at(0x8ac970,50)).hexdigest(),'804b6b863d9405337d2e256db023e9c512614766507073be77773f0d217e2a6d')
        self.assertEqual(hashlib.sha256(data.bytes_at(0xdbe890,647)).hexdigest(),'3c48ce5c57e30e903c5e6790f7dfd2d4a5cf9b7664f433134d6909840fd06bf2')
        for case in itertools.product((0,1,3),(0,1,4),(False,True),range(0,6),(False,True)):
            with self.subTest(case=case):self.assertEqual(readable(*case),native(data,*case))
