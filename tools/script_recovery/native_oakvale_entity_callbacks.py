"""Prove active-process forwarding to native entity callback vtable slots."""
import hashlib,json,struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EIP


def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('native_oakvale_entity_callbacks_witness.json').read_text())
    for row in w['regions']+w['entityTables']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:
            raise ValueError('Native entity callback bytes changed')
    table=struct.unpack('<7I',data.bytes_at(0x12c3594,28))
    assert table[5:7]==(0xce1090,0xce10a0)
    for row in w['entityTables']:
        slots=struct.unpack('<7I',data.bytes_at(row['address'],28))
        assert slots[4]==0xcdebc0 and slots[6]==0xcdebe0
        assert slots[5]==(0xdb7db0 if row['name']=='NOVI_Barrel' else 0xdb8260 if row['name']=='OVI_DeadFather' else 0xcdebd0)
    return w


def execute(interrupted=False,present=True):
    data=RData();w=prove(data);u=Uc(UC_ARCH_X86,UC_MODE_32)
    u.mem_map(0xce1000,4096);u.mem_map(0x200000,0x10000)
    for row in w['regions'][:2]:u.mem_write(row['address'],data.bytes_at(row['address'],row['size']))
    def put(a,v):u.mem_write(a,struct.pack('<I',v))
    process=0x201000;entity=0x202000;table=0x203000;stack=0x20e000;done=0x200100;callback=0x200200
    put(process+0x34,entity if present else 0);put(entity,table);put(table+(0x18 if interrupted else 0x14),callback);put(stack,done)
    u.mem_write(callback,b'\xc3');events=[]
    def hook(uc,pc,size,user):
        if pc==callback:
            assert uc.reg_read(UC_X86_REG_ECX)==entity;events.append('OnInterrupted' if interrupted else 'OnPredicateFail')
    u.hook_add(UC_HOOK_CODE,hook);u.reg_write(UC_X86_REG_ECX,process);u.reg_write(UC_X86_REG_ESP,stack)
    u.emu_start(0xce10a0 if interrupted else 0xce1090,done,count=30)
    assert len(events)==int(present) and u.reg_read(UC_X86_REG_ESP)==stack+4
    return events
