"""Validate typed font sharing and assignment through recovered release bodies."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import pefile,check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE
from font_pointer_sharing_oracle import compare
ROOT=Path(__file__).resolve().parents[2]
SHARE='?ShareData@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@PAVCCPPointerInfo@@@Z'
CONST_SHARE='?ShareData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@PAVCCPPointerInfo@@@Z'
ASSIGN='??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z'
CONST_ASSIGN='??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z'
RELEASE='?ReleaseData@?$CCountedPointer@VCFontBank@@@@QAEXXZ'
CONST_RELEASE='?ReleaseData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXXZ'
SPECS=[(0x4190e2,SHARE,38,0),(0x42f9f2,CONST_SHARE,38,0),(0x419463,ASSIGN,23,0),(0x42fe34,CONST_ASSIGN,23,0),(0x4057a0,RELEASE,53,1),(0x42f948,CONST_RELEASE,44,2)]
RELEASE_SOURCES=['00/40/CCountedPointerFontBank_ReleaseData_004057a0.cpp','00/42/CCountedPointerConstFontBank_ReleaseData_0042f948.cpp']
TARGETS={s:a for a,s,n,i in SPECS};TARGETS['??3@YAXPAX@Z']=0xbfe9bc

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/41/CCountedFontPointer_Sharing_004190e2.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/41/CCountedFontPointer_Sharing_004190e2_test.cpp')
 ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/font-pointer-sharing.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='font-pointer-sharing-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+str(ROOT/'rebuild/include')+';'+env['INCLUDE'];sources=[a.source]+[ROOT/'rebuild/src/compiled'/s for s in RELEASE_SOURCES];objects=[]
 for i,source in enumerate(sources+[a.test]):
  obj=w/('source'+str(i)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 coffs=[Coff(obj) for obj in objects];assert all(symbol not in coffs[-1].symbols for _,symbol,_,_ in SPECS[:4]);candidate={};native={};records=[]
 with pefile.PE(str(EXE)) as pe:
  for address,symbol,length,index in SPECS:
   c=coffs[index];sec,off=c.symbols[symbol];assert off==0;b,rel=c.sections[sec];assert len(b)==length;n=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,length);patched=bytearray(b);checked=[]
   for site,(name,kind) in rel.items():
    target=TARGETS[name];assert kind==20 and b[site:site+4]==bytes(4) and n[site-1]==0xe8 and address+site+4+struct.unpack_from('<i',n,site)[0]==target
    struct.pack_into('<I',patched,site,(target-address-site-4)&0xffffffff);checked.append(dict(offset=site,symbol=name,target=target))
   assert patched==n;candidate[address]=patched;native[address]=n;records.append(dict(address='%08x'%address,symbol=symbol,bytes=length,grade='RELOCATION_MATCH',relocations=checked))
 comparisons=compare(candidate,native);exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 r=subprocess.run(cmd+[str(objects[-1])],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+[str(objects[0]),str(objects[-1])],env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'FONT_POINTER_SHARING_BEHAVIOR_PASS scenarios=1200' in r.stdout,r.stdout+r.stderr
 report=dict(entries=records,comparative_scenarios=comparisons,behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),release_providers=[dict(source=str(p.relative_to(ROOT)),sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sources[1:]])
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('FONT_POINTER_SHARING_PASS compiled=1200 comparative='+str(comparisons)+' new_typed_bodies=4')
if __name__=='__main__':main()
