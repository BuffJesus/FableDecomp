"""Conditional consume linked against native whitespace/quote helpers."""
import json,subprocess,tempfile
from pathlib import Path
import check_one,oracle_string_parser_exists
from gate_game_component_abi import Coff
ROOT=Path(__file__).resolve().parents[2]
def main():
 rows=oracle_string_parser_exists.collect();w=Path(tempfile.mkdtemp(prefix='consume-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE']
 names=['CStringParser_NextItemExists_009ba4f0.cpp','CStringParser_SkipWhitespace_009b9c60.cpp'];sources=[ROOT/'rebuild/src/compiled/00/9b'/n for n in names];test=w/'test.cpp';obj=w/'consume.obj'
 code='\n'.join(['#include "fable_string_parser.h"','#include <stdio.h>','#include <string.h>','char g_FableEmptyNarrowString_0129AAF4[1]={0};',
 'CCharString::CCharString(const char* text,long):storage_(0){storage_=(CCharStringData*)malloc(sizeof(CCharStringData));memset(storage_,0,sizeof(CCharStringData));storage_->text=(char*)malloc(strlen(text)+1);strcpy(storage_->text,text);storage_->unknown04=strlen(text);}',
 'CCharString::~CCharString(){if(storage_){free(storage_->text);free(storage_);}}',
 'int main(){unsigned long raw[10];CStringParser* p=(CStringParser*)raw;'])
 for i,row in enumerate(rows):
  code+='\n{memset(raw,0,sizeof(raw));p->String='+json.dumps(row['source'])+';raw[7]='+str(row['initial_quote'])+'UL;raw[8]='+str(row['initial_position'])+';raw[9]='+str(row['line'])+';CCharString text('+json.dumps(row['needle'])+');bool result=p->NextItemExists();if(result!='+str(int(row['result']))+' || raw[7]!='+str(row['cursor'][0])+'UL || raw[8]!='+str(row['cursor'][1])+' || raw[9]!='+str(row['cursor'][2])+')return '+str(i+1)+';}'
 code+='\nputs("PARSER_EXISTS_OK cases=5");return 0;}';test.write_text(code)
 r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(sources[0])],e,w);assert r.returncode==0,r.stdout+r.stderr
 c=Coff(obj);key=next(k for k in c.symbols if k.startswith('?NextItemExists'));sec,off=c.symbols[key];b,relocs=c.sections[sec]
 r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(test)],e,w);assert r.returncode==0,r.stdout+r.stderr
 r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 (ROOT/'rebuild/compile-gate/string-parser-exists.json').write_text(json.dumps(dict(address='009ba4f0',grade='BEHAVIORAL',compiled_bytes=len(b),retail_bytes=77,linked_cases=5,retail_cases=rows),indent=2)+'\n');print('PARSER_EXISTS_PASS cases=5 bytes='+str(len(b)))
if __name__=='__main__':main()
