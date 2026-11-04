"""Validate typed bank enumeration and native string-return ABI."""
import sys,json,struct,subprocess,argparse,hashlib
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile
from pe_oracle import EXE
from gate_game_component_abi import Coff
from bank_enumeration_oracle import compare
SPECS=[{'file': 'CBankFile_GetEnumerationType_009cbd10.cpp', 'owner': 'CBankFile', 'address': '009cbd10', 'symbol': '?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z', 'bytes': 25, 'relocations': [[15, '??0CCharString@@QAE@ABV0@@Z', 20, 10087472]]}, {'file': 'CFontDataBank_GetEnumerationType_00a63380.cpp', 'owner': 'CFontDataBank', 'address': '00a63380', 'symbol': '?GetEnumerationType@CFontDataBank@@MBE?AVCCharString@@K@Z', 'bytes': 22, 'relocations': [[12, '?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z', 20, 10272016]]}, {'file': 'CStreamingFontDataBank_GetEnumerationType_00a5ef10.cpp', 'owner': 'CStreamingFontDataBank', 'address': '00a5ef10', 'symbol': '?GetEnumerationType@CStreamingFontDataBank@@MBE?AVCCharString@@K@Z', 'bytes': 22, 'relocations': [[12, '?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z', 20, 10272016]]}, {'file': 'NGameText_CDataBank_GetEnumerationType_009c79e0.cpp', 'owner': 'NGameText::CDataBank', 'address': '009c79e0', 'symbol': '?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z', 'bytes': 82, 'relocations': [[16, '??_C@_09KANGMNGK@EGameText?$AA@', 6, 19510372], [23, '??0CCharString@@QAE@PBDJ@Z', 20, 10087408], [43, '??_C@_0L@HNPBNKBJ@ETextGroup?$AA@', 6, 19510360], [50, '??0CCharString@@QAE@PBDJ@Z', 20, 10087408], [65, '??_C@_00CNPNBAHC@?$AA@', 6, 19060494], [72, '??0CCharString@@QAE@PBDJ@Z', 20, 10087408]]}]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--source-dir',type=Path);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/BankEnumeration_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-enumeration.json');args=ap.parse_args()
 import tempfile
 W=Path(tempfile.mkdtemp(prefix='bank-enumeration-'));specs=SPECS;candidate={};native={};providers=[];env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];objects=[]
 with pefile.PE(str(EXE)) as pe:
  for s in specs:
   source=args.source_dir/s['file'] if args.source_dir else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()));obj=W/(Path(s['file']).stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj.resolve()),str(source.resolve())],env,W.resolve());assert not r.returncode,r.stdout+r.stderr;objects.append(obj.resolve());c=Coff(obj);sec,off=c.symbols[s['symbol']];b,rel=c.sections[sec];assert off==0 and len(b)==s['bytes'];patched=bytearray(b);addr=int(s['address'],16)
   for site,name,kind,target in s['relocations']:
    assert rel[site]==(name,kind)
    if kind==6:
     ss,so=c.symbols[name];literal=c.sections[ss][0][so:];assert pe.get_data(target-pe.OPTIONAL_HEADER.ImageBase,len(literal))==literal
    struct.pack_into('<I',patched,site,(target-addr-site-4 if kind==20 else target)&0xffffffff)
   raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,len(b));assert patched==raw;candidate[addr]=patched;native[addr]=raw
 comparisons=compare(candidate,native)
 obj=W/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj.resolve()),str(args.test.resolve())],env,W.resolve());assert not r.returncode,r.stdout+r.stderr;objects.append(obj.resolve());c=Coff(obj);assert all(s['symbol'] not in c.symbols for s in specs)
 cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str((W/'fixture.exe').resolve())]
 for omit in range(4):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str((W/'fixture.exe').resolve())],env=env,capture_output=True,text=True);assert not r.returncode,(r.returncode,r.stdout,r.stderr);print(r.stdout)
 
 assert 'BANK_ENUMERATION_BEHAVIOR_PASS scenarios=640' in r.stdout
 report=dict(entries=[dict(s,grade='RELOCATION_MATCH') for s in specs],comparative_scenarios=comparisons,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),missing_provider_checks=4,boundaries=['CCharString copy constructor','CCharString literal constructor','fixture string destructor'])
 args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n');print('BANK_ENUMERATION_PASS compiled=640 comparative='+str(comparisons)+' providers=4')

if __name__=='__main__':main()
