"""Validate native depth/stencil format selection and compatibility filtering."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from collections import Counter
from pathlib import Path
import pefile,capstone
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from display_depth_format_oracle import *
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?GetClosestDepthBufferFormat@CDisplayManager@@ABE?AVCPixelFormat@@ABU_D3DDISPLAYMODE@@J@Z'
TARGETS={'?GD3DFormats@@3QBVCD3DFormatInfo@@B':FORMATS,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':INIT}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9b/CDisplayManager_GetClosestDepthBufferFormat_009bebb0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9b/CDisplayManager_GetClosestDepthBufferFormat_009bebb0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-depth-format.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-depth-format-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec];assert offset==0 and len(built)==249 and len(relocs)==5
 pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,253);score=compare(built,native,relocs,retail_relocations('009bebb0',253));assert score.near_match,score
 patched=bytearray(built);checked=[]
 for site,(name,kind) in sorted(relocs.items()):
  assert kind in [6,20];target=TARGETS[name]+int.from_bytes(built[site:site+4],'little')
  value=target if kind==6 else target-ENTRY-site-4;struct.pack_into('<I',patched,site,value&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
 decoder=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);decoder.detail=True;targets=[]
 for ins in decoder.disasm(native,ENTRY):
  for op in ins.operands:
   if op.type==capstone.x86.X86_OP_IMM and (ins.mnemonic=='call' or 0x1000000<=op.imm<0x1500000):targets.append(op.imm)
   elif op.type==capstone.x86.X86_OP_MEM and op.mem.disp>=0x1000000:targets.append(op.mem.disp)
 assert Counter(targets)==Counter(c['target'] for c in checked)
 scenarios=list(itertools.product(TARGET_DEPTHS,range(8),range(8),range(2)))
 for scenario in scenarios:assert execute(patched,*scenario)==execute(native,*scenario),scenario
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DISPLAY_DEPTH_FORMAT_BEHAVIOR_PASS scenarios=1536' in r.stdout,r.stdout+r.stderr
 result=dict(address='009bebb0',grade='NEAR_MATCH',built_bytes=249,retail_bytes=253,similarity=score.to_dict(),relocations=checked,behavior=r.stdout.strip(),comparative_scenarios=1536,source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),limitation='Retail indexes GD3DFormats[-1] if no compatible format exists. Tests cover supported selections, without inventing a fallback for that undefined case.')
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('DISPLAY_DEPTH_FORMAT_PASS compiled=1536 comparative=1536')
if __name__=='__main__':main()
