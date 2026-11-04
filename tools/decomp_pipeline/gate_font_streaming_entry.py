"""Validate streaming font entry initialization against retail."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,near_match,check_one
from pe_oracle import EXE
from gate_game_component_abi import Coff
from font_streaming_entry_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?InitStreamingEntry@CStreamingFontDataBank@@QAEXAAVCSymbolMap@@@Z'
TARGETS={'??_C@_09GAGDNKMC@GlyphData?$AA@':0x129d5a4,'??0CCharString@@QAE@PBDJ@Z':0x99ebf0,'??1CCharString@@QAE@XZ':0x99eae0,'?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z':0x9ccdc0,'?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z':0x9d58d0,'??3@YAXPAX@Z':0xbfe9bc}

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/a5/CStreamingFontDataBank_InitStreamingEntry_00a5ef50.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/a5/CStreamingFontDataBank_InitStreamingEntry_00a5ef50_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/font-streaming-entry.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='font-streaming-entry-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];obj=w/'candidate.obj';test=w/'test.obj'
 for source,out in [(a.source,obj),(a.test,test)]:
  r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(out),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 c=Coff(obj);assert SYMBOL not in Coff(test).symbols;sec,off=c.symbols[SYMBOL];assert off==0;b,rel=c.sections[sec];assert len(b)==178;patched=bytearray(b);checked=[]
 with pefile.PE(str(EXE)) as pe:n=pe.get_data(0xa5ef50-pe.OPTIONAL_HEADER.ImageBase,178)
 assert set(rel)=={11,20,32,43,56,106,165}
 native_sites=[11,20,32,43,56,106,165]
 for (site,(symbol,kind)),native_site in zip(rel.items(),native_sites):
  target=TARGETS[symbol];assert b[site:site+4]==bytes(4)
  if kind==6:assert struct.unpack_from('<I',n,native_site)[0]==target
  else:assert kind==20 and n[native_site-1]==0xe8 and 0xa5ef50+native_site+4+struct.unpack_from('<i',n,native_site)[0]==target
  struct.pack_into('<I',patched,site,(target-(0xa5ef50+site+4 if kind==20 else 0))&0xffffffff);checked.append(dict(offset=site,native_offset=native_site,symbol=symbol,kind=kind,target=target))
 score=near_match.compare(b,n,list(rel),[11]);assert score.near_match
 comparative=compare(patched,n)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(test),str(obj)],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'FONT_STREAMING_ENTRY_BEHAVIOR_PASS scenarios=450' in r.stdout,r.stdout+r.stderr
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(dict(behavior=r.stdout.strip(),comparative_scenarios=comparative,grade='MATCH' if bytes(patched)==n else 'NEAR_MATCH',similarity=score.to_dict(),relocations=checked,source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest()),indent=2)+'\n');print('FONT_STREAMING_ENTRY_PASS compiled=450 comparative='+str(comparative))
if __name__=='__main__':main()
