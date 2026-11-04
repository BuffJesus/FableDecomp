"""Check the release MSN event's lookups and ignored controlled-creature result."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
import gate_inventory_events as G
ROOT=G.ROOT
SPEC=dict(address='0062c6e0',file='CWorld_EAOpenPCMSNChatMenu_0062c6e0.cpp',symbol='?EAOpenPCMSNChatMenu@CWorld@@QAEXPBVCGameEvent@@@Z',size=24,grade='RELOCATION_MATCH')
TARGETS=dict(G.TARGETS);TARGETS[SPEC['symbol']]=0x62c6e0
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/MSNMenu_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/msn-menu.json');a=ap.parse_args();env=check_one.env();work=Path(tempfile.mkdtemp(prefix='msn-menu-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate([SPEC]+G.EXTRA):
            source=a.stage/s['file'] if a.stage and i==0 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size']);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for value in range(256):
        for has in [False,True]:
            for keys in G.MAPS:
                for pattern in range(2):
                    for result in [0,0x2300040,0xfffffff0,0xdeadbeef]:assert G.execute(candidate,SPEC,value,has,keys,pattern,inventory_leaves=[],controlled_result=result)==G.execute(native,SPEC,value,has,keys,pattern,inventory_leaves=[],controlled_result=result);scenarios+=1
        if value%32==0:print('MSN_MENU_PROGRESS comparisons=%d'%scenarios,flush=True)
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in [SPEC]+G.EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(3):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'MSN_MENU_BEHAVIOR_PASS scenarios=40960' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=scenarios,compiled_scenarios=40960,missing_provider_checks=3,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('MSN_MENU_PASS providers=3 compiled=40960 comparative=%d'%scenarios)
if __name__=='__main__':main()
