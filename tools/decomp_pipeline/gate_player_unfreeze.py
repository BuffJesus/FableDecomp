"""Verify real unfreeze behavior through the real event/world/player lookup chain."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
FILE='CPlayer_UnfreezeControls_00487120.cpp';SYMBOL='?UnfreezeControls@CPlayer@@QAEXXZ'
CHAIN=[('006291f0','00/62/CWorld_EAControlledCreatureUnfreezeControls_006291f0.cpp','?EAControlledCreatureUnfreezeControls@CWorld@@QAEXPBVCGameEvent@@@Z',24),('0049d850','00/49/CWorld_GetPlayer_0049d850.cpp','?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z',8),('004498c0','00/44/CPlayerManager_GetPlayer_004498c0.cpp','?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z',73)]
TARGETS={'?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z':0x634190,SYMBOL:0x487120,'?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z':0x49d850,'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0}
VALUES=[0,1,127,128,129,254,255,42]
def execute(codes,suspended,value,match,entry):
    player,interface,manager,world,event,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x3008000,0x4000000
    number=value if value<128 else value-256
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x487000,0x1000),(0x629000,0x1000),(0x49d000,0x1000),(0x449000,0x1000),(0x634000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x2200000,0x1000),(0x2300000,0x1000),(0x2400000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    regions={0x2000000:bytearray([suspended]*0xc0),0x2100000:bytearray([255-suspended]*0x900),0x2200000:bytearray([suspended]*0x200),0x2300000:bytearray([255-suspended]*0x218),0x2400000:bytearray([suspended]*0xa8)}
    struct.pack_into('<I',regions[0x2000000],0x4c,interface);struct.pack_into('<I',regions[0x2000000],0x68,(number if match else 256+value)&0xffffffff)
    regions[0x2100000][0x40+0x79c]=suspended
    struct.pack_into('<IIIII',regions[0x2200000],0x4c,manager+0x100,manager+0x104,manager+0x104,0,0);struct.pack_into('<I',regions[0x2200000],0x140,player)
    struct.pack_into('<I',regions[0x2300000],0x4c,manager);regions[0x2400000][0x44]=value
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world if entry else player)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x76540000+i)
    calls=[]
    def hook(uc,pc,size,user):
        if pc!=0x634190:return
        sp=uc.reg_read(UC_X86_REG_ESP);calls.append((uc.reg_read(UC_X86_REG_ECX),struct.unpack('<I',uc.mem_read(sp+4,4))[0],uc.mem_read(interface+0x79c,1)[0]))
        uc.reg_write(UC_X86_REG_EAX,0xaaaa5555);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+8)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x6291f0 if entry else 0x487120,stop,count=500)
    regions[0x2100000][0x40+0x79c]=0
    assert calls==[(player,17,suspended)] and all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if entry else 4) and all(u.reg_read(r)==0x76540000+i for i,r in enumerate(saved))
    return calls
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/48'/FILE)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PlayerUnfreeze_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/player-unfreeze.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='player-unfreeze-'));objects=[];providers=[];candidate={};native={}
    specs=[('00487120',a.source,SYMBOL,22)]+[(addr,ROOT/'rebuild/src/compiled'/path,symbol,size) for addr,path,symbol,size in CHAIN]
    with pefile.PE(str(EXE)) as pe:
        for i,(addr,source,symbol,size) in enumerate(specs):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;address=int(addr,16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail,source
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    comparative=0
    for suspended in range(256):
        for value in VALUES:
            for match in [False,True]:
                for entry in [False,True]:assert execute(candidate,suspended,value,match,entry)==execute(native,suspended,value,match,entry);comparative+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    assert not any(symbol in Coff(test).symbols for _,_,symbol,_ in specs);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(4):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'PLAYER_UNFREEZE_BEHAVIOR_PASS scenarios=8192' in r.stdout,r.stdout+r.stderr
    report=dict(address='00487120',grade='RELOCATION_MATCH',bytes=22,providers=providers,comparative_scenarios=comparative,missing_provider_checks=4,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_player_manager.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PLAYER_UNFREEZE_PASS bytes=22 compiled=8192 comparative=%d'%comparative)
if __name__=='__main__':main()
