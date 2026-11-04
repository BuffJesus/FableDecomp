"""Validate forced D3D state resend and live callback mutations."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from render_state_realise_oracle import ENTRY,execute
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?RealiseRenderState@CRenderStateManager@@QAEXXZ'

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0/CRenderStateManager_RealiseRenderState_00a058c0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a0/CRenderStateManager_RealiseRenderState_00a058c0_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-state-realise.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='render-state-realise-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/O2'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/Oy','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==161 and not relocs
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,167)
    score=compare(built,native,relocs,retail_relocations('00a058c0',167));assert score.near_match,score
    checked=[]
    scenarios=list(itertools.product([-1,0,1,2,17,256],range(6),range(8),range(2),range(2),range(2)))
    for scenario in scenarios:assert execute(built,*scenario)==execute(native,*scenario),scenario
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'RENDER_STATE_REALISE_BEHAVIOR_PASS scenarios=2304' in r.stdout,r.stdout+r.stderr
    proof=dict(address='00a058c0',grade='NEAR_MATCH',built_bytes=161,retail_bytes=167,similarity=score.to_dict(),relocations=checked,comparative_scenarios=2304,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('RENDER_STATE_REALISE_PASS grade=NEAR_MATCH compiled=2304 comparative=2304')

if __name__=='__main__':main()
