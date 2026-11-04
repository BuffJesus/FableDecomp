"""Execute original handle save/reloads and RemoveThing call, with a call-boundary stub."""
import hashlib
import json
from pathlib import Path
import struct
import sys
W=Path(__file__).resolve().parent
R=W.parents[1]
sys.path.insert(0,str(R))
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESI,UC_X86_REG_EBP,UC_X86_REG_ESP
from tools.script_recovery.lift_native_lua import RData
data=RData();witness=json.loads((R/'refs/script_recovery/trader_escort/units/Q_TraderEscort.json').read_text(encoding='utf-8'))['reviewedSelfOperands']['0x00e02e50']
body=data.bytes_at(0xe02e50,0x545)
assert hashlib.sha256(body).hexdigest()==witness['nativeSha256']
results=[]
for reload in (None,0xe02f87,0xe02f8d):
    for owner in (0x201000,0x202000,0x203000):
        u=Uc(UC_ARCH_X86,UC_MODE_32)
        u.mem_map(0xe02000,0x2000);u.mem_write(0xe02e50,body)
        u.mem_map(0x200000,0x30000)
        stack,game,table,callee=0x22e000,0x204000,0x205000,0x206000
        write=lambda a,v:u.mem_write(a,struct.pack('<I',v))
        read=lambda a:struct.unpack('<I',u.mem_read(a,4))[0]
        write(owner+4,game);write(game,table);write(table+0x1b0,callee)
        u.mem_write(callee,b'\xc2\x0c\x00')
        u.reg_write(UC_X86_REG_ECX,owner);u.reg_write(UC_X86_REG_ESP,stack)
        # Stop before the first constructor call. Native save uses the temporary
        # argument stack depth; after its purge and later EBX push, depth agrees.
        u.emu_start(0xe02e50,0xe02e6d,count=30)
        assert read(stack-0x24)==owner+8
        assert u.reg_read(UC_X86_REG_EBP)==owner+8
        if reload:
            u.reg_write(UC_X86_REG_EBP,0xdeadbeef)
            u.emu_start(reload,reload+4,count=1)
            assert u.reg_read(UC_X86_REG_EBP)==owner+8
        assert u.reg_read(UC_X86_REG_ESP)==stack-0x40
        calls=[]
        def record(uc,address,size,user):
            if address==callee:
                sp=uc.reg_read(UC_X86_REG_ESP)
                calls.append([uc.reg_read(UC_X86_REG_ECX),read(sp+4),read(sp+8),read(sp+12)])
        u.hook_add(UC_HOOK_CODE,record)
        u.emu_start(0xe03362,0xe03372,count=20)
        assert calls==[[game,owner+8,0,1]],calls
        assert u.reg_read(UC_X86_REG_ESP)==stack-0x40
        results.append({'owner':hex(owner),'reload':hex(reload) if reload else None,
                        'receiver':hex(game),'actor':hex(calls[0][1]),'flags':calls[0][2:]})
Path(sys.argv[1]).write_text(json.dumps({
    'cases':results,'scope':'Disjoint original prologue/reload/call windows; callee stub, not full Main execution'},indent=2),encoding='utf-8')
print('Nine original retail save/reload/call sequence cases passed; stack balanced')
