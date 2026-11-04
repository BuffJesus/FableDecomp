"""Check genuine close-live-GUI chain, lookup, pause store and manager close."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile,verify_and_land as V
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='00629da0',file='CWorld_EACloseLiveGUI_00629da0.cpp',symbol='?EACloseLiveGUI@CWorld@@QAEXPBVCGameEvent@@@Z',size=57,grade='NEAR_MATCH'),dict(address='00437cd0',file='CPlayerGui_GetPHeroInfoScreensManager_00437cd0.cpp',symbol='?GetPHeroInfoScreensManager@CPlayerGui@@QBEPAVCManager@NHeroInformationScreens@@XZ',size=7,grade='MATCH'),dict(address='0066e73d',file='HeroInformationManager_Close_0066e73d.cpp',symbol='?Close@CManager@NHeroInformationScreens@@QAEXXZ',size=5,grade='MATCH')]
EXTRA=[dict(address='0049d850',file='CWorld_GetPlayer_0049d850.cpp',symbol='?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z',size=8,grade='RELOCATION_MATCH'),dict(address='0049d8f0',file='CWorld_SetAsPaused_0049d8f0.cpp',symbol='?SetAsPaused@CWorld@@IAEX_N@Z',size=21,grade='MATCH')]
TARGETS={s['symbol']:int(s['address'],16) for s in SPECS+EXTRA}
TARGETS.update({'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z':0x634190,'?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A':0x13b8790})
HEADERS=['fable_player_gui.h','fable_player_manager.h','fable_hero_information_screens.h']
def execute(codes,value,open_value,paused,pattern):
    world,event,gui,manager,player,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x449000,0x634000,0x13b8000,0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,data in codes.items():u.mem_write(a,bytes(data))
    regions={base:bytearray(bytes([pattern*71])*0x400) for base in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000]}
    player_manager=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,player_manager);regions[0x2000000][0xe4]=paused;regions[0x2100000][0x44]=value;struct.pack_into('<I',regions[0x2200000],0x36c,manager);regions[0x2300000][0x74]=open_value
    for base,data in regions.items():u.mem_write(base,bytes(data))
    singleton=struct.pack('<II',gui,0x76543210);u.mem_write(0x13b8790,singleton);u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        if pc not in [0x4498c0,0x634190]:return
        assert bytes(uc.mem_read(world+0xa4,1))==b'\0'
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));result=player
        else:trace.append(('remove',this,get(sp+4)));result=0xabcdef12
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_ESP,sp+8);uc.reg_write(UC_X86_REG_EIP,get(sp))
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x629da0,stop,count=100)
    number=(value if value<128 else value-256)&0xffffffff;assert trace==[('lookup',player_manager,number),('remove',player,43)]
    regions[0x2000000][0xe4]=0;regions[0x2300000][0x74]=0
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items()) and bytes(u.mem_read(0x13b8790,8))==singleton
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldCloseGui_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-close-gui.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='world-close-gui-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<3 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            if i==0:
                grade,_,_=V.parity_of(source.read_text(),s['address'],s['symbol'],retail,work,env);assert grade=='NEAR_MATCH',grade
            else:assert patched==retail
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for value in range(256):
        for opened in [0,1,127,255]:
            for paused in [0,1]:
                for pattern in range(4):assert execute(candidate,value,opened,paused,pattern)==execute(native,value,opened,paused,pattern);scenarios+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(5):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'WORLD_CLOSE_GUI_BEHAVIOR_PASS scenarios=8192' in r.stdout,r.stdout+r.stderr
    report=dict(entries=SPECS,providers=providers,comparative_scenarios=scenarios,compiled_scenarios=8192,missing_provider_checks=5,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS})
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_CLOSE_GUI_PASS providers=5 compiled=8192 comparative=%d'%scenarios)
if __name__=='__main__':main()
