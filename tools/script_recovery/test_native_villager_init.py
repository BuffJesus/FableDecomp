import itertools
import unittest
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_villager_init import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EAX


class NativeVillagerInitTests(unittest.TestCase):
    def test_native_bound_actor_and_state_reset_without_null_queries(self):
        data=RData();w=verify(data)
        for storage,hero,oldHit in itertools.product((0,1,0xBADBAD),(0,0x202800),(0,255)):
            uc=Uc(UC_ARCH_X86,UC_MODE_32)
            for address,size in ((0xDAD000,0x2000),(0x100000,0x10000),(0x200000,0x4000)):uc.mem_map(address,size)
            uc.mem_write(w['address'],data.bytes_at(w['address'],w['size']))
            def put(a,v):uc.mem_write(a,v.to_bytes(4,'little'))
            def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
            actor,thread,game,table=0x200008,0x200000,0x201000,0x202000
            put(thread+4,game);put(game,table);put(actor+4,storage);put(actor+8,storage)
            uc.mem_write(thread+0x1c,bytes([oldHit]))
            targets={0x203000:('damage',0x810,2),0x203010:('kill',0x814,3),0x203020:('combo',0x838,2),
                     0x203030:('hero',0x118,0),0x203040:('ally',0x95c,2)}
            for target,(_,slot,_) in targets.items():put(table+slot,target)
            events=[]
            def hook(machine,address,size,user):
                if address not in targets:return
                name,_,n=targets[address];esp=machine.reg_read(UC_X86_REG_ESP)
                assert machine.reg_read(UC_X86_REG_ECX)==game and uc.mem_read(thread+0x1c,1)==b'\0'
                args=[get(esp+4+i*4) for i in range(n)]
                assert args==({'damage':[actor,0],'kill':[actor,0,0],'combo':[actor,0],'hero':[],'ally':[actor,hero]}[name])
                events.append(name);machine.reg_write(UC_X86_REG_EAX,hero if name=='hero' else 0xBADBAD)
                machine.reg_write(UC_X86_REG_ESP,esp+4+n*4);machine.reg_write(UC_X86_REG_EIP,get(esp))
            uc.reg_write(UC_X86_REG_ESP,0x108000);uc.reg_write(UC_X86_REG_ECX,thread)
            uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xDADF00,0xDADF51,count=100)
            self.assertEqual(events,['damage','kill','combo','hero','ally'])
            self.assertEqual((get(actor+4),get(actor+8)),(storage,storage))
            self.assertEqual(uc.reg_read(UC_X86_REG_ESP),0x108000)
