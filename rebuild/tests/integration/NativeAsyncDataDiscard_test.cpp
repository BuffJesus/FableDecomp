#include <stdio.h>
#include <string.h>
#include "async_data_discard_aliases.h"
#include "async_data_discard_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*);
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self,void* argument){unsigned h[]={kind,(unsigned)self,(unsigned)argument};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall Discard(void* self,void*,void* argument){
    record(1,self,argument);memset(self,0x93,32);
    if(mode==1)*(void**)argument=0;
    else *(void**)argument=arena+0x800;
    ((unsigned*)argument)[1]=0xfedcba98;
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+11)=(unsigned)Discard-(unsigned)(code+15);
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];
        for(unsigned i=0;i<2;++i){entry(arena+p[1]);record(2,0,0);}++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_ASYNC_DATA_DISCARD_PASS cases=%u\n",cases);return 0;
}
