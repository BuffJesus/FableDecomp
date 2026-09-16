import itertools
import unittest
import sys
from pathlib import Path

try:
    import unicorn
except ModuleNotFoundError:
    sys.path.append(str(Path(__file__).resolve().parents[2]/'work/runtime_re_tools'))

from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_ECX,UC_X86_REG_EDI,UC_X86_REG_EBX,UC_X86_REG_EAX
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_affair_wife_hit_scopes import verify


def native_trace(data,block,direct,special,excluded):
    machine=Uc(UC_ARCH_X86,UC_MODE_32)
    for address,size in [(block['address']&~0xFFF,0x2000),(0x99E000,0x1000),(0x100000,0x10000),
                         (0x200000,0x1000),(0x210000,0x1000),(0x220000,0x1000)]:
        machine.mem_map(address,size)
    machine.mem_write(block['address'],data.bytes_at(block['address'],block['size']))
    def put(address,value):machine.mem_write(address,int(value).to_bytes(4,'little'))
    def get(address):return int.from_bytes(machine.mem_read(address,4),'little')
    put(0x200000,0x210000)
    targets={0x220010:('direct',direct,4),0x220020:('special',special,4),0x220030:('excluded',excluded,8)}
    for slot,target in [(0x54,0x220010),(0xA8,0x220020),(0xA4,0x220030)]:put(0x210000+slot,target)
    machine.reg_write(UC_X86_REG_ESP,0x108000)
    machine.reg_write(UC_X86_REG_EDI,0x200000)
    machine.reg_write(UC_X86_REG_EBX,0)
    live=[];identities={};events=[]
    def hook(uc,address,size,user):
        stack=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0x99EBF0:
            assert get(stack+4)==0x125D1C8 and get(stack+8)==0xFFFFFFFF
            assert receiver not in live
            identities[receiver]=len(identities)+1;live.append(receiver)
            events.append(('construct',identities[receiver]));pop=8;result=receiver
        elif address==0x99EAE0:
            assert live and live[-1]==receiver
            live.pop();events.append(('destroy',identities[receiver]));pop=0;result=0
        elif address in targets:
            name,result,pop=targets[address]
            assert receiver==0x200000
            if name=='excluded':assert get(stack+4)==14
            string=get(stack+(8 if name=='excluded' else 4))
            assert string in live
            events.append(('query',name));result=int(result)
        else:return
        destination=get(stack)
        uc.reg_write(UC_X86_REG_EAX,result)
        uc.reg_write(UC_X86_REG_ESP,stack+4+pop)
        uc.reg_write(UC_X86_REG_EIP,destination)
    machine.hook_add(UC_HOOK_CODE,hook)
    machine.emu_start(block['address'],block['address']+block['size'],count=1000)
    assert machine.reg_read(UC_X86_REG_EIP)==block['address']+block['size']
    assert not live
    assert machine.reg_read(UC_X86_REG_ESP)==0x108000
    return bool(machine.reg_read(UC_X86_REG_EAX)&255),events


class WifeHitScopeTests(unittest.TestCase):
    def test_native_truth_table_query_order_and_reverse_cleanup(self):
        data=RData();witness=verify(data)
        for block in witness['blocks']:
            for direct,special,excluded in itertools.product((False,True),repeat=3):
                with self.subTest(site=hex(block['address']),direct=direct,special=special,excluded=excluded):
                    result,events=native_trace(data,block,direct,special,excluded)
                    self.assertEqual(result,direct or (special and not excluded))
                    queries=['direct']+([] if direct else ['special']+(['excluded'] if special else []))
                    expected=[]
                    for index,query in enumerate(queries,1):expected.extend([('construct',index),('query',query)])
                    expected.extend(('destroy',index) for index in range(len(queries),0,-1))
                    self.assertEqual(events,expected)

    def test_changed_native_query_or_cleanup_rejects(self):
        from types import SimpleNamespace
        data=RData()
        for site in (0xDB2C35,0xDB2C88,0xDB2CD8,0xDB36FE,0xDB3751,0xDB37A1):
            def read(address,size):
                raw=data.bytes_at(address,size)
                if raw is not None and address<=site<address+size:
                    raw=bytearray(raw);raw[site-address]^=1;return bytes(raw)
                return raw
            with self.subTest(site=hex(site)),self.assertRaises(ValueError):
                verify(SimpleNamespace(bytes_at=read,string_at=data.string_at))
