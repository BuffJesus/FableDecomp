"""Validate the IME settings data against retail initial values and its real loader."""
import argparse,hashlib,json,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff, retail_bytes
from gate_render_state_static_data import coff_data
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
BYTE=['Enabled_013B7CD8','SymbolHeight_013B7CF0','SymbolTranslucence_013B7CF1','SymbolPlacement_013B7CF2','CompositionTranslucence_013B7D18','CaretWidth_013B7D20','CaretYMargin_013B7D21','CancelOnOkClick_013B7D28']
WORD=['SymbolColour_013B7CE4','SymbolColourOff_013B7CE8','SymbolColourText_013B7CEC','CandidateColourBase_013B7CF8','CandidateColourBorder_013B7CFC','CandidateColourText_013B7D00','CandidateColourInput_013B7D04','CandidateColourTargetConverted_013B7D08','CandidateColourConverted_013B7D0C','CandidateColourTargetNotConverted_013B7D10','CandidateColourInputError_013B7D14']
SPECS=[('?g_FableIME_013B7CD8@@3VCIME@NUISystem@@A',0x13b7cd8,84)]
CALLER=ROOT/'rebuild/src/compiled/00/40/CIME_LoadIMESettings_00405350.cpp'

def check_loader(obj):
 c=Coff(obj);symbol='?LoadIMESettings@CIME@NUISystem@@SIXAAVCPersistContext@@@Z'
 section,start=c.symbols[symbol];body,relocations=c.sections[section]
 raw=EXE.read_bytes();retail=retail_bytes(raw,0x405350,765)
 assert start==0 and len(body)==len(retail) and len(relocations)==84
 targets={'?TransferSignedChar@CPersistContext@@QAEXPBDAACABC@Z':0x405a60,
          '??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z':0x4045c0,
          '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
          '??1CCharString@@QAE@XZ':0x99eae0,
          '?TransferIMEColour@@YIIAAVCPersistContext@@ABVCCharString@@I@Z':0x405220,
          '?g_FableIME_013B7CD8@@3VCIME@NUISystem@@A':0x13b7cd8}
 patched=bytearray(body)
 for offset,(name,kind)in relocations.items():
  addend=struct.unpack_from('<I',body,offset)[0]
  if name.startswith('??_C@'):
   assert kind==6 and name in c.symbols
   sec,pos=c.symbols[name];string=c.sections[sec][0][pos+addend:];string=string[:string.index(0)+1]
   target=struct.unpack_from('<I',retail,offset)[0]
   assert retail_bytes(raw,target,len(string))==string,(name,target)
   value=target
  else:
   assert name in targets and kind in (6,20),(name,kind)
   value=targets[name]+addend
   if kind==20:value-=0x405350+offset+4
  assert struct.pack('<I',value&0xffffffff)==retail[offset:offset+4],(name,offset)
  struct.pack_into('<I',patched,offset,value&0xffffffff)
 # Different local slots are possible after giving the two boolean settings
 # their real type; every relocation identity has already been checked.
 from near_match import compare
 score=compare(bytes(patched),retail)
 assert score.near_match,score
 return dict(address='00405350',grade='MATCH'if bytes(patched)==retail else'NEAR_MATCH',
             relocations=84,**score.to_dict())
def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/01/3b/CIME_SettingsData_013b7cd8.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/ImeSettingsData_test.cpp')
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/ime-settings-data.json');a=ap.parse_args()
 env=check_one.env();w=Path(tempfile.mkdtemp(prefix='ime-settings-data-'));objects=[]
 for i,source in enumerate([a.source,CALLER,a.test]):
  obj=w/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 loader=check_loader(objects[1])
 data=Coff(objects[0]);fixture=Coff(objects[2]);assert all(s not in fixture.symbols for s,_,_ in SPECS);checked=[]
 with pefile.PE(str(EXE)) as pe:
  for symbol,address,size in SPECS:
   rva=address-pe.OPTIONAL_HEADER.ImageBase;section=next(s for s in pe.sections if s.VirtualAddress<=rva<s.VirtualAddress+s.Misc_VirtualSize)
   assert section.Name.rstrip(b'\0')==b'.data' and rva+size<=section.VirtualAddress+section.SizeOfRawData,symbol
   native=pe.get_data(rva,size);assert coff_data(data,symbol,size)==native,symbol;checked.append(dict(symbol=symbol,address='%08x'%address,size=size,value=native.hex()))
 assert len(data.symbols.keys()&{s for s,_,_ in SPECS})==len(SPECS)
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in [0,1]:
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,'data provider and real loader must both be required'
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'IME_SETTINGS_DATA_BEHAVIOR_PASS scenarios=76' in r.stdout,r.stdout+r.stderr
 report=dict(data_symbols=checked,data_bytes=sum(s for _,_,s in SPECS),behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),caller_sha256=hashlib.sha256(CALLER.read_bytes()).hexdigest(),missing_provider_checks=2,coverage_claim='data only; no new reconstructed functions')
 report['loader']=loader
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('IME_SETTINGS_DATA_PASS symbols=%d bytes=%d compiled=76'%(len(SPECS),report['data_bytes']))
if __name__=='__main__':main()
