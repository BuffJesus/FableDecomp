"""Validate world frame/region forwarding and byte-exact pause/slow-motion events."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='00629260',file='CWorld_EAMoveHeroToRegion_00629260.cpp',symbol='?EAMoveHeroToRegion@CWorld@@QAEXPBVCGameEvent@@@Z',size=5),
       dict(address='00629270',file='CWorld_EAUpdateFrame_00629270.cpp',symbol='?EAUpdateFrame@CWorld@@QAEXPBVCGameEvent@@@Z',size=8),
       dict(address='006292d0',file='CWorld_EASetPauseMode_006292d0.cpp',symbol='?EASetPauseMode@CWorld@@QAEXPBVCGameEvent@@@Z',size=91),
       dict(address='00629330',file='CWorld_EASetSlowMotion_00629330.cpp',symbol='?EASetSlowMotion@CWorld@@QAEXPBVCGameEvent@@@Z',size=90)]
PACKAGE=ROOT/'rebuild/src/compiled/00/9f/CGameEvent_GetDataPackage_009f17a0.cpp'
PACKAGE_SYMBOL='?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z'
TARGETS={PACKAGE_SYMBOL:0x9f17a0,'?GetData@CGameEventDataPackage@@QAEXJPAX@Z':0x9f18f0,
         '?UpdateFrame@CWorld@@IAEXXZ':0x4a5a40,'?HandleMoveHeroToRegionGameEvent@CWorld@@QAEXABVCGameEvent@@@Z':0x49eaf0,
         '?SetAsPaused@CWorld@@IAEX_N@Z':0x49d8f0,'___security_cookie':0x139c8a8,'@__security_check_cookie@4':0xbfe9f9}

def execute(codes,entry,value,length,real_pause=False):
    obj,event,stack,stop=0x2000040,0x2100020,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x629000,0x1000),(0x9f1000,0x1000),(0x4a5000,0x1000),(0x49d000,0x2000),(0xbfe000,0x1000),(0x139c000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    if entry==0x85ca80:
        u.mem_map(0x85c000,0x1000);u.mem_map(0x416000,0x1000)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    initial=bytearray(bytes([255-value])*0x218)
    if entry==0x85ca80:struct.pack_into('<I',initial,0x48,0x12345678)
    u.mem_write(obj-0x40,bytes(initial))
    data=bytearray(bytes([value])*40);data[37]=length;u.mem_write(event,bytes(data));u.mem_write(0x139c8a8,struct.pack('<I',0x1234abcd))
    u.mem_write(stack,struct.pack('<II',stop,event));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x12450000+i)
    trace=[];readCount=[0]
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0x9f18f0:
            assert get(sp+4)==1 and get(this+32)==length and get(this+36)==0 and get(this+40)==0
            assert bytes(uc.mem_read(this,length))==bytes([value])*length;readCount[0]+=1;return
        if pc==0xbfe9f9:
            assert this==0x1234abcd;pop=0
        elif pc==0x4a5a40:trace.append(('frame',this));pop=0
        elif pc==0x49eaf0:trace.append(('region',this,get(sp+4)));pop=4
        elif pc==0x49d8f0:
            trace.append(('pause',this,get(sp+4)&0xff));pop=4
            if real_pause:return
        elif pc==0x4a5e90:trace.append(('autoload',this));pop=0
        elif pc==0x49dcc0:trace.append(('frontend',this));pop=0
        elif pc==0x4163b7:trace.append(('exclusive',this,get(sp+4)&0xff));pop=4
        else:return
        uc.reg_write(UC_X86_REG_EAX,0xabababab);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef)
        uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(entry,stop,count=1000)
    expected=[];expectedRead=0
    if entry==0x629260:expected=[('region',obj,event)]
    elif entry==0x629270:expected=[('frame',obj)]
    elif entry==0x6292d0:
        expected=[('pause',obj,int(value==1))];expectedRead=1
        if real_pause:initial[0x40+0xa4]=int(value==1)
    elif entry==0x6294a0:
        expected=[('autoload' if value==1 else 'frontend',obj)];expectedRead=1
    elif entry==0x85ca80:
        expected=[('exclusive',0x12345678,int(value!=0))];expectedRead=1
    else:initial[0x40+0xa5]=int(value==1);expectedRead=1
    assert trace==expected and readCount[0]==expectedRead
    assert bytes(u.mem_read(obj-0x40,len(initial)))==initial and bytes(u.mem_read(event,len(data)))==data
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x12450000+i for i,r in enumerate(saved))
    return trace,readCount[0],bytes(initial)

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldEventControl_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-event-control.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='world-event-control-'));objects=[];providers=[];entries=[];candidate={};native={}
    def compile(source,index):
        obj=work/('source%d.obj'%index);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;return obj
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS):
            source=a.stage/s['file'] if a.stage else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=compile(source,i);objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in (6,20);addend=struct.unpack_from('<I',built,site)[0];struct.pack_into('<I',patched,site,(TARGETS[name]+addend-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()));entries.append(dict(s,grade='RELOCATION_MATCH'))
        pobj=compile(PACKAGE,4);objects.append(pobj);c=Coff(pobj);sec,off=c.symbols[PACKAGE_SYMBOL];built,rel=c.sections[sec];assert off==0 and not rel
        retail=pe.get_data(0x9f17a0-0x400000,len(built));assert built==retail;candidate[0x9f17a0]=built;native[0x9f17a0]=retail
        # The general circular reader is still unreconstructed. Execute its retail
        # body in the native comparison; the compiled fixture marks that boundary.
        candidate[0x9f18f0]=native[0x9f18f0]=pe.get_data(0x9f18f0-0x400000,224)
    comparisons=0
    for value in range(256):
        for s in SPECS:
            address=int(s['address'],16)
            for length in ([1,8,32] if address in (0x6292d0,0x629330) else [1]):
                assert execute(candidate,address,value,length)==execute(native,address,value,length);comparisons+=1
    test=compile(a.test,5);assert not any(s['symbol'] in Coff(test).symbols for s in SPECS);objects.append(test)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(5):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'WORLD_EVENT_CONTROL_BEHAVIOR_PASS scenarios=16896' in r.stdout,r.stdout+r.stderr
    report=dict(entries=entries,providers=providers,comparative_scenarios=comparisons,behavior=r.stdout.strip(),missing_provider_checks=5,
                fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),package_sha256=hashlib.sha256(PACKAGE.read_bytes()).hexdigest(),
                headers={n:hashlib.sha256((a.include/n).read_bytes()).hexdigest() for n in ['fable_world.h','fable_game_event.h']})
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_EVENT_CONTROL_PASS providers=4 bytes=194 compiled=16896 comparative=%d'%comparisons)
if __name__=='__main__':main()
