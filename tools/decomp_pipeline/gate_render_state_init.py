"""Validate render-state initialization and callback mutations."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from render_state_init_oracle import ENTRY,GLOBAL,SETUP,DEFAULT,FORCE,execute
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?Init@CRenderStateManager@@QAEXPAUIDirect3DDevice9@@@Z'
TARGETS={
 '?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A':GLOBAL,
 '?SetupRenderStates@CRenderStateManager@@AAEXXZ':SETUP,
 '?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ':DEFAULT,
 '?ForceResendRenderStatesToHardware@CRenderStateManager@@QAEXXZ':FORCE,
}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0/CRenderStateManager_Init_00a07900.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a0/CRenderStateManager_Init_00a07900_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-state-init.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='render-state-init-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/O2'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/Oy','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==117 and len(relocs)==4
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,115)
    score=compare(built,native,relocs,retail_relocations('00a07900',115));assert score.near_match,score
    native_sites=[23,78,102,107];native_targets=[GLOBAL,SETUP,DEFAULT,FORCE]
    for site,target in zip(native_sites,native_targets):
        value=int.from_bytes(native[site:site+4],'little',signed=site!=23)
        assert (value if site==23 else ENTRY+site+4+value)==target
    candidate=bytearray(built);checked=[]
    for site,(symbol,kind) in sorted(relocs.items()):
        target=TARGETS[symbol];assert kind==(6 if target==GLOBAL else 20)
        assert built[site:site+4]==b'\0'*4
        struct.pack_into('<I',candidate,site,target if kind==6 else (target-ENTRY-site-4)&0xffffffff)
        checked.append(dict(offset=site,symbol=symbol,target=hex(target)))
    scenarios=list(itertools.product([-1,0,1,2,17,255,256],range(2),range(16)))
    for scenario in scenarios:assert execute(candidate,*scenario)==execute(native,*scenario),scenario
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'RENDER_STATE_INIT_BEHAVIOR_PASS scenarios=224' in r.stdout,r.stdout+r.stderr
    proof=dict(address='00a07900',grade='NEAR_MATCH',built_bytes=117,retail_bytes=115,similarity=score.to_dict(),relocations=checked,comparative_scenarios=224,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('RENDER_STATE_INIT_PASS grade=NEAR_MATCH compiled=224 comparative=224')

if __name__=='__main__':main()
