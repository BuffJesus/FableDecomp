"""Check typed player creature accessors through real pointer and event providers."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('00487dc0','CPlayer_GetPControlledCreature_00487dc0.cpp','?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ',8),('00487dd0','CPlayer_PeekPControlledCreature_00487dd0.cpp','?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ',8)]
CHAIN=[('00a01b50','00/a0/CBaseIntelligentPointer_Operations_00a01b20.cpp','?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ',54),('0062c6e0','00/62/CWorld_EAOpenPCMSNChatMenu_0062c6e0.cpp','?EAOpenPCMSNChatMenu@CWorld@@QAEXPBVCGameEvent@@@Z',24),('0049d850','00/49/CWorld_GetPlayer_0049d850.cpp','?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z',8),('004498c0','00/44/CPlayerManager_GetPlayer_004498c0.cpp','?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z',73)]
TARGETS={symbol:int(addr,16) for addr,_,symbol,_ in SPECS+CHAIN};TARGETS['??3@YAXPAX@Z']=0xbfe9bc
COUNTS=[0,1,2,0xffffffff]
def execute(codes,entry,state,count,value):
    p,item,data,m,w,event,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base in {addr&~0xfff for addr in codes}|{0xbfe000,0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000,stop}:u.mem_map(base,0x1000)
    u.mem_map(0x3000000,0x10000)
    for addr,code in codes.items():u.mem_write(addr,bytes(code))
    regions={base:bytearray([0xa9]*0x300) for base in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000]}
    def put(addr,n):struct.pack_into('<I',regions[addr&~0xffff],addr&0xffff,n&0xffffffff)
    put(p+0x30,data if state else 0);put(data,item if state==2 else 0);put(data+4,COUNTS[count]);put(p+0x28,value if value<128 else value-256)
    put(w+0xc,m);put(m+0xc,m+0x100);put(m+0x10,m+0x104);put(m+0x14,m+0x104);put(m+0x18,0);put(m+0x100,p);regions[0x2500000][0x44]=value
    for base,bytes_ in regions.items():u.mem_write(base,bytes(bytes_))
    u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,w if entry==2 else p)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
    deletes=[]
    def hook(uc,pc,size,user):
        if pc!=0xbfe9bc:return
        sp=uc.reg_read(UC_X86_REG_ESP);deletes.append(struct.unpack('<I',uc.mem_read(sp+4,4))[0]);uc.reg_write(UC_X86_REG_EAX,0xaaaa5555);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start([0x487dc0,0x487dd0,0x62c6e0][entry],stop,count=500)
    if state==1:put(p+0x30,0);put(data+4,COUNTS[count]-1)
    assert deletes==([data] if state==1 and COUNTS[count]==1 else [])
    if entry<2:assert u.reg_read(UC_X86_REG_EAX)==(item if state==2 else 0)
    assert all(bytes(u.mem_read(base,len(v)))==v for base,v in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if entry==2 else 4) and all(u.reg_read(r)==0x12340000+i for i,r in enumerate(saved))
    return deletes
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PlayerCreature_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/player-creature.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='player-creature-'));objects=[];providers=[];candidate={};native={}
    specs=[(addr,(a.stage/file if a.stage else ROOT/'rebuild/src/compiled/00/48'/file),symbol,size) for addr,file,symbol,size in SPECS]+[(addr,ROOT/'rebuild/src/compiled'/path,symbol,size) for addr,path,symbol,size in CHAIN]
    with pefile.PE(str(EXE)) as pe:
        for i,(addr,source,symbol,size) in enumerate(specs):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;address=int(addr,16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,size);assert patched==retail,source;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for entry in range(3):
        for state in range(3):
            for count in range(4):
                for value in range(256):assert execute(candidate,entry,state,count,value)==execute(native,entry,state,count,value);scenarios+=1
    obj=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert not any(s in Coff(obj).symbols for _,_,s,_ in specs);objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(6):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'PLAYER_CREATURE_BEHAVIOR_PASS scenarios=9216' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,missing_provider_checks=6,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={name:hashlib.sha256((a.include/name).read_bytes()).hexdigest() for name in ['fable_class.h','fable_world.h','fable_player_manager.h','fable_intelligent_pointer.h']})
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PLAYER_CREATURE_PASS methods=2 bytes=16 compiled=9216 comparative=%d'%scenarios)
if __name__=='__main__':main()
