"""Check camera angle arithmetic and its real virtual camera-base composition."""
import argparse,hashlib,json,math,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
FILE='CCamera_GetAngleXY_00a0c380.cpp';SYMBOL='?GetAngleXY@CCamera@@QBEMXZ'
BASE=ROOT/'rebuild/src/compiled/00/69/CAIGameCameraBase_DefaultPrefix_00697030.cpp';BASE_SYMBOL='?GetControlAngleXY@CAIGameCameraBase@@UBEMXZ'
TARGETS={'__real@3e22f983':0x1237f08,'__real@00000000':0x122dedc,'__real@3f800000':0x122ded8,'__real@3ff0000000000000':0x1237f00,'__CIfmod':0xbfeb28}
INPUTS=[0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3e800000,0xbe800000,0x3f800000,0xbf800000,0x41200000,0xc1200000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0xffc12345,0x7f812345]
FPREGS=[UC_X86_REG_FP0,UC_X86_REG_FP1,UC_X86_REG_FP2,UC_X86_REG_FP3,UC_X86_REG_FP4,UC_X86_REG_FP5,UC_X86_REG_FP6,UC_X86_REG_FP7]
def extended(value):
    mantissa,exponent=value;sign=-1 if exponent&0x8000 else 1;e=exponent&0x7fff
    if e==0x7fff:return sign*float('inf') if mantissa==0x8000000000000000 else float('nan')
    return sign*math.ldexp(mantissa,e-16383-63) if mantissa else math.copysign(0.0,sign)
def float_bits(value):
    try:return struct.unpack('<I',struct.pack('<f',value))[0]
    except OverflowError:return 0xff800000 if value<0 else 0x7f800000
def execute(codes,constants,crt,x,y,composed,control,entry=0xa0c380,z=None):
    obj,table,camera,render,stack,stop=0x2000040,0x2100040,0x2200040,0x5000000,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base in {addr&~0xfff for addr in codes}|{addr&~0xfff for addr in constants}|{0xbfe000,0x2000000,0x2100000,0x2200000,render,stop}:u.mem_map(base,0x1000)
    u.mem_map(0x3000000,0x10000);u.mem_map(0x1440000,0x1000);u.mem_map(crt['base'],crt['size']);u.mem_write(crt['base'],crt['image']);u.mem_write(0xbfeb28,crt['thunk']);u.mem_write(0x1440118,struct.pack('<I',crt['entry']))
    for addr,code in codes.items():u.mem_write(addr,bytes(code))
    for addr,data in constants.items():u.mem_write(addr,data)
    regions={base:bytearray([0xa9]*0x100) for base in [0x2000000,0x2100000,0x2200000]};struct.pack_into('<I',regions[0x2000000],0x40,table);struct.pack_into('<I',regions[0x2100000],0x44,render);struct.pack_into('<II',regions[0x2200000],0x40+0x18,x,y)
    if z is not None:struct.pack_into('<I',regions[0x2200000],0x40+0x20,z)
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj if composed else camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc not in [render,crt['entry']]:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if pc==render:
            out,bits=struct.unpack('<II',uc.mem_read(sp+4,8));assert uc.reg_read(UC_X86_REG_ECX)==obj and bits==0x3f800000;assert out==stack-0x38;uc.mem_write(out,bytes(regions[0x2200000][0x40:0x78]));uc.reg_write(UC_X86_REG_EAX,out);cleanup=12;trace.append('render')
        else:
            sw=uc.reg_read(UC_X86_REG_FPSW);top=(sw>>11)&7;divisor=uc.reg_read(FPREGS[top]);dividend=uc.reg_read(FPREGS[(top+1)&7]);assert extended(divisor)==1.0 and -0.50001<=extended(dividend)<=0.0 and dividend[1]&0x8000,(hex(x),hex(y),hex(control),divisor,dividend)
            # Execute the original game's CRT implementation, including its
            # dispatcher and signed-zero behavior; no remainder callback.
            trace.append('retail-crt-remainder');return
        uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x697080 if composed else entry,stop,count=300)
    assert trace.count('render')==int(composed) and trace.count('retail-crt-remainder')<=1
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+4 and u.reg_read(UC_X86_REG_FPCW)==control and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    sw=u.reg_read(UC_X86_REG_FPSW);result=u.reg_read(FPREGS[(sw>>11)&7]);return result,sw,trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraAngle_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-angle.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-angle-'));objects=[];providers=[];candidate={};native={};constants={}
    crt_path=Path(EXE).parent/'MSVCR71.dll'
    with pefile.PE(str(crt_path)) as dll:
        base=dll.OPTIONAL_HEADER.ImageBase;entry=base+next(e.address for e in dll.DIRECTORY_ENTRY_EXPORT.symbols if e.name==b'_CIfmod');crt=dict(base=base,entry=entry,size=(dll.OPTIONAL_HEADER.SizeOfImage+0xfff)&~0xfff,image=dll.get_memory_mapped_image())
    with pefile.PE(str(EXE)) as pe:
        crt['thunk']=pe.get_data(0xbfeb28-0x400000,6);assert crt['thunk']==b'\xff\x25\x18\x01\x44\x01'
        for i,(addr,source,symbol,size) in enumerate([(0xa0c380,a.source,SYMBOL,87),(0x697080,BASE,BASE_SYMBOL,29)]):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;patched=bytearray(built)
            for site,(name,kind) in rel.items():
                target=0xa0c380 if name==SYMBOL else TARGETS[name];assert kind in [6,20];struct.pack_into('<I',patched,site,(target+struct.unpack_from('<I',built,site)[0]-(addr+site+4 if kind==20 else 0))&0xffffffff)
                if name.startswith('__real@'):
                    s,o=c.symbols[name];n=8 if name=='__real@3ff0000000000000' else 4;data=c.sections[s][0][o:o+n];assert data==pe.get_data(target-0x400000,n);constants[target]=data
            retail=pe.get_data(addr-0x400000,size);assert patched==retail;candidate[addr]=patched;native[addr]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0;oracle=[]
    for x in INPUTS:
        for y in INPUTS:
            result,_,_=execute(native,constants,crt,x,y,False,0x27f);oracle.append((x,y,float_bits(extended(result))))
            for composed in [False,True]:
                for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:assert execute(candidate,constants,crt,x,y,composed,control)==execute(native,constants,crt,x,y,composed,control);scenarios+=1
    header=work/'CameraAngle_oracle.h';header.write_text('static const unsigned long angleCases[][3]={\n'+''.join('{0x%08xUL,0x%08xUL,0x%08xUL},\n'%case for case in oracle)+'};\n')
    wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_ANGLE_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n')
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/I'+str(work),'/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(test);assert SYMBOL not in c.symbols and BASE_SYMBOL not in c.symbols;objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CAMERA_ANGLE_BEHAVIOR_PASS scenarios=722' in r.stdout,r.stdout+r.stderr
    report=dict(address='00a0c380',grade='RELOCATION_MATCH',bytes=87,providers=providers,comparative_scenarios=scenarios,missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_camera.h').read_bytes()).hexdigest(),oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest(),crt=dict(path=str(crt_path),sha256=hashlib.sha256(crt_path.read_bytes()).hexdigest(),entry='%08x'%crt['entry']))
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraAngle_oracle.h').write_bytes(header.read_bytes());print('CAMERA_ANGLE_PASS bytes=87 compiled=722 comparative=%d'%scenarios)
if __name__=='__main__':main()
