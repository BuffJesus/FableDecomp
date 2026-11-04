"""Validate native system shutdown against retail with mocked resource ownership."""
import argparse
import hashlib
import itertools
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare, retail_relocations
from pe_oracle import EXE
from system_shutdown_oracle import ENTRY, TARGETS, execute

ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?Uninitialise@CSystemManager@@QAEXXZ'

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_Uninitialise_009a60d0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_Uninitialise_009a60d0_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/system-shutdown.json')
    args=ap.parse_args();work=Path(tempfile.mkdtemp(prefix='system-shutdown-'))
    env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    obj,test_obj=work/'candidate.obj',work/'fixture.obj'
    for source,target in [(args.source,obj),(args.test,test_obj)]:
        r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(target),str(source.resolve())],env,work)
        assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);section,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[section]
    assert offset==0 and len(built)==667 and len(relocs)==31
    pe=pefile.PE(str(EXE));retail=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,667)
    score=compare(built,retail,relocs,retail_relocations('009a60d0',667));assert score.near_match,score
    imports={i.address:i.name for group in pe.DIRECTORY_ENTRY_IMPORT for i in group.imports}
    assert imports[0x1440340]==b'DestroyWindow' and imports[0x143fde4]==b'DeleteCriticalSection'
    assert pe.get_data(0x129ab30-pe.OPTIONAL_HEADER.ImageBase,4)==struct.pack('<I',512)
    assert pe.get_data(0x138e074-pe.OPTIONAL_HEADER.ImageBase,4)==struct.pack('<I',0x13c82f8)
    candidate=bytearray(built);checked=[]
    for site,(symbol,kind) in relocs.items():
        target=TARGETS[symbol];assert kind in [6,20]
        addend=int.from_bytes(built[site:site+4],'little');assert addend in [0,4]
        if addend:assert 'PClass@' in symbol
        value=(target+addend-(ENTRY+site+4 if kind==20 else 0))&0xffffffff
        assert int.from_bytes(retail[site:site+4],'little')==value,(site,symbol)
        struct.pack_into('<I',candidate,site,value)
        checked.append(dict(offset=site,symbol=symbol,kind=kind,target=hex(target),addend=addend))
    # Retain the accepted compare-to-true versus boolean test distinction.
    # All validated bool values obey the public 0/1 ABI.
    scenarios=list(itertools.product(range(2),range(4),range(4),range(2),range(3),[0,1,2,512],[0,1,255]))
    for scenario in scenarios:assert execute(candidate,*scenario)==execute(retail,*scenario),scenario
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    tested=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert tested.returncode==0 and 'SYSTEM_SHUTDOWN_BEHAVIOR_PASS scenarios=192' in tested.stdout,tested.stdout+tested.stderr
    report=dict(address='009a60d0',grade='NEAR_MATCH',bytes=667,similarity=score.to_dict(),
                relocations=checked,comparative_scenarios=len(scenarios),behavior=tested.stdout.strip(),
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('SYSTEM_SHUTDOWN_PASS grade=NEAR_MATCH bytes=667 compiled=192 comparative=2304')

if __name__=='__main__':main()
