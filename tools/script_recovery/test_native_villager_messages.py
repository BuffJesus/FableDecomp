import itertools
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_control_resource import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import native_trace,Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EAX,UC_X86_REG_EBX


class VillagerNativeMessageTests(unittest.TestCase):
    def test_hit_mask_low_byte_and_reverse_strings(self):
        data=RData();verify(data)
        for direct,special,excluded in itertools.product((0xABCD0000,0xABCD0001,0xABCD0080),repeat=3):
            actual,events=native_trace(data,dict(address=0xDAE026,size=0xCC),direct,special,excluded)
            d,s,e=bool(direct&255),bool(special&255),bool(excluded&255)
            self.assertEqual(actual,d or (s and not e))
            queries=['direct']+([] if d else ['special']+(['excluded'] if s else []))
            expected=[]
            for index,query in enumerate(queries,1):expected.extend([('construct',index),('query',query)])
            expected.extend(('destroy',index) for index in range(len(queries),0,-1))
            self.assertEqual(events,expected)

    def test_talk_result_survives_string_destructor(self):
        data=RData();verify(data)
        for value in (0xABCD0000,0xABCD0001,0xABCD0080,0xABCD00FF):
            uc=Uc(UC_ARCH_X86,UC_MODE_32)
            for address,size in ((0xDAE000,0x1000),(0x99E000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):uc.mem_map(address,size)
            uc.mem_write(0xDAE3B5,data.bytes_at(0xDAE3B5,0x27))
            def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
            def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
            put(0x200000,0x201000);put(0x20106c,0x202000)
            uc.reg_write(UC_X86_REG_ESP,0x108000);uc.reg_write(UC_X86_REG_EDI,0x200000);events=[]
            def hook(machine,address,size,user):
                if address not in (0x99EBF0,0x202000,0x99EAE0):return
                esp=machine.reg_read(UC_X86_REG_ESP);ecx=machine.reg_read(UC_X86_REG_ECX);pop=0;result=0x12345600
                if address==0x99EBF0:
                    assert ecx==0x108030 and get(esp+4)==0x125D1C8 and get(esp+8)==0xFFFFFFFF;pop=8;events.append('construct')
                elif address==0x202000:
                    assert ecx==0x200000 and get(esp+4)==0x108030;pop=4;result=value;events.append('query')
                else:assert ecx==0x108030;events.append('destroy')
                machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop);machine.reg_write(UC_X86_REG_EIP,get(esp))
            uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDAE3B5,0xDAE3DC,count=50)
            self.assertEqual(bool(uc.reg_read(UC_X86_REG_EBX)&255),bool(value&255))
            self.assertEqual(events,['construct','query','destroy'])
            self.assertEqual(uc.reg_read(UC_X86_REG_ESP),0x108000)
