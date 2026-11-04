"""Validate display reset callback list ownership without a display device."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *

ROOT=Path(__file__).resolve().parents[2]
ITEMS=[('Add',0x9c0190,47,10,'_malloc',0xbfea0e),('Remove',0x9bf5a0,59,43,'_free',0xbfea14)]
SYSTEM=0x2000000;HEAD=SYSTEM+0x1000;ALTERNATE=SYSTEM+0x1800;ALLOC=SYSTEM+0x1900
STACK=0x3000000;STOP=0x2200000

def execute(code,entry,kind,length,pattern,target,mutate):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for a,n in [(entry&~4095,4096),(0xbfe000,4096),(SYSTEM,8192),(STACK,4096),(STOP,4096)]:uc.mem_map(a,n)
    def put(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    uc.mem_write(entry,bytes(code));uc.mem_write(SYSTEM,b'\xa5'*8192)
    nodes=[HEAD]+[HEAD+16*(i+1) for i in range(length)]
    for i,node in enumerate(nodes):
        put(node,nodes[(i+1)%len(nodes)]);put(node+4,nodes[i-1]);put(node+8,0x12345678 if i==0 else (pattern+i-1)%4)
    put(ALTERNATE,ALTERNATE);put(ALTERNATE+4,ALTERNATE);put(SYSTEM+0x264,HEAD)
    expected=bytearray(uc.mem_read(SYSTEM,8192));events=[];returned=False
    def expect_put(a,v):struct.pack_into('<I',expected,a-SYSTEM,v)
    if kind=='Add':
        previous=nodes[-1];expect_put(ALLOC,HEAD);expect_put(ALLOC+4,previous);expect_put(ALLOC+8,target)
        expect_put(previous,ALLOC);expect_put(HEAD+4,ALLOC)
        if mutate:expect_put(SYSTEM+0x264,ALTERNATE)
    else:
        found=next((node for i,node in enumerate(nodes[1:]) if (pattern+i)%4==target),None)
        if found:
            previous=get(found+4);following=get(found);expect_put(previous,following);expect_put(following+4,previous)
            expected[found-SYSTEM:found-SYSTEM+12]=b'\xdd'*12
            if mutate:expect_put(SYSTEM+0x264,ALTERNATE)
    def hook(uc,a,size,data):
        nonlocal returned
        if a==STOP:returned=True;uc.emu_stop();return
        if a not in [0xbfea0e,0xbfea14]:return
        sp=uc.reg_read(UC_X86_REG_ESP);arg=get(sp+4)
        if a==0xbfea0e:
            assert kind=='Add' and arg==12;events.append(('allocate',arg));result=ALLOC
        else:
            assert kind=='Remove' and arg==found
            assert get(previous)==following and get(following+4)==previous
            events.append(('free',arg));uc.mem_write(arg,b'\xdd'*12);result=0
        if mutate:put(SYSTEM+0x264,ALTERNATE)
        ret=get(sp);uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xdead0001);uc.reg_write(UC_X86_REG_EDX,0xdead0002)
        uc.reg_write(UC_X86_REG_ESP,sp+4);uc.reg_write(UC_X86_REG_EIP,ret)
    uc.hook_add(UC_HOOK_CODE,hook);sp=STACK+0x800;put(sp,STOP);put(sp+4,target)
    uc.reg_write(UC_X86_REG_ESP,sp);uc.reg_write(UC_X86_REG_ECX,SYSTEM)
    preserved=[UC_X86_REG_EBX,UC_X86_REG_EBP,UC_X86_REG_ESI,UC_X86_REG_EDI]
    for i,r in enumerate(preserved):uc.reg_write(r,0xabcd0000+i)
    uc.emu_start(entry,STOP+1,count=10000)
    assert returned and uc.reg_read(UC_X86_REG_ESP)==sp+8
    for i,r in enumerate(preserved):assert uc.reg_read(r)==0xabcd0000+i
    after=bytes(uc.mem_read(SYSTEM,8192));assert after==expected,(kind,length,pattern,target,mutate)
    assert len(events)==(1 if kind=='Add' or found else 0)
    return events,after

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9b/CDisplayManager_DeviceResetCallbacks_009bf5a0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9b/CDisplayManager_DeviceResetCallbacks_009bf5a0_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/device-reset-callbacks.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='device-reset-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out in [(args.source,obj),(args.test,test)]:
        r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);pe=pefile.PE(str(EXE));reports=[]
    for name,address,length,site,callee,target in ITEMS:
        symbol='?'+name+'DeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z'
        sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec]
        assert offset==0 and relocs=={site:(callee,20)} and built[site:site+4]==b'\0'*4
        retail=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length)
        native_site=site if name=='Add' else 48
        assert (address+native_site+4+int.from_bytes(retail[native_site:native_site+4],'little',signed=True))==target
        candidate=bytearray(built);struct.pack_into('<I',candidate,site,(target-address-site-4)&0xffffffff)
        score=compare(built,retail,relocs,retail_relocations(f'{address:08x}',length));assert score.near_match,score
        scenarios=list(itertools.product([0,1,2,5,32],range(4),range(6),range(2)))
        for scenario in scenarios:assert execute(candidate,address,name,*scenario)==execute(retail,address,name,*scenario),scenario
        reports.append(dict(address=f'{address:08x}',grade='RELOCATION_MATCH' if candidate==retail else 'NEAR_MATCH',built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),comparative=len(scenarios)))
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DEVICE_RESET_CALLBACKS_BEHAVIOR_PASS scenarios=216' in r.stdout,r.stdout+r.stderr
    proof=dict(functions=reports,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('DEVICE_RESET_CALLBACKS_PASS functions=2 compiled=216 comparative=480')

if __name__=='__main__':main()
