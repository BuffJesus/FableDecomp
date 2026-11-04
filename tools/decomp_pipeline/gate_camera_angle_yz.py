"""Verify camera pitch from all three forward coordinates with the original CRT."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import TARGETS,INPUTS,execute,extended,float_bits
ROOT=Path(__file__).resolve().parents[2]
FILE='CCamera_GetAngleYZ_00a0c3e0.cpp';SYMBOL='?GetAngleYZ@CCamera@@QBEMXZ';ENTRY=0xa0c3e0
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraAngleYZ_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-angle-yz.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-angle-yz-'));obj=work/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and len(built)==110;patched=bytearray(built);constants={}
    crt_path=Path(EXE).parent/'MSVCR71.dll'
    with pefile.PE(str(crt_path)) as dll:
        base=dll.OPTIONAL_HEADER.ImageBase;entry=base+next(e.address for e in dll.DIRECTORY_ENTRY_EXPORT.symbols if e.name==b'_CIfmod');crt=dict(base=base,entry=entry,size=(dll.OPTIONAL_HEADER.SizeOfImage+0xfff)&~0xfff,image=dll.get_memory_mapped_image())
    with pefile.PE(str(EXE)) as pe:
        crt['thunk']=pe.get_data(0xbfeb28-0x400000,6);assert crt['thunk']==b'\xff\x25\x18\x01\x44\x01'
        for site,(name,kind) in rel.items():
            target=TARGETS[name];assert kind in [6,20];struct.pack_into('<I',patched,site,(target+struct.unpack_from('<I',built,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
            if name.startswith('__real@'):
                s,o=c.symbols[name];n=8 if name=='__real@3ff0000000000000' else 4;data=c.sections[s][0][o:o+n];assert data==pe.get_data(target-0x400000,n);constants[target]=data
        retail=pe.get_data(ENTRY-0x400000,110);assert patched==retail
    scenarios=0;oracle=[]
    for x in INPUTS:
        for y in INPUTS:
            for z in INPUTS:
                result,_,_=execute({ENTRY:retail},constants,crt,x,y,False,0x27f,ENTRY,z);oracle.append((x,y,z,float_bits(extended(result))))
                for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:assert execute({ENTRY:patched},constants,crt,x,y,False,control,ENTRY,z)==execute({ENTRY:retail},constants,crt,x,y,False,control,ENTRY,z);scenarios+=1
        print('CAMERA_ANGLE_YZ_PROGRESS comparisons=%d'%scenarios,flush=True)
    header=work/'CameraAngleYZ_oracle.h';header.write_text('static const unsigned long angleYZCases[][4]={\n'+''.join('{0x%08xUL,0x%08xUL,0x%08xUL,0x%08xUL},\n'%case for case in oracle)+'};\n');wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_ANGLE_YZ_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n')
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)];r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CAMERA_ANGLE_YZ_BEHAVIOR_PASS scenarios=6859' in r.stdout,r.stdout+r.stderr
    report=dict(address='00a0c3e0',grade='RELOCATION_MATCH',bytes=110,providers=[dict(source=str(a.source),sha256=hashlib.sha256(a.source.read_bytes()).hexdigest())],comparative_scenarios=scenarios,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_camera.h').read_bytes()).hexdigest(),oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest(),crt=dict(path=str(crt_path),sha256=hashlib.sha256(crt_path.read_bytes()).hexdigest(),entry='%08x'%crt['entry']))
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraAngleYZ_oracle.h').write_bytes(header.read_bytes());print('CAMERA_ANGLE_YZ_PASS bytes=110 compiled=6859 comparative=%d'%scenarios)
if __name__=='__main__':main()
