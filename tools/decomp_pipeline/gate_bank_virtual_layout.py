"""Check six compiler-emitted bank vtables against PDB names and retail slots."""
import argparse,json,struct,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve()
while not(ROOT/'rebuild').is_dir():ROOT=ROOT.parent
SOURCES={'CBankFile':'00/9c/CBankFile_CBankFile_009cd480.cpp','CBankFileAsync':'00/9d/CBankFileAsync_CBankFileAsync_009d5f80.cpp','CFontDataBank':'00/a6/CFontDataBank_CFontDataBank_00a63630.cpp','CStreamingFontDataBank':'00/a5/CStreamingFontDataBank_CStreamingFontDataBank_00a5f290.cpp','CDataBank@NGameText':'00/9c/NGameText_CDataBank_Constructor_009c85a0.cpp'}
def main():
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--proof',type=Path,default=ROOT/'rebuild/tests/integration/bank_virtual_layout.json');ap.add_argument('--probe',type=Path,default=ROOT/'rebuild/tests/integration/BankVirtualLayout_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/bank-virtual-layout.json');a=ap.parse_args()
 w=Path(tempfile.mkdtemp(prefix='bank-virtual-layout-'));env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];results=[]
 with pefile.PE(str(EXE)) as pe:
  for row in json.loads(a.proof.read_text()):
   owner=row['owner'];assert owner!='CShaderDataBank';source=ROOT/'rebuild/src/compiled'/SOURCES[owner] if owner in SOURCES else a.probe;obj=w/(owner.replace('@','_')+'.obj')
   r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;c=Coff(obj)
   symbol='??_7'+owner+'@@6B@'
   if symbol not in c.symbols:symbol='??_7'+owner+'@@6BCBankFileAsync@@@'
   assert symbol in c.symbols,(owner,[s for s in c.symbols if s.startswith('??_7')]);sec,off=c.symbols[symbol];b,rel=c.sections[sec];assert off==0 and len(b)==80,(owner,len(b))
   names=[rel[i*4][0] for i in range(20)];assert names==row['slots'],(owner,names,row['slots']);assert all(kind==6 for _,kind in rel.values())
   targets=list(struct.unpack('<20I',pe.get_data(row['address']-pe.OPTIONAL_HEADER.ImageBase,80)));assert targets==row['targets'];results.append(row)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(results,indent=2)+'\n');print('BANK_VIRTUAL_LAYOUT_PASS tables=6 slots=120')
if __name__=='__main__':main()
