"""Verify the genuine circular event-data reader and real world-control composition."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_world_event_control import execute as control_execute,SPECS,TARGETS,PACKAGE,PACKAGE_SYMBOL
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
ADDRESS=0x9f18f0
SYMBOL='?GetData@CGameEventDataPackage@@QAEXJPAX@Z'
SOURCE=ROOT/'rebuild/src/compiled/00/9f/CGameEventDataPackage_GetData_009f18f0.cpp'
PAUSE=ROOT/'rebuild/src/compiled/00/49/CWorld_SetAsPaused_0049d8f0.cpp'
def execute(code,start,count,state,pattern):
    obj,dest,stack,stop=0x2000040,0x2100040,0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x9f1000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    u.mem_write(ADDRESS,bytes(code));initial=bytearray([0xa9]*0xac)
    payload=bytes((i*37+pattern*71)&255 for i in range(32));length=[0,32,0xffffffff,0x7fffffff][state]
    initial[0x40:0x6c]=payload+struct.pack('<III',length,start,state*9);u.mem_write(obj-0x40,bytes(initial))
    output=bytearray([0xa7]*0x80);u.mem_write(dest-0x40,bytes(output));u.mem_write(stack,struct.pack('<III',stop,count,dest))
    u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,obj)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x12340000+i)
    u.emu_start(ADDRESS,stop,count=2000)
    output[0x40:0x40+count]=bytes(payload[(start+i)%32] for i in range(count))
    struct.pack_into('<II',initial,0x60,(length-count)&0xffffffff,(start+count)%32)
    got=bytes(u.mem_read(obj-0x40,len(initial))),bytes(u.mem_read(dest-0x40,len(output)))
    assert got==(bytes(initial),bytes(output)),(start,count,state,pattern)
    assert u.reg_read(UC_X86_REG_ESP)==stack+12 and all(u.reg_read(r)==0x12340000+i for i,r in enumerate(saved))
    return got
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=SOURCE)
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/EventReader_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/event-reader.json');a=ap.parse_args()
    env=check_one.env();work=Path(tempfile.mkdtemp(prefix='event-reader-'));objects=[];providers=[];candidate={};native={}
    def compile(source,index,symbol,address,size):
        obj=work/('source%d.obj'%index);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
        objects.append(obj);c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==size
        patched=bytearray(built)
        for site,(name,kind) in rel.items():
            assert kind in (6,20);addend=struct.unpack_from('<I',built,site)[0]
            struct.pack_into('<I',patched,site,(TARGETS[name]+addend-(address+site+4 if kind==20 else 0))&0xffffffff)
        retail=pe.get_data(address-0x400000,len(built));assert patched==retail,(source,symbol)
        candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    with pefile.PE(str(EXE)) as pe:
        compile(a.source,0,SYMBOL,ADDRESS,166)
        compile(PACKAGE,1,PACKAGE_SYMBOL,0x9f17a0,53)
        for i,s in enumerate(SPECS[2:]):
            compile(ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'],i+2,s['symbol'],int(s['address'],16),s['size'])
        compile(PAUSE,4,'?SetAsPaused@CWorld@@IAEX_N@Z',0x49d8f0,21)
    direct=0
    for start in range(32):
        for count in range(33):
            for state in range(4):
                for pattern in range(4):assert execute(candidate[ADDRESS],start,count,state,pattern)==execute(native[ADDRESS],start,count,state,pattern);direct+=1
    composed=0
    for value in range(256):
        for length in [1,8,32]:
            for address in [0x6292d0,0x629330]:
                assert control_execute(candidate,address,value,length,real_pause=True)==control_execute(native,address,value,length,real_pause=True);composed+=1
    test=work/'fixture.obj';r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    assert SYMBOL not in Coff(test).symbols;objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(5):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'EVENT_READER_BEHAVIOR_PASS direct=16896 sequential=1024 composed=16384' in r.stdout,r.stdout+r.stderr
    report=dict(grade='MATCH',bytes=166,providers=providers,comparative_scenarios=direct,composed_scenarios=composed,missing_provider_checks=5,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('EVENT_READER_PASS MATCH bytes=166 direct=%d composed=%d compiled=34304'%(direct,composed))
if __name__=='__main__':main()
