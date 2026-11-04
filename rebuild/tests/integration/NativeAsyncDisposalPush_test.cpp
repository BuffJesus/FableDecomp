#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "async_disposal_push_aliases.h"
#include "async_disposal_push_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,void*);
static unsigned char* arena;
static unsigned char* list;
static unsigned char* value;
static FILE* output;
static unsigned mode,allocations;
static void record(unsigned kind,unsigned argument){unsigned h[]={kind,argument,allocations};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void* __cdecl PushMalloc(unsigned bytes){
    record(1,bytes);if(bytes!=16||allocations>=2)exit(8);
    if(mode==1){*(void**)value=arena+0xc00;*(void**)(value+4)=arena+0xb00;}
    if(mode==2)*(void**)list=arena+0x300;
    void* result=arena+0x900+32*allocations++;memset(result,0x95,16);return result;
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+7)=(unsigned)PushMalloc-(unsigned)(code+11);
    typedef void (__fastcall *Call)(void*,void*,void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];list=arena+p[1];value=arena+p[2];allocations=0;
        for(unsigned i=0;i<2;++i){entry(list,0,value);record(2,i);}++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_ASYNC_DISPOSAL_PUSH_PASS cases=%u\n",cases);return 0;
}
