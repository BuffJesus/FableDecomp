"""Validate free-camera mode and flag events through real package/reader/world providers."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_world_event_control import PACKAGE,PACKAGE_SYMBOL,TARGETS as BASE_TARGETS
from gate_world_respawn import READER,READER_SYMBOL
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
SPECS=[dict(address=a,file='CWorld_'+n+'_'+a+'.cpp',symbol='?'+n+'@CWorld@@QAEXPBVCGameEvent@@@Z',size=z,grade='RELOCATION_MATCH') for a,n,z in [('00629390','EASetFreeCameraMode',141),('00629420','EAUseFreeCamera',113)]]
WORLD=dict(address='0049d850',file='CWorld_GetPlayer_0049d850.cpp',symbol='?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z',size=8)
TARGETS=dict(BASE_TARGETS,**{WORLD['symbol']:0x49d850,'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z':0x6345c0,'?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z':0x634190})
HEADERS=['fable_player_manager.h']
def execute(codes,entry,value,number,length,pattern):
    world,event,player,stack,stop=0x2000040,0x2100040,0x2200040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32);boundary=[0x4498c0,0x6345c0,0x634190,0xbfe9f9]
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x139c000,0x2000000,0x2100000,0x2200000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x300) for a in [0x2000000,0x2100000,0x2200000]};manager=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,manager);regions[0x2100000][0x44]=number;regions[0x2100000][0x45:0x65]=bytes([value])*32;regions[0x2100000][0x65]=length
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(0x139c8a8,struct.pack('<I',0x1234abcd));u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[];reads=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x9f18f0:assert get(sp+4)==1 and get(this+32)==length and get(this+36)==0 and get(this+40)==0;assert bytes(uc.mem_read(this,length))==bytes([value])*length;reads.append(1);return
        if pc not in boundary:return
        pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0x6345c0:trace.append(('add',this,get(sp+4),get(sp+8)&255));pop=8
        elif pc==0x634190:trace.append(('remove',this,get(sp+4)));pop=4
        else:assert this==0x1234abcd
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,stop,count=1000);expected=[('lookup',manager,(number if number<128 else number-256)&0xffffffff)]
    if entry==0x629390:expected+=[('add',player,19,0) if value==1 else ('remove',player,19)]
    else:regions[0x2200000][0x260]=int(value==1)
    assert trace==expected and reads==[1];assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/FreeCameraEvents_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/free-camera-events.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='free-camera-events-gate-'));objects=[];providers=[];candidate={};native={}
    allspec=SPECS+[WORLD,dict(address='009f17a0',source=PACKAGE,symbol=PACKAGE_SYMBOL,size=53),dict(address='009f18f0',source=READER,symbol=READER_SYMBOL,size=166)]
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(allspec):
            source=s.get('source') or (a.stage/s['file'] if a.stage and s in SPECS else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file']);obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'],(s,len(built));address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail,s;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for value in range(256):
        for number in [0,1,17,19,127,128,254,255]:
            for length in [1,2,8,32]:
                for pattern in range(2):
                    for s in SPECS:entry=int(s['address'],16);assert execute(candidate,entry,value,number,length,pattern)==execute(native,entry,value,number,length,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in allspec);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(5):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'FREE_CAMERA_EVENTS_BEHAVIOR_PASS scenarios=2097152' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=2097152,missing_provider_checks=5,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('FREE_CAMERA_EVENTS_PASS compiled=2097152 comparative=%d'%count)
if __name__=='__main__':main()
