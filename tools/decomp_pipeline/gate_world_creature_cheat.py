"""Check main-player cheat event through recovered lookup and pointer providers."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('006292a0','CWorld_EACreatureCheat_006292a0.cpp','?EACreatureCheat@CWorld@@QAEXPBVCGameEvent@@@Z',37)]
CHAIN=[('00449970','00/44/CPlayerManager_GetMainPlayer_00449970.cpp','?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ',10),('004498c0','00/44/CPlayerManager_GetPlayer_004498c0.cpp','?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z',73),('00487dc0','00/48/CPlayer_GetPControlledCreature_00487dc0.cpp','?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ',8),('00a01b50','00/a0/CBaseIntelligentPointer_Operations_00a01b20.cpp','?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ',54)]
TARGETS={symbol:int(addr,16) for addr,_,symbol,_ in SPECS+CHAIN};TARGETS['??3@YAXPAX@Z']=0xbfe9bc;TARGETS['?Kill@CThing@@QAEX_N@Z']=0x4c9b80
COUNTS=[0,1,2,0xffffffff]
def execute(codes,match,state,count,value):
    p,item,data,m,w,event,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base in {addr&~0xfff for addr in codes}|{0x4c9000,0xbfe000,0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000,stop}:u.mem_map(base,0x1000)
    u.mem_map(0x3000000,0x10000)
    for addr,code in codes.items():u.mem_write(addr,bytes(code))
    regions={base:bytearray([0xa9]*0x300) for base in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000]}
    def put(addr,n):struct.pack_into('<I',regions[addr&~0xffff],addr&0xffff,n&0xffffffff)
    put(p+0x30,data if state else 0);put(data,item if state==2 else 0);put(data+4,COUNTS[count]);put(p+0x28,value*1234567+(0 if match else 1))
    put(w+0xc,m);put(m+0xc,m+0x100);put(m+0x10,m+0x104);put(m+0x14,m+0x104);put(m+0x18,0);put(m+0x1c,value*1234567);put(m+0x100,p);regions[0x2500000][0x44]=value
    for base,bytes_ in regions.items():u.mem_write(base,bytes(bytes_))
    u.mem_write(stack,struct.pack('<II',stop,0 if match else event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,w)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
    deletes=[];kills=[]
    def hook(uc,pc,size,user):
        if pc==0x4c9b80:
            sp=uc.reg_read(UC_X86_REG_ESP);kills.append((uc.reg_read(UC_X86_REG_ECX),struct.unpack('<I',uc.mem_read(sp+4,4))[0]));uc.reg_write(UC_X86_REG_EAX,0xaaaa5555);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+8);return
        if pc!=0xbfe9bc:return
        sp=uc.reg_read(UC_X86_REG_ESP);deletes.append(struct.unpack('<I',uc.mem_read(sp+4,4))[0]);uc.reg_write(UC_X86_REG_EAX,0xaaaa5555);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x6292a0,stop,count=500)
    if state==1:put(p+0x30,0);put(data+4,COUNTS[count]-1)
    assert deletes==([data] if state==1 and COUNTS[count]==1 else [])
    assert kills==([(item,1)] if state==2 else [])
    assert all(bytes(u.mem_read(base,len(v)))==v for base,v in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x12340000+i for i,r in enumerate(saved))
    return deletes,kills
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldCreatureCheat_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-creature-cheat.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='world-creature-cheat-'));objects=[];providers=[];candidate={};native={}
    specs=[(addr,(a.stage/file if a.stage else ROOT/'rebuild/src/compiled/00/62'/file),symbol,size) for addr,file,symbol,size in SPECS]+[(addr,ROOT/'rebuild/src/compiled'/path,symbol,size) for addr,path,symbol,size in CHAIN]
    with pefile.PE(str(EXE)) as pe:
        for i,(addr,source,symbol,size) in enumerate(specs):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;address=int(addr,16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,size);assert patched==retail,source;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for match in [False,True]:
        for state in range(3):
            for count in range(4):
                for value in range(256):assert execute(candidate,match,state,count,value)==execute(native,match,state,count,value);scenarios+=1
    obj=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert not any(s in Coff(obj).symbols for _,_,s,_ in specs);objects.append(obj)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(5):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'WORLD_CREATURE_CHEAT_BEHAVIOR_PASS scenarios=6144' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,missing_provider_checks=5,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={name:hashlib.sha256((a.include/name).read_bytes()).hexdigest() for name in ['fable_class.h','fable_world.h','fable_player_manager.h','fable_intelligent_pointer.h']})
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_CREATURE_CHEAT_PASS methods=1 bytes=37 compiled=6144 comparative=%d'%scenarios)
if __name__=='__main__':main()
