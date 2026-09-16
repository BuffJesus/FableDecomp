"""Original constructor/destructor timer regions, before vector/base cleanup."""
import hashlib
import itertools
import json
import unittest
from tools.script_recovery.lift_native_lua import RData,ROOT
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_ESI,UC_X86_REG_EAX


def execute(ambient,watch):
    data=RData()
    witness=json.loads((ROOT/'ghidra_out/script_recovery/new_oakvale_timer_retail_bytes.json').read_text())
    for row in (witness['constructor'],witness['destructor']):
        assert hashlib.sha256(data.bytes_at(int(row['address'],16),row['rangeSize'])).hexdigest().upper()==row['functionBytesSha256']
    machine=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in (0xdaa000,0xdbe000,0x100000,0x200000,0x143e000):machine.mem_map(page,0x1000)
    machine.mem_write(0xdaacae,data.bytes_at(0xdaacae,40));machine.mem_write(0xdbefc0,data.bytes_at(0xdbefc0,47))
    def put(a,v):machine.mem_write(a,(v&0xffffffff).to_bytes(4,'little'))
    def get(a):return int.from_bytes(machine.mem_read(a,4),'little')
    owner,stack,table=0x200000,0x100800,0x200400
    games=[0x200200+16*i for i in range(4)]
    for game in games:put(game,table)
    put(table+0x15c,0x200b00);put(table+0x160,0x200b10)
    for a in (0x200b00,0x200b10):machine.mem_write(a,b'\xc3')
    put(0x143e8f8,games[0]);events=[]
    def hook(uc,address,size,user):
        if address not in (0x200b00,0x200b10):return
        esp=uc.reg_read(UC_X86_REG_ESP);index=len(events)
        assert uc.reg_read(UC_X86_REG_ECX)==games[index]
        if address==0x200b00:
            value=(ambient,watch)[index];events.append(('register',index,value));count=0
        else:
            value=get(esp+4);value=value if value<0x80000000 else value-0x100000000
            events.append(('deregister',index,value));count=1
        if index<3:put(0x143e8f8,games[index+1])
        uc.reg_write(UC_X86_REG_EAX,value&0xffffffff);uc.reg_write(UC_X86_REG_ESP,esp+4+count*4);uc.reg_write(UC_X86_REG_EIP,get(esp))
    machine.hook_add(UC_HOOK_CODE,hook);machine.reg_write(UC_X86_REG_ESP,stack);machine.reg_write(UC_X86_REG_ESI,owner)
    machine.emu_start(0xdaacae,0xdaacd6,count=40)
    assert get(owner+0x104)==ambient&0xffffffff and get(owner+0x108)==watch&0xffffffff
    assert machine.reg_read(UC_X86_REG_ESP)==stack
    machine.reg_write(UC_X86_REG_ECX,owner);machine.emu_start(0xdbefc0,0xdbefef,count=40)
    assert machine.reg_read(UC_X86_REG_ESP)==stack-12
    return events


class NativeOakvaleTimersTests(unittest.TestCase):
    def test_original_timer_creation_and_reverse_destruction(self):
        for ambient,watch in itertools.product((-2147483648,-1,0,1,2147483647),repeat=2):
            self.assertEqual(execute(ambient,watch),[('register',0,ambient),('register',1,watch),
                                                   ('deregister',2,watch),('deregister',3,ambient)])


if __name__=='__main__':unittest.main()
