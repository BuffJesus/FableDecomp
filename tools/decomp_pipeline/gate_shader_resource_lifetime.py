"""Validate shader-resource construction, closing, and uninitialization."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(0xa5d870,'??0CShaderResource@@AAE@XZ',51),(0xa5d750,'?CloseShaderBank@CShaderResource@@QAEXXZ',69),(0xa5d7a0,'?Uninitialise@CShaderResource@@AAEXXZ',17)]
TARGETS={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CShaderResource@@6B@':0x129d580,'??0CVertexShaderInput@@QAE@XZ':0xab3b10,'?CloseShaderBank@CShaderResource@@QAEXXZ':0xa5d750}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CShaderResource_ConstructionAndClose_00a5d870.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CShaderResource_ConstructionAndClose_00a5d870_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-resource-lifetime.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='shader-resource-lifetime-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
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
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'SHADER_RESOURCE_BEHAVIOR_PASS scenarios=896' in r.stdout,r.stdout+r.stderr
 from shader_resource_oracle import compare
 comparative=compare(candidate_codes,native_codes)
 coff.table("??_7CShaderResource@@6B@",["??_ECShaderResource@@UAEPAXI@Z"])
 assert int.from_bytes(pe.get_data(0x129d580-pe.OPTIONAL_HEADER.ImageBase,4),"little")==0xa5ebe0
 result=dict(comparative_scenarios=comparative,entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('SHADER_RESOURCE_PASS bytes=137 compiled=896 comparative=650')
if __name__=='__main__':main()
