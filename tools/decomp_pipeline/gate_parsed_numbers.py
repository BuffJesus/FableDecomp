"""Differential retail numeric conversion results including x87 rounding."""
import json,subprocess,tempfile
from pathlib import Path
import check_one,oracle_parsed_numbers
ROOT=Path(__file__).resolve().parents[2]
def main():
    rows=oracle_parsed_numbers.collect();w=Path(tempfile.mkdtemp(prefix='parsed-numbers-gate-'));e=check_one.env();e['INCLUDE']=str(ROOT/'rebuild/include')+';'+e['INCLUDE']
    fixture=w/'test.cpp'
    cases=',\n'.join('{'+json.dumps(r['text'])+','+str(r['kind'])+','+str(r['bits'])+'u}' for r in rows)
    fixture.write_text('#include "fable_string_parser.h"\n#include <stdio.h>\n#include <string.h>\nint main(){struct Case{const char* text;unsigned kind,bits;}cases[]={'+cases+'};for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i){unsigned long raw[3]={0};CParsedItem* item=reinterpret_cast<CParsedItem*>(raw);if(cases[i].kind==3)item->InitAsInteger(cases[i].text,strlen(cases[i].text));else item->InitAsFloat(cases[i].text,strlen(cases[i].text));if(raw[0]!=cases[i].kind||raw[1]!=cases[i].bits){printf("case %u got %08lx expected %08x\\n",i,raw[1],cases[i].bits);return 1;}}puts("PARSED_NUMBERS_OK");return 0;}')
    sources=[ROOT/'rebuild/src/compiled/00/9b'/name for name in ['CParsedItem_InitAsInteger_009b9700.cpp','CParsedItem_InitAsFloat_009b9770.cpp']]
    r=check_one.cl(['/nologo','/O2','/MT','/Fe'+str(w/'test.exe')]+[str(s)for s in sources]+[str(fixture)],e,w);assert r.returncode==0,r.stdout+r.stderr
    r=subprocess.run([str(w/'test.exe')],capture_output=True,text=True,timeout=30);assert r.returncode==0 and 'PARSED_NUMBERS_OK' in r.stdout,r.stdout
    (ROOT/'rebuild/compile-gate/parsed-numbers.json').write_text(json.dumps(dict(grade='BEHAVIORAL',x87_control_word='027f',retail_fixtures=rows),indent=2)+'\n');print('PARSED_NUMBERS_PASS cases='+str(len(rows)))
if __name__=='__main__':main()
