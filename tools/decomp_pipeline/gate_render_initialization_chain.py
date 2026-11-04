"""Compare the composed native render-state initialization chain."""
import argparse,itertools,json,struct,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from render_initialization_chain_oracle import execute
ROOT=Path(__file__).resolve().parents[2]
SPECS=[('CRenderStateManager_Init_00a07900',0xa07900,115,'?Init@CRenderStateManager@@QAEXPAUIDirect3DDevice9@@@Z'),('CRenderStateManager_StateSetup_00a04580',0xa04630,4609,'?SetupRenderStates@CRenderStateManager@@AAEXXZ'),('CRenderStateManager_SetDefaultRenderStates_00a059e0',0xa059e0,7958,'?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ'),('CRenderStateManager_ForceResendRenderStatesToHardware_00a044e0',0xa044e0,151,'?ForceResendRenderStatesToHardware@CRenderStateManager@@QAEXXZ')]
TARGETS={'?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A':0x13cb508,'?FableRenderWrapStateTypes_01396da8@@3PAW4_D3DRENDERSTATETYPE@@A':0x1396da8,'?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA':0x1396e64}
TARGETS.update({symbol:a for _,a,_,symbol in SPECS})
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-initialization-chain.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='render-initialization-chain-'));pe=pefile.PE(str(EXE));candidate={};retail={}
 for stem,a,n,symbol in SPECS:
  obj=work/(stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/Oy','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/a0'/(stem+'.cpp'))],env,work);assert r.returncode==0,r.stdout+r.stderr
  coff=Coff(obj);sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0;code=bytearray(built)
  for site,(name,kind) in relocs.items():
   target=TARGETS[name] if name in TARGETS else int(name.split('_')[-1].split('@')[0],16)
   addend=int.from_bytes(built[site:site+4],'little');value=target+addend if kind==6 else target+addend-a-site-4;assert kind in [6,20];struct.pack_into('<I',code,site,value&0xffffffff)
  candidate[a]=code;retail[a]=pe.get_data(a-pe.OPTIONAL_HEADER.ImageBase,n)
 data=pe.get_data(0x1396000-pe.OPTIONAL_HEADER.ImageBase,4096);scenarios=list(itertools.product(range(8),range(4),[0,3,231],range(4),range(2)))
 for scenario in scenarios:assert execute(candidate,data,*scenario)==execute(retail,data,*scenario),scenario
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(dict(functions=4,comparative_scenarios=len(scenarios),boundary='D3D GetDeviceCaps and state setters only'),indent=2)+'\n');print('RENDER_INITIALIZATION_CHAIN_PASS functions=4 comparative='+str(len(scenarios)))
if __name__=='__main__':main()
