"""Check original camera FOV angle storage and ordered positive-vertical flags."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import INPUTS
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2];FILE='CCamera_SetFOV_00a0bed0.cpp';SYMBOL='?SetFOV@CCamera@@QAEXMM@Z';ENTRY=0xa0bed0
def execute(code,horizontal,vertical,pattern,control):
    u=Uc(UC_ARCH_X86,UC_MODE_32);camera=0x2000040;stack=0x3008000;stop=0x4000000
    for page in [ENTRY&~0xfff,0x122d000,0x2000000,stop]:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000);u.mem_write(ENTRY,bytes(code));u.mem_write(0x122dedc,b'\0'*4);region=bytearray(bytes([pattern*71])*0x100);u.mem_write(0x2000000,bytes(region));u.mem_write(stack,struct.pack('<III',stop,horizontal,vertical));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    u.emu_start(ENTRY,stop,count=100);after=bytes(u.mem_read(0x2000000,len(region)));expected=bytearray(region);nan=(vertical&0x7f800000)==0x7f800000 and bool(vertical&0x7fffff);flags=3 if not(vertical&0x80000000) and vertical&0x7fffffff and not nan else 1;struct.pack_into('<III',expected,0x68,flags,horizontal,vertical)
    assert after==expected,(horizontal,vertical,after[0x68:0x74],expected[0x68:0x74]);assert u.reg_read(UC_X86_REG_ESP)==stack+12 and u.reg_read(UC_X86_REG_FPCW)==control and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return after,u.reg_read(UC_X86_REG_FPSW)&0x383f
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraFOV_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-fov.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-fov-gate-'));obj=work/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and len(built)==48;patched=bytearray(built)
    for site,(symbol,kind) in rel.items():
        assert symbol=='__real@00000000' and kind==6;constant_sec,constant_off=c.symbols[symbol];assert c.sections[constant_sec][0][constant_off:constant_off+4]==b'\0'*4;struct.pack_into('<I',patched,site,0x122dedc)
    with pefile.PE(str(EXE)) as pe:retail=pe.get_data(ENTRY-0x400000,48);assert pe.get_data(0x122dedc-0x400000,4)==b'\0'*4
    assert patched==retail;scenarios=0
    for horizontal in INPUTS:
        for vertical in INPUTS:
            for pattern in range(4):
                for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:assert execute(patched,horizontal,vertical,pattern,control)==execute(retail,horizontal,vertical,pattern,control);scenarios+=1
        print('CAMERA_FOV_PROGRESS comparisons=%d'%scenarios,flush=True)
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)];r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CAMERA_FOV_BEHAVIOR_PASS scenarios=1444' in r.stdout,r.stdout+r.stderr
    report=dict(address='00a0bed0',grade='RELOCATION_MATCH',providers=[dict(source=str(a.source),sha256=hashlib.sha256(a.source.read_bytes()).hexdigest())],comparative_scenarios=scenarios,compiled_scenarios=1444,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_camera.h').read_bytes()).hexdigest());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CAMERA_FOV_PASS compiled=1444 comparative=%d'%scenarios)
if __name__=='__main__':main()

