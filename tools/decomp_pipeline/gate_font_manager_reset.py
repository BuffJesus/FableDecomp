"""Validate native font-manager pointer reset and emitted deletion thunk."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from font_manager_reset_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
RESET='?Reset@?$CCountedPointer@VCFontManager@@@@QAEXPAVCFontManager@@@Z'
DELETE='?DeleteData@?$CCountedPointer@VCFontManager@@@@CIXPAX@Z'
SPECS=[(0x9a7340,RESET,103,0),(0x9a6c10,DELETE,11,0)]
TARGETS={DELETE:0x9a6c10,'??3@YAXPAX@Z':0xbfe9bc,'??2@YAPAXI@Z':0xbfea1a}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CCountedPointerFontManager_Reset_009a7340.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CCountedPointerFontManager_Reset_009a7340_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/font-manager-reset.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='font-manager-reset-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];sources=[a.source];objects=[]
 for i,source in enumerate(sources+[a.test]):
  obj=w/('source'+str(i)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 coffs=[Coff(obj) for obj in objects];assert all(symbol not in coffs[-1].symbols for _,symbol,_,_ in SPECS[:4]);candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  for address,symbol,length,index in SPECS:
   c=coffs[index];sec,off=c.symbols[symbol];assert off==0;b,rel=c.sections[sec];assert len(b)==length;n=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(b);checked=[]
   for site,(name,kind) in rel.items():
    target=TARGETS[name];assert b[site:site+4]==bytes(4) and kind in (6,20)
    value=target if kind==6 else (target-address-site-4)&0xffffffff
    assert struct.unpack_from('<I',n,site)[0]==value
    struct.pack_into('<I',patched,site,value);checked.append(dict(offset=site,symbol=name,target=target))
   assert patched==n;candidate[address]=patched;native[address]=n;records.append(dict(address='%08x'%address,symbol=symbol,bytes=length,grade='RELOCATION_MATCH',relocations=checked))
 comparisons=compare(candidate,native);exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(objects[-1])],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(objects[0]),str(objects[-1])],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'FONT_MANAGER_RESET_BEHAVIOR_PASS scenarios=90' in r.stdout,r.stdout+r.stderr
 report=dict(entries=records,comparative_scenarios=comparisons,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),release_providers=[dict(source=str(p.relative_to(ROOT)),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources[1:]])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('FONT_MANAGER_RESET_PASS compiled=90 comparative='+str(comparisons))
if __name__=='__main__':main()
