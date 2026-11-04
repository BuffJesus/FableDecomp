"""Retail enum parser with string/map/expression boundaries mocked."""
import struct,json
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
CASES=[dict(source='field=1;',kind=kind)for kind in ('new','existing','inherited','failed')]
def collect():
    raw=EXE.read_bytes();sections=pe_sections(raw);reports=[]
    for case in CASES:
        source=case['source']
        u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
        for _,va,vs,off,size in sections:
            if size:u.mem_write(va+0x400000,raw[off:off+size])
        u.mem_map(0x2000000,0x100000);manager=0x2001000;parser=0x2002000;data=0x2003000;stack=0x2010000;stop=0x200f000;heap=[0x2020000];published=[];errors=[];events=[];transfers=[];subdefs=[];target=0x2007000;parent=0x2008000;vtable=0x2009000
        def read32(ptr):return struct.unpack('<I',u.mem_read(ptr,4))[0]
        def write32(ptr,val):u.mem_write(ptr,struct.pack('<I',val&0xffffffff))
        def cstr(ptr):
            result=bytearray()
            while True:
                c=u.mem_read(ptr+len(result),1)[0]
                if not c:return bytes(result)
                result.append(c)
        def string(ptr):
            block=read32(ptr)
            return cstr(read32(block)) if block else b''
        def assign(ptr,value):
            block=heap[0];heap[0]+=32+((len(value)+4)&~3);buffer=block+20
            u.mem_write(block,bytes(20));write32(block,buffer);write32(block+4,len(value));u.mem_write(buffer,value+bytes(1));write32(ptr,block)
        def ret(value=0,pop=0):
            sp=u.reg_read(UC_X86_REG_ESP);dest=read32(sp);u.reg_write(UC_X86_REG_EAX,value&0xffffffff);u.reg_write(UC_X86_REG_ESP,sp+4+pop);u.reg_write(UC_X86_REG_EIP,dest)
        def hook(uc,address,size,user):
            sp=uc.reg_read(UC_X86_REG_ESP);self=uc.reg_read(UC_X86_REG_ECX)
            if address==stop:uc.emu_stop()
            elif address==0x99e4b0:write32(self,0);ret(self)
            elif address==0x99eae0:ret()
            elif address in (0x99ebf0,0x99ef40):
                ptr=read32(sp+4);length=read32(sp+8);value=cstr(ptr);assign(self,value if length==0xffffffff else value[:length]);ret(self,8)
            elif address in (0x99ec30,0x99efb0):assign(self,string(read32(sp+4)));ret(self,4)
            elif address==0x99efe0:assign(self,cstr(read32(sp+4)));ret(self,4)
            elif address==0x99e4c0:ret(read32(read32(self)) if read32(self) else 0x122d70e)
            elif address==0x4115a0:
                events.append(('compare',cstr(read32(self)).decode(),cstr(read32(sp+4)).decode()));ret(int(cstr(read32(self))==cstr(read32(sp+4))),4)
            elif address in (0xbfeda6,0xbfedb8,0xbfedbe):
                c=read32(sp+4);value=(48<=c<=57) if address==0xbfeda6 else (65<=c<=90 or 97<=c<=122) if address==0xbfedb8 else (c in (9,10,11,12,13,32));ret(int(value))
            elif address in (0x99f570,0x99f600):
                left=string(uc.reg_read(UC_X86_REG_EDX));right=string(read32(sp+4)) if address==0x99f570 else cstr(read32(sp+4));assign(self,left+right);ret(self,4)
            elif address==0x9ac9f0:errors.append(string(read32(sp+8)).decode());ret(0,8)
            elif address==0x9b95f0:raise AssertionError('unexpected parser error '+string(read32(sp+4)).decode())
            elif address==0x9e54e0:
                value=read32(sp+12);value=value-0x100000000 if value&0x80000000 else value;published.append((string(read32(sp+4)).decode(),string(read32(sp+8)).decode(),value));ret(0,12)
            elif address==0xbfea0e:
                ptr=heap[0];heap[0]+=32;u.mem_write(ptr,bytes(32));ret(ptr)
            elif address==0xbfea14:ret()
            elif address==0x99a2d0:write32(self,0x1231710);ret(self)
            elif address==0xbfeb3a:
                hay=read32(sp+4);ix=cstr(hay).find(cstr(read32(sp+8)));ret(hay+ix if ix>=0 else 0)
            elif address==0x9e5170:
                write32(read32(sp+8),1);ret(int(not case.get('unknown')),8)
            elif address==0x9ad6e0:events.append(('load',read32(sp+4)));ret(0,4)
            elif address==0x200f100:ret(0x200b000 if case['kind'] in ('existing','inherited') else 0,4)
            elif address==0x200f300:events.append('remove');ret(0,4)
            elif address==0x9adae0:
                out=read32(sp+4);assert read32(sp+12)==1;new=0x200a000;write32(out,0 if case['kind']=='failed' else new)
                if case['kind']!='failed':write32(new+4,read32(new+4)+1)
                events.append('instantiate');ret(out,12)
            elif address==0x200f200:
                assert read32(sp+4)==parent;u.mem_write(self+36,bytes(u.mem_read(parent+36,1)));events.append('copy');ret(0,4)
            elif address==0x200f400:
                context=read32(sp+4);p=read32(context+8);transfers.append(dict(body=cstr(read32(p+4)).decode(),use_defaults=bool(u.mem_read(context+20,1)[0]),flags=u.mem_read(self+36,1)[0],baseline=read32(p+8)));ret(0,4)
            elif address==0x9bbbf0:events.append('unknown-check');ret()
            elif address==0x99f390:
                index=read32(sp+4);char=read32(sp+8)&255;old=bytearray(string(self));old[index]=char;assign(self,bytes(old));ret(0,8)
            elif address==0x99f690:assign(self,cstr(uc.reg_read(UC_X86_REG_EDX))+string(read32(sp+4)));ret(self,4)
        u.mem_write(manager,bytes(0xd0));write32(manager+0x58,0x2004000);write32(manager+0xa4,0x2004100);write32(0x2004104,0x2004200);write32(0x200420c,target);write32(0x2004108,0x2004300);write32(0x200430c,parent)
        for obj,flags in [(target,0xa0),(parent,0xa4),(0x200a000,0xb2)]:
            u.mem_write(obj,bytes(40));write32(obj,vtable);write32(obj+4,2);u.mem_write(obj+36,bytes([flags]))
        write32(vtable+60,0x200f100);write32(vtable+52,0x200f300);write32(vtable+72,0x200f400);write32(vtable+76,0x200f200);write32(0x200b000,2);write32(0x200b004,1 if case['kind']=='existing' else 0)
        assign(0x2005000,source.encode());assign(0x2005100,b'Part');u.mem_write(stack,struct.pack('<IIII',stop,read32(0x2005000),0x2005100,1));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.reg_write(UC_X86_REG_FPCW,0x027f);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9ae150,stop,count=1000000)
        assert all(read32(obj+4)==2 for obj in (target,parent,0x200a000))
        if case['kind']=='failed':assert not transfers and events==[('load',1),'instantiate']
        else:
            assert len(transfers)==1 and transfers[0]['body']==source and transfers[0]['baseline']==0
            assert transfers[0]['use_defaults']==(case['kind']=='new')
            assert events[-1]=='unknown-check'
            assert u.mem_read((parent if case['kind']=='existing' else 0x200a000)+36,1)[0]==(0xb7 if case['kind']=='new' else 0xa5)
            if case['kind']=='inherited':assert events==[('load',1),('load',2),'remove','instantiate','copy','unknown-check']
        reports.append(dict(case=case,transfers=transfers,events=events,refs=[read32(obj+4)for obj in (target,parent,0x200a000)]))
    return reports
if __name__=='__main__':
    rows=collect();print('RETAIL_COMPILE_SUBDEFINITION_PASS cases='+str(len(rows)))
