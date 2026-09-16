import unittest
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX,UC_X86_REG_ESI
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_loop_termination import recover
from pathlib import Path


class VillagerLoopTerminationTests(unittest.TestCase):
    def test_original_branches_use_nonzero_al(self):
        data=RData()
        for start,size,continued in ((0xDADFF9,15,0xDAE008),(0xDAE996,15,0xDAE00A)):
            for result in range(256):
                uc=Uc(UC_ARCH_X86,UC_MODE_32)
                for address,length in ((0xDAD000,0x2000),(0xF35000,0x1000),(0x100000,0x10000)):uc.mem_map(address,length)
                uc.mem_write(start,data.bytes_at(start,size));uc.reg_write(UC_X86_REG_ESP,0x108000);uc.reg_write(UC_X86_REG_ESI,0x101000)
                destination=[]
                def hook(machine,address,length,user):
                    if address in (continued,0xDAE9A5):destination.append(address);machine.emu_stop()
                    elif address==0xF35B30:
                        esp=machine.reg_read(UC_X86_REG_ESP);ret=int.from_bytes(machine.mem_read(esp,4),'little')
                        machine.reg_write(UC_X86_REG_EAX,0xAABBCC00|result)
                        machine.reg_write(UC_X86_REG_ESP,esp+4);machine.reg_write(UC_X86_REG_EIP,ret)
                uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(start,0xDAF000,count=20)
                self.assertEqual(destination,[0xDAE9A5 if result else continued])

    def test_source_uses_termination_boolean(self):
        source=Path('refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Villager.lua').read_text()
        output,_=recover(source,RData())
        self.assertNotIn('extraout_AL_00',output);self.assertNotIn('extraout_AL_32',output)
        self.assertEqual(output.count('cVar6 = not alive'),2)
        self.assertIn('if cVar6 then',output)
