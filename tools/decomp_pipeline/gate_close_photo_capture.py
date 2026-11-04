"""Verify photo-capture close through seven independently compiled native providers."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_world_close_gui import SPECS as GUI,EXTRA as WORLD,TARGETS as GUI_TARGETS
from gate_inventory_events import SPECS as INVENTORY,EXTRA as PLAYER
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
FILE='CWorld_EAClosePhotoCapture_0062b4d0.cpp';SYMBOL='?EAClosePhotoCapture@CWorld@@QAEXPBVCGameEvent@@@Z';SPEC=dict(address='0062b4d0',file=FILE,symbol=SYMBOL,size=122,grade='RELOCATION_MATCH');EXTRA=WORLD+GUI[1:]+[PLAYER[1],INVENTORY[-1]];TARGETS=dict(GUI_TARGETS,**{s['symbol']:int(s['address'],16) for s in EXTRA});TARGETS.update({'?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ':0xa01b50,'?DestroyScreen@CTCPhotojournalCapture@@QAEXXZ':0x55d6c0})
HEADERS=['fable_player_manager.h','fable_thing_components.h'];MAPS=[[],[270],[-0x80000000,-1,17,270,270,271,0x7fffffff],[269,271],[0,17,48,54,269,273]]
def execute(codes,value,keys,has,pause,opened,pattern):
    world,event,gui,manager,player,creature,table,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x2600040,0x3008000,0x4000000;boundary=[0x4498c0,0x634190,0xa01b50,0x55d6c0];u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{0x13b8000,0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000,0x2600000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x400) for a in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,0x2500000,0x2600000]};pm=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,pm);regions[0x2000000][0xe4]=pause;regions[0x2100000][0x44]=value;struct.pack_into('<I',regions[0x2200000],0x36c,manager);regions[0x2300000][0x74]=opened;struct.pack_into('<I',regions[0x2500000],0x80,(0xffffbfff if pattern else 0)|(0x4000 if has else 0));struct.pack_into('<III',regions[0x2500000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,k in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2600000],0x40+i*8,k&0xffffffff,0x2700040+i*0x20)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    singleton=struct.pack('<II',gui,0x76543210);u.mem_write(0x13b8790,singleton);u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x66e73d:trace.append(('close',this));return
        if pc not in boundary:return
        assert bytes(uc.mem_read(world+0xa4,1))==b'\0';pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0x634190:trace.append(('remove',this,get(sp+4)));pop=4
        elif pc==0xa01b50:assert bytes(uc.mem_read(manager+0x34,1))==b'\0';trace.append(('creature',this));result=creature
        else:trace.append(('destroy',this))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x62b4d0,stop,count=300);expected=[('lookup',pm,(value if value<128 else value-256)&0xffffffff),('remove',player,45),('close',manager),('creature',player+0x2c)]
    if has:idx=next((i for i,k in enumerate(keys) if k==270),len(keys));expected.append(('destroy',0x2700040+idx*0x20))
    assert trace==expected;regions[0x2000000][0xe4]=0;regions[0x2300000][0x74]=0;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items()) and bytes(u.mem_read(0x13b8790,8))==singleton;assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/62'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ClosePhotoCapture_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/close-photo-capture.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='close-photo-capture-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate([SPEC]+EXTRA):
            source=a.source if i==0 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for value in range(256):
        for keys in MAPS:
            for has in [False,True]:
                for pause in [0,1]:
                    for opened in [0,255]:
                        for pattern in range(2):assert execute(candidate,value,keys,has,pause,opened,pattern)==execute(native,value,keys,has,pause,opened,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in [SPEC]+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(7):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CLOSE_PHOTO_CAPTURE_BEHAVIOR_PASS scenarios=81920' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=81920,missing_provider_checks=7,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CLOSE_PHOTO_CAPTURE_PASS compiled=81920 comparative=%d'%count)
if __name__=='__main__':main()
