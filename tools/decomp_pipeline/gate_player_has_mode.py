"""Verify typed CPlayer mode membership against the retail circular-list walk."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_MEM_READ
from unicorn.x86_const import *
FILE='CPlayer_HasMode_00634160.cpp';SYMBOL='?HasMode@CPlayer@@QBE_NW4EPlayerMode@@@Z';HEADERS=['fable_player_manager.h','fable_stlport_algo.h']
LISTS=[[],[1],[19,1,40],[1,1,1],[-0x80000000,-1,0,1,19,40,0x7fffffff],[40,19,2,43,17,16,1]];QUERIES=[-0x80000000,-1,0,1,2,16,17,19,40,43,273,0x7fffffff]
def execute(code,keys,query,sentinel,pattern):
    u=Uc(UC_ARCH_X86,UC_MODE_32);player,nodes,stack,stop=0x2000040,0x2100040,0x3008000,0x4000000
    for page in [0x634000,0x2000000,0x2100000,stop]:u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000);u.mem_write(0x634160,code);regions={a:bytearray(bytes([pattern*71])*0x300) for a in [0x2000000,0x2100000]};struct.pack_into('<I',regions[0x2000000],0x250,nodes);n=len(keys)
    for i,k in enumerate([sentinel]+keys):struct.pack_into('<III',regions[0x2100000],0x40+i*12,(nodes+12 if n else nodes) if i==0 else (nodes+(i+1)*12 if i<n else nodes),nodes+n*12 if i==0 else nodes+(i-1)*12,k&0xffffffff)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,query&0xffffffff));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,player);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    reads=[]
    def read(uc,access,address,size,value,user):
        if nodes<=address<nodes+(n+1)*12 and (address-nodes)%12==8:reads.append((address-nodes)//12)
    u.hook_add(UC_HOOK_MEM_READ,read);u.emu_start(0x634160,stop,count=200);found=next((i for i,k in enumerate(keys) if k==query),None);expected=list(range(1,(found+2 if found is not None else n+1)));assert reads==expected;assert u.reg_read(UC_X86_REG_EAX)&255==int(found is not None);assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return reads
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/63'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PlayerHasMode_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/player-has-mode.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='player-has-mode-gate-'));obj=work/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(a.source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj);sec,off=c.symbols[SYMBOL];built,rel=c.sections[sec];assert off==0 and len(built)==39 and not rel
    with pefile.PE(str(EXE)) as pe:native=pe.get_data(0x234160,39)
    assert built==native;count=0
    for keys in LISTS:
        for query in QUERIES:
            for sentinel in QUERIES:
                for pattern in range(4):assert execute(built,keys,query,sentinel,pattern)==execute(native,keys,query,sentinel,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert SYMBOL not in Coff(test).symbols;exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)];r=subprocess.run(link+[str(test)],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout);r=subprocess.run(link+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True);assert not r.returncode and 'PLAYER_HAS_MODE_BEHAVIOR_PASS scenarios=3456' in r.stdout,r.stdout+r.stderr
    report=dict(comparative_scenarios=count,compiled_scenarios=3456,missing_provider_checks=1,source=str(a.source),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PLAYER_HAS_MODE_PASS compiled=3456 comparative=%d'%count)
if __name__=='__main__':main()
