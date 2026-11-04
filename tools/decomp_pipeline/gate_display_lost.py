"""Validate lost-device retries and the native creation-parameter copy constructor."""
import argparse,hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from display_lost_oracle import *
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?ProcessLostDevice@CDisplayManager@@QAE_NXZ'
COPY_SYMBOL='??0CCreateDeviceParameters@CDisplayManager@@QAE@ABU01@@Z'
TARGETS={COPY_SYMBOL:COPY,'?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z':RESET}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9c/CDisplayManager_ProcessLostDevice_009c00c0.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9c/CDisplayManager_ProcessLostDevice_009c00c0_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/display-lost.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='display-lost-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));sec,offset=coff.symbols[SYMBOL];built,relocs=coff.sections[sec];assert offset==0 and len(built)==211 and len(relocs)==4
 native=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,207);score=compare(built,native,relocs,retail_relocations('009c00c0',207));assert score.near_match,score
 sec,offset=coff.symbols[COPY_SYMBOL];copy,copy_relocs=coff.sections[sec];native_copy=pe.get_data(COPY-pe.OPTIONAL_HEADER.ImageBase,73);assert offset==0 and not copy_relocs and copy==native_copy
 patched=bytearray(built);checked=[]
 for site,(name,kind) in sorted(relocs.items()):
  assert kind==20 and built[site:site+4]==b'\0'*4;target=TARGETS[name];struct.pack_into('<I',patched,site,(target-ENTRY-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=hex(target)))
 assert [c['target'] for c in checked]==[hex(RESET),hex(COPY),hex(RESET),hex(RESET)]
 assert [ENTRY+i+4+int.from_bytes(native[i:i+4],'little',signed=True) for i in [59,95,123,179]]==[RESET,COPY,RESET,RESET]
 scenarios=[]
 for (width,height),sample,a,b,c,mutation in itertools.product([(640,480),(1920,1080),(-1,3),(1025,767),(1024,768)],[0,2],[0,1,2,7],[0,1,2,7],[0,1,2,7],range(3)):
  scenarios.append((width,height,sample,0x88760869,(a,b,c),mutation))
 for hr,sample in itertools.product([0,1,0x88760868,0x80004005],range(4)):scenarios.append((1920,1080,sample*2,hr,(7,7,7),0))
 for scenario in scenarios:assert execute(patched,copy,*scenario)==execute(native,native_copy,*scenario),scenario
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'DISPLAY_LOST_BEHAVIOR_PASS scenarios=1936' in r.stdout,r.stdout+r.stderr
 proof=dict(entries=[dict(address='009c00c0',grade='NEAR_MATCH',built_bytes=211,retail_bytes=207,similarity=score.to_dict(),relocations=checked),dict(address='009c11a0',grade='MATCH',bytes=73)],comparative_scenarios=len(scenarios),behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(proof,indent=2)+'\n');print('DISPLAY_LOST_PASS compiled=1936 comparative=1936 copy_constructor=MATCH')
if __name__=='__main__':main()
