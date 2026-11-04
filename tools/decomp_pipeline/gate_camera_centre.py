"""Verify manager query overrides and ignored-result world camera centring."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('006295a0','CWorld_EACentreCamera_006295a0.cpp','?EACentreCamera@CWorld@@QAEXPBVCGameEvent@@@Z',25,'00/62'),('0069e1d0','CGameCameraManager_GetCameraManager_0069e1d0.cpp','?GetCameraManager@CGameCameraManager@@UBE_NPAPBV1@@Z',11,'00/69'),('0069e1e0','CGameCameraManager_GetCameraManager_0069e1e0.cpp','?GetCameraManager@CGameCameraManager@@UAE_NPAPAV1@@Z',11,'00/69')]
TARGETS={'?RecentreCamera@CGameCameraManager@@QAEXXZ':0x6978c0}
DEFAULTS=ROOT/'rebuild/src/compiled/00/69/CAIGameCameraBase_DefaultPrefix_00697030.cpp'
def execute(codes,op,pattern):
    world,camera,manager,vtable,out,query,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x5000000,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base in {addr&~0xfff for addr in codes}|{0x697000,0x2000000,0x2100000,0x2200000,0x2300000,0x2400000,query,stop}:u.mem_map(base,0x1000)
    u.mem_map(0x3000000,0x10000)
    for addr,code in codes.items():u.mem_write(addr,bytes(code))
    regions={base:bytearray([pattern]*0x180) for base in [0x2000000,0x2100000,0x2200000,0x2300000,0x2400000]}
    def put(addr,n):struct.pack_into('<I',regions[addr&~0xffff],addr&0xffff,n&0xffffffff)
    put(world+0x34,camera);put(camera,vtable);put(vtable+0x24,query if op==3 else 0x69e1e0);put(vtable+0x28,0x69e1d0)
    for base,data in regions.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,0 if op>=2 else out));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world if op>=2 else camera)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x67890000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc not in [query,0x6978c0]:return
        sp=uc.reg_read(UC_X86_REG_ESP)
        if pc==query:
            assert uc.reg_read(UC_X86_REG_ECX)==camera;dest=struct.unpack('<I',uc.mem_read(sp+4,4))[0];assert stack-4<=dest and dest+4<=stack;uc.mem_write(dest,struct.pack('<I',manager));trace.append(('query',camera,pattern&1));result=pattern&1;cleanup=8
        else:assert uc.reg_read(UC_X86_REG_ECX)==(manager if op==3 else camera);trace.append(('recentre',uc.reg_read(UC_X86_REG_ECX)));result=0xaaaa5555;cleanup=4
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcccccccc);uc.reg_write(UC_X86_REG_EDX,0xdddddddd);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0]);uc.reg_write(UC_X86_REG_ESP,sp+cleanup)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start([0x69e1e0,0x69e1d0,0x6295a0,0x6295a0][op],stop,count=100)
    if op<2:put(out,camera);assert u.reg_read(UC_X86_REG_EAX)&255==1
    assert trace==([('query',camera,pattern&1),('recentre',manager)] if op==3 else [('recentre',camera)] if op==2 else [])
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in regions.items())
    assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x67890000+i for i,r in enumerate(saved))
    return trace
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraCentre_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-centre.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-centre-'));objects=[];providers=[];candidate={};native={};entries=[]
    with pefile.PE(str(EXE)) as pe:
        for i,(addr,file,symbol,size,shard) in enumerate(SPECS):
            source=a.stage/file if a.stage else ROOT/'rebuild/src/compiled'/shard/file;obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size;address=int(addr,16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,size);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()));entries.append(dict(address=addr,grade='RELOCATION_MATCH' if rel else 'MATCH',bytes=size))
    scenarios=0
    for op in range(4):
        for pattern in range(256):assert execute(candidate,op,pattern)==execute(native,op,pattern);scenarios+=1
    obj=work/'defaults.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(DEFAULTS)],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);providers.append(dict(source=str(DEFAULTS),sha256=hashlib.sha256(DEFAULTS.read_bytes()).hexdigest()))
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(test);assert not any(symbol in c.symbols for _,_,symbol,_,_ in SPECS)
    sec,off=c.symbols['??_7ProbeManager@@6B@'];_,rel=c.sections[sec];assert rel[off+0x24][0]==SPECS[2][2] and rel[off+0x28][0]==SPECS[1][2];objects.append(test)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(4):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CAMERA_CENTRE_BEHAVIOR_PASS scenarios=1024' in r.stdout,r.stdout+r.stderr
    report=dict(entries=entries,providers=providers,comparative_scenarios=scenarios,missing_provider_checks=4,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_game_camera_manager.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CAMERA_CENTRE_PASS methods=3 bytes=47 compiled=1024 comparative=%d'%scenarios)
if __name__=='__main__':main()
