"""Execute BOTH built DLL hook bodies, including original FSE's retail epilogue."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'work/runtime_re_tools'))
import pefile,capstone
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

def main():
    p=argparse.ArgumentParser()
    for name in ['original','addon','output']:p.add_argument('--'+name,type=Path,required=True)
    a=p.parse_args();results=[]
    for original_base,addon_base in [(0x10000000,0x20000000),(0x22000000,0x18000000)]:
        images=[]
        for path,base,pattern in [(a.original,original_base,bytes.fromhex('6083ec289bd93424')),
                                  (a.addon,addon_base,bytes.fromhex('9c6081ec30020000'))]:
            pe=pefile.PE(str(path));pe.relocate_image(base);data=pe.get_memory_mapped_image()
            assert data.count(pattern)==1
            start=base+data.index(pattern)
            cs=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);cs.detail=True
            ins=list(cs.disasm(data[start-base:start-base+48],start))
            call=next(i.operands[0].imm for i in ins if i.mnemonic=='call')
            images.append((base,data,start,call,ins))
        for flags in [0x202,0x247,0xA96]:
            u=Uc(UC_ARCH_X86,UC_MODE_32)
            for base,data,_,_,_ in images:
                u.mem_map(base,(len(data)+4095)&~4095);u.mem_write(base,data)
            u.mem_map(0x3000000,0x200000)
            stack,stop=0x31f0000,0x3001000
            def w(addr,value):u.mem_write(addr,struct.pack('<I',value))
            def r(addr):return struct.unpack('<I',u.mem_read(addr,4))[0]
            previous=next(i.operands[0].mem.disp for i in images[1][4] if i.mnemonic=='jmp')
            w(previous,images[0][2])
            for offset,value in [(0,0x11111111),(4,0x22222222),(8,0x33333333),(12,0x44444444),(40,stop)]:w(stack+offset,value)
            u.reg_write(UC_X86_REG_ESP,stack)
            registers={UC_X86_REG_EAX:0x101,UC_X86_REG_EBX:0x202,UC_X86_REG_ECX:0x303,
                       UC_X86_REG_EDX:0x404,UC_X86_REG_ESI:0x505,UC_X86_REG_EDI:0x606,UC_X86_REG_EBP:0x707}
            for reg,value in registers.items():u.reg_write(reg,value)
            u.reg_write(UC_X86_REG_XMM0,0x123456789abcdef)
            u.reg_write(UC_X86_REG_EFLAGS,flags)
            calls=[]
            def hook(uc,pc,size,data):
                if pc==stop:u.emu_stop();return
                if pc==images[0][2]:
                    assert all(u.reg_read(k)==v for k,v in registers.items())
                    assert u.reg_read(UC_X86_REG_EFLAGS)==flags
                    assert u.reg_read(UC_X86_REG_XMM0)==0x123456789abcdef
                for name,entry in [('addon',images[1][3]),('original',images[0][3])]:
                    if pc==entry:
                        calls.append(name)
                        assert u.reg_read(UC_X86_REG_ESI)==registers[UC_X86_REG_ESI]
                        for reg in registers:u.reg_write(reg,0xBAD)
                        if name=='addon':u.reg_write(UC_X86_REG_XMM0,0)
                        sp=u.reg_read(UC_X86_REG_ESP)
                        u.reg_write(UC_X86_REG_EIP,r(sp));u.reg_write(UC_X86_REG_ESP,sp+4)
                        return
            u.hook_add(UC_HOOK_CODE,hook)
            u.emu_start(images[1][2],0,count=1000)
            assert calls==['addon','original'] and u.reg_read(UC_X86_REG_ESP)==stack+44
            assert u.reg_read(UC_X86_REG_EDI)==0x11111111 and u.reg_read(UC_X86_REG_ESI)==0x22222222
            assert u.reg_read(UC_X86_REG_EBP)==0x33333333 and u.reg_read(UC_X86_REG_EBX)==0x44444444
            results.append(dict(original_base=original_base,addon_base=addon_base,flags=flags,calls=calls))
    with a.output.open('x') as f:json.dump(dict(passed=True,cases=results,
        hashes={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in [a.original,a.addon,Path(__file__)]},
        limits='Native hook bodies execute; InjectCustomScripts bodies are doubles. Not a game lifecycle test.'),f,indent=2)
    print('6 actual-binary chain/ASLR/flags/epilogue cases passed')
if __name__=='__main__':main()
