import unittest
from tools.script_recovery.native_oakvale_objective import verify
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_ESI,UC_X86_REG_EAX


def execute(active):
    data=RData();verify(data)
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdac000,0x891000,0x99e000,0x100000,0x200000,0x13b8000):uc.mem_map(page,0x1000)
    uc.mem_write(0xdac198,data.bytes_at(0xdac198,0x81));uc.mem_write(0x891880,data.bytes_at(0x891880,62))
    for a in (0x99ebf0,0x99ec30,0x99eae0,0x200e00):uc.mem_write(a,b'\xc3')
    def put(a,v):uc.mem_write(a,int(v).to_bytes(4,'little'))
    def get(a):return int.from_bytes(uc.mem_read(a,4),'little')
    thread,game,table,manager,current,stack=0x200000,0x200100,0x200200,0x200d00,0x200c00,0x100800
    put(thread+0x40,game);put(game,table);put(table+0xa3c,0x891880);put(table+0x4a0,0x200e00)
    put(0x13b89fc,manager);put(manager+0x88,current if active else 0)
    names={stack+0x18:'region2',stack+0x14:'region1',stack+0x10:'objective',stack+0x1c:'active'}
    events=[];live=set()
    def hook(machine,address,size,user):
        esp=machine.reg_read(UC_X86_REG_ESP);ecx=machine.reg_read(UC_X86_REG_ECX)
        count,result=0,0
        if address==0x891880:
            assert ecx==game and get(esp+4)==stack+0x1c
            # Three objective arguments were already pushed before the getter.
            assert [get(esp+8+i*4) for i in range(3)]==[stack+0x10,stack+0x14,stack+0x18]
            events.append(('get',));return
        if address==0x99ebf0:
            assert ecx in names and ecx not in live and get(esp+8)==0xffffffff
            text='' if data.bytes_at(get(esp+4),1)==b'\0' else data.string_at(get(esp+4))
            assert text==('TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01' if names[ecx]=='objective' else '')
            live.add(ecx);events.append(('construct',names[ecx],text));count,result=2,ecx
        elif address==0x99ec30:
            assert active and ecx==stack+0x1c and get(esp+4)==current+0x30
            live.add(ecx);events.append(('copy','active'));count,result=1,ecx
        elif address==0x200e00:
            assert ecx==game
            args=[get(esp+4+i*4) for i in range(4)]
            assert args==[stack+0x1c,stack+0x10,stack+0x14,stack+0x18] and len(live)==4
            events.append(('objective',));count=4
        elif address==0x99eae0:
            assert ecx in live;live.remove(ecx);events.append(('destroy',names[ecx]))
        else:return
        machine.reg_write(UC_X86_REG_ESP,esp+4+count*4);machine.reg_write(UC_X86_REG_EIP,get(esp));machine.reg_write(UC_X86_REG_EAX,result)
    uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ESI,thread)
    uc.hook_add(UC_HOOK_CODE,hook);uc.emu_start(0xdac198,0xdac219,count=150)
    assert uc.reg_read(UC_X86_REG_ESP)==stack and not live
    return events


class NativeOakvaleObjectiveTests(unittest.TestCase):
    def test_original_caller_and_actual_getter(self):
        for active in (False,True):
            self.assertEqual(execute(active),[
                ('construct','region2',''),('construct','region1',''),
                ('construct','objective','TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01'),('get',),
                ('copy','active') if active else ('construct','active',''),('objective',),
                ('destroy','active'),('destroy','objective'),('destroy','region1'),('destroy','region2')])

    def test_changed_native_operands_and_api_rejected(self):
        class Changed(RData):
            def bytes_at(self,address,size):
                raw=super().bytes_at(address,size)
                if address<=self.pc<address+size:
                    raw=bytearray(raw);raw[self.pc-address]^=1;raw=bytes(raw)
                return raw
        for pc in (0xdac1e5,0xdac1ef,0x8918b4,0x896b53,0x1260f0c+0xa3c,0x122d70e):
            data=Changed();data.pc=pc
            with self.assertRaises(ValueError):verify(data)


if __name__=='__main__':unittest.main()
