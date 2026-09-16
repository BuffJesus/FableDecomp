import struct,unittest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.bully_hit_conversation import SOURCE
from tools.script_recovery.bully_full_resource_candidate import generate
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData

def native(conversation):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0xdbc000,4096);u.mem_map(0x99e000,4096);u.mem_map(0x200000,0x20000)
    u.mem_write(0xdbc4b9,data.bytes_at(0xdbc4b9,0x99))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;me=0x204000
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    def signed(v):return v-0x100000000 if v>0x7fffffff else v
    write(owner+4,game);write(game,table)
    for offset,pc in ((0x5b0,0x205000),(0x5b4,0x205010),(0x5b8,0x205020)):write(table+offset,pc);u.mem_write(pc,b'\xc3')
    for pc in (0x99ebf0,0x99eae0):u.mem_write(pc,b'\xc3')
    for reg,value in ((UC_X86_REG_ESP,stack),(UC_X86_REG_EBP,owner),(UC_X86_REG_EDI,me)):u.reg_write(reg,value)
    trace=[];strings={}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            strings[this]=data.string_at(read(sp+4));trace.append(('string.new',strings[this]));pop=8
        elif pc==0x99eae0:trace.append(('string.destroy',strings.pop(this)));pop=0
        elif pc==0x205000:
            assert this==game and (read(sp+4),read(sp+8),read(sp+12))==(me,0,0)
            trace.append(('conversation',));result=conversation;pop=12
        elif pc==0x205010:
            assert this==game and read(sp+8)==stack+44
            trace.append(('person',signed(read(sp+4)),'victim'));pop=8
        elif pc==0x205020:
            assert this==game and read(sp+12)==0 and read(sp+16)==me and read(sp+20)==stack+44
            trace.append(('line',signed(read(sp+4)),strings[read(sp+8)],'me','victim',False));pop=20
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xdbc4b9,0xdbc552,count=1000)
    assert u.reg_read(UC_X86_REG_EIP)==0xdbc552 and not strings
    return trace

class BullyHitConversationTests(unittest.TestCase):
    def test_original_ids_actors_flags_and_two_string_scopes(self):
        for conversation in (-1,0,73,2147483647):
            lua=LuaRuntime();lua.execute(SOURCE);r=lua.table();trace=[]
            def begin(_,actor,a,b):
                self.assertEqual((actor,a,b),('me',False,False));trace.append(('conversation',));return conversation
            r.NewConversation=begin;r.AddConversationPerson=lambda _,c,p:trace.append(('person',c,p))
            def line(_,c,key,actor,listener,flag):
                trace.extend([('string.new',key),('line',c,key,actor,listener,flag),('string.destroy',key)])
            r.AddConversationLine=line;lua.globals().BullyHitConversation(r,'me','victim')
            self.assertEqual(native(conversation),trace)

    def test_body_composes_conversation_without_unknown_participants(self):
        source,report=generate()
        self.assertIn('BullyHitConversation(resources, me, r1)',source)
        self.assertNotIn('ppVar11 = quest:AddNewConversation(nil',source)
        self.assertEqual(report['hitConversation']['addPerson'],0xdbc4d6)

if __name__=='__main__':unittest.main()
