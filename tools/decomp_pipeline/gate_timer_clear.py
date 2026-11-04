"""Validate timer cleanup, including reentrant COM Release list mutations."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from timer_clear_oracle import ENTRY,SIZE,CONSTRUCT,DECREMENT,POP,execute
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?ClearAllQueries@CVsyncCallbackTimer@@AAEXXZ'
TARGETS={
    '?size@?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@QBEIXZ':SIZE,
    '??0?$_List_iterator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@QAE@PAU_List_node_base@1@@Z':CONSTRUCT,
    '??F?$_List_iterator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@QAEAAU01@XZ':DECREMENT,
    '?pop_back@?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@QAEXXZ':POP,
}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9f/CVsyncCallbackTimer_ClearAllQueries_009f7250.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9f/CVsyncCallbackTimer_ClearAllQueries_009f7250_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/timer-clear.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='timer-clear-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/Od'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==169 and len(relocs)==8
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,211)
    score=compare(built,native,relocs,retail_relocations('009f7250',211));assert score.near_match,score
    # Two identical helper sequences, independently anchored in retail.
    native_sites=[25,53,61,105,121,149,157,201]
    native_targets=[SIZE,CONSTRUCT,DECREMENT,POP]*2
    for site,target in zip(native_sites,native_targets):
        assert ENTRY+site+4+int.from_bytes(native[site:site+4],'little',signed=True)==target
    candidate=bytearray(built);checked=[]
    for (site,(symbol,kind)),target in zip(sorted(relocs.items()),native_targets):
        assert kind==20 and TARGETS[symbol]==target and built[site:site+4]==b'\0'*4
        struct.pack_into('<I',candidate,site,(target-ENTRY-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=symbol,target=hex(target)))
    scenarios=list(itertools.product(range(5),range(5),range(4),range(3)))
    for scenario in scenarios:assert execute(candidate,*scenario)==execute(native,*scenario),scenario
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'TIMER_CLEAR_BEHAVIOR_PASS scenarios=300' in r.stdout,r.stdout+r.stderr
    proof=dict(address='009f7250',grade='NEAR_MATCH',built_bytes=169,retail_bytes=211,similarity=score.to_dict(),relocations=checked,comparative_scenarios=300,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('TIMER_CLEAR_PASS grade=NEAR_MATCH compiled=300 comparative=300')

if __name__=='__main__':main()
