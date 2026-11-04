"""Validate presentation-interval selection and back-buffer copy texture setup."""
import argparse,hashlib,json,struct,subprocess,tempfile,itertools
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from near_match import compare,retail_relocations
from display_reset_helpers_oracle import execute,ENTRY,PIXEL,TEXTURE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(0x9be2d0,'?CalculateD3DPresentationInterval@CDisplayManager@@AAEIABVCPresentationIntervalDesc@1@@Z',84),(0x9bee40,'?SetupDepthAndBackBufferTextures@CDisplayManager@@AAE_NXZ',101)]
TARGETS={'?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ':PIXEL,'?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z':TEXTURE}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9b/CDisplayManager_ResetHelpers_009be2d0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9b/CDisplayManager_ResetHelpers_009be2d0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-reset-helpers.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-reset-helpers-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[]
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert built[site:site+4]==b'\0'*4
   if kind==6:
    target_sec,target_offset=coff.symbols[name];assert target_sec==sec
    target=entry+target_offset
   else:
    assert kind==20;target=TARGETS[name]
   value=target if kind==6 else target-entry-site-4
   struct.pack_into('<I',patched,site,value&0xffffffff);checked.append(dict(offset=site,symbol=name,target=hex(target)))
  if entry==0x9be2d0:
   # The complete body includes a five-entry absolute jump table at +64.
   assert patched==native and len(relocs)==6
   assert [int.from_bytes(patched[i:i+4],'little')-entry for i in range(64,84,4)]==[24,24,32,40,48]
   proof.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH',bytes=length,code_bytes=64,table_bytes=20,relocations=checked))
  else:
   score=compare(built,native,relocs,retail_relocations('%08x'%entry,length));assert score.near_match,score
   assert [c['target'] for c in checked]==[hex(PIXEL),hex(TEXTURE)]
   assert ENTRY+15+4+int.from_bytes(native[15:19],'little',signed=True)==PIXEL
   assert ENTRY+87+4+int.from_bytes(native[87:91],'little',signed=True)==TEXTURE
   scenarios=list(itertools.product([0,1,2,3,0x80000000,0x80000002,0xfffffffd,0xffffffff],[-1,0,640,1920],[-1,0,640,1920],range(2),range(4)))
   for scenario in scenarios:assert execute(patched,*scenario)==execute(native,*scenario),scenario
   proof.append(dict(address='%08x'%entry,grade='NEAR_MATCH',bytes=length,similarity=score.to_dict(),comparative_scenarios=len(scenarios),relocations=checked))
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DISPLAY_RESET_HELPERS_BEHAVIOR_PASS intervals=2056 textures=1024' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('DISPLAY_RESET_HELPERS_PASS interval_cases=2056 texture_cases=1024 comparative=1024')
if __name__=='__main__':main()
