"""Verify combat block clearing with required mode-manager component and real map lookup."""
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
SPECS=[dict(address='007338a0',file='CTCCombat_UnsetBlocking_007338a0.cpp',symbol='?UnsetBlocking@CTCCombat@@QAEXXZ',size=102,grade='RELOCATION_MATCH')]
EXTRA=[INVENTORY[-1]];TARGETS={s['symbol']:int(s['address'],16) for s in SPECS+EXTRA};TARGETS['?RemoveMode@CTCCreatureModeManager@@QAEXK@Z']=0x73aba0
HEADERS=['fable_thing_components.h'];MAPS=[[],[49],[-0x80000000,-1,17,49,49,50,0x7fffffff],[48,50],[0,17,23,48,54,273]]

def execute(codes,value,keys,mutation,pattern):
    combat,parent,table,stack,stop=0x2000040,0x2100040,0x2200040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x73a000,0x2000000,0x2100000,0x2200000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x300) for a in [0x2000000,0x2100000,0x2200000]};struct.pack_into('<I',regions[0x2000000],0x44,parent);regions[0x2000000][0xa2]=value;regions[0x2000000][0xa3]=(value*17+97)&255;struct.pack_into('<I',regions[0x2100000],0x64,(0xfffdffff if pattern else 0)|0x20000);struct.pack_into('<III',regions[0x2100000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,k in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2200000],0x40+i*8,k&0xffffffff,0x2300040+i*0x20)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(stack,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,combat);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[];idx=next((i for i,k in enumerate(keys) if k==49),len(keys))
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        if pc!=0x73aba0:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);trace.append((this,get(sp+4)));assert bytes(uc.mem_read(combat+0x62,2))==bytes([value,(value*17+97)&255])
        if mutation==1:uc.mem_write(combat+0x62,b'\x7f\xff')
        if mutation==2:struct.pack_into('<I',regions[0x2000000],0x44,0xdeadbeef);uc.mem_write(combat+4,struct.pack('<I',0xdeadbeef))
        uc.reg_write(UC_X86_REG_EAX,0xabababab);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x7338a0,stop,count=300);regions[0x2000000][0xa2:0xa4]=b'\0\0';assert trace==[(0x2300040+idx*0x20,12)];assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+4 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/UnsetBlocking_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/unset-blocking.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='unset-blocking-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<1 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for value in range(256):
        for keys in MAPS:
            for mutation in range(3):
                for pattern in range(2):assert execute(candidate,value,keys,mutation,pattern)==execute(native,value,keys,mutation,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'UNSET_BLOCKING_BEHAVIOR_PASS scenarios=1966080' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=1966080,missing_provider_checks=2,required_component=49,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('UNSET_BLOCKING_PASS compiled=1966080 comparative=%d'%count)
if __name__=='__main__':main()
