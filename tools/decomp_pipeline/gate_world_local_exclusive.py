"""Verify local exclusive-mode conversion with genuine event package and circular reader."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_world_event_control import execute,PACKAGE,PACKAGE_SYMBOL,TARGETS as CONTROL_TARGETS
ROOT=Path(__file__).resolve().parents[2];FILE='CWorld_LEASetExclusiveMode_0085ca80.cpp';SYMBOL='?LEASetExclusiveMode@CWorld@@QAEXPBVCGameEvent@@@Z';ENTRY=0x85ca80
READER=ROOT/'rebuild/src/compiled/00/9f/CGameEventDataPackage_GetData_009f18f0.cpp';READER_SYMBOL='?GetData@CGameEventDataPackage@@QAEXJPAX@Z'
TARGETS=dict(CONTROL_TARGETS,**{'?SetExclusiveMode@CMainGameComponent@@QAEX_N@Z':0x4163b7})
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/85'/FILE);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldLocalExclusive_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-local-exclusive.json');a=ap.parse_args()
    env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='world-local-exclusive-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,(address,source,symbol,length) in enumerate([(ENTRY,a.source,SYMBOL,103),(0x9f17a0,PACKAGE,PACKAGE_SYMBOL,53),(0x9f18f0,READER,READER_SYMBOL,166)]):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==length,(symbol,len(built));patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,length);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for value in range(256):
        for length in range(1,33):assert execute(candidate,ENTRY,value,length)==execute(native,ENTRY,value,length);scenarios+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(symbol not in Coff(test).symbols for symbol in [SYMBOL,PACKAGE_SYMBOL,READER_SYMBOL]);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(3):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'WORLD_LOCAL_EXCLUSIVE_BEHAVIOR_PASS scenarios=32768' in r.stdout,r.stdout+r.stderr
    report=dict(address='0085ca80',grade='RELOCATION_MATCH',bytes=103,providers=providers,comparative_scenarios=scenarios,compiled_scenarios=32768,missing_provider_checks=3,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in ['fable_world.h','fable_game_event.h']})
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_LOCAL_EXCLUSIVE_PASS bytes=103 compiled=32768 comparative=%d'%scenarios)
if __name__=='__main__':main()
