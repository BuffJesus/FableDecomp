"""Validate timer lifetime bodies, allocation and member destruction."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from timer_constructor_oracle import execute
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('CVsyncCallbackTimer_Constructor_009f70e0',0x9f70e0,354,342,'??0CVsyncCallbackTimer@@QAE@XZ'),('CVsyncCallbackTimer_Destructor_009f7330',0x9f7330,107,107,'??1CVsyncCallbackTimer@@QAE@XZ')]
TARGETS={'_malloc':0xbfea0e,'_free':0xbfea14,'?ClearAllQueries@CVsyncCallbackTimer@@AAEXXZ':0x9f7250,'?clear@?$list@UCPendingQuery@CVsyncCallbackTimer@@V?$allocator@UCPendingQuery@CVsyncCallbackTimer@@@_STL@@@_STL@@QAEXXZ':0x9f7830,'?CPUFrameCount@CVsyncCallbackTimer@@0JA':0x13cb4f0,'?LatestBlockIndex@CVsyncCallbackTimer@@0JC':0x13cb4f4,'__real@0000000000000000':0x122ed70,'?GPUBlocks@CVsyncCallbackTimer@@0RCUCGPUInfoBlock@1@C':0x13cb4b0,'?CPUTimeStamp@CVsyncCallbackTimer@@0NA':0x13cb4e0,'?CPUPrevTimeStamp@CVsyncCallbackTimer@@0NA':0x13cb4e8}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/timer-lifetime.json');args=ap.parse_args()
 work=Path(tempfile.mkdtemp(prefix='timer-lifetime-'));env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];pe=pefile.PE(str(EXE));objects=[];rows=[]
 for stem,address,length,built_length,symbol in SPECS:
  source=args.stage/(stem+'.cpp') if args.stage else ROOT/'rebuild/src/compiled/00/9f'/(stem+'.cpp');obj=work/(stem+'.obj');objects.append(obj)
  r=check_one.cl(['/nologo','/c','/Od','/Ob1','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
  coff=Coff(obj);sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==built_length
  native=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length);score=compare(built,native,relocs,retail_relocations(f'{address:08x}',length));assert score.near_match,score
  candidate=bytearray(built);checks=[]
  for site,(name,kind) in sorted(relocs.items()):
   target=TARGETS[name];addend=int.from_bytes(built[site:site+4],'little');assert kind in [6,20]
   value=target+addend if kind==6 else target-address-site-4+addend
   struct.pack_into('<I',candidate,site,value&0xffffffff);checks.append(dict(offset=site,symbol=name,target=hex(target),addend=addend))
  if address==0x9f7330:assert candidate==native
  else:
   # Native relocations retain the same ordered target/addend sequence.
   sites=[46,139,197,207,217,223,229,235,241,247,253,259,265,271,277,283,289,299,309,319]
   for site,check in zip(sites,checks):
    value=int.from_bytes(native[site:site+4],'little');target=int(check['target'],16)+check['addend']
    assert value==(target if check['symbol'] not in ['_malloc'] else (target-address-site-4)&0xffffffff)
   for scenario in itertools.product(range(0,256,16),range(4)):assert execute(candidate,*scenario)==execute(native,*scenario),scenario
  rows.append(dict(address=f'{address:08x}',grade='RELOCATION_MATCH' if candidate==native else 'NEAR_MATCH',source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),similarity=score.to_dict(),relocations=checks))
 test=args.stage/'lifetime_test.cpp' if args.stage else ROOT/'rebuild/include/test_support/timer_lifetime_test.h';test_obj=work/'test.obj'
 r=check_one.cl(['/nologo','/c','/TP','/O2','/MT','/Fo'+str(test_obj),str(test.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(cmd+[str(p) for p in objects]+[str(test_obj)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'TIMER_LIFETIME_BEHAVIOR_PASS scenarios=1024' in r.stdout,r.stdout+r.stderr
 proof=dict(functions=rows,comparative_scenarios=64,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(test.read_bytes()).hexdigest());args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n');print('TIMER_LIFETIME_PASS compiled=1024 comparative=64')
if __name__=='__main__':main()
