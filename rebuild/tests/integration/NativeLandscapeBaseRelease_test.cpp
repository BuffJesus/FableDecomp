#include <stdio.h>
#include <string.h>
#include "landscape_base_release_aliases.h"
#include "landscape_base_release_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall Release(void* self,void*,long key){
    record(1,self,(unsigned)key);
    if(mode==1)owner[0x5e]=1;
    if(mode==2){*(void**)(owner+0x20)=arena+0xa00;*(unsigned*)(owner+0x30)=0x80000000;}
    if(mode==3)memset(owner,0x93,0x60);
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+18)=(unsigned)Release-(unsigned)(code+22);
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];owner=arena+p[1];
        entry(owner);record(2,0,0);entry(owner);record(3,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_LANDSCAPE_BASE_RELEASE_PASS cases=%u\n",cases);return 0;
}
