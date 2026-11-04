"""Validate post-swap query recycling, wait/failure paths and timing writes."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from collections import Counter
from pathlib import Path
import pefile,capstone
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from timer_post_swap_oracle import *
from gate_timer_clear import TARGETS as LIST_TARGETS
from gate_timer_lifetime import TARGETS as LIFETIME_TARGETS
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ'
TARGETS=dict(LIST_TARGETS,**LIFETIME_TARGETS)
TARGETS.update({'?push_back@?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@QAEXABUCPendingQuery@CVsyncCallbackTimer@@@Z':BACK,'?push_front@?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@QAEXABUCPendingQuery@CVsyncCallbackTimer@@@Z':FRONT,'?GFGetTime@@YINXZ':CLOCK,'__imp__Sleep@4':0x143fe1c})
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9f/CVsyncCallbackTimer_OnPostSwapScreens_009f73c0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9f/CVsyncCallbackTimer_OnPostSwapScreens_009f73c0_test.cpp');ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/timer-post-swap.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='timer-post-swap-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out,opt in [(args.source,obj,'/Od'),(args.test,test,'/O2')]:
  r=check_one.cl(['/nologo','/c',opt,'/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec];assert offset==0 and len(built)==603 and len(relocs)==35
 pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,732);score=compare(built,native,relocs,retail_relocations('009f73c0',732));assert score.near_match,score
 candidate=bytearray(built);checked=[];targets=[]
 for site,(name,kind) in sorted(relocs.items()):
  target=TARGETS[name]+int.from_bytes(built[site:site+4],'little');assert kind in [6,20];targets.append(target)
  value=target if kind==6 else target-ENTRY-site-4;struct.pack_into('<I',candidate,site,value&0xffffffff);checked.append(dict(offset=site,symbol=name,target=hex(target)))
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;native_targets=[]
 for ins in decoder.disasm(native,ENTRY):
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_IMM and (ins.mnemonic=='call' or 0x1000000<=op.imm<0x1500000):native_targets.append(op.imm)
   elif op.type==capstone.x86.X86_OP_MEM and op.mem.disp>=0x1000000:native_targets.append(op.mem.disp)
 assert Counter(targets)==Counter(native_targets),(Counter(targets),Counter(native_targets))
 # Use a compiled C++ x87-return stub; the emulator supplies its clock value.
 clock_source=work/'clock.cpp';clock_source.write_text('extern double FableClockValue; double __fastcall TimerClockResult(){return FableClockValue;}')
 clock_obj=work/'clock.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(clock_obj),str(clock_source)],env,work);assert r.returncode==0,r.stdout+r.stderr
 clock=Coff(clock_obj);sec,offset=clock.symbols['?TimerClockResult@@YINXZ'];clock_bytes,clock_relocs=clock.sections[sec];assert offset==0 and len(clock_relocs)==1;clock_code=bytearray(clock_bytes)
 for site,(name,kind) in clock_relocs.items():assert name=='?FableClockValue@@3NA' and kind==6;struct.pack_into('<I',clock_code,site,TIME_VALUE)
 scenarios=list(itertools.product(range(4),range(3),range(4),range(3),range(2),range(2),range(4)))
 for scenario in scenarios:assert execute(candidate,clock_code,*scenario)==execute(native,clock_code,*scenario),scenario
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'TIMER_POST_SWAP_BEHAVIOR_PASS scenarios=2304' in r.stdout,r.stdout+r.stderr
 proof=dict(address='009f73c0',grade='NEAR_MATCH',built_bytes=603,retail_bytes=732,similarity=score.to_dict(),relocations=checked,comparative_scenarios=len(scenarios),behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),limitation='Newly-created query Frame/TimeStamp are uninitialized in retail; only defined payload bytes compared.')
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n');print('TIMER_POST_SWAP_PASS compiled=2304 comparative='+str(len(scenarios)))
if __name__=='__main__':main()
