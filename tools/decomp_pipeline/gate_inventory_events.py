"""Check inventory event dispatch and genuine signed component-map searches."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
LOWER_SYMBOL='?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@_STL@@ABW4ETCInterfaceType@@@Z'
SPECS=[dict(address=a,file='CWorld_'+n+'_'+a+'.cpp',symbol='?'+n+'@CWorld@@QAEXPBVCGameEvent@@@Z',size=80,grade='RELOCATION_MATCH',leaf=leaf) for a,n,leaf in [('0062c8e0','EAOpenPCMapMenu',0x589a30),('0062c930','EAOpenPCStatusMenu',0x58a763),('0062c980','EADeleteMenuComponents',0x587a41)]]+[dict(address='0040f020',file='CVectorMapThingComponents_LowerBound_0040f020.cpp',symbol=LOWER_SYMBOL,size=60,grade='MATCH')]
EXTRA=[dict(address='0049d850',file='CWorld_GetPlayer_0049d850.cpp',symbol='?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z',size=8),dict(address='00487dc0',file='CPlayer_GetPControlledCreature_00487dc0.cpp',symbol='?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ',size=8)]
TARGETS={s['symbol']:int(s['address'],16) for s in SPECS+EXTRA}
TARGETS.update({'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ':0xa01b50,'?OpenPCMapMenu@CTCInventory@@QAEXXZ':0x589a30,'?OpenPCStatusMenu@CTCInventory@@QAEXXZ':0x58a763,'?DeleteMenuComponents@CTCInventory@@QAEXXZ':0x587a41})
MAPS=[[],[17],[-0x80000000,-1,0,17,17,18,0x7fffffff],[16,18],[0,1,16,48,54,273]]
QUERIES=[-0x80000000,-1,0,1,16,17,18,273,274,0x7fffffff]
HEADERS=['fable_world.h','fable_thing_components.h']
def execute(codes,s,value,has,keys,pattern,query=None,inventory_leaves=None,component_type=17,controlled_result=None):
    world,event,player,creature,table,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    boundary=[0x4498c0,0xa01b50]+(list(inventory_leaves) if inventory_leaves is not None else [0x589a30,0x58a763,0x587a41])
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in boundary}|{a&~0xfff for a in [world,event,player,creature,table,stop]}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,data in codes.items():u.mem_write(a,bytes(data))
    for a in boundary:u.mem_write(a,b'\xc3')
    regions={base:bytearray(bytes([pattern*71])*0x400) for base in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000]}
    manager=0x14500000+pattern*0x1000;struct.pack_into('<I',regions[0x2000000],0x4c,manager);regions[0x2100000][0x44]=value;struct.pack_into('<I',regions[0x2300000],0x60,((0xffffffff & ~(1<<component_type)) if pattern else 0)|((1<<component_type) if has else 0));struct.pack_into('<III',regions[0x2300000],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,key in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2400000],0x40+i*8,key&0xffffffff,0x2500040+i*0x20)
    for base,data in regions.items():u.mem_write(base,bytes(data))
    lower=query is not None;u.mem_write(stack,struct.pack('<II',stop,stack+0x80 if lower else event));u.mem_write(stack+0x80,struct.pack('<I',(query or 0)&0xffffffff));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,creature+0x44 if lower else world)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        if pc not in boundary:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);pop=0;result=0xabababab
        if pc==0x4498c0:trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0xa01b50:trace.append(('creature',this));result=creature if controlled_result is None else controlled_result
        else:trace.append(('inventory',pc,this))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x40f020 if lower else int(s['address'],16),stop,count=200)
    if lower:
        index=next((i for i,k in enumerate(keys) if k>=query),len(keys));assert u.reg_read(UC_X86_REG_EAX)==table+index*8 and not trace
    else:
        expected=[('lookup',manager,(value if value<128 else value-256)&0xffffffff),('creature',player+0x2c)]
        if has and s.get('leaf') is not None:
            index=next((i for i,k in enumerate(keys) if k==component_type),len(keys));expected.append(('inventory',s['leaf'],0x2500040+index*0x20))
        assert trace==expected,(s,value,has,keys,trace,expected)
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items()) and u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return trace,u.reg_read(UC_X86_REG_EAX) if lower else 0
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/InventoryEvents_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/inventory-events.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='inventory-events-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<4 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for value in range(256):
        for s in SPECS[:3]:
            for has in [False,True]:
                for keys in MAPS:
                    for pattern in range(2):assert execute(candidate,s,value,has,keys,pattern)==execute(native,s,value,has,keys,pattern);scenarios+=1
        if value%32==0:print('INVENTORY_EVENTS_PROGRESS comparisons=%d'%scenarios,flush=True)
    for keys in MAPS:
        for query in QUERIES:
            for pattern in range(4):assert execute(candidate,SPECS[-1],0,False,keys,pattern,query)==execute(native,SPECS[-1],0,False,keys,pattern,query);scenarios+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(6):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'INVENTORY_EVENTS_BEHAVIOR_PASS scenarios=30920' in r.stdout,r.stdout+r.stderr
    report=dict(entries=SPECS,providers=providers,comparative_scenarios=scenarios,compiled_scenarios=30920,missing_provider_checks=6,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('INVENTORY_EVENTS_PASS providers=6 compiled=30920 comparative=%d'%scenarios)
if __name__=='__main__':main()

