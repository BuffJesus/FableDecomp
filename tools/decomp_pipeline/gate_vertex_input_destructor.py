"""Validate implicit vertex-input and stream-range destruction."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
RANGE='??$__destroy_aux@PAVCStreamInfo@CVertexShaderInput@@@_STL@@YIXPAVCStreamInfo@CVertexShaderInput@@0ABU__false_type@0@@Z'
SPECS=[(0xa5eb70,'??1CVertexShaderInput@@UAE@XZ',62),(0xa5e900,RANGE,55),(0xab4690,'??_GCVertexShaderInput@@UAEPAXI@Z',30)]
TARGETS={'_free':0xbfea14,'??1CBaseClass@@UAE@XZ':0x99a2e0,RANGE:0xa5e900,'??1CVertexShaderInput@@UAE@XZ':0xa5eb70,'??3@YAXPAX@Z':0xbfe9bc}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/ab/CVertexShaderInput_Constructor_00ab3b10.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CVertexShaderInput_Destructor_00a5eb70_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/vertex-input-destructor.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='vertex-input-destructor-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 fixture_coff=Coff(test)
 for _,symbol,_ in SPECS:assert symbol not in fixture_coff.symbols,'fixture must call the candidate body'
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[];native_codes={};candidate_codes={}
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   assert (entry+site+4+int.from_bytes(native[site:site+4],'little',signed=True) if kind==20 else int.from_bytes(native[site:site+4],'little'))==target
   struct.pack_into('<I',patched,site,((target-entry-site-4) if kind==20 else target)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=hex(target)))
  assert patched==native;native_codes[entry]=native;candidate_codes[entry]=patched;proof.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH' if relocs else 'MATCH',bytes=length,relocations=checked))
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'VERTEX_INPUT_DESTRUCTOR_BEHAVIOR_PASS scenarios=2400' in r.stdout,r.stdout+r.stderr
 from vertex_input_destructor_oracle import compare
 comparative=compare(candidate_codes,native_codes)
 result=dict(comparative_scenarios=comparative,entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('VERTEX_INPUT_DESTRUCTOR_PASS bytes=147 compiled=2400 comparative=672')
if __name__=='__main__':main()
