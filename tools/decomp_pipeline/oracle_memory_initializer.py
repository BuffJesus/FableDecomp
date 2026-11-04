"""Retail boot.ini scanner with recorded Win32 and character boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
CASES=[dict(data=data,fail=fail,short=short,read_ok=ok)for data,fail,short,ok in [(b'',True,0,True),(b'',False,0,True),(b' \t\r\n',False,4,True),(b'// comment\r\n',False,12,True),(b'one=two',False,7,True),(b' one \t= \t two \r\nother=3\n',False,26,True),(b'bare line\nnext',False,14,True),(b'=emptykey\nkey=\n',False,15,True),(b'\n\r\n//skip\n/=3\n/',False,18,True),(b'  = value \n',False,11,True),(b'a=1\n//tail',False,10,True),(b'a=one=two',False,9,True),(b'a\0b=c\0d',False,7,True),(b'\x80key=\xffvalue\n',False,12,True),(b'key=value',False,3,False),(b'key=value',False,0,False)]]

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);buffer=0x2008004;stack=0x2010000;stop=0x200f000;rows=[];events=[];case=None
    boundaries={0x143fd48:(1,7),0x143fd58:(2,0),0x143fdf0:(3,2),0x143fdd4:(4,2),0x143fed0:(5,5),0x143fe30:(6,1),0x143fdf8:(8,1)}
    hooks={}
    for index,(iat,(kind,count))in enumerate(boundaries.items()):
        address=0x2001000+index*16;uc.mem_write(iat,struct.pack('<I',address));hooks[address]=(kind,count)
    def hook(uc,address,size,user):
        if address==0xbfedbe:
            esp=uc.reg_read(UC_X86_REG_ESP);ret,arg=struct.unpack('<2I',bytes(uc.mem_read(esp,8)));events.append([7,arg,0,0]);uc.reg_write(UC_X86_REG_EAX,int(arg in [9,10,11,12,13,32]));uc.reg_write(UC_X86_REG_ESP,esp+4);uc.reg_write(UC_X86_REG_EIP,ret);return
        if address not in hooks:return
        kind,count=hooks[address];esp=uc.reg_read(UC_X86_REG_ESP);words=struct.unpack('<'+str(count+1)+'I',bytes(uc.mem_read(esp,4*(count+1))));ret=words[0];args=words[1:];value=0
        if kind==1:
            assert bytes(uc.mem_read(args[0],9))==b'boot.ini\0'and list(args[1:])==[0x80000000,1,0,3,0x08000000,0];events.append([1,0x80000000,1,0x08000000]);value=0xffffffff if case['fail']else 0x1234
        elif kind==2:events.append([2,0,0,0]);value=2
        elif kind==3:assert args==(0x1234,0);value=len(case['data']);events.append([3,value,0,0])
        elif kind==4:
            assert args==(0x40,len(case['data']));uc.mem_write(buffer-4,b'\x17'*4+bytes(args[1])+b'\x31'*4);events.append([4,args[0],args[1],0]);value=buffer
        elif kind==5:
            assert args[0]==0x1234 and args[1]==buffer and args[2]==len(case['data'])and args[4]==0;n=min(case['short'],args[2]);uc.mem_write(buffer,case['data'][:n]);uc.mem_write(args[3],struct.pack('<I',n));value=int(case['read_ok']);events.append([5,args[2],n,value])
        elif kind==6:assert args==(0x1234,);events.append([6,0,0,0]);value=1
        elif kind==8:assert args==(buffer,);events.append([8,0,0,0])
        uc.reg_write(UC_X86_REG_EAX,value);uc.reg_write(UC_X86_REG_ESP,esp+4*(count+1));uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook)
    for case in CASES:
        events.clear();uc.mem_write(stack,struct.pack('<I',stop));uc.reg_write(UC_X86_REG_ESP,stack);uc.emu_start(0x9f1d20,stop,count=100000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4
        if not case['fail']:assert bytes(uc.mem_read(buffer-4,4))==b'\x17'*4 and bytes(uc.mem_read(buffer+len(case['data']),4))==b'\x31'*4
        rows.append(dict(data=case['data'].hex(),fail=case['fail'],short=case['short'],read_ok=case['read_ok'],events=list(events)))
    return rows
