"""Execute retail entity allocation, Thing retention and final counted release."""
import hashlib
import json
import struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_state_native import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_EAX,UC_X86_REG_EIP

REGIONS=((0xdb7d00,145,'d1ceccca0201b8c60688933a7f1ca825abf36acd09916b08fd2092ccc65e4dda'),
 (0x4abe90,33,'3c497d08782ebf50f8d8813805f1e16fc8c68869028ff7033e4ecbfed3ef1da9'),
 (0xf35b40,73,'65047e8c7272b866715afaae6293c918b3e75910f9ebe28a58c29acbe1138920'),
 (0xdb7df0,30,'8c89cb0a58898cfd7231163544bc2eda67eb42a1a350f5174f19b6b701ceef44'),
 (0xcdee00,11,'87956db89092b2e0797d42672f056ff7c01c2b2cf6a4716f6975fc6a0abf209e'),
 (0xce1000,53,'ae1a047b11beb47cd20b6eac30a3377fccd2dcc6740adbc09f634f0db66cc265'),
 (0x99a2e0,7,'b391b1043a81dbc361b75b6ef3b6f5551ecf4643eb44e742e4f605ce2382115a'),
 (0x4aa840,66,'5aa58f8e6b624fb6abbc2496c0f63791f6a7ae480b3e86d9e93f148d23fe4afa'))


def prove(data=None):
    d=data or RData()
    for address,size,sha in REGIONS:
        if hashlib.sha256(d.bytes_at(address,size)).hexdigest()!=sha:raise ValueError('Entity ownership native bytes changed')
    if int.from_bytes(d.bytes_at(0x12d94f0,4),'little')!=0xdb7df0:raise ValueError('Native Barrel destructor slot changed')
    witness=json.loads(Path(__file__).with_name('native_oakvale_entity_factories.json').read_text())
    from tools.script_recovery.lift_native_lua import ROOT
    if hashlib.sha256((ROOT/witness['bindingWitness']).read_bytes()).hexdigest()!=witness['bindingWitnessSha256']:raise ValueError('Entity factory binding witness changed')
    for row in witness['factories']:
        if hashlib.sha256(d.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('Native entity factory changed: '+row['name'])
        if hashlib.sha256(d.bytes_at(row['destructor'],row['destructorSize'])).hexdigest()!=row['destructorSha256']:raise ValueError('Native entity destructor changed: '+row['name'])
        if int.from_bytes(d.bytes_at(row['vtable'],4),'little')!=row['destructor']:raise ValueError('Native entity destructor vtable changed')
    return dict(regions=REGIONS,factories=witness['factories'],initialScriptRefcount=1,limits='Original allocation/copy/release instructions; only allocation/free are doubled.')


def execute(count=1,has_info=True,shared=False,name='NOVI_Barrel',scalar_flags=None):
    d=RData();evidence=prove(d);row=next(row for row in evidence['factories'] if row['name']==name);u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in sorted({a&~4095 for a,_,_ in REGIONS}|{0xbfe000,row['vtable']&~4095,row['address']&~4095,row['destructor']&~4095}):u.mem_map(page,4096)
    u.mem_map(0x200000,0x20000)
    for a,n,_ in REGIONS:u.mem_write(a,d.bytes_at(a,n))
    u.mem_write(row['address'],d.bytes_at(row['address'],row['size']));u.mem_write(row['destructor'],d.bytes_at(row['destructor'],row['destructorSize']))
    def put(a,v):u.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    parent=0x201000;game=0x202000;thing=0x204000;info=0x205000;actor=0x206000;result=0x207000
    entity=0x208000;block=0x209000;finish=0x20a000;master=0x20b000;other=0x20c000;stack=0x21e000
    put(parent+0x40,game);put(thing,0x1238c8c);put(thing+4,actor);put(thing+8,info if has_info else 0);put(info,count)
    put(row['vtable'],row['destructor']);events=[];allocations=0
    for pc in (0xbfea1a,0xbfe9bc):u.mem_write(pc,b'\xc3')
    def hook(uc,pc,size,user):
        nonlocal allocations
        if pc not in (0xbfea1a,0xbfe9bc):return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if pc==0xbfea1a:
            expected=(row['entitySize'],12)[allocations];assert get(sp+4)==expected;value=(entity,block)[allocations];allocations+=1;events.append(('allocate',expected));uc.reg_write(UC_X86_REG_EAX,value)
        else:
            pointer=get(sp+4);assert pointer in (entity,block);events.append(('free','entity' if pointer==entity else 'script.info'))
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);put(stack,finish);put(stack+4,master);put(stack+8,thing)
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,result);u.reg_write(UC_X86_REG_EDX,parent)
    u.emu_start(row['address'],finish,count=1000)
    assert u.reg_read(UC_X86_REG_EAX)==result and u.reg_read(UC_X86_REG_ESP)==stack+12
    assert get(result)==entity and get(result+4)==block and get(block)==1 and get(block+4)==0xcdee00 and get(block+8)==entity
    assert get(entity+4)==game and get(entity+0x14)==parent and get(entity+0x18)==master
    assert get(entity+12)==actor and get(entity+16)==(info if has_info else 0)
    retained=get(info);assert retained==count+int(has_info)
    if scalar_flags is not None:
        put(stack,finish);put(stack+4,scalar_flags)
        u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,entity)
        u.emu_start(row['destructor'],finish,count=1000)
        assert u.reg_read(UC_X86_REG_EAX)==entity and u.reg_read(UC_X86_REG_ESP)==stack+8
        assert get(info)==count
        assert events==[('allocate',row['entitySize']),('allocate',12)]+([('free','entity')] if scalar_flags&1 else [])
        return dict(returnedThis=True,thingReleased=True,ownerFreed=bool(scalar_flags&1))
    holders=[result]
    if shared:put(other,entity);put(other+4,block);put(block,2);holders.append(other)
    for index,holder in enumerate(holders):
        put(stack,finish);u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,holder)
        u.emu_start(0xce1000,finish,count=1000)
        assert get(holder)==get(holder+4)==0 and u.reg_read(UC_X86_REG_ESP)==stack+4
        if shared and index==0:assert len(events)==2 and get(info)==retained
    assert get(info)==count and events==[('allocate',row['entitySize']),('allocate',12),('free','entity'),('free','script.info')]
    return dict(initialScriptRefcount=1,retainedThingCount=retained,finalThingCount=get(info),events=events)
