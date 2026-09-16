import struct,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.bully_actor_inputs import ALLIANCE
from tools.script_recovery.bully_runoff_movie import evidence
from tools.script_recovery.bully_full_resource_candidate import generate

def native_alliance(heroes):
    data=RData();evidence(data);u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0xdbc000,4096);u.mem_map(0x200000,0x10000)
    u.mem_write(0xdbc46a,data.bytes_at(0xdbc46a,0x2c));stack=0x20f000;owner=0x201000;game=0x202000;table=0x203000;me=0x207000
    def write(at,v):u.mem_write(at,struct.pack('<I',v))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    write(owner+4,game);write(game,table);write(table+0x118,0x204000);write(table+0x95c,0x204010)
    u.mem_write(0x204000,b'\xc3');u.mem_write(0x204010,b'\xc3')
    for r,v in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EBP,owner),(UC_X86_REG_EDI,me)):u.reg_write(r,v)
    trace=[];values=iter(heroes)
    def hook(uc,pc,size,user):
        if pc not in (0x204000,0x204010):return
        sp=uc.reg_read(UC_X86_REG_ESP);assert uc.reg_read(UC_X86_REG_ECX)==game
        if pc==0x204000:result=next(values);trace.append(('hero',result));pop=0
        else:trace.append(('ally',read(sp+4),read(sp+8)));result=0;pop=8
        ret=read(sp);uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbc46a,0xdbc496,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==0xdbc496
    return trace

class BullyActorInputTests(unittest.TestCase):
    def test_fresh_hero_each_direction_and_no_null_filter(self):
        for heroes in ((0x208000,0x209000),(0,0x209000),(0x208000,0),(0,0)):
            lua=LuaRuntime();lua.execute(ALLIANCE);quest=lua.table();resources=lua.table();trace=[];values=iter(heroes)
            def hero(_):
                value=next(values);trace.append(('hero',value));return value or None
            quest.GetHero=hero;resources.SetThingAsAlly=lambda _,a,b:trace.append(('ally',a or 0,b or 0))
            lua.globals().BullySetHeroAlliance(quest,resources,0x207000)
            self.assertEqual(native_alliance(heroes),trace)

    def test_native_mutated_map_actor_flags_hud_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.changed<address+size:
                    raw=bytearray(raw);raw[self.changed-address]^=1;return bytes(raw)
                return raw
        for pc in (0xdbc478,0xdbc485,0xdbc55f,0xdbc885,0xdbca2c,0xdbca6d,0xdbcb77,0xdbcbe0,0xdbccd0):
            data=Changed();data.changed=pc
            with self.assertRaises(ValueError):evidence(data)

    def test_correct_bound_clear_and_runtime_hud_values_are_emitted(self):
        source,_=generate()
        self.assertEqual(source.count('quest:ClearThingHasInformation(me)'),2)
        self.assertNotIn('iStack_94',source)
        self.assertNotIn('RemoveQuestInfoElement(0)',source)
        self.assertIn('quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBullyHealthCounter"))',source)
        self.assertIn('GetStateInt("HitsTaken"), -1.0, -1.0)',source)

if __name__=='__main__':unittest.main()
