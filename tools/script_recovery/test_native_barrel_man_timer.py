import copy
import unittest
from tools.script_recovery import test_native_barrel_man_resources as fixtures
from tools.script_recovery.native_barrel_man_timer import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX


class BarrelTimerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        fixtures.BarrelResourceTests.setUpClass()
        cls.function=fixtures.BarrelResourceTests.function;cls.data=fixtures.BarrelResourceTests.data

    def test_every_timer_path_has_one_owner(self):
        w=verify(self.function,self.data)
        for index in range(len(w['events'])):
            altered=copy.deepcopy(w);altered['events'].pop(index)
            with self.assertRaisesRegex(ValueError,'event coverage'):
                verify(self.function,self.data,altered)

    def test_actual_constructor_and_destructor_use_stored_id_and_fresh_global(self):
        w=verify(self.function,self.data)
        for timer_id in (0,1,73,0xFFFFFFFF):
            for changed_global in (False,True):
                uc=Uc(UC_ARCH_X86,UC_MODE_32)
                for address,size in ((0xCD4000,0x1000),(0x143E000,0x1000),(0x100000,0x10000),(0x200000,0x3000)):
                    uc.mem_map(address,size)
                for p in w['profiles']:uc.mem_write(p['address'],self.data.bytes_at(p['address'],p['size']))
                def put(address,value):uc.mem_write(address,int(value).to_bytes(4,'little'))
                def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
                stack,owner,first,second,table,register,deregister,stop=0x108000,0x200000,0x200100,0x200200,0x201000,0x202000,0x202010,0x202100
                put(first,table);put(second,table);put(table+0x15C,register);put(table+0x160,deregister)
                put(w['globalInterface'],first);events=[]
                def hook(machine,address,size,user):
                    if address not in (register,deregister):return
                    esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
                    if address==register:
                        assert receiver==first;events.append('register');pop=0;result=timer_id
                    else:
                        assert receiver==(second if changed_global else first) and get(esp+4)==timer_id
                        events.append('deregister');pop=4;result=0xDEADBEEF
                    machine.reg_write(UC_X86_REG_EAX,result);machine.reg_write(UC_X86_REG_ESP,esp+4+pop)
                    machine.reg_write(UC_X86_REG_EIP,get(esp))
                uc.hook_add(UC_HOOK_CODE,hook)
                for method in (0xCD4450,0xCD4470):
                    put(stack,stop);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,owner)
                    uc.emu_start(method,stop,count=50)
                    self.assertEqual(uc.reg_read(UC_X86_REG_ESP),stack+4)
                    self.assertEqual(get(owner),timer_id)
                    if changed_global:put(w['globalInterface'],second)
                self.assertEqual(events,['register','deregister'])
