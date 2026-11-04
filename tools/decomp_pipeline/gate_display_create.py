"""Validate native device creation, capability flags, timer attachment and viewport."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from collections import Counter
from pathlib import Path
import pefile,capstone
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from display_create_oracle import *
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?CreateDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z'
TARGETS={
 '?PreDeviceReset@CDisplayManager@@AAEXXZ':PRE,
 '?Uninitialise@CSurface@@QAEXXZ':SURFACE_CLEAR,
 '?Uninitialise@CTexture@@QAEXXZ':TEXTURE_CLEAR,
 '?GetClosestMode@CDisplayManager@@ABE?AU_D3DDISPLAYMODE@@ABVC2DExtentsI@@J_N@Z':CLOSEST,
 '?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':PIXEL_INIT,
 '?GD3DFormats@@3QBVCD3DFormatInfo@@B':FORMATS,
 '?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z':DEPTH_FORMAT,
 '?CalculateD3DPresentationInterval@CDisplayManager@@AAEIABVCPresentationIntervalDesc@1@@Z':INTERVAL,
 '__TI2?AVCGenericException@@':THROW_INFO,
 '__CxxThrowException@8':THROW,
 '?InitialiseFromPreallocatedSurface@CSurface@@QAEXPAUIDirect3DSurface9@@_N@Z':SURFACE_INIT,
 '?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ':DIMENSIONS,
 '?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ':PIXEL,
 '??4CSurface@@QAEAAV0@ABV0@@Z':ASSIGN,
 '?SetupDepthAndBackBufferTextures@CDisplayManager@@AAE_NXZ':SETUP,
 '?SetIntegerViewportEndpoint@CDisplayManager@@QAEXABVC2DBoxI@@@Z':VIEWPORT}

from gate_display_reset import exception_metadata

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9b/CDisplayManager_CreateDevice_009bf7e0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9b/CDisplayManager_CreateDevice_009bf7e0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-create.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-create-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out,extra in [(args.source,obj,[]),(args.test,test,['/EHsc'])]:
  r=check_one.cl(['/nologo','/c','/O2','/MT']+extra+['/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec];assert offset==0 and len(built)==835 and len(relocs)==17
 pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,846);score=compare(built,native,relocs,retail_relocations('009bf7e0',846));assert score.near_match,score
 metadata=exception_metadata(coff,pe);patched=bytearray(built);checked=[]
 for site,(name,kind) in sorted(relocs.items()):
  assert built[site:site+4]==b'\0'*4 and kind in [6,20];target=TARGETS[name]
  value=target if kind==6 else target-ENTRY-site-4;struct.pack_into('<I',patched,site,value&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;targets=[]
 for ins in decoder.disasm(native,ENTRY):
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_IMM and (ins.mnemonic=='call' or 0x1000000<=op.imm<0x1500000):targets.append(op.imm)
   elif op.type==capstone.x86.X86_OP_MEM and op.mem.disp>=0x1000000:targets.append(op.mem.disp)
 native_targets=Counter(targets);candidate_targets=Counter(c['target'] for c in checked)
 # Retail keeps one interval call in each exclusive/windowed branch; VC7.1
 # merges them here. Every executed path still calls it exactly once.
 assert native_targets[INTERVAL]==2 and candidate_targets[INTERVAL]==1
 native_targets[INTERVAL]-=1
 assert native_targets==candidate_targets
 scenarios=list(itertools.product(range(2),range(2),range(3),range(3),range(5),range(4),range(2),range(2)))
 for scenario in scenarios:assert execute(patched,*scenario)==execute(native,*scenario),scenario
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DISPLAY_CREATE_BEHAVIOR_PASS scenarios=2880' in r.stdout,r.stdout+r.stderr
 result=dict(address='009bf7e0',grade='NEAR_MATCH',built_bytes=835,retail_bytes=846,similarity=score.to_dict(),relocations=checked,exception_types=metadata,behavior=r.stdout.strip(),comparative_scenarios=len(scenarios),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('DISPLAY_CREATE_PASS compiled=2880 comparative=2880')
if __name__=='__main__':main()
