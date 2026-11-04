"""Verify the world click-past action's singleton/subtitle dispatch and ignored result."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?EAClickPastText@CWorld@@QAEXPBVCGameEvent@@@Z'
FILE='CWorld_EAClickPastText_00629f30.cpp'
TARGETS={'?ClickPast@CSubtitleRenderer@@QAE_NXZ':0x493281,'?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A':0x13b8790}
def execute(code,pattern,result,offset,null_event):
    world,gui,event,stack,stop=0x2000040,0x2100040+offset,0x2200040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x629000,0x1000),(0x493000,0x1000),(0x13b8000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x2200000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    u.mem_write(0x629f30,bytes(code));singleton=struct.pack('<II',gui,0x76543210);u.mem_write(0x13b8790,singleton)
    before=[bytes([pattern])*0x218,bytes([255-pattern])*0x218,bytes([pattern])*0xa8]
    for address,data in zip([world-0x40,gui-0x40,event-0x40],before):u.mem_write(address,data)
    u.mem_write(stack,struct.pack('<II',stop,0 if null_event else event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x14320000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc!=0x493281:return
        trace.append(uc.reg_read(UC_X86_REG_ECX));sp=uc.reg_read(UC_X86_REG_ESP)
        uc.reg_write(UC_X86_REG_EAX,0x12345600|result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x629f30,stop,count=30)
    assert trace==[gui+0x18] and bytes(u.mem_read(0x13b8790,8))==singleton
    assert all(bytes(u.mem_read(address,len(data)))==data for address,data in zip([world-0x40,gui-0x40,event-0x40],before))
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x14320000+i for i,r in enumerate(saved))
    return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/62'/FILE)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ClickPastEvent_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/click-past-event.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='click-past-event-'));obj=work/'source.obj'
    r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and len(built)==17;patched=bytearray(built)
    for site,(name,kind) in rel.items():
        assert kind in (6,20);struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(0x629f30+site+4 if kind==20 else 0))&0xffffffff)
    with pefile.PE(str(EXE)) as pe:retail=pe.get_data(0x629f30-0x400000,17)
    assert patched==retail;comparative=0
    for pattern in range(256):
        for result in [0,1]:
            for offset in [0,0x80]:
                for null in [False,True]:assert execute(patched,pattern,result,offset,null)==execute(retail,pattern,result,offset,null);comparative+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CLICK_PAST_EVENT_BEHAVIOR_PASS scenarios=512' in r.stdout,r.stdout+r.stderr
    report=dict(grade='RELOCATION_MATCH',bytes=17,source=str(a.source),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),comparative_scenarios=comparative,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_player_gui.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CLICK_PAST_EVENT_PASS bytes=17 comparative=%d compiled=512'%comparative)
if __name__=='__main__':main()
