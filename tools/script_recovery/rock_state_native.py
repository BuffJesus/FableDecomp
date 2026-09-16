"""Run original Init/OnPersist instructions with narrow engine/persistence doubles."""
import struct
import sys
from pathlib import Path
try:
    import unicorn
except ModuleNotFoundError:
    sys.path.append(str(Path(__file__).resolve().parents[2]/'work/runtime_re_tools'))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_candidate import verify
from tools.script_recovery.rock_state_program import FIELDS


def execute(role,initial=None,mode='save',saved=None):
    data=RData();verify(data)
    machine=Uc(UC_ARCH_X86,UC_MODE_32)
    machine.mem_map(0x200000,0x20000)
    owner=0x201000;game=0x202000;table=0x203000;callback=0x204000;context=0x205000;stop=0x206000;stack=0x21f000
    def write(address,value):machine.mem_write(address,struct.pack('<I',value&0xffffffff))
    def read(address):return struct.unpack('<I',machine.mem_read(address,4))[0]
    for page in (0xec3000,0xec4000,0x99e000,0x404000,0x410000):machine.mem_map(page,4096)
    address,size=(0xec3c50,77) if role=='Init' else (0xec4970,144)
    machine.mem_write(address,data.bytes_at(address,size))
    machine.mem_write(owner+0x48,b'\x7f'*16)
    for offset,name,kind in FIELDS:
        value=(initial or {}).get(name,123 if kind=='Int' else True)
        if kind=='Int':write(owner+offset,value)
        else:machine.mem_write(owner+offset,bytes([int(value)]))
    write(owner+0x40,game);write(game,table);write(table+0x8b8,callback)
    write(stack,stop);write(stack+4,context)
    machine.reg_write(UC_X86_REG_ECX,owner);machine.reg_write(UC_X86_REG_ESP,stack)
    events=[];storage=dict(saved or {})
    def hook(uc,pc,size,user):
        esp=uc.reg_read(UC_X86_REG_ESP);pop=None
        if pc==0x99ebf0:
            assert data.string_at(read(esp+4))=='Witchwood1' and read(esp+8)==0xffffffff
            pop=8
        elif pc==callback:
            assert uc.reg_read(UC_X86_REG_ECX)==game and read(esp+8)==0
            events.append(('generators','Witchwood1',False));pop=8
        elif pc==0x99eae0:pop=0
        elif pc in (0x4045c0,0x410be0):
            assert uc.reg_read(UC_X86_REG_ECX)==context
            name=data.string_at(read(esp+4));target=read(esp+8);default=read(esp+12)
            kind='Int' if pc==0x410be0 else 'Bool';width=4 if kind=='Int' else 1
            def value(at):return int.from_bytes(uc.mem_read(at,width),'little',signed=kind=='Int') if kind=='Int' else bool(uc.mem_read(at,1)[0])
            current=value(target);fallback=value(default)
            events.append(('transfer',name,kind,current,fallback))
            if mode=='save':storage[name]=current
            else:
                result=storage.get(name,fallback) if mode=='load' else fallback
                uc.mem_write(target,int(result).to_bytes(width,'little',signed=kind=='Int'))
            pop=12
        if pop is not None:
            uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4+pop)
    machine.hook_add(UC_HOOK_CODE,hook);machine.emu_start(address,stop,count=2000)
    assert machine.reg_read(UC_X86_REG_EIP)==stop
    result={name:(int.from_bytes(machine.mem_read(owner+offset,4),'little',signed=True) if kind=='Int' else bool(machine.mem_read(owner+offset,1)[0])) for offset,name,kind in FIELDS}
    assert machine.mem_read(owner+0x4b,2)==b'\x7f\x7f'
    return result,events,storage
