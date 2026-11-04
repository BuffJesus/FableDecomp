"""Validate shader-resource destruction and its linked owned-resource chain."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
RANGE='??$__destroy_aux@PAVCStreamInfo@CVertexShaderInput@@@_STL@@YIXPAVCStreamInfo@CVertexShaderInput@@0ABU__false_type@0@@Z'
SPECS=[(0xa5d800,'??1CShaderResource@@UAE@XZ',108),(0xa5ebe0,'??_GCShaderResource@@UAEPAXI@Z',30)]
LINKED=[(0xa5d750,'?CloseShaderBank@CShaderResource@@QAEXXZ',69),(0xa5e900,RANGE,55)]
TARGETS={'_free':0xbfea14,'??1CBaseClass@@UAE@XZ':0x99a2e0,RANGE:0xa5e900,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??_7CShaderResource@@6B@':0x129d580,'?CloseShaderBank@CShaderResource@@QAEXXZ':0xa5d750,'??1CShaderResource@@UAE@XZ':0xa5d800,'??3@YAXPAX@Z':0xbfe9bc}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CShaderResource_ConstructionAndClose_00a5d870.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CShaderResource_Destructor_00a5d800_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-resource-destructor.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='shader-resource-destructor-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 fixture_coff=Coff(test)
 for _,symbol,_ in SPECS:assert symbol not in fixture_coff.symbols,'fixture must call the candidate body'
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[];native_codes={};candidate_codes={}
 for entry,symbol,length in SPECS+LINKED:
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
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'SHADER_RESOURCE_DESTRUCTOR_BEHAVIOR_PASS scenarios=4096' in r.stdout,r.stdout+r.stderr
 from shader_resource_destructor_oracle import compare
 comparative=compare(candidate_codes,native_codes)
 result=dict(comparative_scenarios=comparative,entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('SHADER_RESOURCE_DESTRUCTOR_PASS bytes=138 compiled=4096 comparative=1152')
if __name__=='__main__':main()
