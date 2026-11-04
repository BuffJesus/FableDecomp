"""Verify target aliasing, vertical lock and four accurate-normalizer calls."""
import argparse,hashlib,json,random,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import INPUTS,float_bits
from gate_camera_view import SPECS as VIEW_SPECS,CONTROLS
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2];FILE='CCamera_PointAt_00a0c2b0.cpp';SYMBOL='?PointAt@CCamera@@QAEXABVC3DVector@@@Z';SPEC=dict(address='00a0c2b0',file=FILE,symbol=SYMBOL,size=208);NORMAL=VIEW_SPECS[1];HEADERS=['fable_camera.h','fable_2d_vector.h']
def cases():
    position=(0x3f000000,0xbf800000,0x41200000);forward=(0x3f800000,0,0);ups=[(0,0,0x3f800000),(0,0,0),(0x7fc12345,0,0),(0x7f7fffff,0x7f7fffff,0x7f7fffff)];rows=[]
    for axis in range(3):
        for value in INPUTS:
            target=list(position);target[axis]=value
            for up in ups:
                for locked in [0,1]:rows.append(position+up+forward+tuple(target)+(locked,0))
    for value in INPUTS:
        for up in ups:
            for locked in [0,1]:rows.append(position+up+forward+(value,value,value,locked,0))
    rng=random.Random(0xa0c2b0)
    for i in range(128):
        values=tuple(float_bits(rng.uniform(-100,100)) for _ in range(12))
        for locked in [0,1]:rows.append(values+(locked,0))
    for kind in range(4):
        for value in INPUTS:
            for locked in [0,1]:rows.append((value,0xbf800000,0x41200000)+ups[0]+forward+(value,value,value,locked,kind))
    return rows
def execute(codes,values,pattern,control):
    camera,target,stack,stop=0x2000040,0x2100040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x122d000,0x2000000,0x2100000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    u.mem_write(0x122ded8,struct.pack('<II',0x3f800000,0));regions={base:bytearray(bytes([pattern*213])*0x100) for base in [0x2000000,0x2100000]};struct.pack_into('<9I',regions[0x2000000],0x40,*values[:9]);regions[0x2000000][0x65]=values[12];struct.pack_into('<3I',regions[0x2100000],0x40,*values[9:12])
    for base,data in regions.items():u.mem_write(base,bytes(data))
    argument=[target,camera,camera+12,camera+24][values[13]];u.mem_write(stack,struct.pack('<II',stop,argument));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    calls=[]
    def hook(uc,pc,size,user):
        if pc==0xa14440:calls.append(uc.reg_read(UC_X86_REG_ECX))
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0xa0c2b0,stop,count=500);after=bytes(u.mem_read(0x2000000,0x100));before=regions[0x2000000];assert calls==[camera+12,camera+24,camera+12,camera+24];assert after[:0x4c]==before[:0x4c] and after[0x64:]==before[0x64:] and bytes(u.mem_read(0x2100000,0x100))==regions[0x2100000];assert u.reg_read(UC_X86_REG_ESP)==stack+8 and u.reg_read(UC_X86_REG_FPCW)==control and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return struct.unpack_from('<6I',after,0x4c),u.reg_read(UC_X86_REG_FPSW)&0x383f
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraPointAt_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-point-at.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-point-at-gate-'));objects=[];providers=[];candidate={};native={};targets={s['symbol']:int(s['address'],16) for s in [SPEC,NORMAL]};targets.update({'__real@3f800000':0x122ded8,'__real@00000000':0x122dedc})
    with pefile.PE(str(EXE)) as pe:
        assert pe.get_data(0x122ded8-0x400000,8)==struct.pack('<II',0x3f800000,0)
        for i,s in enumerate([SPEC,NORMAL]):
            source=a.stage/s['file'] if a.stage and i==0 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(symbol,kind) in rel.items():
                assert kind in [6,20]
                if symbol.startswith('__real@'):cs,co=c.symbols[symbol];assert c.sections[cs][0][co:co+4]==struct.pack('<I',0x3f800000 if symbol=='__real@3f800000' else 0)
                struct.pack_into('<I',patched,site,(targets[symbol]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0;oracle=[]
    for i,values in enumerate(cases()):
        result,_=execute(native,values,0,0x27f);oracle.append(values+result)
        for pattern in range(2):
            for control in CONTROLS:assert execute(candidate,values,pattern,control)==execute(native,values,pattern,control);scenarios+=1
        if i%128==0:print('CAMERA_POINT_AT_PROGRESS comparisons=%d'%scenarios,flush=True)
    header=work/'CameraPointAt_oracle.h';header.write_text('static const unsigned long pointCases[][20]={\n'+''.join('{'+','.join('0x%08xUL'%v for v in row)+'},\n' for row in oracle)+'};\n');wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_POINT_AT_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n');test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in [SPEC,NORMAL]);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);compiled=4*len(oracle);assert not r.returncode and ('CAMERA_POINT_AT_BEHAVIOR_PASS scenarios=%d'%compiled) in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,compiled_scenarios=compiled,input_cases=len(oracle),missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS},oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraPointAt_oracle.h').write_bytes(header.read_bytes());print('CAMERA_POINT_AT_PASS compiled=%d comparative=%d'%(compiled,scenarios))
if __name__=='__main__':main()
