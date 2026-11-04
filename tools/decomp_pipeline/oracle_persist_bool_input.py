"""Retail enum parser with string/map/expression boundaries mocked."""
import struct,json
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_FPCW
from pe_oracle import EXE,pe_sections
CASES=[('TRUE;',0,1,4),('true;',0,1,4),('BTRUE;',0,1,5),('bTrUe;',0,1,5),('FALSE;',1,0,5),('false;',1,0,5),('BFALSE;',1,0,6),('bFaLsE;',1,0,6),('Unknown;',0,0,7),('Unknown;',1,1,7),('1;',0,0,1),('0;',1,1,1)]
def collect():
    raw=EXE.read_bytes();sections=pe_sections(raw);reports=[]
    for source,initial,expected,expected_position in CASES:
        u=Uc(UC_ARCH_X86,UC_MODE_32);u.mem_map(0x400000,0x1200000)
        for _,va,vs,off,size in sections:
            if size:u.mem_write(va+0x400000,raw[off:off+size])
        u.mem_map(0x2000000,0x100000);manager=0x2001000;parser=0x2002000;data=0x2003000;stack=0x2010000;stop=0x200f000;heap=[0x2020000];published=[];errors=[];parser_errors=[]
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
            elif address==0x4115a0:ret(int(cstr(read32(self))==cstr(read32(sp+4))),4)
            elif address in (0xbfeda6,0xbfedb8,0xbfedbe):
                c=read32(sp+4);value=(48<=c<=57) if address==0xbfeda6 else (65<=c<=90 or 97<=c<=122) if address==0xbfedb8 else (c in (9,10,11,12,13,32));ret(int(value))
            elif address in (0x99f570,0x99f600):
                left=string(uc.reg_read(UC_X86_REG_EDX));right=string(read32(sp+4)) if address==0x99f570 else cstr(read32(sp+4));assign(self,left+right);ret(self,4)
            elif address==0x9ac9f0:errors.append(string(read32(sp+8)).decode());ret(0,8)
            elif address==0x9b95f0:parser_errors.append(string(read32(sp+4)).decode());ret(0,4)
            elif address==0xbfeba8:ret(0 if cstr(read32(sp+4)).lower()==cstr(read32(sp+8)).lower() else 1)
            elif address==0x9e54e0:
                value=read32(sp+12);value=value-0x100000000 if value&0x80000000 else value;published.append((string(read32(sp+4)).decode(),string(read32(sp+8)).decode(),value));ret(0,12)
            elif address==0x9e5c60:ret()
            elif address==0x9e5170:
                name=string(read32(sp+4));write32(read32(sp+8),5 if name==b'Known' else 0);ret(int(name==b'Known'),8)
            elif address==0x9baa90:errors.append(string(read32(sp+8)).decode());ret(0,8)
            elif address==0x99f690:
                left=cstr(uc.reg_read(UC_X86_REG_EDX));right=string(read32(sp+4));assign(self,left+right);ret(self,4)
        u.mem_write(manager,bytes(0xd0));u.mem_write(parser,bytes(40));write32(parser+4,data);u.mem_write(data,source.encode()+bytes(1));u.mem_write(0x2005000,bytes([initial]));u.mem_write(stack,struct.pack('<IIII',stop,manager,parser,0x2005000));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,0x2006000);u.reg_write(UC_X86_REG_FPCW,0x027f);u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x403ff0,stop,count=500000)
        value=u.mem_read(0x2005000,1)[0];position=read32(parser+32)
        assert value==expected and position==expected_position,(source,value,position,errors)
        assert len(errors)==int(source in ('Unknown;','1;','0;')),(source,errors)
        assert len(parser_errors)==int(source in ('1;','0;')),(source,parser_errors)
        reports.append(dict(source=source,value=value,position=position,errors=errors))
    return reports
if __name__=='__main__':
    rows=collect();print('RETAIL_BOOL_INPUT_ORACLE_PASS cases='+str(len(rows)))
