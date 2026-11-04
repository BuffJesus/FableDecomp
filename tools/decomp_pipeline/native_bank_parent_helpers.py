from pathlib import Path
import sys,json,struct
R=Path(__file__).resolve().parents[2];sys.path[:0]=[str(R/'tools/decomp_pipeline')];from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from gate_native_file_absolute_path import ROOTS as N,ADDRESS as NA
from gate_native_file_absolute_path_wide import ROOTS as W,ADDRESS as WA
import NativeParserReadUntilItem_model as item_model
import NativeBasicStringRepeated_model as repeated_model
import check_one as V

def run(b):
 r=json.loads((b/'receipt.json').read_text());records=[]
 for root,targets in [(N[0],NA),(W[0],WA)]:
  obj=Path(next(o for p,o in r['source_objects'].items() if Path(p).name==root['source'].name));c=Coff(obj);sec,off=c.symbols[root['symbol']];body,rels=c.sections[sec];assert not off and len(body)==root['size'];fixed=bytearray(body)
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,((0xbfea14 if name=='_FixtureVectorFree' else targets[name])+struct.unpack_from('<i',body,site)[0]-(root['address']+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),root['address'],len(body));records.append(dict(symbol=root['symbol'],grade='MATCH',bytes=len(body)))
 for filename,symbol,address,size in [('CStringParser_ReadAsStringUntilStringItem_009ba110.cpp','?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z',0x9ba110,93),('CBasicString_char_SetRepeated_009a0390.cpp','?SetString@?$CBasicString@D@@IAEXKD@Z',0x9a0390,158)]:
  obj=Path(next(o for p,o in r['source_objects'].items() if Path(p).name==filename));c=Coff(obj);sec,off=c.symbols[symbol];body,rels=c.sections[sec];grade=compare(body,read_virtual_data(EXE.read_bytes(),address,size),tuple(rels),retail_relocations('%08x'%address,size)).to_dict();assert grade['near_match'];records.append(dict(symbol=symbol,object=str(obj),bytes=len(body),grade=grade,relocations=rels))
 for filename,model,expected,label in [('CStringParser_ReadAsStringUntilStringItem_009ba110.cpp',item_model,304,'item'),('CBasicString_char_SetRepeated_009a0390.cpp',repeated_model,440,'repeated')]:
  obj=Path(next(o for p,o in r['source_objects'].items() if Path(p).name==filename));out=b/(label+'-model.json');model.run(obj,out);assert json.loads(out.read_text())['cases']==expected
 controls=[('item_false','CStringParser_ReadAsStringUntilStringItem_009ba110.cpp','return true;','return false;',item_model),('item_type','CStringParser_ReadAsStringUntilStringItem_009ba110.cpp','item.InitAsString(CCharString(start,static_cast<long>(String+StringPos-start)));','item.String=CCharString(start,static_cast<long>(String+StringPos-start));',item_model),('item_start','CStringParser_ReadAsStringUntilStringItem_009ba110.cpp','const char* start=String+StringPos;','const char* start=String+StringPos+1;',item_model),('repeat_char','CBasicString_char_SetRepeated_009a0390.cpp','memset(PData,character,length);','memset(PData,0,length);',repeated_model),('repeat_length','CBasicString_char_SetRepeated_009a0390.cpp','StringLength=length;','StringLength=0;',repeated_model),('repeat_tail','CBasicString_char_SetRepeated_009a0390.cpp','memset(PData+start,0,((start+3)&~3UL)-start);',';',repeated_model)]
 records_controls=[]
 for name,source,old,new,model in controls:
  text=Path(next(o for p,o in r['source_objects'].items() if Path(p).name==source)).with_suffix('.cpp').read_text();assert text.count(old)==1;cpp=b/(name+'.cpp');cpp.write_text(text.replace(old,new));obj=b/(name+'.obj')
  compiled=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(b/'include'),'/Fo'+str(obj),str(cpp)],V.env(),b);assert not compiled.returncode,compiled.stdout+compiled.stderr
  rejected=False
  try:model.run(obj,b/(name+'.unexpected.json'))
  except AssertionError:rejected=True
  assert rejected,name;records_controls.append(dict(name=name,rejected=True));print(name,'rejected')
 (b/'helper-controls.json').write_text(json.dumps(dict(status='PASS',controls=records_controls),indent=2))
 report=dict(status='PASS',roots=records,item_model=json.loads((b/'item-model.json').read_text()),repeated_model=json.loads((b/'repeated-model.json').read_text()),controls=records_controls)
 (b/'parent-helper-proof.json').write_text(json.dumps(report,indent=2));return report
