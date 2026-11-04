"""Check the shared system/sound ABI used by the native per-frame update."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_Update_009a6460.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_Update_009a6460_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-update.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='system-update-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);section,offset=coff.symbols['?Update@CSystemManager@@QAE?AW4ESystemReturn@@XZ'];built,relocs=coff.sections[section]
    assert offset==0 and len(built)==65 and relocs=={4:('?UpdateActive@CSystemManager@@QAEXXZ',20)}
    assert built[4:8]==b'\0'*4
    pe=pefile.PE(str(EXE));retail=pe.get_data(0x9a6460-pe.OPTIONAL_HEADER.ImageBase,65)
    candidate=bytearray(built);struct.pack_into('<I',candidate,4,(0x9a6370-0x9a6468)&0xffffffff);assert candidate==retail
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    tested=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert tested.returncode==0 and 'SYSTEM_UPDATE_BEHAVIOR_PASS scenarios=512' in tested.stdout,tested.stdout+tested.stderr
    report=dict(address='009a6460',grade='RELOCATION_MATCH',bytes=65,behavior=tested.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SYSTEM_UPDATE_PASS grade=RELOCATION_MATCH bytes=65 compiled=512')

if __name__=='__main__':main()
