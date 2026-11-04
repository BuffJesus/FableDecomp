"""Validate the real CWorld pause setter and compose it with the real pause event."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from gate_world_event_control import SPECS,PACKAGE,PACKAGE_SYMBOL,TARGETS,execute
from pe_oracle import EXE
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP,UC_X86_REG_ECX
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?SetAsPaused@CWorld@@IAEX_N@Z'

def setter(code,old,value):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x49d000,4096),(0x2000000,4096),(0x3000000,4096),(0x4000000,4096)]:u.mem_map(base,size)
    initial=bytearray(bytes([old])*0x218);obj=0x2000040;sp=0x3000800
    u.mem_write(0x49d8f0,bytes(code));u.mem_write(obj-0x40,bytes(initial));u.mem_write(sp,struct.pack('<II',0x4000000,0xaabbcc00|value))
    u.reg_write(UC_X86_REG_ECX,obj);u.reg_write(UC_X86_REG_ESP,sp);u.emu_start(0x49d8f0,0x4000000,count=30)
    initial[0x40+0xa4]=value;assert bytes(u.mem_read(obj-0x40,len(initial)))==initial and u.reg_read(UC_X86_REG_ESP)==sp+8
    return bytes(initial)

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/49/CWorld_SetAsPaused_0049d8f0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldPause_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-pause.json');a=ap.parse_args()
    env=check_one.env();work=Path(tempfile.mkdtemp(prefix='world-pause-'));objects=[];candidate={};native={};providers=[]
    sources=[a.source]+[ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'] for s in SPECS]+[PACKAGE,a.test]
    with pefile.PE(str(EXE)) as pe:
        for i,source in enumerate(sources):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
            objects.append(obj);providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
            if i==len(sources)-1:continue
            symbol=SYMBOL if i==0 else (SPECS[i-1]['symbol'] if i<=4 else PACKAGE_SYMBOL)
            address=0x49d8f0 if i==0 else (int(SPECS[i-1]['address'],16) if i<=4 else 0x9f17a0)
            c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0;patched=bytearray(built)
            for site,(name,kind) in rel.items():struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail
            candidate[address]=patched;native[address]=retail
        candidate[0x9f18f0]=native[0x9f18f0]=pe.get_data(0x9f18f0-0x400000,224)
    assert len(candidate[0x49d8f0])==21
    comparative=0
    for old in range(256):
        for value in [0,1,127,255]:assert setter(candidate[0x49d8f0],old,value)==setter(native[0x49d8f0],old,value);comparative+=1
    composed=0
    for value in range(256):
        for length in [1,8,32]:assert execute(candidate,0x6292d0,value,length,real_pause=True)==execute(native,0x6292d0,value,length,real_pause=True);composed+=1
    fixture=Coff(objects[-1]);assert SYMBOL not in fixture.symbols and not any(s['symbol'] in fixture.symbols for s in SPECS)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(len(objects)-1):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'WORLD_PAUSE_BEHAVIOR_PASS scenarios=1024' in r.stdout and 'WORLD_EVENT_CONTROL_BEHAVIOR_PASS scenarios=16896' in r.stdout,r.stdout+r.stderr
    report=dict(address='0049d8f0',grade='MATCH',bytes=21,providers=providers,comparative_scenarios=comparative,composed_scenarios=composed,behavior=r.stdout.strip(),missing_provider_checks=len(objects)-1,
                fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),control_fixture_sha256=hashlib.sha256((ROOT/'rebuild/tests/integration/WorldEventControl_test.cpp').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_PAUSE_PASS compiled=1024 control=16896 comparative=1024 composed=768')
if __name__=='__main__':main()
