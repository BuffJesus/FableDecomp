"""Check system-manager construction/destruction and member lifetime order."""
import argparse
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT=Path(__file__).resolve().parents[2]
ITEMS=[(0x9a7440,118,'??0CSystemManager@@QAE@XZ'),(0x9a5800,416,'??1CSystemManager@@UAE@XZ')]
TARGETS={
    '??0CInitBaseClass@@QAE@XZ':0x99a310,
    '??_7CSystemManager@@6B@':0x129affc,
    '??0CWideString@@QAE@XZ':0x99aed0,
    '??0CCharString@@QAE@XZ':0x99e4b0,
    '??1CCharString@@QAE@XZ':0x99eae0,
    '??1CWideString@@QAE@XZ':0x99b510,
    '??3@YAXPAX@Z':0xbfe9bc,
    '??1CInitBaseClass@@UAE@XZ':0x99a410,
}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_Lifetime_009a7440.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_Lifetime_009a7440_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-lifetime.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='system-lifetime-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,target in [(args.source,obj),(args.test,test)]:
        r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);pe=pefile.PE(str(EXE));reports=[]
    for address,length,symbol in ITEMS:
        section,offset=coff.symbols[symbol];built,relocs=coff.sections[section]
        assert offset==0 and len(built)==length
        retail=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length);candidate=bytearray(built)
        assert len(relocs)==(5 if address==0x9a7440 else 13)
        for site,(name,kind) in relocs.items():
            assert built[site:site+4]==b'\0'*4
            assert kind==(6 if name=='??_7CSystemManager@@6B@' else 20)
            target=TARGETS[name];value=(target-(address+site+4 if kind==20 else 0))&0xffffffff
            assert int.from_bytes(retail[site:site+4],'little')==value
            struct.pack_into('<I',candidate,site,value)
        assert candidate==retail
        reports.append(dict(address=f'{address:08x}',bytes=length,relocations=len(relocs),grade='RELOCATION_MATCH'))
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    tested=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert tested.returncode==0 and 'SYSTEM_LIFETIME_BEHAVIOR_PASS scenarios=16' in tested.stdout,tested.stdout+tested.stderr
    report=dict(functions=reports,behavior=tested.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SYSTEM_LIFETIME_PASS functions=2 bytes=534 compiled=16')

if __name__=='__main__':main()
