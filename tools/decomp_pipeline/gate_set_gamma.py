"""Validate gamma-ramp construction without calling a real D3D device."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from near_match import compare,retail_relocations
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
ENTRY=0x009beaa0
SYMBOL='?SetGamma@CDisplayManager@@QAEXM_N@Z'
TARGETS={'___security_cookie':0x139c8a8,'__real@3e99999a':0x1238170,
         '__real@40c00000':0x125a2a0,'__real@00000000':0x122dedc,
         '__real@477f0000':0x129b3f0,'__ftol2':0xbfea70,'@__security_check_cookie@4':0xbfe9f9}
INPUTS=[0,0x80000000,1,0x80000001,0x00800000,0xbf800000,0x3e999999,0x3e99999a,
        0x3e99999b,0x3f800000,0x3fa00000,0x40200000,0x40bfffff,0x40c00000,
        0x40c00001,0x41800000,0x7f800000,0xff800000,0x7fc12345,0xffc12345,0x7f812345]

def execute(code,pe,bits,flags,control):
    uc=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x9be000,4096),(0xbfe000,4096),(0x139c000,4096),(0x1238000,4096),
                      (0x125a000,4096),(0x122d000,4096),(0x129b000,4096),
                      (0x2000000,4096),(0x2100000,4096),(0x3000000,65536)]:uc.mem_map(base,size)
    def put(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def get(a):return struct.unpack('<I',uc.mem_read(a,4))[0]
    uc.mem_write(ENTRY,bytes(code))
    for name,address in TARGETS.items():
        if name.startswith('__real@'):uc.mem_write(address,pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,4))
    # Execute the actual retail float-to-integer helper, including its x87
    # rounding correction and invalid-value behavior. Only the cookie check
    # and COM call are mocked.
    uc.mem_write(0xbfea70,pe.get_data(0xbfea70-pe.OPTIONAL_HEADER.ImageBase,117))
    put(0x139c8a8,0x12345678)
    manager,device,table=0x2000000,0x2000400,0x2000500
    callback,stop=0x2100010,0x2100000
    uc.mem_write(manager,b'\xa5'*0x2c8);put(manager+0x58,device);put(device,table);put(table+0x54,callback)
    for a in [callback,0xbfe9f9]:uc.mem_write(a,b'\xc3')
    saved_memory=bytes(uc.mem_read(manager,4096));ramps=[];checked=[];done=[]
    def hook(uc,address,size,data):
        if address==stop:done.append(True);uc.emu_stop();return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xbfe9f9:
            assert uc.reg_read(UC_X86_REG_ECX)==0x12345678
            checked.append(True);argc=0
        else:
            assert address==callback and get(sp+4)==device and get(sp+8)==0 and get(sp+12)==flags&1
            ramps.append((get(manager+0x2c4),bytes(uc.mem_read(get(sp+16),0x600)).hex()))
            if flags&2:put(manager+0x2c4,0x40100000)
            argc=4
        uc.reg_write(UC_X86_REG_EAX,0xaaaaaaa0);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd)
        uc.reg_write(UC_X86_REG_ESP,sp+4+argc*4);uc.reg_write(UC_X86_REG_EIP,get(sp))
    uc.hook_add(UC_HOOK_CODE,hook,begin=stop,end=callback)
    uc.hook_add(UC_HOOK_CODE,hook,begin=0xbfe9f9,end=0xbfe9f9)
    sp=0x3008000;uc.mem_write(sp,struct.pack('<III',stop,bits,flags&1))
    uc.reg_write(UC_X86_REG_ECX,manager);uc.reg_write(UC_X86_REG_ESP,sp)
    uc.reg_write(UC_X86_REG_FPCW,control);uc.reg_write(UC_X86_REG_FPTAG,0xffff)
    saved={UC_X86_REG_EBX:0x11111111,UC_X86_REG_ESI:0x22222222,UC_X86_REG_EDI:0x33333333,UC_X86_REG_EBP:0x44444444}
    for r,v in saved.items():uc.reg_write(r,v)
    uc.emu_start(ENTRY,stop+1,count=100000)
    assert done and len(ramps)==1 and len(checked)==1 and uc.reg_read(UC_X86_REG_ESP)==sp+12
    assert all(uc.reg_read(r)==v for r,v in saved.items()) and uc.reg_read(UC_X86_REG_FPCW)==control
    after=bytes(uc.mem_read(manager,4096));assert after[:0x2c4]==saved_memory[:0x2c4] and after[0x2c8:]==saved_memory[0x2c8:]
    return ramps[0],get(manager+0x2c4),uc.reg_read(UC_X86_REG_FPSW)

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9b/CDisplayManager_SetGamma_009beaa0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9b/CDisplayManager_SetGamma_009beaa0_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/set-gamma.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='set-gamma-'));obj,test=work/'candidate.obj',work/'test.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        result=check_one.cl(['/nologo','/c','/O2','/Oy','/GS','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj);sec,off=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert off==0 and len(built)==266 and len(relocs)==9
    pe=pefile.PE(str(EXE));retail=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,266)
    candidate=bytearray(built);checked=[]
    for site,(symbol,kind) in relocs.items():
        target=TARGETS[symbol];assert kind==(20 if symbol in ['__ftol2','@__security_check_cookie@4'] else 6)
        assert built[site:site+4]==b'\0'*4
        if symbol.startswith('__real@'):
            constsec,constoff=coff.symbols[symbol];const=coff.sections[constsec][0][constoff:constoff+4]
            assert const==pe.get_data(target-pe.OPTIONAL_HEADER.ImageBase,4)
        value=target-(ENTRY+site+4 if kind==20 else 0);struct.pack_into('<I',candidate,site,value&0xffffffff)
        checked.append(dict(offset=site,symbol=symbol,target=f'{target:08x}'))
    score=compare(built,retail,relocs,retail_relocations('009beaa0',266));assert score.near_match
    scenarios=[]
    for bits in INPUTS:
        for flags in range(4):
            for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:
                native=execute(retail,pe,bits,flags,control);recovered=execute(candidate,pe,bits,flags,control)
                assert native==recovered,(hex(bits),flags,hex(control))
                scenarios.append(dict(input=f'{bits:08x}',flags=flags,control=f'{control:04x}',ramp_sha256=hashlib.sha256(bytes.fromhex(native[0][1])).hexdigest()))
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    alone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert alone.returncode!=0
    linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert linked.returncode==0,linked.stdout+linked.stderr
    result=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'SET_GAMMA_BEHAVIOR_PASS scenarios=84' in result.stdout,result.stdout+result.stderr
    grade='RELOCATION_MATCH' if candidate==retail else 'NEAR_MATCH'
    report=dict(address='009beaa0',grade=grade,bytes=266,similarity=score.to_dict(),relocations=checked,
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                behavior=result.stdout.strip(),comparative_scenarios=scenarios)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SET_GAMMA_PASS grade='+grade+' bytes=266 compiled=84 comparative='+str(len(scenarios)))

if __name__=='__main__':main()
