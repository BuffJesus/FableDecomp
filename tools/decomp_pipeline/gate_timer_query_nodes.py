"""Validate the timer's STLport node allocation and front/back insertion."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,capstone
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from timer_query_nodes_oracle import *
ROOT=Path(__file__).resolve().parents[2]
OWNER='?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@'
CREATE_SYMBOL='?_M_create_node@'+OWNER+'IAEPAU?$_List_node@UCPendingQuery@CVsyncCallbackTimer@@@2@ABUCPendingQuery@CVsyncCallbackTimer@@@Z'
ITER_SYMBOL='??0?$_List_iterator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@QAE@PAU_List_node_base@1@@Z'
SPECS=[(CREATE,CREATE_SYMBOL,131,137),(FRONT,'?push_front@'+OWNER+'QAEXABUCPendingQuery@CVsyncCallbackTimer@@@Z',113,119),(BACK,'?push_back@'+OWNER+'QAEXABUCPendingQuery@CVsyncCallbackTimer@@@Z',111,117)]
TARGETS={'_malloc':MALLOC,'??2@YAPAXIPAX@Z':PLACEMENT,ITER_SYMBOL:ITERATOR,CREATE_SYMBOL:CREATE}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9f/CVsyncPendingQueryList_NodeOperations_009f7ad0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9f/CVsyncPendingQueryList_NodeOperations_009f7ad0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/timer-query-nodes.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='timer-query-nodes-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out,opt in [(args.source,obj,'/Od'),(args.test,test,'/O2')]:
  r=check_one.cl(['/nologo','/c',opt,'/Ob1','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));candidate={};native={};proof=[]
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True
 for entry,symbol,built_length,native_length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==built_length
  native[entry]=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,native_length)
  score=compare(built,native[entry],relocs,retail_relocations('%08x'%entry,native_length));assert score.near_match,score
  patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind==20 and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   struct.pack_into('<I',patched,site,(target-entry-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
  calls=[ins.operands[0].imm for ins in decoder.disasm(native[entry],entry) if ins.mnemonic=='call']
  assert calls==[c['target'] for c in checked],(calls,checked)
  candidate[entry]=patched;proof.append(dict(address='%08x'%entry,grade='NEAR_MATCH',built_bytes=built_length,retail_bytes=native_length,similarity=score.to_dict(),relocations=checked))
 scenarios=list(itertools.product([0,1,2,7,31],range(8),range(3),range(2)))
 for entry,_,_,_ in SPECS:
  for scenario in scenarios:assert execute(candidate,entry,*scenario)==execute(native,entry,*scenario),(hex(entry),scenario)
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'TIMER_QUERY_NODES_BEHAVIOR_PASS scenarios=768' in r.stdout,r.stdout+r.stderr
 behavior=r.stdout.strip()
 post_source=ROOT/'rebuild/src/compiled/00/9f/CVsyncCallbackTimer_OnPostSwapScreens_009f73c0.cpp'
 post_test=ROOT/'rebuild/tests/00/9f/CVsyncCallbackTimer_OnPostSwapScreens_009f73c0_test.cpp'
 post_obj,post_fixture=work/'post.obj',work/'post_test.obj'
 for source,out in [(post_source,post_obj),(post_test,post_fixture)]:
  r=check_one.cl(['/nologo','/c','/Od','/MT','/DTIMER_POST_USE_NATIVE_NODES','/Fo'+str(out),str(source)],env,work);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run(cmd+[str(post_obj),str(post_fixture)],env=env,capture_output=True,text=True);assert r.returncode!=0,'native node provider required'
 r=subprocess.run(cmd+[str(post_obj),str(obj),str(post_fixture)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'TIMER_POST_SWAP_BEHAVIOR_PASS scenarios=2304' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,comparative_scenarios=len(scenarios)*3,behavior=behavior,composed_behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),post_fixture_sha256=hashlib.sha256(post_test.read_bytes()).hexdigest(),limitation='Allocation failure is outside the defined retail behavior: the native body adds eight to the allocation result before placement construction.')
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('TIMER_QUERY_NODES_PASS compiled=768 insertions=24576 comparative='+str(len(scenarios)*3))
if __name__=='__main__':main()
