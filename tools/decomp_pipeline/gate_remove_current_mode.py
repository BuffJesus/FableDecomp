"""Verify signed mode removal with cached creature and real event readers."""
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
from gate_world_event_control import PACKAGE,PACKAGE_SYMBOL,TARGETS as CONTROL_TARGETS
from gate_world_respawn import READER,READER_SYMBOL
SPECS=[dict(address='0062c4b0',file='CWorld_EARemoveCurrentMode_0062c4b0.cpp',symbol='?EARemoveCurrentMode@CWorld@@QAEXPBVCGameEvent@@@Z',size=189,grade='RELOCATION_MATCH')]
EXTRA=PLAYER+[INVENTORY[-1],dict(address='009f17a0',file=PACKAGE.name,symbol=PACKAGE_SYMBOL,size=53),dict(address='009f18f0',file=READER.name,symbol=READER_SYMBOL,size=166)]
TARGETS=dict(CONTROL_TARGETS,**{s['symbol']:int(s['address'],16) for s in SPECS+EXTRA});TARGETS.update({'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ':0xa01b50,'?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z':0x634190,'?ResetProjectileWeaponMode@CTCHeroRangedAttacks@@QAEXXZ':0x789b30})
HEADERS=['fable_game_event.h','fable_thing_components.h'];NUMBERS=[0,1,-1,-0x80000000,0x7fffffff,129];MAPS=[[],[129],[-0x80000000,-1,17,129,129,130,0x7fffffff],[128,130],[0,17,48,54,273]];STATES=[None,0,1,255,2,254]

def execute(codes,value,mode,keys,has,state,length,mutation,pattern):
    world,event,player,creature,table,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x3008000,0x4000000;boundary=[0x4498c0,0xa01b50,0x634190,0x789b30,0xbfe9f9];u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x139c000,0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x400) for a in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000]};manager=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,manager);regions[0x2100000][0x44]=value;struct.pack_into('<I',regions[0x2100000],0x45,mode&0xffffffff);regions[0x2100000][0x65]=length;regions[0x2300000][0xd1]=state or 0;struct.pack_into('<I',regions[0x2300000],0x70,(0xfffffffd if pattern else 0)|(2 if has else 0));struct.pack_into('<III',regions[0x2300000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,k in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2400000],0x40+i*8,k&0xffffffff,0x2600040+i*0x20)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(0x139c8a8,struct.pack('<I',0x1234abcd));u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x9f17a0:trace.append(('package',this));return
        if pc==0x9f18f0:
            assert get(sp+4)==4 and get(this+32)==length and get(this+36)==0 and get(this+40)==0
            assert bytes(uc.mem_read(this,length))==regions[0x2100000][0x45:0x45+length];trace.append(('read',4));return
        if pc not in boundary:return
        pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0xa01b50:trace.append(('creature',this));result=0 if state is None else creature
        elif pc==0x634190:
            trace.append(('remove',this,get(sp+4)));pop=4
            if mutation==1:struct.pack_into('<I',regions[0x2200000],0x6c,0xdeadbeef);uc.mem_write(player+0x2c,struct.pack('<I',0xdeadbeef))
            if mutation==2 and state is not None:regions[0x2300000][0xd1]^=1;uc.mem_write(creature+0x91,bytes([regions[0x2300000][0xd1]]))
        elif pc==0x789b30:trace.append(('reset',this))
        else:assert this==0x1234abcd;trace.append(('cookie',this))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x62c4b0,stop,count=1000);expected=[('lookup',manager,(value if value<128 else value-256)&0xffffffff),('creature',player+0x2c),('package',event),('read',4),('remove',player,mode&0xffffffff)]
    if state is not None and not(regions[0x2300000][0xd1]&1) and has:idx=next((i for i,k in enumerate(keys) if k==129),len(keys));expected.append(('reset',0x2600040+idx*0x20))
    expected.append(('cookie',0x1234abcd));assert trace==expected,(trace,expected);assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/RemoveCurrentMode_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/remove-current-mode.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='remove-current-mode-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<1 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for value in [0,127,128,255]:
        for mode in NUMBERS:
            for keys in MAPS:
                for has in [False,True]:
                    for state in STATES:
                        for length in [4,8,32]:
                            for mutation in range(3):
                                for pattern in range(2):assert execute(candidate,value,mode,keys,has,state,length,mutation,pattern)==execute(native,value,mode,keys,has,state,length,mutation,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(6):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'REMOVE_CURRENT_MODE_BEHAVIOR_PASS scenarios=1658880' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=1658880,missing_provider_checks=6,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('REMOVE_CURRENT_MODE_PASS compiled=1658880 comparative=%d'%count)
if __name__=='__main__':main()
