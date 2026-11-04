"""Verify console toggling and lazy singleton call order with two real providers."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_player_lookup import SPECS as LOOKUP
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
FILE='CWorld_EAConsoleToggleState_0062a110.cpp';SYMBOL='?EAConsoleToggleState@CWorld@@QAEXPBVCGameEvent@@@Z';SPEC=dict(address='0062a110',file=FILE,symbol=SYMBOL,size=103,grade='RELOCATION_MATCH');EXTRA=[LOOKUP[3]];HEADERS=['fable_game.h','fable_console.h','fable_player_manager.h']
TARGETS={EXTRA[0]['symbol']:0x449970,'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?PMainGameComponent@CMainGameComponent@@1PAV1@A':0x13b86a0,'?g_FableConsolePClass_013CAA40@@3V?$CCountedPointer@VCConsole@@@@A':0x13caa40,'??2@YAPAXI@Z':0xbfea1a,'??0CConsole@@QAE@XZ':0x9ecd80,'?Reset@?$CCountedPointer@VCConsole@@@@QAEXPAVCConsole@@@Z':0x413520,'?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z':0x634190,'?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z':0x6345c0}
NUMBERS=[0,1,-1,127,-128,-0x80000000,0x7fffffff,19]
def execute(codes,number,lazy,active,pattern):
    component,manager,player,console,allocated,world,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32);boundary=[0x4498c0,0xbfea1a,0x9ecd80,0x413520,0x634190,0x6345c0]
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x13b8000,0x13ca000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x2000000,0x18000);u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*(0x16400 if a==0x2000000 else 0x400)) for a in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000]};struct.pack_into('<I',regions[0x2000000],0x5c,manager);struct.pack_into('<I',regions[0x2100000],0x5c,number&0xffffffff);regions[0x2300000][0xb8]=active
    for a,data in regions.items():u.mem_write(a,bytes(data))
    main=struct.pack('<I',component);singleton=struct.pack('<II',0 if lazy else console,0);u.mem_write(0x13b86a0,main);u.mem_write(0x13caa40,singleton);u.mem_write(stack,struct.pack('<II',stop,0xdeadbeef));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        if pc not in boundary:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0xbfea1a:trace.append(('allocate',get(sp+4)));result=allocated
        elif pc==0x9ecd80:trace.append(('construct',this));assert this==allocated;uc.mem_write(allocated+0x78,bytes([active]));result=allocated
        elif pc==0x413520:trace.append(('reset',this,get(sp+4)));pop=4;assert this==0x13caa40;uc.mem_write(this,struct.pack('<II',get(sp+4),0))
        elif pc==0x634190:trace.append(('remove',this,get(sp+4)));pop=4
        else:trace.append(('add',this,get(sp+4),get(sp+8)&255));pop=8
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x62a110,stop,count=200);expected=[('lookup',manager,number&0xffffffff)]
    if lazy:expected += [('allocate',0x9c),('construct',allocated),('reset',0x13caa40,allocated)];regions[0x2400000][0xb8]=active
    expected.append(('remove',player,44) if active else ('add',player,44,0));assert trace==expected;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert bytes(u.mem_read(0x13b86a0,4))==main and bytes(u.mem_read(0x13caa40,8))==struct.pack('<II',allocated if lazy else console,0);assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/62'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ConsoleToggleEvent_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/console-toggle-event.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='console-toggle-event-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate([SPEC]+EXTRA):
            source=a.source if i==0 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for number in NUMBERS:
        for lazy in [False,True]:
            for active in range(256):
                for pattern in range(4):assert execute(candidate,number,lazy,active,pattern)==execute(native,number,lazy,active,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in [SPEC]+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'CONSOLE_TOGGLE_EVENT_BEHAVIOR_PASS scenarios=16384' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=16384,missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CONSOLE_TOGGLE_EVENT_PASS compiled=16384 comparative=%d'%count)
if __name__=='__main__':main()
