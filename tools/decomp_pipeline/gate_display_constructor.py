"""Validate the shared display-constructor post-swap queue processing and its exact native bytes."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
SPECS=[(0x9c0880,'??0CDisplayManager@@QAE@AAVCSystemManager@@@Z',1475)]
TARGETS={'??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,'??_7CDisplayManager@@6B@':0x129b3f8,'??_7CSurface@@6B@':0x122f84c,'??0CWideString@@QAE@XZ':0x99aed0,'_malloc':0xbfea0e,'??0CBusyResourceManager@@QAE@XZ':0xa61880,'?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830}
from near_match import compare
from display_constructor_oracle import execute
import itertools,capstone

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9c/CDisplayManager_Constructor_009c0880.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9c/CDisplayManager_Constructor_009c0880_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-constructor.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-constructor-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[]
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==1478
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind in (6,20) and built[site:site+4]==b'\0'*4
   if name.startswith('?_M_insert_overflow@?$vector@VCPixelFormat'):target=0x9c2e30
   elif name.startswith('?_M_insert_overflow@?$vector@W4ESurfaceMultisampleType'):target=0x9c2ca0
   else:target=TARGETS[name]
   # Native operands are checked independently below; offsets may differ.
   struct.pack_into('<I',patched,site,((target-entry-site-4)&0xffffffff) if kind==20 else target);checked.append(dict(offset=site,symbol=name,target=hex(target),kind=kind))
  score=compare(built,native,relocs);assert score.near_match,score
  calls=[int(i.op_str,16) for i in capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32).disasm(native,entry) if i.mnemonic=='call' and i.op_str.startswith('0x')]
  assert calls==[int(c['target'],16) for c in checked if c['kind']==20]
  for target in [0x129b3f8,0x122f84c]:assert struct.pack('<I',target) in native
  cases=list(itertools.product(range(0,256,17),range(3),range(4)))
  busy=pe.get_data(0xa61880-pe.OPTIONAL_HEADER.ImageBase,66)
  for case in cases:assert execute(patched,busy,*case)==execute(native,busy,*case),case
  proof.append(dict(address='%08x'%entry,grade='NEAR_MATCH',built_bytes=len(built),retail_bytes=length,similarity=score.to_dict(),relocations=checked,comparative_scenarios=len(cases)))
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DISPLAY_CONSTRUCTOR_BEHAVIOR_PASS scenarios=3072' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('DISPLAY_CONSTRUCTOR_PASS compiled=3072 comparative=192')
if __name__=='__main__':main()
