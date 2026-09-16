"""Execute original OnPersist and verify its one-byte value/default arguments."""
import hashlib
import struct
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EIP


def execute(initial=False,reading=False,saved=None):
    d=RData();body=d.bytes_at(0xdaada0,33)
    if hashlib.sha256(body).hexdigest()!='1c64d4f6e084ec19a985b5c030ba9fc35b6c711e5c69be7fc336c883d1e3702f':raise ValueError('Oakvale OnPersist bytes changed')
    if d.string_at(0x12d7a58)!='AttackOver':raise ValueError('Oakvale persistence key changed')
    u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0xdaa000,4096);u.mem_map(0x404000,4096);u.mem_map(0x200000,0x10000)
    u.mem_write(0xdaada0,body);u.mem_write(0x4045c0,b'\xc3')
    owner=0x201000;context=0x202000;finish=0x203000;stack=0x20e000
    def put(a,v):u.mem_write(a,struct.pack('<I',v))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    u.mem_write(owner,bytes([0xa5])*256);u.mem_write(owner+0x50,bytes([initial]));before=bytes(u.mem_read(owner,256))
    put(stack,finish);put(stack+4,context);calls=[];storage=saved
    def hook(uc,pc,size,user):
        nonlocal storage
        if pc!=0x4045c0:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        assert uc.reg_read(UC_X86_REG_ECX)==context and get(sp+4)==0x12d7a58 and get(sp+8)==owner+0x50
        fallback=bytes(u.mem_read(get(sp+12),1));assert fallback==b'\0'
        value=bool(u.mem_read(owner+0x50,1)[0]);calls.append(('AttackOver',value,False))
        if reading:u.mem_write(owner+0x50,bytes([False if storage is None else storage]))
        else:storage=value
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+16)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,owner);u.hook_add(UC_HOOK_CODE,hook)
    u.emu_start(0xdaada0,finish,count=100)
    after=bytes(u.mem_read(owner,256));assert before[:0x50]==after[:0x50] and before[0x51:]==after[0x51:]
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and len(calls)==1
    return dict(attackOver=bool(after[0x50]),saved=storage,transfers=calls)
