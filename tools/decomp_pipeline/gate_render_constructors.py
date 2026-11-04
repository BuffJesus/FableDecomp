"""Validate native render-state records and owning core construction/destruction."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from render_constructors_oracle import *
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(STATE,'??0CRenderStateManager@@QAE@K@Z',108),
       (DEVICE,'??0CDeviceState@CRenderStateManager@@QAE@XZ',4950),
       (CORE,'??0CRenderManagerCore@@QAE@AAVCDisplayManager@@K@Z',116),
       (DESTROY,'??1CRenderManagerCore@@UAE@XZ',50)]
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a0/CRenderManager_Constructors_00a05970.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a0/CRenderManager_Constructors_00a05970_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-constructors.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='render-constructors-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[];codes={};natives={}
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length,(hex(entry),len(built))
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   value=target if kind==6 else (target-entry-site-4)&0xffffffff;assert int.from_bytes(native[site:site+4],'little')==value
   struct.pack_into('<I',patched,site,value);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
  assert patched==native,hex(entry);codes[entry]=patched;natives[entry]=native
  proof.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH' if relocs else 'MATCH',bytes=length,relocations=checked))
 state_destructor=pe.get_data(STATE_DESTROY-pe.OPTIONAL_HEADER.ImageBase,21);cases=0
 for entry,fill,flags,mutation,valid in itertools.product([STATE,DEVICE,CORE,DESTROY],range(0,256,17),[0,1,0x80000000,0xffffffff],range(3),range(2)):
  case=(entry,fill,flags,mutation,valid)
  assert execute(codes,state_destructor,*case)==execute(natives,state_destructor,*case),case;cases+=1
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'RENDER_CONSTRUCTORS_BEHAVIOR_PASS scenarios=10752' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,behavior=r.stdout.strip(),comparative_scenarios=cases,source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('RENDER_CONSTRUCTORS_PASS compiled=10752 comparative='+str(cases))
if __name__=='__main__':main()
