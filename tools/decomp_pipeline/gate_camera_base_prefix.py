"""Verify genuine camera-base virtual defaults and typed by-value camera forwarding."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
FILE='CAIGameCameraBase_DefaultPrefix_00697030.cpp'
SPECS=[('00697030','PreparePrimitivesForRendering','?PreparePrimitivesForRendering@CAIGameCameraBase@@UBEXABVCEnginePrimitivePrepareInfo@@@Z',3,1),('00697040','IsPosInViewingFrustrum','?IsPosInViewingFrustrum@CAIGameCameraBase@@UBE_NABVC3DVector@@@Z',5,1),('00697050','Is2DAreaInViewingFrustrum','?Is2DAreaInViewingFrustrum@CAIGameCameraBase@@UBE_NABVC3DVector@@M@Z',5,2),('00697060','IsVolumeInViewingFrustrum','?IsVolumeInViewingFrustrum@CAIGameCameraBase@@UBE_NABVC3DVector@@M@Z',5,2),('00697070','IsLineOfSightToPos','?IsLineOfSightToPos@CAIGameCameraBase@@UBE_NABVC3DVector@@@Z',5,1),('00697080','GetControlAngleXY','?GetControlAngleXY@CAIGameCameraBase@@UBEMXZ',29,0),('006970a0','SetChangeInControlDirection','?SetChangeInControlDirection@CAIGameCameraBase@@UAEXXZ',1,0),('006970b0','GetCameraManager','?GetCameraManager@CAIGameCameraBase@@UBE_NPAPBVCGameCameraManager@@@Z',5,1),('006970c0','GetCameraManager','?GetCameraManager@CAIGameCameraBase@@UAE_NPAPAVCGameCameraManager@@@Z',5,1)]
TARGETS={'?GetAngleXY@CCamera@@QBEMXZ':0xa0c380}
def execute(code,op,value):
    obj,vtable,arg,render,stack,stop=0x2000040,0x2100040,0x2200040,0x5000000,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base in [0x697000,0xa0c000,0x2000000,0x2100000,0x2200000,stop,render]:u.mem_map(base,0x1000)
    u.mem_map(0x3000000,0x10000);addr=int(SPECS[op][0],16);u.mem_write(addr,bytes(code))
    regions={base:bytearray([value]*0x100) for base in [0x2000000,0x2100000,0x2200000]};struct.pack_into('<I',regions[0x2000000],0x40,vtable);struct.pack_into('<I',regions[0x2100000],0x44,render)
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<III',stop,arg,value*0x1020304&0xffffffff));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj);u.reg_write(UC_X86_REG_EAX,0xabcdef00|value)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x78900000+i)
    trace=[];temp=[]
    def hook(uc,pc,size,user):
        if pc not in [render,0xa0c380]:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if pc==render:
            out,bits=struct.unpack('<II',uc.mem_read(sp+4,8));assert uc.reg_read(UC_X86_REG_ECX)==obj and bits==0x3f800000;assert stack-0x38<=out and out+0x38<=stack;uc.mem_write(out,bytes([value]*0x38));temp.append(out);trace.append('render');uc.reg_write(UC_X86_REG_EAX,out);cleanup=12
        else:
            assert temp and uc.reg_read(UC_X86_REG_ECX)==temp[0] and bytes(uc.mem_read(temp[0],0x38))==bytes([value]*0x38);trace.append('angle');uc.reg_write(UC_X86_REG_FPSW,0);uc.reg_write(UC_X86_REG_FPTAG,0xfffc);uc.reg_write(UC_X86_REG_FP0,(0xc000000000000000,0x4000));uc.reg_write(UC_X86_REG_EAX,0xaaaa5555);cleanup=4
        uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(addr,stop,count=100)
    assert trace==(['render','angle'] if op==5 else [])
    if op==5:assert u.reg_read(UC_X86_REG_FP0)==(0xc000000000000000,0x4000)
    if op in [1,2,3,4,7,8]:assert u.reg_read(UC_X86_REG_EAX)==0xabcdef00
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+4+4*SPECS[op][4] and all(u.reg_read(r)==0x78900000+i for i,r in enumerate(saved))
    return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/69'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraBasePrefix_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-base-prefix.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-base-prefix-'));obj=work/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);entries=[];candidate=[];native=[]
    with pefile.PE(str(EXE)) as pe:
        for addr,name,symbol,size,_ in SPECS:
            sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;patched=bytearray(built);address=int(addr,16)
            for site,(name_,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name_]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,size);assert patched==retail,(addr,name);candidate.append(patched);native.append(retail);entries.append(dict(address=addr,symbol=symbol,bytes=size,grade='RELOCATION_MATCH' if rel else 'MATCH'))
    scenarios=0
    for op in range(9):
        for value in range(256):assert execute(candidate[op],op,value)==execute(native[op],op,value);scenarios+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(test);assert not any(symbol in c.symbols for _,_,symbol,_,_ in SPECS)
    sec,off=c.symbols['??_7ProbeCamera@@6B@'];_,rel=c.sections[sec]
    for index,(_,_,symbol,_,_) in enumerate(SPECS):assert rel[off+4*([2,3,4,5,6,7,8,10,9][index])][0]==symbol
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)];r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CAMERA_BASE_PREFIX_BEHAVIOR_PASS scenarios=2304' in r.stdout,r.stdout+r.stderr
    report=dict(entries=entries,providers=[dict(source=str(a.source),sha256=hashlib.sha256(a.source.read_bytes()).hexdigest())],comparative_scenarios=scenarios,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_camera.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CAMERA_BASE_PREFIX_PASS methods=9 bytes=63 compiled=2304 comparative=%d'%scenarios)
if __name__=='__main__':main()
