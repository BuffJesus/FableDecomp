import unittest
from types import SimpleNamespace
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EDI,UC_X86_REG_ECX,UC_X86_REG_EAX,UC_X86_REG_EBX,UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_affair_wife_talk import verify


class WifeTalkTests(unittest.TestCase):
    def test_original_instructions_preserve_result_across_string_destruction(self):
        data=RData()
        for block in verify(data)['blocks']:
            for result in (0,1,2,255):
                uc=Uc(UC_ARCH_X86,UC_MODE_32)
                for address,size in ((0xDB2000,0x2000),(0x99E000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
                    uc.mem_map(address,size)
                def put(address,value):uc.mem_write(address,value.to_bytes(4,'little'))
                def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
                put(0x200000,0x201000);put(0x20106C,0x202000)
                uc.mem_write(block['address'],data.bytes_at(block['address'],block['size']))
                uc.reg_write(UC_X86_REG_ESP,0x108000);uc.reg_write(UC_X86_REG_EDI,0x200000)
                live=[];events=[]
                def hook(machine,address,size,user):
                    stack=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
                    if address==0x99EBF0:
                        self.assertEqual((get(stack+4),get(stack+8)),(0x125D1C8,0xFFFFFFFF))
                        self.assertFalse(live);live.append(receiver);events.append('construct');pop=8;value=receiver
                    elif address==0x202000:
                        self.assertEqual(receiver,0x200000);self.assertEqual(live,[get(stack+4)])
                        events.append('query');pop=4;value=result
                    elif address==0x99EAE0:
                        self.assertEqual(live,[receiver]);live.clear();events.append('destroy');pop=0
                        value=0xDEADBEEF  # Destructor may overwrite EAX; BL must retain the query.
                    else:return
                    target=get(stack);machine.reg_write(UC_X86_REG_EAX,value)
                    machine.reg_write(UC_X86_REG_ESP,stack+4+pop);machine.reg_write(UC_X86_REG_EIP,target)
                uc.hook_add(UC_HOOK_CODE,hook)
                end=block['address']+block['size'];uc.emu_start(block['address'],end,count=100)
                with self.subTest(site=hex(block['address']),result=result):
                    self.assertEqual(uc.reg_read(UC_X86_REG_EIP),end)
                    self.assertEqual(uc.reg_read(UC_X86_REG_ESP),0x108000)
                    self.assertEqual(uc.reg_read(UC_X86_REG_EBX)&255,result)
                    self.assertEqual(events,['construct','query','destroy']);self.assertFalse(live)

    def test_changed_native_bytes_reject(self):
        data=RData()
        for block in verify(data)['blocks']:
            for offset in (2,11,25,32,34):
                def read(address,size):
                    raw=data.bytes_at(address,size)
                    if address==block['address']:
                        raw=bytearray(raw);raw[offset]^=1;return bytes(raw)
                    return raw
                with self.assertRaises(ValueError):verify(SimpleNamespace(bytes_at=read,string_at=data.string_at))
