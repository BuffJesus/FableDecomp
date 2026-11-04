"""Validate typed graphics and mesh bank counted-pointer copy constructors."""
import argparse,hashlib,json,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from bank_pointer_copy_oracle import compare
ROOT=check_one.ROOT
SPECS=[dict(address='0041ba30',file='CCountedPointerCGraphicDataBank_Copy_0041ba30.cpp',symbol='??0?$CCountedPointer@VCGraphicDataBank@@@@QAE@ABV0@@Z',size=25),dict(address='00428905',file='CCountedPointerCMeshDataBank_Copy_00428905.cpp',symbol='??0?$CCountedPointer@VCMeshDataBank@@@@QAE@ABV0@@Z',size=25)]
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--source-dir',type=Path);ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/BankPointerCopies_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-pointer-copies.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='bank-pointer-copy-'));env=check_one.env();objects=[];providers=[];candidate={};native={};entries=[]
 with pefile.PE(str(EXE)) as pe:
  for s in SPECS:
   source=(a.source_dir or ROOT/'rebuild/src/compiled'/('00/41' if s['address']=='0041ba30' else '00/42'))/s['file'];obj=w/(s['address']+'.obj')
   r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
   c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0 and not rel and len(built)==s['size']
   addr=int(s['address'],16);raw=pe.get_data(addr-pe.OPTIONAL_HEADER.ImageBase,s['size']);assert built==raw,s
   candidate[addr]=built;native[addr]=raw;entries.append(dict(s,grade='MATCH'));providers.append(dict(source=str(source.resolve()),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
 cases=compare(candidate,native);test=w/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr
 assert not ({s['symbol'] for s in SPECS}&Coff(test).symbols.keys())
 cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),str(test)]
 for omit in range(2):
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,('omission linked',omit)
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode and 'BANK_POINTER_COPIES_BEHAVIOR_PASS cases=288' in r.stdout,(r.returncode,r.stdout,r.stderr)
 report=dict(entries=entries,comparative_scenarios=cases,behavior=r.stdout.strip(),providers=providers,fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),missing_provider_checks=2)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('BANK_POINTER_COPIES_PASS native='+str(cases)+' compiled=288')
if __name__=='__main__':main()
