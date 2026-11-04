"""Validate native display-mode selection and windowed adapter/device queries."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from collections import Counter
from pathlib import Path
import pefile,capstone
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from display_closest_mode_oracle import *
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?GetClosestMode@CDisplayManager@@ABE?AU_D3DDISPLAYMODE@@ABVC2DExtentsI@@J_N@Z'
TARGETS={'?GD3DFormats@@3QBVCD3DFormatInfo@@B':FORMATS,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':INIT,'?GetColourDepth@CPixelFormat@@QBEKXZ':DEPTH}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9b/CDisplayManager_GetClosestMode_009bf600.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9b/CDisplayManager_GetClosestMode_009bf600_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-closest-mode.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-closest-mode-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec];assert offset==0 and len(built)==463 and len(relocs)==5
 pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,469);score=compare(built,native,relocs,retail_relocations('009bf600',469));assert score.near_match,score
 patched=bytearray(built);checked=[]
 for site,(name,kind) in sorted(relocs.items()):
  assert built[site:site+4]==b'\0'*4 and kind in [6,20];target=TARGETS[name]
  value=target if kind==6 else target-ENTRY-site-4;struct.pack_into('<I',patched,site,value&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;targets=[]
 for ins in decoder.disasm(native,ENTRY):
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_IMM and ins.mnemonic=='call':targets.append(op.imm)
   elif op.type==capstone.x86.X86_OP_MEM and op.mem.disp>=0x1000000:targets.append(op.mem.disp)
 assert Counter(targets)==Counter(c['target'] for c in checked)
 scenarios=list(itertools.product(range(6),range(3),range(3),range(2),range(2),range(2)))
 for scenario in scenarios:assert execute(patched,*scenario)==execute(native,*scenario),scenario
 for device,request,mutation,failure in itertools.product(range(2),range(3),range(2),range(2)):
  scenario=(3,request,1,0,mutation,failure,False,bool(device));assert execute(patched,*scenario)==execute(native,*scenario),scenario
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DISPLAY_CLOSEST_MODE_BEHAVIOR_PASS scenarios=456' in r.stdout,r.stdout+r.stderr
 result=dict(address='009bf600',grade='NEAR_MATCH',built_bytes=463,retail_bytes=469,similarity=score.to_dict(),relocations=checked,behavior=r.stdout.strip(),comparative_scenarios=456,source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),limitation='No enumerated mode leaves Width/Height/RefreshRate undefined in retail; only Format=D3DFMT_UNKNOWN is verified in that case.')
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('DISPLAY_CLOSEST_MODE_PASS compiled=456 comparative=456')
if __name__=='__main__':main()
