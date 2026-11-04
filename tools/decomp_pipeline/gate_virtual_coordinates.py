"""Validate the shared shared-ABI virtual-coordinate helpers and its exact native bytes."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(0x9e1d00,'?GFRoundVXToNearestPixel@@YIMN@Z',76),(0x9e1d50,'?GFRoundVYToNearestPixel@@YIMN@Z',77),(0x9e1da0,'?GFAXToVX@@YIMM@Z',40)]
TARGETS={'?GFGetSystemManager@@YIPAUCSystemManager@@XZ':0x9a4ec0,'?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ':0x9bedc0,'?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,'?g_FableVirtualHeight_013961F0@@3NA':0x13961f0,'_floor':0xbfeb8a,'__ftol2':0xbfea70}

# Class spelling is PDB-canonical; old struct spelling supports baseline audits.
TARGETS['?GFGetSystemManager@@YIPAVCSystemManager@@XZ']=0x9a4ec0

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9e/Global_VirtualCoordinateHelpers_009e1d00.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9e/Global_VirtualCoordinateHelpers_009e1d00_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/virtual-coordinates.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='virtual-coordinates-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[]
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   assert (entry+site+4+int.from_bytes(native[site:site+4],'little',signed=True) if kind==20 else int.from_bytes(native[site:site+4],'little'))==target
   struct.pack_into('<I',patched,site,((target-entry-site-4)&0xffffffff) if kind==20 else target);checked.append(dict(offset=site,symbol=name,target=hex(target)))
  assert patched==native;proof.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH',bytes=length,relocations=checked))
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'VIRTUAL_COORDINATES_BEHAVIOR_PASS scenarios=6144' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('VIRTUAL_COORDINATES_PASS relocated_bytes=193 compiled=6144')
if __name__=='__main__':main()
