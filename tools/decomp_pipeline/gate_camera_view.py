"""Validate original camera view copying and accurate vector normalization."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import INPUTS
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='00a0be30',file='CCamera_SetView_00a0be30.cpp',symbol='?SetView@CCamera@@QAEXABVC3DVector@@0@Z',size=68),dict(address='00a14440',file='C3DVector_AccurateNormalise_00a14440.cpp',symbol='?AccurateNormalise@C3DVector@@QAEAAV1@XZ',size=61)]
HEADERS=['fable_camera.h','fable_2d_vector.h','fable_vector_magnitude.h']
CONTROLS=[0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]
def execute(codes,values,pattern,control,view=False):
    camera,first,second,stack,stop=0x2000040,0x2100040,0x2200040,0x3008000,0x4000000;u=Uc(UC_ARCH_X86,UC_MODE_32)
    for page in {a&~0xfff for a in codes}|{0x122d000,0x2000000,0x2100000,0x2200000,stop}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    u.mem_write(0x122ded8,struct.pack('<I',0x3f800000));regions={base:bytearray(bytes([pattern*213])*0x100) for base in [0x2000000,0x2100000,0x2200000]}
    if view:
        struct.pack_into('<9I',regions[0x2000000],0x40,*values[:9]);struct.pack_into('<3I',regions[0x2100000],0x40,*values[9:12]);struct.pack_into('<3I',regions[0x2200000],0x40,*values[12:15]);up=[first,camera,camera+12,camera+24][values[15]];forward=[second,camera,camera+12,camera+24][values[16]];u.mem_write(stack,struct.pack('<III',stop,forward,up));entry=0xa0be30;changed_start,changed_end=0x4c,0x64
    else:struct.pack_into('<3I',regions[0x2000000],0x40,*values);u.mem_write(stack,struct.pack('<I',stop));entry=0xa14440;changed_start,changed_end=0x40,0x4c
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    u.emu_start(entry,stop,count=300);after=bytes(u.mem_read(0x2000000,0x100));before=regions[0x2000000];assert after[:changed_start]==before[:changed_start] and after[changed_end:]==before[changed_end:];assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items() if base!=0x2000000);assert u.reg_read(UC_X86_REG_ESP)==stack+(12 if view else 4) and u.reg_read(UC_X86_REG_FPCW)==control and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    if not view:assert u.reg_read(UC_X86_REG_EAX)==camera
    return struct.unpack_from('<'+('6I' if view else '3I'),after,changed_start),u.reg_read(UC_X86_REG_FPSW)&0x383f
def view_cases():
    position=(0x3f000000,0xbf800000,0x41200000);rows=[]
    for up in INPUTS:
        for forward in INPUTS:rows.append(position+(up,up,up)+(forward,forward,forward)+(up,0,0x3f800000)+(0x3f800000,forward,0)+(0,0))
    selected=[0,0x80000000,1,0x3f800000,0xbf800000,0x7f7fffff,0x7f800000,0x7fc12345,0x7f812345,0xff800000]
    for value in selected:
        for up in range(4):
            for forward in range(4):rows.append(position+(value,0x3f000000,0x3f800000)+(0x40000000,value,0xbf800000)+(value,0x40400000,0x40800000)+(0x40a00000,value,0x40c00000)+(up,forward))
    return rows
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraView_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-view.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-view-gate-'));objects=[];providers=[];candidate={};native={};targets={s['symbol']:int(s['address'],16) for s in SPECS};targets['__real@3f800000']=0x122ded8
    with pefile.PE(str(EXE)) as pe:
        assert pe.get_data(0x122ded8-0x400000,4)==struct.pack('<I',0x3f800000)
        for i,s in enumerate(SPECS):
            source=a.stage/s['file'] if a.stage else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(symbol,kind) in rel.items():
                assert kind in [6,20]
                if symbol=='__real@3f800000':cs,co=c.symbols[symbol];assert c.sections[cs][0][co:co+4]==struct.pack('<I',0x3f800000)
                struct.pack_into('<I',patched,site,(targets[symbol]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0;normal_oracle=[];view_oracle=[]
    for i,values in enumerate(itertools.product(INPUTS,repeat=3)):
        result,_=execute(native,values,0,0x27f);normal_oracle.append(values+result)
        for pattern in range(2):
            for control in CONTROLS:assert execute(candidate,values,pattern,control)==execute(native,values,pattern,control);scenarios+=1
        if i%512==0:print('CAMERA_VIEW_PROGRESS normalize=%d comparisons=%d'%(i,scenarios),flush=True)
    for i,values in enumerate(view_cases()):
        result,_=execute(native,values,0,0x27f,True);view_oracle.append(values+result)
        for pattern in range(2):
            for control in CONTROLS:assert execute(candidate,values,pattern,control,True)==execute(native,values,pattern,control,True);scenarios+=1
        if i%128==0:print('CAMERA_VIEW_PROGRESS view=%d comparisons=%d'%(i,scenarios),flush=True)
    header=work/'CameraView_oracle.h';header.write_text('static const unsigned long normalCases[][6]={\n'+''.join('{'+','.join('0x%08xUL'%v for v in row)+'},\n' for row in normal_oracle)+'};\nstatic const unsigned long viewCases[][23]={\n'+''.join('{'+','.join('0x%08xUL'%v for v in row)+'},\n' for row in view_oracle)+'};\n');wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_VIEW_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n');test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);compiled=2*(len(normal_oracle)+len(view_oracle));assert not r.returncode and ('CAMERA_VIEW_BEHAVIOR_PASS scenarios=%d'%compiled) in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,compiled_scenarios=compiled,normalization_cases=len(normal_oracle),view_cases=len(view_oracle),missing_provider_checks=2,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS},oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraView_oracle.h').write_bytes(header.read_bytes());print('CAMERA_VIEW_PASS compiled=%d comparative=%d'%(compiled,scenarios))
if __name__=='__main__':main()
