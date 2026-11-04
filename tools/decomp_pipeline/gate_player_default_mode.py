"""Verify current mode and creature-default events with real typed providers."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_inventory_events import EXTRA
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
SPECS=[dict(address='00633be0',file='CPlayer_GetCurrentMode_00633be0.cpp',symbol='?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ',size=13,grade='MATCH'),dict(address='006298f0',file='CWorld_EAControlledCreatureDefault_006298f0.cpp',symbol='?EAControlledCreatureDefault@CWorld@@QAEXPBVCGameEvent@@@Z',size=64,grade='RELOCATION_MATCH')]
TARGETS={s['symbol']:int(s['address'],16) for s in SPECS+EXTRA};TARGETS.update({'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ':0xa01b50,'?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z':0x6345c0})
HEADERS=['fable_player_manager.h','fable_stlport_list.h'];MODES=[1,0,2,19,-1,-0x80000000,0x7fffffff,16,17,40,43,273];STATES=[None,0,1,255,2,254]
def execute(codes,mode,count,state,number,pattern,getter):
    world,event,player,creature,nodes,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32);boundary=[0x4498c0,0xa01b50,0x6345c0]
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x300) for a in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000]};manager=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,manager);regions[0x2100000][0x44]=number;struct.pack_into('<I',regions[0x2200000],0x250,nodes);regions[0x2300000][0xd1]=state or 0
    for i in range(count+1):struct.pack_into('<III',regions[0x2400000],0x40+i*12,(nodes+12 if count else nodes) if i==0 else (nodes+(i+1)*12 if i<count else nodes),nodes+count*12 if i==0 else nodes+(i-1)*12,(mode if i==0 or i==count else 40+i)&0xffffffff)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,player if getter else world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x633be0:trace.append(('mode',this));return
        if pc not in boundary:return
        pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0xa01b50:trace.append(('creature',this));result=0 if state is None else creature
        else:trace.append(('add',this,get(sp+4),get(sp+8)&255));pop=8
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x633be0 if getter else 0x6298f0,stop,count=200)
    if getter:expected=[('mode',player)];assert u.reg_read(UC_X86_REG_EAX)==mode&0xffffffff
    else:
        expected=[('lookup',manager,(number if number<128 else number-256)&0xffffffff),('creature',player+0x2c)]
        if state is not None and not(state&1):
            expected.append(('mode',player))
            if mode!=1:expected.append(('add',player,1,1))
    assert trace==expected;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+(4 if getter else 8) and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PlayerDefaultMode_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/player-default-mode.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='player-default-mode-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and s in SPECS else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for mode in MODES:
        for n in [0,1,3]:
            for pattern in range(2):
                assert execute(candidate,mode,n,None,0,pattern,True)==execute(native,mode,n,None,0,pattern,True);count+=1
                for state in STATES:
                    for number in [0,1,17,19,127,128,254,255]:assert execute(candidate,mode,n,state,number,pattern,False)==execute(native,mode,n,state,number,pattern,False);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(4):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'PLAYER_DEFAULT_MODE_BEHAVIOR_PASS scenarios=221328' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=221328,missing_provider_checks=4,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PLAYER_DEFAULT_MODE_PASS compiled=221328 comparative=%d'%count)
if __name__=='__main__':main()
