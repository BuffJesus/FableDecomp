"""Original whole-function control flow with explicit world/movie/query boundaries."""
import hashlib
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_ESI,UC_X86_REG_EAX,UC_X86_REG_EBX


def native(start_delay,near_delay,cancel):
    data=RData();raw=data.bytes_at(0xdbeb20,1095)
    assert hashlib.sha256(raw).hexdigest()=='8dc72f018eb5e9a030d02a244b9b86416b83ae59c0864a3db40e0b5e9a3a13e7'
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbe000,0xcb7000,0x4aa000,0x100000,0x200000):uc.mem_map(page,0x1000)
    uc.mem_write(0xdbeb20,raw)
    for a in (0xcb7940,0x4aa840,0x200e00,0x200f00):uc.mem_write(a,b'\xc3')
    def put(a,v):uc.mem_write(a,int(v).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    owner,game,table,stack=0x200000,0x200100,0x200200,0x100900
    put(owner+0x40,game);put(game,table);put(table+0x1c,0x200e00);put(stack,0x200f00)
    events=[];polls=[0,0];queries=0;retained=False
    def hook(machine,address,size,user):
        nonlocal queries,retained
        if address==0x200f00:machine.emu_stop();return
        if address==0xdbeb30:
            alive=polls[0]>=start_delay;polls[0]+=1;events.append(('start',alive))
            machine.reg_write(UC_X86_REG_EBX,int(not alive));machine.reg_write(UC_X86_REG_EIP,0xdbebb3);return
        if address==0xdbebe4:
            events.append(('prepare',));machine.reg_write(UC_X86_REG_EIP,0xdbecc6);return
        if address==0xdbecdd:
            events.append(('trigger.new',));retained=True;machine.reg_write(UC_X86_REG_EIP,0xdbed3b);return
        if address in (0xdbed3b,0xdbed77):
            assert retained;near=polls[1]>=near_delay;polls[1]+=1;events.append(('near',near))
            machine.reg_write(UC_X86_REG_EAX,int(near));machine.reg_write(UC_X86_REG_EIP,0xdbed56 if address==0xdbed3b else 0xdbed92);return
        if address==0xdbeda5:
            events.append(('movie',));machine.reg_write(UC_X86_REG_EIP,0xdbeebc);return
        if address==0xdbeebc:
            events.append(('restore',));machine.reg_write(UC_X86_REG_EIP,0xdbef57);return
        if address not in (0xcb7940,0x200e00,0x4aa840):return
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        value=0
        if address==0xcb7940:
            assert receiver==owner;queries+=1;value=queries>=cancel;events.append(('term',value))
        elif address==0x200e00:assert receiver==game;events.append(('frame',))
        else:
            assert retained and receiver==stack-72+28;retained=False;events.append(('trigger.destroy',))
        machine.reg_write(UC_X86_REG_EAX,int(value));machine.reg_write(UC_X86_REG_ESP,esp+4);machine.reg_write(UC_X86_REG_EIP,get(esp))
    uc.hook_add(UC_HOOK_CODE,hook);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,owner)
    uc.emu_start(0xdbeb20,0x200f01,count=1500)
    assert uc.reg_read(UC_X86_REG_ESP)==stack+4 and not retained
    return events


def lua_trace(start_delay,near_delay,cancel,source=None):
    lua=LuaRuntime()
    lua.globals().__post_attack_movie=lambda *args:events.append(('movie',))
    if source is None:body=lua.execute(Path(__file__).with_name('post_attack_body.lua').read_text())
    else:body=lua.execute(source+'\nplayPostAttackDadCutscene=__post_attack_movie\nreturn runPostAttack')
    q,r=lua.table(),lua.table();events=[];polls=[0,0];queries=0
    def start(_):value=polls[0]>=start_delay;polls[0]+=1;events.append(('start',value));return value
    def near(_,id):assert id==28;value=polls[1]>=near_delay;polls[1]+=1;events.append(('near',value));return value
    def term(_):
        nonlocal queries
        queries+=1;value=queries>=cancel;events.append(('term',value));return value
    def music(_,id):
        assert id in (45,57)
        if id==45:events.append(('prepare',))
    def lookup(_,key):assert key=='MK_OVI_DADTRIGGER';events.append(('trigger.new',));return 28
    def destroy(_,id):assert id==28;events.append(('trigger.destroy',))
    def limbo(_,value):
        if not value:events.append(('restore',))
    r.PostAttackStartIsAlive=start;r.PostAttackHeroNearTrigger=near;r.NewThingFromScriptName=lookup;r.DestroyThing=destroy
    r.TeleportToPostAttackStart=lambda _:None;r.SetPostAttackVillageLimbo=limbo
    q.NewScriptFrame=lambda _:events.append(('frame',));q.IsActiveThreadTerminating=term;q.CacheMusicSet=music
    q.DisplayMoneyBag=lambda _,value:None;q.TakeObjectFromHero=lambda _,key:None;q.AddLogbookStoryEntry=lambda _,index:None
    q.CameraResetToViewBehindHero=lambda _,value:None;q.CameraDefault=lambda _:None;q.FadeScreenIn=lambda _:None
    q.SetTimeAsStopped=lambda _,value:None;q.DeactivateQuest=lambda _,key,value:None
    q.ResetToDefaultTheme=lambda _,value:None;q.StopOverrideMusic=lambda _,value:None
    lua.globals().playPostAttackDadCutscene=lambda *args:events.append(('movie',))
    body(q,r);return events


class PostAttackDispatcherTests(unittest.TestCase):
    def test_original_wait_branches_cancellation_and_retained_trigger(self):
        for start,near,cancel in itertools.product((0,1,3),(0,1,3),range(1,13)):
            self.assertEqual(lua_trace(start,near,cancel),native(start,near,cancel),(start,near,cancel))


if __name__=='__main__':unittest.main()
