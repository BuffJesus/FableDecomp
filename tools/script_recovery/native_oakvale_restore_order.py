"""Check retail activation-tail ordering through the original virtual dispatcher."""
import hashlib
import json
import struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EIP


def prove(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('native_oakvale_restore_order_witness.json').read_text())
    for row in witness['regions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:
            raise ValueError('Native restore-order bytes changed')
    return witness


def execute(saved, attached):
    data=RData();witness=prove(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0x4b3000,0x2000);u.mem_map(0xcb7000,0x2000);u.mem_map(0x200000,0x10000)
    for row in witness['regions'][:2]:u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    def put(address,value):u.mem_write(address,struct.pack('<I',value))
    def get(address):return struct.unpack('<I',u.mem_read(address,4))[0]
    owner=0x201000;active=0x202000;table=0x203000;stack=0x20e000;save=0x204000
    put(owner,table);put(table+12,0x200100);put(table+4,0x200200)
    put(active+8,owner if attached else 0)
    put(stack+0x20,owner);put(stack+0x10,active);put(stack+0x14,save if saved else 0)
    events=[]
    for target in (0x200100,0x200200,0xcb8690):u.mem_write(target,b'\xc3')
    def hook(uc,pc,size,user):
        if pc not in (0x200100,0x200200,0xcb8690):return
        assert uc.reg_read(UC_X86_REG_ECX)==owner
        sp=uc.reg_read(UC_X86_REG_ESP);pop=0
        if pc==0xcb8690:assert get(sp+4)==save;pop=4
        events.append({0x200100:'Init',0x200200:'RegisterMain',0xcb8690:'LoadGameState'}[pc])
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.reg_write(UC_X86_REG_ESP,stack);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0x4b3fe8,0x4b400a,count=100)
    assert events==['Init','RegisterMain']+(['LoadGameState'] if saved and attached else [])
    assert u.reg_read(UC_X86_REG_ESP)==stack
    return events
