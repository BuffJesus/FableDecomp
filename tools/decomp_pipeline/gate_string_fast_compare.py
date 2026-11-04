"""Validate complete native fast string ordering, including padded word reads."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one,near_match
from gate_game_component_abi import Coff
from pe_oracle import EXE
from string_fast_compare_oracle import compare
ROOT=Path(__file__).resolve()
while not (ROOT/'rebuild').is_dir():ROOT=ROOT.parent
SYMBOL='?FastCompare@CCharString@@QBE_NABV1@@Z'
TARGETS={}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9e/CCharString_FastCompare_009e6530.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9e/CCharString_FastCompare_009e6530_test.cpp');ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/string-fast-compare.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='string-fast-compare-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[]
 for i,p in enumerate([a.source,a.test]):
  obj=w/('source'+str(i)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 c=Coff(objects[0]);sec,off=c.symbols[SYMBOL];assert off==0;b,rel=c.sections[sec];assert len(b)==108;patched=bytearray(b);checks=[]
 with pefile.PE(str(EXE)) as pe:
  n=pe.get_data(0x9e6530-pe.OPTIONAL_HEADER.ImageBase,108)
  for site,(name,kind) in rel.items():
   target=TARGETS[name];assert b[site:site+4]==bytes(4) and kind in (6,20);value=target if kind==6 else (target-0x9e6530-site-4)&0xffffffff
   assert struct.unpack_from('<I',n,site)[0]==value;struct.pack_into('<I',patched,site,value);checks.append(dict(offset=site,symbol=name,target=target))
 assert not rel;similarity=near_match.compare(b,n);assert similarity.near_match;comparisons=compare(b,n)
 assert SYMBOL not in Coff(objects[1]).symbols
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(p) for p in objects],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'STRING_FAST_COMPARE_BEHAVIOR_PASS scenarios=2592' in r.stdout,r.stdout+r.stderr
 report=dict(address='009e6530',symbol=SYMBOL,bytes=108,grade='NEAR_MATCH',relocations=checks,comparative_scenarios=comparisons,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),boundaries=[],similarity=similarity.to_dict())
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('STRING_FAST_COMPARE_PASS compiled=2592 comparative='+str(comparisons))
if __name__=='__main__':main()
