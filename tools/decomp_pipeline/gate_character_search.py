"""Validate typed character search and the signed char CRT boundary."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from character_search_oracle import compare
ROOT=Path(__file__).resolve()
while not (ROOT/'rebuild').is_dir():ROOT=ROOT.parent
SYMBOL='?FindCharPos@CCharString@@QBEJD@Z'
TARGETS={'?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,'_strchr':0xbfeda0}
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/99/CCharString_FindCharPos_0099e5a0.cpp');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/99/CCharString_FindCharPos_0099e5a0_test.cpp');ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/character-search.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='character-search-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[]
 for i,p in enumerate([a.source,a.test]):
  obj=w/('source'+str(i)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(p.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 c=Coff(objects[0]);sec,off=c.symbols[SYMBOL];assert off==0;b,rel=c.sections[sec];assert len(b)==69;patched=bytearray(b);checks=[]
 with pefile.PE(str(EXE)) as pe:
  n=pe.get_data(0x99e5a0-pe.OPTIONAL_HEADER.ImageBase,69);assert pe.get_data(0x129aaf4-pe.OPTIONAL_HEADER.ImageBase,1)==b'\0'
  for site,(name,kind) in rel.items():
   target=TARGETS[name];assert b[site:site+4]==bytes(4) and kind in (6,20);value=target if kind==6 else (target-0x99e5a0-site-4)&0xffffffff
   assert struct.unpack_from('<I',n,site)[0]==value;struct.pack_into('<I',patched,site,value);checks.append(dict(offset=site,symbol=name,target=target))
 assert patched==n;comparisons=compare(patched,n)
 assert SYMBOL not in Coff(objects[1]).symbols
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(p) for p in objects],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CHARACTER_SEARCH_BEHAVIOR_PASS scenarios=2304' in r.stdout,r.stdout+r.stderr
 report=dict(address='0099e5a0',symbol=SYMBOL,bytes=69,grade='RELOCATION_MATCH',relocations=checks,comparative_scenarios=comparisons,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),boundaries=['CRT strchr'])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CHARACTER_SEARCH_PASS compiled=2304 comparative='+str(comparisons))
if __name__=='__main__':main()
