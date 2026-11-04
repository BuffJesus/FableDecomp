"""Validate the shared busy-post-swap post-swap queue processing and its exact native bytes."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(0xa618d0,'?OnPostSwapScreens@CBusyResourceManager@@QAEXXZ',421)]
TARGETS={'_malloc':0xbfea0e,'_free':0xbfea14}
from near_match import compare
from busy_post_swap_oracle import execute
import itertools,capstone

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a6/CBusyResourceManager_OnPostSwapScreens_00a618d0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a6/CBusyResourceManager_OnPostSwapScreens_00a618d0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/busy-post-swap.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='busy-post-swap-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[]
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind==20 and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   # Native operands are checked independently below; offsets may differ.
   struct.pack_into('<I',patched,site,(target-entry-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=hex(target)))
  score=compare(built,native,relocs);assert score.near_match,score
  calls=[int(i.op_str,16) for i in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(native,entry) if i.mnemonic=='call' and i.op_str.startswith('0x')]
  assert calls==[TARGETS[c['symbol']] for c in checked]
  cases=list(itertools.product([0,1,8,9,10,16],range(3),range(4),[0,1,0x80004005],[0,0x80004005],range(8)))
  for case in cases:assert execute(patched,*case)==execute(native,*case),case
  proof.append(dict(address='%08x'%entry,grade='NEAR_MATCH',bytes=length,similarity=score.to_dict(),relocations=checked,comparative_scenarios=len(cases)))
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'BUSY_POST_SWAP_BEHAVIOR_PASS scenarios=3456' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('BUSY_POST_SWAP_PASS compiled=3456 comparative=3456')
if __name__=='__main__':main()
