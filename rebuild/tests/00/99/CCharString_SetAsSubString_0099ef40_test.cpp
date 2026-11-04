#include "fable_string.h"
#include <stdio.h>
static int releases,allocations;static long observedLength;static const char* observedText;
void CCharString::UnassignString(){++releases;storage_=0;}
CCharStringData* CCharString::AllocStringData(const char* text,long length){++allocations;observedText=text;observedLength=length;return reinterpret_cast<CCharStringData*>(0x12345678);}
int main(){struct Case{const char* text;long length;bool owned;bool allocate;}cases[]={{0,3,false,false},{"",3,true,false},{"abc",2,false,true},{"abc",0,true,true},{"abc",-1,true,true}};
 for(unsigned i=0;i<5;++i){unsigned long raw=cases[i].owned?0x87654321:0;CCharString* p=reinterpret_cast<CCharString*>(&raw);releases=allocations=0;p->SetAsSubString(cases[i].text,cases[i].length);
 if(releases!=(cases[i].owned?1:0)||allocations!=(cases[i].allocate?1:0)||raw!=(cases[i].allocate?0x12345678u:0u))return 1;
 if(allocations&&(observedText!=cases[i].text||observedLength!=cases[i].length))return 2;}
 puts("CHAR_SUBSTRING_OK cases=5");return 0;}
