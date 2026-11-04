"""Validate the shared busy-destructor and list destruction and its exact native bytes."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
LIST_DTOR='??1?$list@UCPendingQuery@CBusyResourceManager@@V?$allocator@UCPendingQuery@CBusyResourceManager@@@_STL@@@_STL@@QAE@XZ'
SPECS=[(0xa61b10,'??1CBusyResourceManager@@QAE@XZ',24),(0xa61c40,'?clear@?$list@UCPendingQuery@CBusyResourceManager@@V?$allocator@UCPendingQuery@CBusyResourceManager@@@_STL@@@_STL@@QAEXXZ',47),(0xa61d20,LIST_DTOR,62)]
TARGETS={'?ClearAllQueries@CBusyResourceManager@@QAEXXZ':0xa61a80,LIST_DTOR:0xa61d20,'_free':0xbfea14}
from busy_destructor_oracle import execute

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a6/CBusyResourceManager_Destructor_00a61b10.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a6/CBusyResourceManager_Destructor_00a61b10_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/busy-destructor.json');args=ap.parse_args()
 env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='busy-destructor-'));obj,test=work/'candidate.obj',work/'test.obj'
 for source,out in [(args.source,obj),(args.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,work);assert r.returncode==0,r.stdout+r.stderr
 coff=Coff(obj);pe=pefile.PE(str(EXE));proof=[];native_codes={};candidate_codes={}
 for entry,symbol,length in SPECS:
  sec,offset=coff.symbols[symbol];built,relocs=coff.sections[sec];assert offset==0 and len(built)==length
  native=pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(built);checked=[]
  for site,(name,kind) in sorted(relocs.items()):
   assert kind==20 and built[site:site+4]==b'\0'*4;target=TARGETS[name]
   assert entry+site+4+int.from_bytes(native[site:site+4],'little',signed=True)==target
   struct.pack_into('<I',patched,site,(target-entry-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=hex(target)))
  assert patched==native;native_codes[entry]=native;candidate_codes[entry]=patched;proof.append(dict(address='%08x'%entry,grade='RELOCATION_MATCH',bytes=length,relocations=checked))
 for entry in [0xa61b10,0xa61c40,0xa61d20]:
  for first in range(16):
   for second in range(16):
    for mutation in range(2):assert execute(candidate_codes,entry,first,second,mutation)==execute(native_codes,entry,first,second,mutation),(entry,first,second,mutation)
 exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode!=0,'candidate required'
 r=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'BUSY_DESTRUCTOR_BEHAVIOR_PASS scenarios=1024' in r.stdout,r.stdout+r.stderr
 result=dict(entries=proof,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(result,indent=2)+'\n');print('BUSY_DESTRUCTOR_PASS relocated_bytes=133 compiled=1024 comparative=1536')
if __name__=='__main__':main()
