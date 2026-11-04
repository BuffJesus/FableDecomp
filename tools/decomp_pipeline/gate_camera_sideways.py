"""Compare the genuine sideways camera calculation against retail x87 behavior."""
import argparse,hashlib,json,random,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile,verify_and_land as V
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import INPUTS,float_bits
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2];FILE='CCamera_MoveSideways_00a0c1b0.cpp';SYMBOL='?MoveSideways@CCamera@@QAEXM@Z';ENTRY=0xa0c1b0;HEADERS=['fable_camera.h']
def same(actual,expected):
    if (expected&0x7f800000)==0x7f800000 and expected&0x7fffff:return (actual&0x7f800000)==0x7f800000 and bool(actual&0x7fffff)
    if (expected&0x7fffffff)==0 or (expected&0x7f800000)==0x7f800000:return actual==expected
    if actual>>31!=expected>>31:return False
    return abs(actual-expected)<=2
def execute(code,values,control):
    u=Uc(UC_ARCH_X86,UC_MODE_32);camera=0x2000040;stack=0x3008000;stop=0x4000000
    for page in [ENTRY&~0xfff,0x2000000,stop]:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000);u.mem_write(ENTRY,code);region=bytearray(b'\xa9'*0x100);struct.pack_into('<9I',region,0x40,*values[:9]);region[0x64]=values[9]
    u.mem_write(0x2000000,bytes(region));u.mem_write(stack,struct.pack('<II',stop,values[10]));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    u.emu_start(ENTRY,stop,count=300);after=bytes(u.mem_read(0x2000000,len(region)));assert after[:0x40]==region[:0x40] and after[0x4c:]==region[0x4c:]
    if values[9]:assert after[0x48:0x4c]==region[0x48:0x4c]
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and u.reg_read(UC_X86_REG_FPCW)==control and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    return struct.unpack_from('<3I',after,0x40),u.reg_read(UC_X86_REG_FPSW)&0x383f
def cases():
    rows=[];axes=[(0x3f800000,0,0),(0,0x3f800000,0),(0,0,0x3f800000)]
    for value in INPUTS:
        for distance in INPUTS:
            for up in axes:
                for locked in [0,1]:rows.append((0x3f000000,0xbf800000,0x41200000)+up+(value,value,value,locked,distance))
    for up in axes:
        for forward in axes:
            for distance in INPUTS:
                for locked in [0,1]:rows.append((0x3f000000,0xbf800000,0x41200000)+up+forward+(locked,distance))
    rng=random.Random(0xa0c1b0)
    for i in range(128):
        row=tuple(float_bits(rng.uniform(-100,100)) for _ in range(9))
        for locked in [0,1]:rows.append(row+(locked,float_bits(rng.uniform(-4,4))))
    return rows
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraSideways_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-sideways.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-sideways-gate-'))
    with pefile.PE(str(EXE)) as pe:retail=pe.get_data(ENTRY-0x400000,152)
    grade,built,_=V.parity_of(a.source.read_text(),'00a0c1b0',SYMBOL,retail,work,env);assert grade in ['MATCH','RELOCATION_MATCH','NEAR_MATCH'],grade
    obj=work/'00a0c1b0.obj';c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and not rel;scenarios=0;oracle=[]
    for i,values in enumerate(cases()):
        result,_=execute(retail,values,0x27f);oracle.append(values+result)
        for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:
            actual,flags=execute(built,values,control);expected,expected_flags=execute(retail,values,control);assert all(same(x,y) for x,y in zip(actual,expected)) and flags==expected_flags,(i,control,actual,expected,flags,expected_flags);scenarios+=1
        if i%128==0:print('CAMERA_SIDEWAYS_PROGRESS comparisons=%d'%scenarios,flush=True)
    header=work/'CameraSideways_oracle.h';header.write_text('static const unsigned long sidewaysCases[][14]={\n'+''.join('{'+','.join('0x%08xUL'%v for v in row)+'},\n' for row in oracle)+'};\n');wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_SIDEWAYS_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n');test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and ('CAMERA_SIDEWAYS_BEHAVIOR_PASS scenarios=%d'%len(oracle)) in r.stdout,r.stdout+r.stderr
    report=dict(address='00a0c1b0',grade=grade,bytes=len(built),providers=[dict(source=str(a.source),sha256=hashlib.sha256(a.source.read_bytes()).hexdigest())],comparative_scenarios=scenarios,compiled_scenarios=len(oracle),missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS},oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraSideways_oracle.h').write_bytes(header.read_bytes());print('CAMERA_SIDEWAYS_PASS grade=%s compiled=%d comparative=%d'%(grade,len(oracle),scenarios))
if __name__=='__main__':main()
