"""Verify block termination with real main-player identity and component lookup."""
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
SPECS=[dict(address='0062b000',file='CWorld_EAControlledCreatureBlockEnd_0062b000.cpp',symbol='?EAControlledCreatureBlockEnd@CWorld@@QAEXPBVCGameEvent@@@Z',size=110,grade='NEAR_MATCH'),dict(address='00487520',file='CPlayer_IsMainPlayer_00487520.cpp',symbol='?IsMainPlayer@CPlayer@@QBE_NXZ',size=19,grade='RELOCATION_MATCH')]
EXTRA=PLAYER+[INVENTORY[-1],LOOKUP[-1]];TARGETS={s['symbol']:int(s['address'],16) for s in SPECS+EXTRA};TARGETS.update({'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ':0xa01b50,'?UnsetBlocking@CTCCombat@@QAEXXZ':0x7338a0})
HEADERS=['fable_world.h','fable_player_manager.h','fable_thing_components.h'];NUMBERS=[0,1,-1,127,-128,-0x80000000,0x7fffffff,19,48,17,270,273];MAPS=[[],[48],[-0x80000000,-1,17,48,48,49,0x7fffffff],[47,49],[0,17,47,54,273]];STATES=[None,0,1,255,2,254]
def execute(codes,value,keys,has,state,ownNumber,mainNumber,pattern,identity=False):
    world,event,player,creature,table,ownManager,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x3008000,0x4000000;boundary=[0x4498c0,0xa01b50,0x7338a0];u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x400) for a in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000]};lookup=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,lookup);regions[0x2100000][0x44]=value;struct.pack_into('<I',regions[0x2200000],0x50,ownManager);struct.pack_into('<I',regions[0x2200000],0x68,ownNumber&0xffffffff);struct.pack_into('<I',regions[0x2500000],0x5c,mainNumber&0xffffffff);regions[0x2300000][0xd1]=state or 0;struct.pack_into('<I',regions[0x2300000],0x64,(0xfffeffff if pattern else 0)|(0x10000 if has else 0));struct.pack_into('<III',regions[0x2300000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,k in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2400000],0x40+i*8,k&0xffffffff,0x2600040+i*0x20)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,player if identity else world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x487520:trace.append(('identity',this));return
        if pc==0x449700:trace.append(('number',this));return
        if pc not in boundary:return
        pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0xa01b50:trace.append(('creature',this));result=0 if state is None else creature
        else:trace.append(('unset',this))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x487520 if identity else 0x62b000,stop,count=300);expected=[('identity',player),('number',ownManager)]
    if identity:assert u.reg_read(UC_X86_REG_EAX)&255==int(ownNumber==mainNumber)
    else:
        expected=[('lookup',lookup,(value if value<128 else value-256)&0xffffffff),('creature',player+0x2c)]+expected
        if ownNumber==mainNumber and state is not None and not(state&1) and has:idx=next((i for i,k in enumerate(keys) if k==48),len(keys));expected.append(('unset',0x2600040+idx*0x20))
    assert trace==expected;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+(4 if identity else 8) and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/BlockEndEvent_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/block-end-event.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='block-end-event-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<2 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            if i==0:grade,_,_=V.parity_of(source.read_text(),s['address'],s['symbol'],retail,work,env);assert grade=='NEAR_MATCH'
            else:assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for pattern in range(4):
        for own in NUMBERS:
            for main in NUMBERS:assert execute(candidate,0,[],False,None,own,main,pattern,True)==execute(native,0,[],False,None,own,main,pattern,True);count+=1
    for value in [0,1,17,19,127,128,254,255]:
        for keys in MAPS:
            for has in [False,True]:
                for state in STATES:
                    for main in [False,True]:
                        for pattern in range(2):own=NUMBERS[value%12];target=own if main else own^0x80000000;assert execute(candidate,value,keys,has,state,own,target,pattern)==execute(native,value,keys,has,state,own,target,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(6):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'BLOCK_END_EVENT_BEHAVIOR_PASS scenarios=123456' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=123456,missing_provider_checks=6,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('BLOCK_END_EVENT_PASS compiled=123456 comparative=%d'%count)
if __name__=='__main__':main()
