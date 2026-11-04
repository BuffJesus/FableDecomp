#include "fable_string_parser.h"
#include <stdio.h>
int main()
{
    struct Case { char* text;unsigned long start;long count; };
    Case cases[]={{"",0,0},{"name",0,0},{"  name",0,2},{"\t\r\nname",0,3},{"   ",0,3},{"x  y",1,2},{"x",1,0}};
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i)
    {
        unsigned long raw[10]={0};CStringParser* p=reinterpret_cast<CStringParser*>(raw);
        p->String=cases[i].text;p->StringPos=cases[i].start;p->InQuotes=true;p->Line=31;
        long count=p->SkipWhitespace();
        if(count!=cases[i].count || p->StringPos!=cases[i].start+count || !p->InQuotes || p->Line!=31)return 1;
    }
    puts("PARSER_WHITESPACE_OK cases=7");return 0;
}
