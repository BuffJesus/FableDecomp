"""Validate complete device-state registration and capability branches."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
ENTRY=0xa04630
SYMBOL='?SetupRenderStates@CRenderStateManager@@AAEXXZ'
TARGETS={'?FableRenderWrapStateTypes_01396da8@@3PAW4_D3DRENDERSTATETYPE@@A':0x1396da8}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0/CRenderStateManager_StateSetup_00a04580.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a0/CRenderStateManager_StateSetup_00a04580_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-state-setup.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='render-state-setup-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/O2'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/Oy','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==4609 and len(relocs)==8
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,4609)
    candidate=bytearray(built);checked=[]
    for site,(name,kind) in sorted(relocs.items()):
        assert kind in [6,20];addend=int.from_bytes(built[site:site+4],'little');target=TARGETS[name]
        value=target+addend if kind==6 else target+addend-ENTRY-site-4
        struct.pack_into('<I',candidate,site,value&0xffffffff)
        checked.append(dict(offset=site,symbol=name,target=hex(target),addend=addend))
    assert candidate==native
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'RENDER_STATE_SETUP_BEHAVIOR_PASS scenarios=3072' in r.stdout,r.stdout+r.stderr
    proof=dict(address='00a04630',grade='RELOCATION_MATCH',built_bytes=4609,retail_bytes=4609,relocations=checked,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('RENDER_STATE_SETUP_PASS grade=RELOCATION_MATCH compiled=3072')

if __name__=='__main__':main()
