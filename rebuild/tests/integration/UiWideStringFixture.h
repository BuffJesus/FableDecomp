// Shared deterministic allocation and snapshot support for wide-string gates.
#include "fable_ui_wide_strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
fable_i32 g_CWideStringInstanceCount_013BCA20;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
static unsigned char records[32][32],buffers[32][1024];
static unsigned recordCount,bufferCount,attempt,failAt,sizes[32];
static unsigned recordSizes[32];
static FableUiWideStringValue values[5];
static unsigned Id(const void* p,const void* base,unsigned stride)
{ return p ? (static_cast<const unsigned char*>(p)-static_cast<const unsigned char*>(base))/stride+1 : 0; }
void* __cdecl FableUiAllocateStringRecord(unsigned n)
{
    printf(" A%u",n); if(++attempt==failAt) { printf(" FAIL"); return 0; }
    if((n!=16 && n!=17) || recordCount==32) abort(); recordSizes[recordCount]=n; return records[recordCount++];
}
void __cdecl FableUiFreeStringRecord(void* p) { printf(" R%u",Id(p,records,32)); }
void* __cdecl FableUiAllocateWideBuffer(unsigned n)
{ printf(" B%u",n); if(n>1000 || bufferCount==32) abort(); sizes[bufferCount]=n; return buffers[bufferCount++]; }
void __cdecl FableUiFreeWideBuffer(void* p) { printf(" F%u",Id(p,buffers,1024)); }
__declspec(noreturn) void __fastcall FableUiWideLengthError(CWideStringData*,void*) { abort(); }
static void Snapshot()
{
    printf(" S:%d",g_CWideStringInstanceCount_013BCA20);
    for(unsigned i=0;i<5;++i) printf(":%u",Id(values[i].Storage,records,32));
    for(unsigned j=0;j<recordCount;++j)
    {
        CWideStringData* d=reinterpret_cast<CWideStringData*>(records[j]);
        if(recordSizes[j]==17)
        {
            CCharStringData* c=reinterpret_cast<CCharStringData*>(d);
            printf(" C%u:%u:%u:%08x:%02x:%d",j,Id(c->text,buffers,1),c->unknown04,c->unknown08,c->flags0C,c->owners);
            for(unsigned cp=17;cp<32;++cp) printf("%02x",records[j][cp]);
            continue;
        }
        printf(" H%u:%u:%u:%u:%d",j,Id(d->text,buffers,1),Id(reinterpret_cast<void*>(d->unknown04),buffers,1),Id(reinterpret_cast<void*>(d->unknown08),buffers,1),d->owners);
        for(unsigned p=16;p<32;++p) printf("%02x",records[j][p]);
    }
    for(unsigned k=0;k<bufferCount;++k)
    { printf(" T%u:",k); for(unsigned x=0;x<sizes[k]+4;++x) printf("%02x",buffers[k][x]); }
}
