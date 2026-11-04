#include <stdio.h>
#include <string.h>
#include "detail_map_visibility_aliases.h"
#include "detail_map_visibility_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,bool);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self,void* argument){
    unsigned h[]={kind,(unsigned)self,(unsigned)argument};fwrite(h,4,3,output);fwrite(arena,1,4096,output);
}
extern "C" void __fastcall Unload(void* self,void*,void* manager){
    record(1,self,manager);memset(self,0x91,0x58);
    if(mode==1)*(void**)(owner+4)=0;
    if(mode==2)*(void**)(owner+4)=arena+0xa00;
}
extern "C" void __fastcall Finish(void* self){record(2,self,0);memset(self,0x92,32);}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+19)=(unsigned)Unload-(unsigned)(code+23);*(unsigned*)(code+31)=(unsigned)Finish-(unsigned)(code+35);
    typedef void (__fastcall *Call)(void*,void*,bool);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];owner=arena+p[1];entry(owner,0,p[2]!=0);record(3,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_DETAIL_MAP_VISIBILITY_PASS cases=%u\n",cases);return 0;
}
