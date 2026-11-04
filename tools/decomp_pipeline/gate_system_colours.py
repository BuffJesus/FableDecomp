"""Check the named system palette against retail and a mocked GetSysColor."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
ENTRY=0x9a5490
SYMBOL='?InitSystemColours@CSystemManager@@AAEXXZ'
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_InitSystemColours_009a5490.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_InitSystemColours_009a5490_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-colours.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='system-colours-'));obj,test=work/'candidate.obj',work/'test.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,off=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert off==0 and len(built)==792 and relocs=={5:('__imp__GetSysColor@4',6)}
    assert built[5:9]==b'\0'*4
    candidate=bytearray(built);struct.pack_into('<I',candidate,5,0x1440374)
    pe=pefile.PE(str(EXE));retail=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,792)
    imports={i.address:i.name for group in pe.DIRECTORY_ENTRY_IMPORT for i in group.imports}
    assert imports[0x1440374]==b'GetSysColor' and candidate==retail
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    alone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert alone.returncode!=0
    linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert linked.returncode==0,linked.stdout+linked.stderr
    tested=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert tested.returncode==0 and 'SYSTEM_COLOURS_BEHAVIOR_PASS scenarios=512' in tested.stdout,tested.stdout+tested.stderr
    report=dict(address='009a5490',grade='RELOCATION_MATCH',bytes=792,behavior=tested.stdout.strip(),
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SYSTEM_COLOURS_PASS grade=RELOCATION_MATCH bytes=792 compiled=512')
if __name__=='__main__':main()
