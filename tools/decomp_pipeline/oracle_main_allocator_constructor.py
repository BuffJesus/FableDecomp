"""Retail constructor with real size-class code and recorded OS/CRT allocation."""
import struct
import pefile
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EDX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections,va_to_off

def cases(raw,sections):
    at=va_to_off(sections,0x1396868);original=[list(v)for v in struct.iter_unpack('<7I',raw[at:at+840])];out=[]
    def disabled():return [[0,4096,0,4,0,3,0xdead0000+i*4]for i in range(30)]
    out.append(dict(name='default',table=original,fail=-1))
    out.append(dict(name='no_rows',table=disabled(),fail=-1))
    table=disabled();table[28]=original[28][:];table[29]=original[29][:];out.append(dict(name='variable_only',table=table,fail=-1))
    table=disabled()
    for i,(maximum,alignment)in enumerate([(1,4),(5,8),(9,16),(13,4),(17,8)]):table[i]=[maximum,4096,0,alignment,14,3,0]
    table[28]=original[28][:];table[28][4]=14;out.append(dict(name='rounded_category14',table=table,fail=-1))
    table=disabled();table[0]=[3,4096,0,4,0,3,0];table[1]=[11,4096,0,4,0,3,0];out.append(dict(name='fixed_only_hole',table=table,fail=-1))
    table=disabled();table[0]=[4,4096,0,4,0,3,0];table[1]=[8,4096,0,4,1,3,0];table[2]=[12,4096,0,4,0,3,0];table[28]=original[28][:];table[29]=original[28][:];table[29][4]=1;out.append(dict(name='interleaved_categories',table=table,fail=-1))
    table=disabled();table[0]=[0x10000000,4096,0,4,0,3,0];out.append(dict(name='overflow_early_return',table=table,fail=-1))
    out.append(dict(name='variable_allocation_failure',table=original,fail=28))
    return out

def collect():
    raw=EXE.read_bytes();sections=pe_sections(raw);uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in sections:
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2200000,0x20000);uc.mem_map(0x2400000,0x100000);owner=0x2204000;stack=0x2210000;stop=0x220f000;rows=[];events=[];allocations=[];next_area=0x2400000;lock=0;virtual_calls=0;case=None;pattern=0
    names=['InitializeCriticalSection','EnterCriticalSection','LeaveCriticalSection','VirtualAlloc','CreateFileA','GetLastError']
    with pefile.PE(data=raw)as pe:
        imports={i.name.decode():i.address for entry in pe.DIRECTORY_ENTRY_IMPORT for i in entry.imports if i.name}
    hooks={}
    for index,name in enumerate(names):
        address=0x2201000+index*16;uc.mem_write(imports[name],struct.pack('<I',address));hooks[address]=name
    def allocate(size,kind):
        nonlocal next_area
        address=next_area;next_area+=(size+15)&~15;assert next_area<0x2500000
        uc.mem_write(address,bytes([pattern if kind==1 else 0xa5])*size);allocations.append(dict(address=address,size=size,kind=kind));return address
    def hook(uc,address,size,user):
        nonlocal lock,virtual_calls
        if address not in hooks and address not in [0xbfea0e,0xbfeb50]:return
        esp=uc.reg_read(UC_X86_REG_ESP);ret=struct.unpack('<I',bytes(uc.mem_read(esp,4)))[0];value=0;pop=4
        if address==0xbfeb50:
            a,b=struct.unpack('<2Q',bytes(uc.mem_read(esp+4,16)));product=(a*b)&0xffffffffffffffff;value=product&0xffffffff;uc.reg_write(UC_X86_REG_EDX,product>>32);pop=20
        elif address==0xbfea0e:
            amount=struct.unpack('<I',bytes(uc.mem_read(esp+4,4)))[0];events.append([5,amount,0,0]);value=allocate(amount,2)
        else:
            name=hooks[address];count={'InitializeCriticalSection':1,'EnterCriticalSection':1,'LeaveCriticalSection':1,'VirtualAlloc':4,'CreateFileA':7,'GetLastError':0}[name];args=struct.unpack('<'+str(count)+'I',bytes(uc.mem_read(esp+4,4*count)))if count else ();pop=4+4*count
            if name in names[:3]:
                assert args==(0x13caa70,);kind=names.index(name)+1;events.append([kind,0,0,0]);lock+=1 if kind==2 else -1 if kind==3 else 0
            elif name=='VirtualAlloc':
                assert args[0]==0 and args[2:]==(0x3000,4);events.append([4,args[1],0,0]);number=virtual_calls;virtual_calls+=1
                if number==case['fail']:allocations.append(dict(address=0,size=args[1],kind=1))
                else:value=allocate(args[1],1)
            elif name=='CreateFileA':assert bytes(uc.mem_read(args[0],9))==b'boot.ini\0';events.append([6,0,0,0]);value=0xffffffff
            else:events.append([7,0,0,0]);value=2
        uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ESP,esp+pop);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    def token(pointer):
        for index,area in enumerate(allocations):
            if area['address']and pointer==area['address']:return 0xf0000000+index
        return pointer
    for case in cases(raw,sections):
        for pattern in [0x5a,0xa5]:
            next_area=0x2400000;lock=0;virtual_calls=0;events.clear();allocations.clear();uc.mem_write(owner-4,b'\x17'*4+bytes([pattern])*0x49c+b'\x31'*4)
            uc.mem_write(0x1396868,b''.join(struct.pack('<7I',*row)for row in case['table']));uc.mem_write(stack,struct.pack('<2I',stop,0x12345678));uc.reg_write(UC_X86_REG_ECX,owner);uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f20b0,stop,count=1000000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8 and uc.reg_read(UC_X86_REG_EAX)==owner
            assert bytes(uc.mem_read(owner-4,4))==b'\x17'*4 and bytes(uc.mem_read(owner+0x49c,4))==b'\x31'*4
            owner_bytes=bytearray(uc.mem_read(owner,0x49c));table_bytes=bytearray(uc.mem_read(0x1396868,840))
            for offset in range(0x460,0x49c,4):struct.pack_into('<I',owner_bytes,offset,token(struct.unpack_from('<I',owner_bytes,offset)[0]))
            for offset in range(24,840,28):struct.pack_into('<I',table_bytes,offset,token(struct.unpack_from('<I',table_bytes,offset)[0]))
            heaps=[]
            for area in allocations:
                if not area['address']:heaps.append(dict(size=area['size'],kind=area['kind'],bytes=None));continue
                content=bytearray(uc.mem_read(area['address'],area['size']))
                if area['kind']==1:content[:4]=b'\0'*4
                else:
                    for offset in range(0,area['size'],8):struct.pack_into('<I',content,offset,token(struct.unpack_from('<I',content,offset)[0]))
                heaps.append(dict(size=area['size'],kind=area['kind'],bytes=content.hex()))
            rows.append(dict(name=case['name'],pattern=pattern,fail=case['fail'],input_table=case['table'],owner=owner_bytes.hex(),table=table_bytes.hex(),allocations=heaps,events=list(events),lock_depth=lock))
    return rows

if __name__=='__main__':
    for row in collect():print(row['name'],hex(row['pattern']),len(row['allocations']),row['lock_depth'])
