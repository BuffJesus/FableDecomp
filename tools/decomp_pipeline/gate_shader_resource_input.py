"""Validate shader-resource vertex-input assignment and declaration binding."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from shader_resource_input_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?SetVertexShaderInput@CShaderResource@@QAEXABVCVertexShaderInput@@@Z'
TARGETS={'??4?$vector@VCStreamInfo@CVertexShaderInput@@V?$allocator@VCStreamInfo@CVertexShaderInput@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z':0xa5e9c0,'??4?$vector@U_D3DVERTEXELEMENT9@@V?$allocator@U_D3DVERTEXELEMENT9@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z':0xa5e2e0}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CShaderResource_SetVertexShaderInput_00a5d8b0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CShaderResource_SetVertexShaderInput_00a5d8b0_test.cpp');ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-resource-input.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='shader-resource-input-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert r.returncode==0,r.stdout+r.stderr
 c=Coff(obj);f=Coff(test);assert SYMBOL not in f.symbols;sec,offset=c.symbols[SYMBOL];assert offset==0
 built,relocs=c.sections[sec];patched=bytearray(built)
 with pefile.PE(str(EXE)) as pe:native=pe.get_data(0xa5d8b0-pe.OPTIONAL_HEADER.ImageBase,66)
 assert len(relocs)==2
 checked=[]
 for site,(symbol,kind) in sorted(relocs.items()):
  assert kind==20 and built[site:site+4]==b'\0'*4 and native[site-1]==0xe8
  target=TARGETS[symbol];assert 0xa5d8b0+site+4+int.from_bytes(native[site:site+4],'little',signed=True)==target
  struct.pack_into('<I',patched,site,(target-0xa5d8b0-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=symbol,target=target))
 score=near_match.compare(built,native,list(relocs),[]);assert score.near_match
 comparative=compare(patched,native)
 exe=w/'fixture.exe';command=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(command+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0
 r=subprocess.run(command+[str(obj),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'SHADER_RESOURCE_INPUT_BEHAVIOR_PASS scenarios=288' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(behavior=r.stdout.strip(),comparative_scenarios=comparative,similarity=score.to_dict(),relocations=checked,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('SHADER_RESOURCE_INPUT_PASS compiled=288 comparative='+str(comparative))
if __name__=='__main__':main()
