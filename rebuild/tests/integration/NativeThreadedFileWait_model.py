"""Standalone independent Wait boundary model, copied reviewed144-case domain."""
import itertools,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
def check(body):
    assert len(body)==10
    arena_base=0x200000;manager_page=0x13bc000;manager=0x13bc870;sp=arena_base+0x7000;stop=0x100100
    saved={UC_X86_REG_EBX:0x12345678,UC_X86_REG_ESI:0x87654321,UC_X86_REG_EDI:0xfedcba98,UC_X86_REG_EBP:0xabcdef01}
    cases=checkpoints=0;root_hits=child_hits=0
    for uid,this,mutation,returned in itertools.product((0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff),(0,arena_base+0x100,0x81234567,0xffffffff),(0,1,2),(0,0xfedcba98)):
        arena=bytearray((i*17+cases)&255 for i in range(0x8000));glob=bytearray((i*31+cases)&255 for i in range(0x1000))
        struct.pack_into('<II',arena,sp-arena_base,stop,uid)
        expected=bytearray(arena);expected_global=bytearray(glob)
        if mutation:
            expected[0x180]^=0x51;expected_global[0x9c0]^=0x73
        if mutation==2:struct.pack_into('<I',expected,sp+4-arena_base,0x01234567)
        u=Uc(UC_ARCH_X86,UC_MODE_32)
        for address,size in ((arena_base,0x8000),(manager_page,0x1000),(0x980000,0x20000),(0x100000,0x1000)):u.mem_map(address,size)
        u.mem_write(arena_base,bytes(arena));u.mem_write(manager_page,bytes(glob));u.mem_write(0x98f370,body)
        u.mem_write(0x98f1e0,b'\xc2\x04\x00')
        for reg,value in saved.items():u.reg_write(reg,value)
        u.reg_write(UC_X86_REG_ECX,this);u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_EFLAGS,0x246)
        entries=[]
        def hook(machine,pc,size,user):
            nonlocal checkpoints,root_hits,child_hits
            if pc==0x98f370:root_hits+=1
            if pc==0x98f1e0:
                assert not entries;entries.append(pc);child_hits+=1
                assert machine.reg_read(UC_X86_REG_ECX)==manager and machine.reg_read(UC_X86_REG_ESP)==sp
                assert bytes(machine.mem_read(sp,8))==struct.pack('<II',stop,uid)
                assert bytes(machine.mem_read(arena_base,0x8000))==bytes(arena)
                assert bytes(machine.mem_read(manager_page,0x1000))==bytes(glob)
                assert machine.reg_read(UC_X86_REG_EFLAGS)==0x246
                checkpoints+=1
                machine.mem_write(arena_base,bytes(expected));machine.mem_write(manager_page,bytes(expected_global))
                machine.reg_write(UC_X86_REG_EAX,returned);machine.reg_write(UC_X86_REG_ECX,0x76543210);machine.reg_write(UC_X86_REG_EDX,0x89abcdef)
            elif pc==stop:machine.emu_stop()
        u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x98f370,stop,count=32)
        assert entries==[0x98f1e0] and u.reg_read(UC_X86_REG_EIP)==stop and u.reg_read(UC_X86_REG_ESP)==sp+8
        assert all(u.reg_read(reg)==value for reg,value in saved.items())
        assert u.reg_read(UC_X86_REG_EAX)==returned and u.reg_read(UC_X86_REG_ECX)==0x76543210 and u.reg_read(UC_X86_REG_EDX)==0x89abcdef
        assert u.reg_read(UC_X86_REG_EFLAGS)==0x246
        assert bytes(u.mem_read(arena_base,0x8000))==bytes(expected) and bytes(u.mem_read(manager_page,0x1000))==bytes(expected_global)
        cases+=1;checkpoints+=1
    assert (cases,checkpoints,root_hits,child_hits)==(144,288,144,144)
    return dict(cases=cases,checkpoints=checkpoints,root_hits=root_hits,child_hits=child_hits)
