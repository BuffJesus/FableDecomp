"""Conditional consume linked against native whitespace/quote helpers."""
import json,subprocess,tempfile,struct
from pathlib import Path
import check_one,oracle_string_parser_delimiters
from gate_game_component_abi import Coff
from pe_oracle import EXE,pe_sections,va_to_off
ROOT=Path(__file__).resolve().parents[2]
def main():
 rows=oracle_string_parser_delimiters.collect();w=Path(tempfile.mkdtemp(prefix='consume-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE']
 names=['CStringParser_SkipUntilString_009b9a20.cpp','CStringParser_SkipPastString_009b9ba0.cpp','CStringParser_ReadAsStringUntilString_009ba330.cpp','CStringParser_ReadAsStringUntilPastString_009ba3e0.cpp','CStringParser_SkipUntilWhitespace_009b9ce0.cpp','CStringParser_ReadAsStringUntilWhitespace_009ba4a0.cpp','CStringParser_SetStringPos_009b9940.cpp','CStringParser_CalcInQuotesTyped_009b9570.cpp'];sources=[ROOT/'rebuild/src/compiled/00/9b'/n for n in names];test=w/'test.cpp';obj=w/'consume.obj'
 fixture=(ROOT/'rebuild/tests/00/9a/CDefinitionManager_ParseEnumForSymbols_009acee0_test.cpp').read_text();code=fixture[:fixture.index('static int count,errors')]
 code+='\nint main(){unsigned long raw[10];CStringParser* p=(CStringParser*)raw;'
 for i,row in enumerate(rows):
  call={0x9b9a20:'SkipUntilString',0x9b9ba0:'SkipPastString',0x9ba330:'ReadAsStringUntilString',0x9ba3e0:'ReadAsStringUntilPastString',0x9b9ce0:'SkipUntilWhitespace',0x9ba4a0:'ReadAsStringUntilWhitespace'}[row['address']]
  argument='' if row['address'] in (0x9b9ce0,0x9ba4a0) else 'needle'
  check=('CCharString result=p->'+call+'('+argument+');if(strcmp(text(result),'+json.dumps(row['output'])+')' if row['address'] in (0x9ba330,0x9ba3e0,0x9ba4a0) else 'long result=p->'+call+'('+argument+');if(result!='+str(row['value'] if row['address']==0x9b9ce0 else int(row['result'])))
  code+='\n{memset(raw,0,sizeof(raw));p->String='+json.dumps(row['source'])+';raw[7]='+str(row['initial_quote'])+'UL;raw[8]='+str(row['initial_position'])+';raw[9]='+str(row['line'])+';CCharString needle('+json.dumps(row['needle'])+');'+check+' || raw[7]!='+str(row['cursor'][0])+'UL || raw[8]!='+str(row['cursor'][1])+' || raw[9]!='+str(row['cursor'][2])+')return '+str(i+1)+';}'
 code+='\nputs("PARSER_DELIMITERS_OK cases=48");return 0;}';test.write_text(code)
 r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
 c=Coff(obj);key=next(k for k in c.symbols if k.startswith('?SkipUntilString'));sec,off=c.symbols[key];b,relocs=c.sections[sec]
 parity={};raw=EXE.read_bytes();sections=pe_sections(raw)
 for index,address,dependency in [(2,0x9ba330,0x9b9a20),(3,0x9ba3e0,0x9b9ba0)]:
  o=w/('wrapper'+str(index)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(o),str(sources[index])],e,w);assert r.returncode==0,r.stdout+r.stderr
  c=Coff(o);key=next(k for k in c.symbols if k.startswith('?ReadAsStringUntil'));sec,off=c.symbols[key];body,relocs=c.sections[sec];patched=bytearray(body);assert len(body)==81
  for pos,(target,kind) in relocs.items():
   dest=0x122d70e if kind==6 else 0x99ebf0 if target.startswith('??0CCharString') else dependency
   struct.pack_into('<I',patched,pos,(dest+struct.unpack_from('<i',body,pos)[0]-address-pos-4 if kind==20 else dest)&0xffffffff)
  off=va_to_off(sections,address);assert bytes(patched)==raw[off:off+81];parity[hex(address)]='RELOCATION_MATCH:81'
 r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/string-parser-delimiters.json').write_text(json.dumps(dict(address='009b9a20',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=125,linked_cases=48,retail_cases=rows,wrapper_parity=parity),indent=2)+'\n');print('PARSER_DELIMITERS_PASS cases=48 bytes='+str(len(b)))
if __name__=='__main__':main()
