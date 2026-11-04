"""Verify hero-screen closing with repeated lookups and original global unpause policy."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile,verify_and_land as V
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_inventory_events import EXTRA as PLAYER,SPECS as INVENTORY
from gate_player_lookup import SPECS as LOOKUP
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
from gate_world_close_gui import SPECS as GUI,EXTRA as WORLD,TARGETS as GUI_TARGETS
SPECS=[dict(address='0062b3f0',file='CWorld_EACloseHeroInformationScreen_0062b3f0.cpp',symbol='?EACloseHeroInformationScreen@CWorld@@QAEXPBVCGameEvent@@@Z',size=214,grade='NEAR_MATCH')]
EXTRA=WORLD+GUI[1:]+[PLAYER[1],INVENTORY[-1],dict(address='00634160',file='CPlayer_HasMode_00634160.cpp',symbol='?HasMode@CPlayer@@QBE_NW4EPlayerMode@@@Z',size=39)]
TARGETS=dict(GUI_TARGETS,**{s['symbol']:int(s['address'],16) for s in SPECS+EXTRA});TARGETS.update({'?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ':0xa01b50,'?RemoveAllPlayerModes@CManager@NHeroInformationScreens@@QBEXXZ':0x66e985,'?ShowWarningIfNeeded@CTCInventoryTrade@@QAEXXZ':0x59dabb,'?GUnpauseAfterClosingHeroInformationScreen@@3_NA':0x137d76c})
HEADERS=['fable_world.h','fable_player_manager.h','fable_thing_components.h','fable_hero_information_screens.h'];MAPS=[[],[23],[-0x80000000,-1,17,23,23,24,0x7fffffff],[22,24],[0,17,48,54,273]];STATES=[None,0,1,255,2,254]

def execute(codes,value,mode,keys,has,state,unpause,mutation,pattern):
    world,event,gui,manager,player,first,last,table,gui2,manager2,nodes,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x2600040,0x2900040,0x2700040,0x2800040,0x2a00040,0x3008000,0x4000000;boundary=[0x4498c0,0xa01b50,0x66e985,0x59dabb];u=Uc(UC_ARCH_X86,UC_MODE_32)
    bases=[0x2000000+i*0x100000 for i in range(11)]
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x137d000,0x13b8000,stop}|set(bases):u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x400) for a in bases};pm=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,pm);regions[0x2000000][0xe4]=1;regions[0x2100000][0x44]=value;struct.pack_into('<I',regions[0x2200000],0x36c,manager);struct.pack_into('<I',regions[0x2700000],0x36c,manager2);regions[0x2300000][0x74]=255 if pattern else 127;regions[0x2800000][0x74]=127 if pattern else 255;regions[0x2500000][0xd1]=state or 0;struct.pack_into('<I',regions[0x2600000],0x60,(0xff7fffff if pattern else 0)|(0x800000 if has else 0));struct.pack_into('<III',regions[0x2600000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,k in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2900000],0x40+i*8,k&0xffffffff,0x2b00040+i*0x20)
    struct.pack_into('<I',regions[0x2400000],0x250,nodes)
    for i,k in enumerate([47,47 if mode else 1,19]):struct.pack_into('<III',regions[0x2a00000],0x40+i*12,nodes+(i+1)*12 if i<1 else nodes,nodes+24 if i==0 else nodes+(i-1)*12,k)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    singleton=bytearray(struct.pack('<II',gui,0x76543210));globalFlag=unpause;u.mem_write(0x13b8790,bytes(singleton));u.mem_write(0x137d76c,bytes([globalFlag]));u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[];lookupCount=0;controlledCount=0
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        nonlocal globalFlag,lookupCount,controlledCount
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x634160:trace.append(('hasmode',this,get(sp+4)));return
        if pc==0x437cd0:trace.append(('gui',this));return
        if pc==0x49d8f0:trace.append(('pause',this,get(sp+4)));return
        if pc==0x66e73d:trace.append(('close',this));return
        if pc not in boundary:return
        pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));lookupCount+=1;pop=4;result=player
        elif pc==0xa01b50:trace.append(('creature',this));result=(0 if state is None else first) if not controlledCount else last;controlledCount+=1
        elif pc==0x66e985:
            trace.append(('removeall',this))
            if mutation:
                globalFlag^=1;uc.mem_write(0x137d76c,bytes([globalFlag]));regions[0x2100000][0x44]^=128;uc.mem_write(event+4,bytes([regions[0x2100000][0x44]]));struct.pack_into('<I',singleton,0,gui2);uc.mem_write(0x13b8790,bytes(singleton))
        else:trace.append(('warning',this));assert bytes(uc.mem_read(0x137d76c,1))==b'\1'
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x62b3f0,stop,count=1000);signed=lambda n:(n if n<128 else n-256)&0xffffffff;expected=[('lookup',pm,signed(value)),('hasmode',player,47)];proceed=not mode and state is not None and not(state&1)
    if not mode:expected +=[('lookup',pm,signed(value)),('creature',player+0x2c)]
    if proceed:
        expected +=[('gui',gui),('removeall',manager)]
        if globalFlag:expected.append(('pause',world,0));regions[0x2000000][0xe4]=0
        expected +=[('gui',gui2 if mutation else gui),('close',manager2 if mutation else manager),('lookup',pm,signed(value^128 if mutation else value)),('creature',player+0x2c)];regions[0x2800000 if mutation else 0x2300000][0x74]=0;globalFlag=1
        if has:idx=next((i for i,k in enumerate(keys) if k==23),len(keys));expected.append(('warning',0x2b00040+idx*0x20))
    assert trace==expected,(trace,expected);assert bytes(u.mem_read(0x137d76c,1))==bytes([globalFlag]);assert bytes(u.mem_read(0x13b8790,8))==singleton;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CloseHeroInformation_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/close-hero-information.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='close-hero-information-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<1 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            if i==0:grade,_,_=V.parity_of(source.read_text(),s['address'],s['symbol'],retail,work,env);assert grade=='NEAR_MATCH'
            else:assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for value in [0,127,128,255]:
        for mode in range(2):
            for keys in MAPS:
                for has in [False,True]:
                    for state in STATES:
                        for unpause in range(2):
                            for mutation in range(2):
                                for pattern in range(2):assert execute(candidate,value,mode,keys,has,state,unpause,mutation,pattern)==execute(native,value,mode,keys,has,state,unpause,mutation,pattern);count+=1
                                    # All native lookup receivers and side effects are asserted above.
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(8):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CLOSE_HERO_INFORMATION_BEHAVIOR_PASS scenarios=245760' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=245760,missing_provider_checks=8,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CLOSE_HERO_INFORMATION_PASS compiled=245760 comparative=%d'%count)
if __name__=='__main__':main()
