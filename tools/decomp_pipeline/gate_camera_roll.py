"""Verify camera roll against retail instructions and the game's original CRT."""
import argparse,hashlib,json,math,random,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_camera_angle import extended,float_bits,FPREGS,INPUTS
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
FILE='CCamera_GetAngleXZ_00a0c450.cpp';SYMBOL='?GetAngleXZ@CCamera@@QBEMXZ';ENTRY=0xa0c450
TARGETS={'__real@3e22f983':0x1237f08,'__real@00000000':0x122dedc,'__real@3f800000':0x122ded8,'__real@3ff0000000000000':0x1237f00,'__CIfmod':0xbfeb28,'__real@3f000000':0x122f59c,'__real@3e800000':0x123078c,'__real@401921fb60000000':0x124f2b8,'__CIacos':0xbfeba2}
HEADERS=['fable_camera.h','fable_2d_vector.h','fable_vector_angles.h']

def execute(code,constants,crt,values,control):
    u=Uc(UC_ARCH_X86,UC_MODE_32);stack=0x3008000;stop=0x4000000;camera=0x2000040
    for page in {ENTRY&~0xfff,0xbfe000,0x1440000,0x2000000,stop,0x6000000,0x6100000}|{a&~0xfff for a in constants}:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000);u.mem_map(crt['base'],crt['size']);u.mem_write(crt['base'],crt['image']);u.mem_write(ENTRY,bytes(code))
    for a,data in constants.items():u.mem_write(a,data)
    for a,data in crt['thunks'].items():u.mem_write(a,data)
    u.mem_write(0x1440118,struct.pack('<I',crt['exports'][b'_CIfmod']));u.mem_write(0x14400f0,struct.pack('<I',crt['exports'][b'_CIacos']))
    api={}
    for i,(iat,name) in enumerate(crt['imports']):
        address=0x6000000+i*16;api[address]=name;u.mem_write(iat,struct.pack('<I',address))
    # Original loader fallback at 7C341985..7C34198A copies the imported
    # TlsGetValue pointer to this slot. Supply an already initialized thread.
    u.mem_write(0x7c38b428,bytes(u.mem_read(0x7c37a064,4)))
    u.mem_write(0x7c38b430,struct.pack('<I',0)) # allocated fixture TLS index
    region=bytearray(b'\xa9'*0x100);struct.pack_into('<6I',region,0x4c,*values);u.mem_write(0x2000000,bytes(region))
    u.mem_write(stack,struct.pack('<I',stop));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,camera);u.reg_write(UC_X86_REG_FPCW,control);u.reg_write(UC_X86_REG_FPTAG,0xffff)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[]
    def hook(uc,pc,size,user):
        if pc not in api:return
        name=api[pc];sp=uc.reg_read(UC_X86_REG_ESP);trace.append(name)
        if name=='GetLastError':result=0;argc=0
        elif name=='SetLastError':assert struct.unpack('<I',uc.mem_read(sp+4,4))[0]==0;result=0;argc=1
        elif name=='TlsGetValue':assert struct.unpack('<I',uc.mem_read(sp+4,4))[0]==0;result=0x6100040;argc=1
        else:raise AssertionError(('unexpected Windows API',name,values))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ESP,sp+4+argc*4);uc.reg_write(UC_X86_REG_EIP,struct.unpack('<I',uc.mem_read(sp,4))[0])
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(ENTRY,stop,count=10000)
    assert bytes(u.mem_read(0x2000000,len(region)))==region
    assert u.reg_read(UC_X86_REG_ESP)==stack+4 and u.reg_read(UC_X86_REG_FPCW)==control
    assert all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved))
    sw=u.reg_read(UC_X86_REG_FPSW);result=u.reg_read(FPREGS[(sw>>11)&7]);errno=struct.unpack('<I',u.mem_read(0x6100048,4))[0]
    return result,sw,trace,errno

def cases():
    rows=[]
    # Axis orientations and finite invalid lengths, then every special raw
    # float in each of the six coordinates of a normal identity camera.
    axes=[(0,0,1),(0,0,-1),(1,0,0),(-1,0,0),(0,1,0),(0,-1,0),(0,0,0),(0,0,2)]
    for up in axes:
        for forward in axes:rows.append(tuple(float_bits(v) for v in up+forward))
    default=[0,0,0x3f800000,0,0x3f800000,0]
    for coordinate in range(6):
        for value in INPUTS:
            row=list(default);row[coordinate]=value;rows.append(tuple(row))
    rng=random.Random(0xa0c450)
    for i in range(256):
        forward=[rng.uniform(-1,1) for _ in range(3)];length=math.sqrt(sum(v*v for v in forward));forward=[v/length for v in forward]
        ref=[0,0,1] if abs(forward[2])<0.9 else [0,1,0]
        right=[forward[1]*ref[2]-forward[2]*ref[1],forward[2]*ref[0]-forward[0]*ref[2],forward[0]*ref[1]-forward[1]*ref[0]];length=math.sqrt(sum(v*v for v in right));right=[v/length for v in right]
        up=[right[1]*forward[2]-right[2]*forward[1],right[2]*forward[0]-right[0]*forward[2],right[0]*forward[1]-right[1]*forward[0]]
        angle=rng.uniform(-math.pi,math.pi);up=[a*math.cos(angle)+b*math.sin(angle) for a,b in zip(up,right)]
        rows.append(tuple(float_bits(v) for v in up+forward))
    return rows

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CameraRoll_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/camera-roll.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='camera-roll-gate-'));obj=work/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and len(built)==288;patched=bytearray(built);constants={}
    crt_path=Path(EXE).parent/'MSVCR71.dll'
    with pefile.PE(str(crt_path)) as dll:
        base=dll.OPTIONAL_HEADER.ImageBase;crt=dict(base=base,size=(dll.OPTIONAL_HEADER.SizeOfImage+0xfff)&~0xfff,image=dll.get_memory_mapped_image(),exports={e.name:base+e.address for e in dll.DIRECTORY_ENTRY_EXPORT.symbols if e.name in [b'_CIfmod',b'_CIacos']},imports=[(i.address,(i.name or b'ordinal').decode()) for d in dll.DIRECTORY_ENTRY_IMPORT for i in d.imports])
        assert dll.get_data(0x1985,10)==bytes.fromhex('a164a0377ca328b4387c')
    with pefile.PE(str(EXE)) as pe:
        crt['thunks']={0xbfeb28:pe.get_data(0xbfeb28-0x400000,6),0xbfeba2:pe.get_data(0xbfeba2-0x400000,6)}
        assert crt['thunks']=={0xbfeb28:bytes.fromhex('ff2518014401'),0xbfeba2:bytes.fromhex('ff25f0004401')}
        for site,(name,kind) in rel.items():
            target=TARGETS[name];assert kind in [6,20];struct.pack_into('<I',patched,site,(target+struct.unpack_from('<I',built,site)[0]-(ENTRY+site+4 if kind==20 else 0))&0xffffffff)
            if name.startswith('__real@'):
                s,o=c.symbols[name];n=8 if name in ['__real@3ff0000000000000','__real@401921fb60000000'] else 4;data=c.sections[s][0][o:o+n];assert data==pe.get_data(target-0x400000,n);constants[target]=data
        retail=pe.get_data(ENTRY-0x400000,288);assert patched==retail
    scenarios=0;oracle=[];errors=0
    for i,values in enumerate(cases()):
        result,_,_,errno=execute(retail,constants,crt,values,0x27f);oracle.append(values+(float_bits(extended(result)),errno));errors+=int(errno!=0)
        for control in [0x37f,0x77f,0xb7f,0xf7f,0x27f,0x07f]:assert execute(patched,constants,crt,values,control)==execute(retail,constants,crt,values,control);scenarios+=1
        if i%64==0:print('CAMERA_ROLL_PROGRESS comparisons=%d'%scenarios,flush=True)
    header=work/'CameraRoll_oracle.h';header.write_text('static const unsigned long rollCases[][8]={\n'+''.join('{'+','.join('0x%08xUL'%v for v in row)+'},\n' for row in oracle)+'};\n');wrapper=work/'test.cpp';wrapper.write_text('#define FABLE_CAMERA_ROLL_ORACLE_HEADER "'+header.as_posix()+'"\n#include "'+a.test.resolve().as_posix()+'"\n')
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(wrapper)],env,work);assert not r.returncode,r.stdout+r.stderr;assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)];r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and ('CAMERA_ROLL_BEHAVIOR_PASS scenarios=%d'%len(oracle)) in r.stdout,r.stdout+r.stderr
    report=dict(address='00a0c450',grade='RELOCATION_MATCH',bytes=288,providers=[dict(source=str(a.source),sha256=hashlib.sha256(a.source.read_bytes()).hexdigest())],comparative_scenarios=scenarios,compiled_scenarios=len(oracle),domain_error_scenarios=errors,missing_provider_checks=1,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS},oracle_sha256=hashlib.sha256(header.read_bytes()).hexdigest(),crt=dict(path=str(crt_path),sha256=hashlib.sha256(crt_path.read_bytes()).hexdigest(),exports={k.decode():'%08x'%v for k,v in crt['exports'].items()},loader_fixture='retail TlsGetValue fallback and existing 0x8c-byte thread storage; Windows API boundaries only'))
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');(a.output.parent/'CameraRoll_oracle.h').write_bytes(header.read_bytes());print('CAMERA_ROLL_PASS bytes=288 compiled=%d comparative=%d domain_errors=%d'%(len(oracle),scenarios,errors))
if __name__=='__main__':main()
