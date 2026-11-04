"""Validate the shader-base default debug name and string construction."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
ENTRY=0x987cf0;SYMBOL='?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ';LENGTH=25
LITERAL='??_C@_09ONJBBFFA@?$CIUnknown?$CJ?$AA@'
TARGETS={LITERAL:0x129a03c,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/98/CShaderBase_GetDebugName_00987cf0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/98/CShaderBase_GetDebugName_00987cf0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/shader-debug-name.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='shader-debug-name-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec];assert offset==0 and len(built)==LENGTH
 pe=pefile.PE(str(EXE));native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,LENGTH);patched=bytearray(built);checked=[]
 for site,(name,kind) in sorted(relocs.items()):
  assert kind in (6,20) and built[site:site+4]==b'\0'*4;target=TARGETS[name]
  value=target if kind==6 else (target-ENTRY-site-4)&0xffffffff;assert int.from_bytes(native[site:site+4],'little')==value
  struct.pack_into('<I',patched,site,value);checked.append(dict(offset=site,symbol=name,target=target,kind=kind))
 assert patched==native
 sec,off=coff.symbols[LITERAL];literal=coff.sections[sec][0][off:off+10];assert literal==b'(Unknown)\0' and pe.get_data(0x129a03c-pe.OPTIONAL_HEADER.ImageBase,10)==literal
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'SHADER_DEBUG_NAME_BEHAVIOR_PASS scenarios=256' in r.stdout,r.stdout+r.stderr
 result=dict(address='%08x'%ENTRY,grade='RELOCATION_MATCH',bytes=LENGTH,relocations=checked,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('SHADER_DEBUG_NAME_PASS compiled=256 relocated_bytes=25')
if __name__=='__main__':main()
