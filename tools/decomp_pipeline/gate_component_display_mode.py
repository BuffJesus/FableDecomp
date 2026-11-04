"""Verify display switch and actual world-event composition against retail."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_world_local_display import PACKAGE,PACKAGE_SYMBOL,READER,READER_SYMBOL,TARGETS as EVENT_TARGETS
from component_display_mode_oracle import execute,cases
ROOT=Path(__file__).resolve().parents[2];FILE='CMainGameComponent_SetDisplayMode_0041641f.cpp';SYMBOL='?SetDisplayMode@CMainGameComponent@@QAEXKKE@Z';ENTRY=0x41641f
EVENT=ROOT/'rebuild/src/compiled/00/85/CWorld_LEASetDisplayMode_0085caf0.cpp';EVENT_SYMBOL='?LEASetDisplayMode@CWorld@@QAEXPBVCGameEvent@@@Z'
TARGETS=dict(EVENT_TARGETS,**{'?PreChangeResolution@CDisplayEngine@@QAEXXZ':0x434a10,'?SetDisplayMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@ABVC2DExtentsI@@KW4ESurfaceMultisampleType@@K@Z':0x9bff40,'_g_FableDisplayManager_013B8390':0x13b8390,'?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ':0x9f2f90,'?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ':0x9f2f60,'?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820,'?GetHeight@CSurface@@QBEKXZ':0x415bf0,'?PostChangeResolution@CMainGameComponent@@QAEXKKK@Z':0x416365})
TARGETS['?g_FableDisplayManager_013B8390@@3PAVCDisplayManager@@A']=0x13b8390
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/41'/FILE);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ComponentDisplayMode_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/component-display-mode.json');a=ap.parse_args();env=check_one.env();work=Path(tempfile.mkdtemp(prefix='component-display-mode-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,(address,source,symbol,length) in enumerate([(ENTRY,a.source,SYMBOL,125),(0x85caf0,EVENT,EVENT_SYMBOL,132),(0x9f17a0,PACKAGE,PACKAGE_SYMBOL,53),(0x9f18f0,READER,READER_SYMBOL,166)]):
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[symbol];built,rel=c.sections[sec];assert off==0 and len(built)==length,(symbol,len(built));patched=bytearray(built)
            for site,(name,kind) in rel.items():
                assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,length);assert patched==retail;candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    scenarios=0
    for i,(width,height,depth) in enumerate(cases()):
        for status in [0,0xffffffff]:
            for rebind in range(4):
                for composed in [False,True]:assert execute(candidate,width,height,depth,status,rebind,composed)==execute(native,width,height,depth,status,rebind,composed);scenarios+=1
        if i%128==0:print('COMPONENT_DISPLAY_MODE_PROGRESS comparisons=%d'%scenarios,flush=True)
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/GS','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(symbol not in Coff(test).symbols for symbol in [SYMBOL,EVENT_SYMBOL,PACKAGE_SYMBOL,READER_SYMBOL]);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(4):
        r=subprocess.run(link+[str(obj) for i,obj in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'COMPONENT_DISPLAY_MODE_BEHAVIOR_PASS scenarios=10304' in r.stdout,r.stdout+r.stderr
    report=dict(address='0041641f',grade='RELOCATION_MATCH',bytes=125,providers=providers,comparative_scenarios=scenarios,compiled_scenarios=10304,missing_provider_checks=4,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest());a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('COMPONENT_DISPLAY_MODE_PASS bytes=125 compiled=10304 comparative=%d'%scenarios)
if __name__=='__main__':main()
