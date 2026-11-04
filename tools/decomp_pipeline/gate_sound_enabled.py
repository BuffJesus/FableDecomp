"""Validate the corrected owner and candidate-linked sound enable helper."""
import argparse
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?UnknownSetEnabled_009915e0@CASoundManager@@QAEX_N@Z'

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/99/CASoundManager_UnknownSetEnabled_009915e0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/99/CASoundManager_UnknownSetEnabled_009915e0_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/sound-enabled.json')
    args=ap.parse_args(); work=Path(tempfile.mkdtemp(prefix='sound-enabled-'))
    env=check_one.env(); env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    obj,test=work/'candidate.obj',work/'test.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert result.returncode==0,result.stdout+result.stderr
    coff=Coff(obj); sec,off=coff.symbols[SYMBOL]; built,relocs=coff.sections[sec]
    with pefile.PE(str(EXE),fast_load=True) as pe:
        retail=pe.get_data(0x009915e0-pe.OPTIONAL_HEADER.ImageBase,32)
    assert off==0 and not relocs and built==retail
    exe=work/'fixture.exe'; cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    standalone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True)
    assert standalone.returncode!=0,'test must require reconstructed candidate'
    linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True)
    assert linked.returncode==0,linked.stdout+linked.stderr
    result=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert result.returncode==0 and 'SOUND_ENABLED_PASS scenarios=8' in result.stdout,result.stdout+result.stderr
    report=dict(address='009915e0',grade='MATCH',bytes=32,behavior=result.stdout.strip(),
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True); args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SOUND_ENABLED_PASS grade=MATCH bytes=32 compiled=8 candidate_required=true')

if __name__=='__main__': main()
