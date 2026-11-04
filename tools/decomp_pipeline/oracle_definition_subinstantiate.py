"""Retail definition instantiation with allocator/map/vector boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
def collect():
    raw=EXE.read_bytes();sections=pe_sections(raw);reports=[]
    for case in range(4):
        unknown=case==0;global_index=3;class_index=0 if case==3 else 2;growth=case==2;loads=[]
        u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
        for _,va,vs,off,size in sections:
            if size:u.mem_write(va+0x400000,raw[off:off+size])
        u.mem_map(0x2000000,0x100000);manager=0x2001000;info=0x2002000;definition=0x2005000;table=0x2006000;stack=0x2010000;stop=0x200f000;heap=[0x2020000];pubs=[];interns=[];names=[];grows=[];allocations=[]
        def r32(ptr):return struct.unpack('<I',u.mem_read(ptr,4))[0]
        def w32(ptr,value):u.mem_write(ptr,struct.pack('<I',value&0xffffffff))
        def alloc(size):p=heap[0];heap[0]+=((size+15)&~15);u.mem_write(p,bytes(size));return p
        def cstr(ptr):
            value=bytearray()
            while True:
                c=u.mem_read(ptr+len(value),1)[0]
                if not c:return value.decode()
                value.append(c)
        def string(ptr):return cstr(r32(r32(ptr)))
        def assign(ptr,text):
            block=alloc(20+len(text)+1);w32(block,block+20);w32(block+4,len(text));u.mem_write(block+20,text.encode()+bytes(1));w32(ptr,block)
        def ret(value=0,pop=0):
            sp=u.reg_read(UC_X86_REG_ESP);dest=r32(sp);u.reg_write(UC_X86_REG_EAX,value);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,dest)
        def vector(ptr,count,capacity):
            start=alloc(capacity*4) if capacity else 0;w32(ptr,start);w32(ptr+4,start+count*4);w32(ptr+8,start+capacity*4)
        def hook(uc,address,size,user):
            sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX)
            if address==stop:uc.emu_stop()
            elif address==0x9ad580:ret(0 if unknown else info,4)
            elif address==0x2008100:allocations.append(1);ret(definition)
            elif address==0x2008000:raise AssertionError("unexpected parent query")
            elif address==0x2008300:names.append((string(r32(sp+4)),r32(sp+8),r32(sp+12)));ret(0,12)
            elif address==0x9ad6e0:loads.append(r32(sp+4));ret(0,4)
            elif address==0x2008200:raise AssertionError('unexpected final unlock deletion')
            elif address==0x9d5060:
                name=string(r32(sp+4));interns.append(name);w32(self,100 if name=='CANON' else 200 if name=='INSTANCE' else 0xffffffff);ret(0,4)
            elif address==0xbfea1a:ret(alloc(r32(sp+4)))
            elif address in (0x9b4bd0,0x464ace):
                assert r32(sp+16)==1 and r32(sp+20)==1
                start=r32(self);finish=r32(self+4);count=(finish-start)//4;assert r32(sp+4)==finish
                value=r32(r32(sp+8));capacity=count+max(count,1);dest=alloc(capacity*4)
                if count:u.mem_write(dest,bytes(u.mem_read(start,count*4)))
                w32(dest+count*4,value);w32(self,dest);w32(self+4,dest+(count+1)*4);w32(self+8,dest+capacity*4);grows.append(address);ret(0,20)
            elif address==0x9e5420:pubs.append(('name',string(r32(sp+4)),r32(sp+8)));ret(0,8)
            elif address==0x9e54e0:pubs.append(('group',string(r32(sp+4)),string(r32(sp+8)),r32(sp+12)));ret(0,12)
        u.mem_write(manager,bytes(0xd0));u.mem_write(info,bytes(20));assign(info,'CANON');w32(info+4,0x2008100);vector(manager+0xa4,global_index,global_index if growth else 8);vector(info+8,class_index,class_index if case>=2 else 8);w32(manager+0x58,0x200b000);w32(manager+0x60,0x200c000)
        u.mem_write(definition,bytes(37));w32(definition,table);w32(definition+4,1);u.mem_write(definition+0x24,bytes([0xa7]));u.mem_write(table,bytes(100));w32(table+4,0x2008200);w32(table+12*4,0x2008300);w32(table+16*4,0x2008000);assign(0x2009000,'requested');u.mem_write(0x200b000,bytes(37));w32(0x200b000,table);w32(0x200b004,2);u.mem_write(0x200c000,bytes(16));w32(0x200c00c,0x200b000);w32(r32(manager+0xa4)+4,0x200c000);u.mem_write(stack,struct.pack('<IIII',stop,0x200a000,0x2009000,1));u.reg_write(UC_X86_REG_ECX,manager);u.reg_write(UC_X86_REG_ESP,stack);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9adae0,stop,count=500000)
        result=r32(0x200a000);assert bool(result)==(not unknown) and loads==[1] and r32(0x200b004)==2
        if unknown:assert not allocations and not names and not interns
        else:
            assert len(allocations)==1 and len(grows)==(2 if case==2 else 1 if case==3 else 0)
            assert names==[('CANON',3,1)] and interns==['CANON'] and not pubs
            item=r32(r32(manager+0xa4)+global_index*4);assert struct.unpack('<IIII',u.mem_read(item,16))==(100,0xffffffff,class_index,definition)
            assert result==definition and r32(definition+4)==2 and r32(definition+0x1c)==manager and r32(definition+0x20)==global_index
            assert u.mem_read(definition+0x24,1)[0]==0xa5
            assert (r32(manager+0xa8)-r32(manager+0xa4))//4==global_index+1
            assert (r32(info+0xc)-r32(info+8))//4==class_index+1 and r32(r32(info+8)+class_index*4)==global_index
        reports.append(dict(case=case,result=result,publications=pubs,parent_entries=names,vector_growths=len(grows)))
    return reports
if __name__=='__main__':print('RETAIL_INSTANTIATE_SUB_ORACLE_PASS cases='+str(len(collect())))
