import struct,unittest
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.bully_full_resource_candidate import generate

def native(hud,current=4,handle=-73):
    data=RData();recover(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdbb000,0xdbc000,0x99e000):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000);u.mem_write(0xdbb000,data.bytes_at(0xdbb000,4096));u.mem_write(0xdbc000,data.bytes_at(0xdbc000,4096))
    stack=0x21e000;owner=0x201000;game=0x202000;table=0x203000;parent=0x204000;api=0x205000
    def write(at,v):u.mem_write(at,struct.pack('<I',v&0xffffffff))
    def read(at):return struct.unpack('<I',u.mem_read(at,4))[0]
    def float_at(at):return struct.unpack('<f',u.mem_read(at,4))[0]
    write(owner+4,game);write(owner+20,parent);write(game,table);write(owner+28,current);write(table+(0x510 if hud else 0x1c8),api)
    for pc in (0x99ebf0,0x99eae0,api):u.mem_write(pc,b'\xc3')
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_EBP,owner);trace=[];strings={}
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=None;result=0
        if pc==0x99ebf0:
            strings[this]='' if read(sp+4)==0x122d70e else data.string_at(read(sp+4));trace.append(('new',strings[this]));pop=8
        elif pc==0x99eae0:trace.append(('destroy',strings.pop(this)));pop=0
        elif pc==api:
            assert this==game
            if hud:
                trace.append(('hud',float_at(sp+4),float_at(sp+8),bytes(u.mem_read(read(sp+12),4)),bytes(u.mem_read(read(sp+16),4)),strings[read(sp+20)],strings[read(sp+24)],float_at(sp+28)))
                result=handle;pop=28
            else:trace.append(('question',*(strings[read(sp+n)] for n in (4,8,12,16)),read(sp+20)));pop=20
        if pop is not None:
            ret=read(sp);uc.reg_write(UC_X86_REG_EAX,result&0xffffffff);uc.reg_write(UC_X86_REG_ESP,sp+4+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);start,end=(0xdbc3bf,0xdbc46a) if hud else (0xdbb7c8,0xdbb871)
    u.emu_start(start,end,count=10000);assert u.reg_read(UC_X86_REG_EIP)==end and not strings
    if hud:assert read(parent+100)==handle&0xffffffff
    return trace

class BullyLiteralScopeTests(unittest.TestCase):
    def test_original_question_flag_and_four_nested_string_scopes(self):
        question='TEXT_QST_048_GIVE_TEDDY_TO_BULLY';yes='TEXT_OBJECT_HERO_ANSWER_YES';no='TEXT_OBJECT_HERO_ANSWER_NO'
        self.assertEqual(native(False),[('new',''),('new',no),('new',yes),('new',question),('question',question,yes,no,'',1),('destroy',question),('destroy',yes),('destroy',no),('destroy','')])

    def test_original_hud_bgra_scale_float_conversion_and_returned_handle(self):
        for current,handle in ((4,-73),(0,0),(-3,-1),(16777217,42),(2147483647,2147483647)):
            with self.subTest(current=current):
                rounded=struct.unpack('<f',struct.pack('<f',current))[0]
                self.assertEqual(native(True,current,handle),[('new',''),('new','HUD_QUEST_ICON_GRANDSON'),('hud',rounded,0.0,bytes([0,255,0,255]),bytes([0,0,255,255]),'HUD_QUEST_ICON_GRANDSON','',1.0),('destroy','HUD_QUEST_ICON_GRANDSON'),('destroy','')])

    def test_composed_body_uses_scoped_capabilities(self):
        source,report=generate()
        self.assertIn('resources:AddBullyHealthBar(__native_entity_state:GetStateInt("InitialHealth"))',source)
        self.assertIn('resources:GiveBullyTeddyQuestion()',source)
        self.assertNotIn('quest:AddQuestInfoBar(',source);self.assertNotIn('quest:GiveHeroYesNoQuestion(',source)
        self.assertEqual(report['literalScopes']['hud']['scale'],1.0)

if __name__=='__main__':unittest.main()
