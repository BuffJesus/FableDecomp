"""Validate shared inline CDefPointer assignment; this helper claims no retail function coverage."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from def_pointer_assignment_oracle import compare
ROOT=check_one.ROOT
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/tests/integration/DefPointerAssign_provider.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/DefPointerAssign_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/def-pointer-assignment.json');a=ap.parse_args()
    work=Path(tempfile.mkdtemp(prefix='def-pointer-assignment-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];objs=[]
    for i,p in enumerate((a.source,a.test)):
        obj=work/('s%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,work);assert not r.returncode,r.stdout;objs.append(obj)
    c=Coff(objs[0]);sec,off=c.symbols['@AssignDefPointer@8'];code,rels=c.sections[sec];assert off==0 and not rels
    with pefile.PE(str(ROOT/'debug_build/FableWin.exe')) as pe:donor=pe.get_data(0x1da50d0-pe.OPTIONAL_HEADER.ImageBase,81)
    assert donor[-3:]==b'\xc2\x04\0';cases=compare(code,donor)
    cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(work/'f.exe'),str(objs[1])]
    r=subprocess.run(cmd,env=env,capture_output=True,text=True);assert r.returncode,'assignment provider omission linked'
    r=subprocess.run(cmd+[str(objs[0])],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout
    r=subprocess.run([str(work/'f.exe')],env=env,capture_output=True,text=True,timeout=60)
    assert not r.returncode and 'DEF_POINTER_ASSIGNMENT_BEHAVIOR_PASS cases=4096' in r.stdout,(r.returncode,r.stdout)
    report=dict(comparative_scenarios=cases,behavior=r.stdout.strip(),missing_provider_checks=1,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((a.include/'fable_def_pointer.h').read_bytes()).hexdigest(),coverage_claim='shared inline contract only; no new retail function')
    a.output.write_text(json.dumps(report,indent=2)+'\n');print('DEF_POINTER_ASSIGNMENT_PASS comparative=%d compiled=4096'%cases)
if __name__=='__main__':main()
