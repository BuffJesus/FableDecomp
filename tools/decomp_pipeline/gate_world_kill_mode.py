"""Check player kill-mode toggling through the recovered world/player lookup."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
FILE='CWorld_EAToggleKillEverythingMode_00629570.cpp';SYMBOL='?EAToggleKillEverythingMode@CWorld@@QAEXPBVCGameEvent@@@Z'
CHAIN=[('0049d850','00/49/CWorld_GetPlayer_0049d850.cpp','?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z',8),('004498c0','00/44/CPlayerManager_GetPlayer_004498c0.cpp','?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z',73)]
TARGETS={symbol:int(addr,16) for addr,_,symbol,_ in CHAIN}
VALUES=[0,1,2,15,16,40,42,63,64,126,127,128,129,253,254,255]
def execute(codes,old,value,match):
    p,m,w,event,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base in {addr&~0xfff for addr in codes}|{0x2000000,0x2100000,0x2200000,0x2300000,stop}:u.mem_map(base,0x1000)
    u.mem_map(0x3000000,0x10000)
    for addr,code in codes.items():u.mem_write(addr,bytes(code))
    regions={base:bytearray([old]*0x300) for base in [0x2000000,0x2100000,0x2200000,0x2300000]}
    def put(addr,n):struct.pack_into('<I',regions[addr&~0xffff],addr&0xffff,n&0xffffffff)
    number=value if value<128 else value-256
    put(p+0x28,number if match else 256+value);put(w+0xc,m);put(m+0xc,m+0x100);put(m+0x10,m+0x104);put(m+0x14,m+0x104);put(m+0x18,0);put(m+0x100,p);regions[0x2300000][0x44]=value
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,w)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    u.emu_start(0x629570,stop,count=500);regions[0x2000000][0x40+0x21b]=int(old==0)
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return [bytes(data) for data in regions.values()]
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/62'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldKillMode_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-kill-mode.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='world-kill-mode-'));objects=[];providers=[];candidate={};native={}
    specs=[('00629570',a.source,SYMBOL,34)]+[(addr,ROOT/'rebuild/src/compiled'/path,symbol,size) for addr,path,symbol,size in CHAIN]
    with pefile.PE(str(EXE)) as pe:
        for i,(addr,source,symbol,size) in enumerate(specs):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;address=int(addr,16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,size);assert patched==retail,source;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for old in range(256):
        for value in VALUES:
            for match in [False,True]:assert execute(candidate,old,value,match)==execute(native,old,value,match);scenarios+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert not any(symbol in Coff(test).symbols for _,_,symbol,_ in specs);objects.append(test)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(3):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'WORLD_KILL_MODE_BEHAVIOR_PASS scenarios=8192' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,missing_provider_checks=3,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_player_manager.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_KILL_MODE_PASS bytes=34 compiled=8192 comparative=%d'%scenarios)
if __name__=='__main__':main()
