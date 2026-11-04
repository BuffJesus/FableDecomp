"""Check the retail-empty world event handlers through the real dispatch table."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_display_engine_leaves import execute
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[dict(address='00629210',file='CWorld_EAApplyAllScriptedMapBrushes_00629210.cpp',symbol='?EAApplyAllScriptedMapBrushes@CWorld@@QAEXPBVCGameEvent@@@Z'),
       dict(address='0085cb80',file='CWorld_LEASetEditorMode_0085cb80.cpp',symbol='?LEASetEditorMode@CWorld@@QAEXPBVCGameEvent@@@Z')]
TABLE=ROOT/'rebuild/src/compiled/01/3b/CMainGameComponent_EventDispatch_013b9288.cpp'

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path)
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/WorldEventDefaults_test.cpp')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/world-event-defaults.json');a=ap.parse_args()
    env=check_one.env();work=Path(tempfile.mkdtemp(prefix='world-event-defaults-'));objects=[];providers=[];entries=[];comparisons=0
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS):
            source=a.stage/s['file'] if a.stage else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file']
            obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr
            c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and not rel and built==b'\xc2\x04\x00'
            native=pe.get_data(int(s['address'],16)-0x400000,3);assert built==native
            for pattern in range(256):
                args=dict(address=s['address'],argc=1);assert execute(built,args,pattern,0)==execute(native,args,pattern,0);comparisons+=1
            objects.append(obj);providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()));entries.append(dict(s,bytes=3,grade='MATCH'))
    for i,source in enumerate([TABLE,a.test]):
        obj=work/('dep%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
    assert not any(s['symbol'] in Coff(objects[-1]).symbols for s in SPECS)
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(3):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True)
        assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert not r.returncode and 'WORLD_EVENT_DEFAULTS_BEHAVIOR_PASS scenarios=512' in r.stdout and 'DISPATCH_TABLE_BEHAVIOR_PASS scenarios=22272' in r.stdout,r.stdout+r.stderr
    report=dict(entries=entries,providers=providers,comparative_scenarios=comparisons,behavior=r.stdout.strip(),missing_provider_checks=3,
                fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),table_sha256=hashlib.sha256(TABLE.read_bytes()).hexdigest(),
                dispatch_fixture_sha256=hashlib.sha256((ROOT/'rebuild/tests/integration/DispatchTable_test.cpp').read_bytes()).hexdigest())
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('WORLD_EVENT_DEFAULTS_PASS providers=2 compiled=512 dispatch=22272 comparative=512')
if __name__=='__main__':main()
