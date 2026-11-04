"""Validate typed player-event forwarders, signed player IDs, and the real world lookup."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
ROWS=[('0049d850','GetPlayer',8,0,0),('006291f0','EAControlledCreatureUnfreezeControls',24,3,0),('00629220','EAControlledCreatureStartThrowObject',28,1,16),('00629280','EAFirstPersonViewStart',28,1,16),('00629510','EAFirstPersonTargeting',28,1,22),('00629240','EAControlledCreatureDeactivateZTarget',29,2,2),('00629530','EAControlledCreatureDeactivateLightning',29,2,40),('00629550','EATogglePassiveAggressiveMode',24,4,0),('0062c6e0','EAOpenPCMSNChatMenu',24,5,0)]
SPECS=[dict(address=a,name=n,size=b,kind=k,mode=m,file='CWorld_'+n+'_'+a+'.cpp',symbol='?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z' if a=='0049d850' else '?'+n+'@CWorld@@QAEXPBVCGameEvent@@@Z') for a,n,b,k,m in ROWS]
TARGETS={'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z':0x49d850,'?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z':0x6345c0,'?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z':0x634190,'?UnfreezeControls@CPlayer@@QAEXXZ':0x487120,'?ToggleAggressiveMode@CPlayer@@QAEXXZ':0x487e90,'?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ':0x487dc0}
def execute(codes,s,value,pattern):
    world,event,stack,stop=0x2000040,0x2100040,0x3008000,0x4000000
    manager=0x12345000+pattern*0x1000;number=value if not s['kind'] else (value if value<128 else value-256)
    number&=0xffffffff;player=0x2200000+(number&3)*0x100
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x49d000,0x1000),(0x629000,0x1000),(0x62c000,0x1000),(0x634000,0x1000),(0x449000,0x1000),(0x487000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    before=bytearray([pattern*71]*0x218);struct.pack_into('<I',before,0x40+12,manager);u.mem_write(world-0x40,bytes(before))
    data=bytearray([value&255]*0xa8);u.mem_write(event-0x40,bytes(data));arg=event if s['kind'] else value
    u.mem_write(stack,struct.pack('<II',stop,arg));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x11440000+i)
    trace=[]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0xabcdef12
        if pc==0x4498c0:
            trace.append(('lookup',this,get(sp+4)));pop=4;result=player
        elif pc==0x6345c0:trace.append(('add',this,get(sp+4),get(sp+8)));pop=8
        elif pc==0x634190:trace.append(('remove',this,get(sp+4)));pop=4
        elif pc==0x487120:trace.append(('unfreeze',this));pop=0
        elif pc==0x487e90:trace.append(('toggle',this));pop=0
        elif pc==0x487dc0:trace.append(('creature',this));pop=0;result=0
        else:return
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(int(s['address'],16),stop,count=200)
    expected=[('lookup',manager,number)]
    if s['kind']==1:expected.append(('add',player,s['mode'],0))
    elif s['kind']==2:expected.append(('remove',player,s['mode']))
    elif s['kind'] in (3,4,5):expected.append(({3:'unfreeze',4:'toggle',5:'creature'}[s['kind']],player))
    assert trace==expected,(s,value,trace,expected)
    if not s['kind']:assert u.reg_read(UC_X86_REG_EAX)==player
    assert bytes(u.mem_read(world-0x40,len(before)))==before and bytes(u.mem_read(event-0x40,len(data)))==data
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x11440000+i for i,r in enumerate(saved))
    return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PlayerEvents_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/player-events.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='player-events-'));objects=[];providers=[];candidate={};native={}
    def compile(source,index):
        obj=work/('source%d.obj'%index);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;return obj
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS):
            source=a.stage/s['file'] if a.stage else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,i);objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    comparisons=0
    for s in SPECS:
        for value in ([0,1,127,128,255,0xffffffff,0x80000000,0x7fffffff] if not s['kind'] else range(256)):
            for pattern in range(4):assert execute(candidate,s,value,pattern)==execute(native,s,value,pattern);comparisons+=1
    test=compile(a.test,9);assert not any(s['symbol'] in Coff(test).symbols for s in SPECS);objects.append(test)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(9):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'PLAYER_EVENTS_BEHAVIOR_PASS scenarios=8224' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,entries=[dict(s,grade='RELOCATION_MATCH') for s in SPECS],comparative_scenarios=comparisons,missing_provider_checks=9,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={n:hashlib.sha256((a.include/n).read_bytes()).hexdigest() for n in ['fable_world.h','fable_player_manager.h']})
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PLAYER_EVENTS_PASS providers=9 bytes=222 compiled=8224 comparative=%d'%comparisons)
if __name__=='__main__':main()
