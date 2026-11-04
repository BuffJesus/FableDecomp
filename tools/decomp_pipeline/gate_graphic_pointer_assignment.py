"""Validate graphics-bank counted-pointer release, share and assignment."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from graphic_pointer_assignment_oracle import compare
ROOT=check_one.ROOT
SOURCE='CCountedPointerCGraphicDataBank_Assign_004194ba.cpp'
SPECS=[dict(address='00419108',prefix='?ReleaseData@',size=44),dict(address='00419134',prefix='?ShareData@',size=38),dict(address='004194ba',prefix='??4?$CCountedPointer@',size=23)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/41'/SOURCE);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/GraphicPointerAssignment_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/graphic-pointer-assignment.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='graphic-pointer-assignment-'));env=check_one.env();src=w/'source.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(src),str(a.source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(src)
 candidate={};native={};records=[];targets={'??3@YAXPAX@Z':0xbfe9bc}
 for s in SPECS:
  matches=[n for n in c.symbols if n.startswith(s['prefix'])];assert len(matches)==1,matches;s['symbol']=matches[0];targets[matches[0]]=int(s['address'],16)
 with pefile.PE(str(EXE)) as pe:
  for s in SPECS:
   addr=int(s['address'],16);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and len(built)==s['size'];patched=bytearray(built)
   for site,(name,kind) in rel.items():
    assert kind==20 and built[site:site+4]==bytes(4)
    struct.pack_into('<I',patched,site,(targets[name]-addr-site-4)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,s['size']);assert patched==raw,s
   candidate[addr]=patched;native[addr]=raw;records.append(dict(s,grade='RELOCATION_MATCH'))
 cases=compare(candidate,native);test=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 assert not ({s['symbol'] for s in SPECS}&Coff(test).symbols.keys())
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode,'provider omission linked'
 r=subprocess.run(cmd+[str(src),str(test)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'GRAPHIC_POINTER_ASSIGNMENT_BEHAVIOR_PASS cases=864' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=records,comparative_scenarios=cases,behavior=r.stdout.strip(),provider_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=1,boundaries=['deletion callback','operator delete'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('GRAPHIC_POINTER_ASSIGNMENT_PASS native='+str(cases)+' compiled=864')
if __name__=='__main__':main()
