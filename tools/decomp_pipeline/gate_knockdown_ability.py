"""Verify typed knockdown query and definition getter temporary lifetime."""
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
SPECS=[dict(address='007331a0',file='CTCCombat_IsAbleToBeKnockedDown_007331a0.cpp',symbol='?IsAbleToBeKnockedDown@CTCCombat@@QBE_NXZ',size=20,grade='RELOCATION_MATCH'),dict(address='00666d10',file='CThingCreatureBase_GetCombatTypeDef_00666d10.cpp',symbol='?GetCombatTypeDef@CThingCreatureBase@@QBEABVCCombatTypeDef@@XZ',size=47,grade='RELOCATION_MATCH')]
EXTRA=[];TARGETS={s['symbol']:int(s['address'],16) for s in SPECS};TARGETS['?GetPCombatTypeDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCCombatTypeDef@@@@XZ']=0x664d40;HEADERS=['fable_thing_components.h','fable_def_pointer.h','fable_creature_combat.h']

def execute(codes,value,ref,effect,selected,entry,pattern):
    combat,creature,defs,vtable,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x3008000,0x4000000;getter,deleter=0x664d40,0x4100000;u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{getter&~0xfff,deleter,stop,0x2000000,0x2100000,0x2200000,0x2300000}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x400) for a in [0x2000000,0x2100000,0x2200000,0x2300000]};struct.pack_into('<I',regions[0x2000000],0x64,creature)
    for i in range(2):struct.pack_into('<II',regions[0x2200000],0x40+i*0x100,vtable,ref);regions[0x2200000][0x40+i*0x100+0x9c]=value if i==selected else 93
    struct.pack_into('<II',regions[0x2300000],0x40,stop,deleter);expected={a:bytearray(data) for a,data in regions.items()};remaining=(ref-1)&0xffffffff;struct.pack_into('<I',expected[0x2200000],0x44+selected*0x100,remaining);flag=value
    if not remaining and effect==1:flag=0 if value else 255;expected[0x2200000][0x40+selected*0x100+0x9c]=flag
    if (not remaining and effect==2) or effect==3:struct.pack_into('<I',expected[0x2000000],0x64,creature+0x100)
    if not remaining and effect==2:expected[0x2200000][0x40+(1-selected)*0x100+0x9c]=77
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(stack,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,combat if entry==0 else creature);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc not in [getter,deleter]:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);ret=struct.unpack('<I',uc.mem_read(sp,4))[0]
        if pc==getter:
            assert this==creature;trace.append(('get',this));result=struct.unpack('<I',uc.mem_read(sp+4,4))[0];uc.mem_write(result,struct.pack('<I',defs+selected*0x100));uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ESP,sp+8)
            if effect==3:uc.mem_write(combat+0x24,struct.pack('<I',creature+0x100))
        else:
            assert this==defs+selected*0x100 and struct.unpack('<I',uc.mem_read(this+4,4))[0]==0;trace.append(('delete',this));uc.reg_write(UC_X86_REG_ESP,sp+4)
            if effect==1:uc.mem_write(this+0x9c,bytes([flag]))
            if effect==2:uc.mem_write(combat+0x24,struct.pack('<I',creature+0x100));uc.mem_write(defs+(1-selected)*0x100+0x9c,bytes([77]))
        uc.reg_write(UC_X86_REG_ECX,0xbad00001);uc.reg_write(UC_X86_REG_EDX,0xbad00002);uc.reg_write(UC_X86_REG_EIP,ret)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x7331a0 if entry==0 else 0x666d10,stop,count=300);assert u.reg_read(UC_X86_REG_EIP)==stop;result=u.reg_read(UC_X86_REG_EAX)&255 if entry==0 else u.reg_read(UC_X86_REG_EAX);assert result==(int(flag!=0) if entry==0 else defs+selected*0x100)
    assert trace==[('get',creature)]+([('delete',defs+selected*0x100)] if not remaining else [])
    for a,data in expected.items():assert bytes(u.mem_read(a,len(data)))==data
    assert u.reg_read(UC_X86_REG_ESP)==stack+4 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return result,trace

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/KnockdownAbility_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/knockdown-ability.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='knockdown-ability-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<len(SPECS) else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    count=0
    for value in [0,1,2,127,128,255]:
        for ref in [1,2,0xffffffff,0]:
            for effect in range(4):
                for selected in range(2):
                    for entry in range(2):
                        for pattern in range(4):assert execute(candidate,value,ref,effect,selected,entry,pattern)==execute(native,value,ref,effect,selected,entry,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'KNOCKDOWN_ABILITY_BEHAVIOR_PASS scenarios=65536' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=65536,missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('KNOCKDOWN_ABILITY_PASS compiled=65536 comparative=%d'%count)
if __name__=='__main__':main()
