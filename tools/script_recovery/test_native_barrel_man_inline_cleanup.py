import itertools
import unittest
from types import SimpleNamespace
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EBP,UC_X86_REG_ECX,UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_barrel_man_inline_cleanup import verify


def execute(data,counts,empty,inline):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in ((0xDB6000,0x1000),(0x4AA000,0x1000),(0x7E7000,0x1000),(0x99A000,0x1000),
                         (0xBFE000,0x1000),(0x100000,0x10000),(0x200000,0x1000),(0x300000,0x1000),(0x900000,0x1000)):
        uc.mem_map(address,size)
    for profile in verify(data)['profiles']:uc.mem_write(profile['address'],data.bytes_at(profile['address'],profile['size']))
    def put(address,value):uc.mem_write(address,int(value).to_bytes(4,'little'))
    def get(address):return int.from_bytes(uc.mem_read(address,4),'little')
    stack=0x108000;slots=(48,36,20);infos=[]
    for index,(slot,count,is_empty) in enumerate(zip(slots,counts,empty)):
        info=0x200000+index*16;infos.append(info)
        data_offset=8 if slot==20 else 4
        put(stack+slot,0x127094C if slot==20 else 0x1238C8C)
        put(stack+slot+data_offset,0 if is_empty else 0x400000+index*16)
        put(stack+slot+data_offset+4,0 if count is None else info)
        put(info,0 if count is None else count);put(info+4,0x300000+index*16)
        put(info+8,0 if is_empty else 0x400000+index*16)
    calls=bytearray()
    for slot in slots:
        calls+=b'\xb9'+(stack+slot).to_bytes(4,'little')
        call_address=0x900000+len(calls);target=0x7E74D0 if slot==20 else 0x4AA840
        calls+=b'\xe8'+((target-call_address-5)&0xFFFFFFFF).to_bytes(4,'little')
    uc.mem_write(0x900000,bytes(calls));uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_EBP,0)
    events=[]
    def hook(machine,address,size,user):
        esp=machine.reg_read(UC_X86_REG_ESP);receiver=machine.reg_read(UC_X86_REG_ECX)
        if address in (0x300000,0x300010,0x300020):
            index=(address-0x300000)//16
            assert get(infos[index])==0 and receiver==get(infos[index]+8)
            events.append(('delete',slots[index],receiver))
        elif address==0xBFE9BC:
            info=get(esp+4);assert get(info)==0
            events.append(('free',slots[infos.index(info)]))
        elif address in (0x99A2E0,0x99A430):
            slot=receiver-stack;assert slot in slots
            offset=8 if slot==20 else 4
            assert get(receiver+offset)==get(receiver+offset+4)==0
            assert get(receiver)==(0x126008C if slot==20 else 0x1238C8C)
            events.append(('base',slot))
            if address==0x99A2E0:return  # Execute the actual seven-byte base destructor.
        else:return
        machine.reg_write(UC_X86_REG_EIP,get(esp));machine.reg_write(UC_X86_REG_ESP,esp+4)
    uc.hook_add(UC_HOOK_CODE,hook)
    start,end=(0xDB694C,0xDB6A03) if inline else (0x900000,0x900000+len(calls))
    uc.emu_start(start,end,count=500)
    assert uc.reg_read(UC_X86_REG_EIP)==end and uc.reg_read(UC_X86_REG_ESP)==stack
    return events,[get(info) for info in infos],bytes(uc.mem_read(stack+20,40))


class BarrelInlineCleanupTests(unittest.TestCase):
    def test_native_inline_and_destructor_calls_match(self):
        data=RData()
        for counts in itertools.product((None,1,2),repeat=3):
            for empty in itertools.product((False,True),repeat=3):
                with self.subTest(counts=counts,empty=empty):
                    self.assertEqual(execute(data,counts,empty,True),execute(data,counts,empty,False))

    def test_changed_release_order_or_destructor_rejects(self):
        data=RData()
        for site in (0xDB695D,0xDB6997,0xDB69CD,0x4AA850,0x7E74DA):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.assertRaises(ValueError):verify(SimpleNamespace(bytes_at=read))
