"""Validate ordered render defaults, update queues and bookmark storage."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from render_state_defaults_oracle import ENTRY,execute
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ'

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0/CRenderStateManager_SetDefaultRenderStates_00a059e0.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a0/CRenderStateManager_SetDefaultRenderStates_00a059e0_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-state-defaults.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='render-state-defaults-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/O2'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/Oy','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==7388 and len(relocs)==33
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,7958)
    score=compare(built,native,relocs,retail_relocations('00a059e0',7958));assert score.near_match,score
    candidate=bytearray(built);checked=[];targets=[]
    for site,(name,kind) in sorted(relocs.items()):
        assert kind==6
        if name=='?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA':target=0x1396e64
        else:
            assert name.startswith('?FableRenderDefaultValue_') and name.endswith('@@3KA')
            target=int(name.split('_')[-1].split('@')[0],16)
        addend=int.from_bytes(built[site:site+4],'little');target+=addend;targets.append(target)
        struct.pack_into('<I',candidate,site,target);checked.append(dict(offset=site,symbol=name,target=hex(target)))
    import capstone
    from collections import Counter
    decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
    native_targets=[]
    for ins in decoder.disasm(native,ENTRY):
        for operand in ins.operands:
            if operand.type==capstone.x86.X86_OP_MEM and not operand.mem.base and not operand.mem.index:
                assert operand.access==capstone.CS_AC_READ
                native_targets.append(operand.mem.disp)
    assert set(targets)==set(native_targets), (set(targets),set(native_targets))
    scenarios=list(itertools.product(range(3),range(3),range(3),range(3),range(3),range(2)))
    for scenario in scenarios:assert execute(candidate,*scenario)==execute(native,*scenario),scenario
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'RENDER_STATE_DEFAULTS_BEHAVIOR_PASS scenarios=486' in r.stdout,r.stdout+r.stderr
    proof=dict(native_table_reads=len(native_targets),compiled_table_reads=len(targets),address='00a059e0',grade='NEAR_MATCH',built_bytes=7388,retail_bytes=7958,similarity=score.to_dict(),relocations=checked,comparative_scenarios=486,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('RENDER_STATE_DEFAULTS_PASS grade=NEAR_MATCH compiled=486 comparative=486')

if __name__=='__main__':main()
