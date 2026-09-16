"""Execute original native cancellation instructions with a bounded process double."""
import hashlib
import json
import struct
from pathlib import Path
import sys
# Reuse the isolated emulator package used by the existing native Maze proofs.
try:
    import unicorn
except ModuleNotFoundError:
    sys.path.append(str(Path(__file__).resolve().parents[2]/'work/runtime_re_tools'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from tools.script_recovery.lift_native_lua import RData


def validate(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('maze_teardown_witness.json').read_text())
    for region in witness['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Maze native teardown evidence changed: '+region['name'])
    return data,witness


def execute(kind='terminate', active=True, resumes=1, id_matches=True, data=None):
    data,_=validate(data)
    machine=Uc(UC_ARCH_X86,UC_MODE_32)
    machine.mem_map(0x200000,0x20000)
    process=0x201000;table=0x202000;owner=0x203000;head=0x204000;node=0x205000;key=0x206000
    callback=0x207000;stop=0x208000;stack=0x21f000
    def write(address,value):machine.mem_write(address,struct.pack('<I',value))
    def read(address):return struct.unpack('<I',machine.mem_read(address,4))[0]
    address,size=(0xa4b200,32) if kind=='terminate' else (0xcb7aa0,92)
    machine.mem_map(address&~4095,4096)
    machine.mem_write(address,data.bytes_at(address,size))
    write(process,table);write(table+4,callback)
    machine.mem_write(process+4,bytes((int(active),0)))
    write(owner+4,head);write(head,node);write(head+4,node)
    write(node,head);write(node+4,head);write(node+8,process)
    write(process+0x20,0x209000);write(key,0x209000);write(process+0x14,7)
    write(stack,stop);write(stack+4,key);write(stack+8,7 if id_matches else 8)
    machine.reg_write(UC_X86_REG_ESP,stack)
    machine.reg_write(UC_X86_REG_ECX,process if kind=='terminate' else owner)
    calls=[]
    def hook(uc,pc,size,user):
        if pc==callback:
            if read(table+4)!=pc or uc.reg_read(UC_X86_REG_ECX)!=process:
                raise AssertionError('Native process callback operand changed')
            calls.append({'terminationFlag':int(uc.mem_read(process+5,1)[0])})
            if resumes is not None and len(calls)>=resumes:uc.mem_write(process+4,b'\0')
            esp=uc.reg_read(UC_X86_REG_ESP);uc.reg_write(UC_X86_REG_EIP,read(esp));uc.reg_write(UC_X86_REG_ESP,esp+4)
    machine.hook_add(UC_HOOK_CODE,hook)
    machine.emu_start(address,stop,count=500)
    return {'returned':machine.reg_read(UC_X86_REG_EIP)==stop,'callbacks':calls,
        'active':bool(machine.mem_read(process+4,1)[0]),
        'terminationFlag':bool(machine.mem_read(process+5,1)[0]),
        'stillLinked':read(head)==node and read(node+8)==process}
