"""Validate pending-query list node cleanup."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from timer_list_clear_oracle import ENTRY,FREE,execute
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?clear@?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@QAEXXZ'
TARGETS={'_free':FREE}

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9f/CVsyncPendingQueryList_Clear_009f7830.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9f/CVsyncPendingQueryList_Clear_009f7830_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/timer-list-clear.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='timer-list-clear-'));obj,test=work/'candidate.obj',work/'fixture.obj'
    for source,out,opt in [(args.source,obj,'/Od'),(args.test,test,'/O2')]:
        r=check_one.cl(['/nologo','/c',opt,'/Ob1','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
    coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert offset==0 and len(built)==92 and len(relocs)==1
    pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,92)
    score=compare(built,native,relocs,retail_relocations('009f7830',92));assert score.near_match,score
    assert ENTRY+54+4+int.from_bytes(native[54:58],'little',signed=True)==FREE
    assert relocs=={48:('_free',20)} and built[48:52]==b'\0'*4
    candidate=bytearray(built);struct.pack_into('<I',candidate,48,(FREE-ENTRY-52)&0xffffffff)
    checked=[dict(offset=48,symbol='_free',target=hex(FREE))]
    scenarios=list(itertools.product(range(128),range(4)))
    for scenario in scenarios:assert execute(candidate,*scenario)==execute(native,*scenario),scenario
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
    r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'TIMER_LIST_CLEAR_BEHAVIOR_PASS scenarios=512' in r.stdout,r.stdout+r.stderr
    proof=dict(address='009f7830',grade='NEAR_MATCH',built_bytes=92,retail_bytes=92,similarity=score.to_dict(),relocations=checked,comparative_scenarios=512,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n')
    print('TIMER_LIST_CLEAR_PASS grade=NEAR_MATCH compiled=512 comparative=512')

if __name__=='__main__':main()
