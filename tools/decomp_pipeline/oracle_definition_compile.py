"""Retail enum parser with string/map/expression boundaries mocked."""
import struct,json
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
CASES=[dict(source='field=1; #end_definition',flags=0xa0),dict(source='field=1; #end_definition',flags=0xa4),dict(source='specialises PARENT field=1; #end_definition',flags=0xa2,parent='ready'),dict(source='specialises PARENT field=1; #end_definition',flags=0xa0,parent='missing'),dict(source='specialises PARENT field=1; #end_definition',flags=0xa0,parent='ordinary'),dict(source='field=1; #end_definition',flags=0xa0,unknown=True),dict(source='<Part>\na=1;\n<\\Part>\nfield=2; #end_definition',flags=0xa0)]
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
            elif address==0x9ad6e0:events.append('load');ret(0,4)
            elif address==0x9ada90:
                events.append('get-parent');out=read32(sp+4);write32(out,0 if case.get('parent')=='missing' else parent)
                if case.get('parent')!='missing':write32(parent+4,read32(parent+4)+1)
                ret(out,8)
            elif address==0x200f100:write32(read32(sp+4),42);ret(read32(sp+4),4)
            elif address==0x415dd0:out=read32(sp+4);assign(out,b'INSTANCE');ret(out,4)
            elif address==0x200f200:u.mem_write(target+36,bytes([0xe7]));events.append('copy');ret(0,4)
            elif address==0x9d5060:write32(self,42);ret(0,4)
            elif address==0x200f300:assert read32(sp+4)==42;events.append('name');ret(0,4)
            elif address==0x200f400:
                context=read32(sp+4);p=read32(context+8);transfers.append(dict(body=cstr(read32(p+4)).decode(),use_defaults=bool(u.mem_read(context+20,1)[0]),flags=u.mem_read(target+36,1)[0],baseline=read32(p+8)));ret(0,4)
            elif address==0x9bbbf0:events.append('unknown-check');ret()
            elif address==0x9ae150:subdefs.append((string(sp+4).decode(),string(read32(sp+8)).decode(),read32(sp+12)));ret(0,12)
            elif address==0x99f390:
                index=read32(sp+4);char=read32(sp+8)&255;old=bytearray(string(self));old[index]=char;assign(self,bytes(old));ret(0,8)
            elif address==0x99f690:assign(self,cstr(uc.reg_read(UC_X86_REG_EDX))+string(read32(sp+4)));ret(self,4)
        u.mem_write(manager,bytes(0xd0));write32(manager+0x58,0x2004000);write32(manager+0x60,0x2004000);write32(manager+0xa4,0x2004100);write32(0x2004104,0x2004200);write32(0x200420c,target)
        u.mem_write(target,bytes(40));write32(target,vtable);write32(target+4,2);u.mem_write(target+36,bytes([case['flags']]));u.mem_write(parent,bytes(40));write32(parent,vtable);write32(parent+4,2);u.mem_write(parent+36,bytes([1 if case.get('parent')=='ready' else 2]));write32(vtable+40,0x200f100);write32(vtable+44,0x200f300);write32(vtable+72,0x200f400);write32(vtable+76,0x200f200)
        u.mem_write(parser,bytes(40));write32(parser+4,data);write32(parser+36,17);u.mem_write(data,source.encode()+bytes(1));assign(0x2005000,b'CLASS');assign(0x2005100,b'INSTANCE');u.mem_write(stack,struct.pack('<IIII',stop,parser,0x2005000,0x2005100));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.reg_write(UC_X86_REG_FPCW,0x027f);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x9ae7c0,stop,count=1000000)
        flags=u.mem_read(target+36,1)[0];assert read32(target+4)==2 and read32(parent+4)==2
        if case.get('unknown'):assert errors==['Unable to find def class called INSTANCE'] and not transfers and not events
        else:
            assert len(transfers)==1 and transfers[0]['baseline']==17 and events[-1]=='unknown-check'
            assert transfers[0]['use_defaults']==(not(case.get('parent')=='ready' or case['flags']&4)),(case,transfers,events,errors)
            assert flags==(0xe5|(case['flags']&2) if case.get('parent')=='ready' else case['flags']|5)
            expected_errors=['Def PARENT does not exist.'] if case.get('parent')=='missing' else ['Def PARENT is not setup to specialise.'] if case.get('parent')=='ordinary' else []
            assert errors==expected_errors,(case,errors)
            if source.startswith('<Part>'):
                assert subdefs==[('\na=1;\n','Part',1)],subdefs
                assert transfers[0]['body']=='      \n    \n       \nfield=2; ',transfers
        reports.append(dict(case=case,transfers=transfers,subdefs=subdefs,events=events,errors=errors,flags=flags,position=read32(parser+32)))
    return reports
if __name__=='__main__':
    rows=collect();print('RETAIL_COMPILE_DEFINITION_PASS cases='+str(len(rows)))
