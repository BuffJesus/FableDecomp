"""Verify real player lookup, first-match/neutral fallback, and main/world composition."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32
from unicorn.x86_const import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='004498c0',name='GetPlayer',file='CPlayerManager_GetPlayer_004498c0.cpp',symbol='?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z',size=73,grade='MATCH'),dict(address='00449910',name='GetPlayer',file='CPlayerManager_PeekPlayer_00449910.cpp',symbol='?GetPlayer@CPlayerManager@@QBEABVCPlayer@@J@Z',size=73,grade='MATCH'),dict(address='00449960',name='GetMainPlayer',file='CPlayerManager_PeekMainPlayer_00449960.cpp',symbol='?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ',size=10,grade='RELOCATION_MATCH'),dict(address='00449970',name='GetMainPlayer',file='CPlayerManager_GetMainPlayer_00449970.cpp',symbol='?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ',size=10,grade='RELOCATION_MATCH'),dict(address='00449700',name='GetMainPlayerNumber',file='CPlayerManager_GetMainPlayerNumber_00449700.cpp',symbol='?GetMainPlayerNumber@CPlayerManager@@QBEJXZ',size=4,grade='MATCH')]
WORLD=ROOT/'rebuild/src/compiled/00/49/CWorld_GetPlayer_0049d850.cpp'
WORLD_SYMBOL='?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z'
TARGETS={'?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z':0x4498c0,'?GetPlayer@CPlayerManager@@QBEABVCPlayer@@J@Z':0x449910}
NUMBERS=[[0,1,2,3,4,5,6,7],[0,0xffffffff,0x80000000,0x7fffffff,7,7,127,128],[7]*8,[255,0xffffff80,127,0xffffffff,0,1,42,0xffffffd6]]
QUERIES=[0,1,7,42,0xffffffff,127,0xffffff80,255,128,0x80000000,0x7fffffff,0xffffffd6]
def execute(codes,index,pattern,count,neutral,query):
    manager,entries,world,players,stack,stop=0x2000040,0x2000200,0x2200040,[0x2100040+i*0x100 for i in range(8)],0x3008000,0x4000000
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x449000,0x1000),(0x49d000,0x1000),(0x2000000,0x1000),(0x2100000,0x1000),(0x2200000,0x1000),(0x3000000,0x10000),(stop,0x1000)]:u.mem_map(base,size)
    for address,code in codes.items():u.mem_write(address,bytes(code))
    memory={0x2000000:bytearray([pattern*71]*0x300),0x2100000:bytearray([pattern*71]*0x900),0x2200000:bytearray([255-pattern*71]*0x218)}
    struct.pack_into('<IIIII',memory[0x2000000],0x4c,entries,entries+count*4,entries+32,neutral,query)
    struct.pack_into('<8I',memory[0x2000000],0x200,*players)
    for i,p in enumerate(players):struct.pack_into('<I',memory[0x2100000],p-0x2100000+0x28,NUMBERS[pattern][i])
    struct.pack_into('<I',memory[0x2200000],0x4c,manager)
    for base,data in memory.items():u.mem_write(base,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,query));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,world if index==5 else manager)
    saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x65430000+i)
    address=0x49d850 if index==5 else int(SPECS[index]['address'],16);u.emu_start(address,stop,count=1000)
    selected=next((i for i in range(count) if NUMBERS[pattern][i]==query),neutral);expected=query if index==4 else players[selected]
    assert u.reg_read(UC_X86_REG_EAX)==expected,(index,pattern,count,neutral,query)
    assert u.reg_read(UC_X86_REG_ESP)==stack+(8 if index in [0,1,5] else 4) and all(u.reg_read(r)==0x65430000+i for i,r in enumerate(saved))
    assert all(bytes(u.mem_read(base,len(data)))==data for base,data in memory.items())
    return expected
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path)
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/PlayerLookup_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/player-lookup.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='player-lookup-'));objects=[];providers=[];candidate={};native={}
    allspec=SPECS+[dict(address='0049d850',symbol=WORLD_SYMBOL,size=8,grade='RELOCATION_MATCH')]
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(allspec):
            source=WORLD if i==5 else (a.stage/s['file'] if a.stage else ROOT/'rebuild/src/compiled/00/44'/s['file']);obj=work/('source%d.obj'%i)
            r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
            c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind==20;struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-address-site-4)&0xffffffff)
            retail=pe.get_data(address-0x400000,len(built));assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    comparative=0
    for pattern in range(4):
        for count in range(9):
            for neutral in range(count or 1):
                for query in QUERIES:
                    for index in range(6):assert execute(candidate,index,pattern,count,neutral,query)==execute(native,index,pattern,count,neutral,query);comparative+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
    assert not any(s['symbol'] in Coff(test).symbols for s in allspec);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(6):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'PLAYER_LOOKUP_BEHAVIOR_PASS scenarios=10656' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,entries=SPECS,comparative_scenarios=comparative,missing_provider_checks=6,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_player_manager.h').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('PLAYER_LOOKUP_PASS methods=5 bytes=170 compiled=10656 comparative=%d'%comparative)
if __name__=='__main__':main()
