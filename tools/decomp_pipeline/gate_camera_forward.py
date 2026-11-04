"""Verify camera forward movement and height lock against retail x87 behavior."""
import argparse,hashlib,json,random,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import INPUTS,float_bits
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2];FILE='CCamera_MoveForward_00a0c250.cpp';SYMBOL='?MoveForward@CCamera@@QAEXM@Z';ENTRY=0xa0c250;HEADERS=['fable_camera.h','fable_2d_vector.h']
def execute(code,values,control):
    u=Uc(UC_ARCH_X86,UC_MODE_32);camera=0x2000040;stack=0x3008000;stop=0x4000000
    for page in [ENTRY&~0xfff,0x2000000,stop]:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000);u.mem_write(ENTRY,code);region=bytearray(b'\xa9'*0x100)
    struct.pack_into('<3I',region,0x40,*values[:3]);struct.pack_into('<3I',region,0x58,*values[3:6]);region[0x64]=values[6]
    u.mem_write(0x2000000,bytes(region));u.mem_write(stack,struct.pack('<II',stop,values[7]));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    u.emu_start(ENTRY,stop,count=100);after=bytes(u.mem_read(0x2000000,len(region)))
    assert after[:0x40]==region[:0x40] and after[0x4c:]==region[0x4c:]
    if values[6]:assert after[0x48:0x4c]==region[0x48:0x4c]
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and u.reg_read(UC_X86_REG_FPCW)==control and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return struct.unpack_from('<3I',after,0x40),u.reg_read(UC_X86_REG_FPSW)
def cases():
    rows=[]
    for value in INPUTS:
        for distance in INPUTS:
            for locked in [0,1]:rows.append((0x3f000000,0xbf800000,0x41200000,value,value,value,locked,distance))
    rng=random.Random(0xa0c250)
    for i in range(128):
        row=tuple(float_bits(rng.uniform(-100,100)) for _ in range(6))
        for locked in [0,1]:rows.append(row+(locked,float_bits(rng.uniform(-4,4))))
    return rows
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraForward_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-forward.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-forward-gate-'));obj=work/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and len(built)==86 and not rel
    with pefile.PE(str(EXE)) as pe:retail=pe.get_data(ENTRY-0x400000,86)
    assert built==retail;scenarios=0;oracle=[]
    for i,values in enumerate(cases()):
        result,_=execute(retail,values,0x27f);oracle.append(values+result)
        for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:assert execute(built,values,control)==execute(retail,values,control);scenarios+=1
        if i%128==0:print('CAMERA_FORWARD_PROGRESS comparisons=%d'%scenarios,flush=True)
    header=work/'CameraForward_oracle.h';header.write_text('static const unsigned long forwardCases[][11]={\n'+''.join('{'+','.join('0x%08xUL'%v for v in row)+'},\n' for row in oracle)+'};\n');wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_FORWARD_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n')
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)];r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and ('CAMERA_FORWARD_BEHAVIOR_PASS scenarios=%d'%len(oracle)) in r.stdout,r.stdout+r.stderr
    report=dict(address='00a0c250',grade='MATCH',bytes=86,providers=[dict(source=str(a.source),sha256=hashlib.sha256(a.source.read_bytes()).hexdigest())],comparative_scenarios=scenarios,compiled_scenarios=len(oracle),missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS},oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraForward_oracle.h').write_bytes(header.read_bytes());print('CAMERA_FORWARD_PASS bytes=86 compiled=%d comparative=%d'%(len(oracle),scenarios))
if __name__=='__main__':main()
