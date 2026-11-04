#include <stdio.h>
#include <string.h>
#include "game_map_load_aliases.h"
#include "game_map_load_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* MapGlobal;
extern "C" void __fastcall Entry(void*,void*,long);
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static unsigned index;
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg,(unsigned)MapGlobal};fwrite(h,4,4,output);fwrite(arena,1,4096,output);}
extern "C" void* __fastcall GetInfo(void* self,void*,void* result,long id){
    record(1,self,id);memcpy(result,arena+0xe00,32);
    if(mode==1)*(void**)(arena+0x20)=arena+0x340;
    if(mode==2)MapGlobal=arena+0x200;
    if(mode==3)*(void**)(arena+0x400+index*0x100)=arena+0x900;
    if(mode==4)*(void**)(arena+0x308+index*4)=arena+0x700;
    if(mode==5)memset(arena+0x400+index*0x100,0x95,0x60);
    return result;
}
extern "C" void __fastcall DestroyString(void* self){
    record(2,0,*(unsigned*)self);fwrite((unsigned char*)self-8,1,32,output);
    memset(arena+0xf00,0x92,32);
}
int main(int argc,char** argv){
    if(argc!=4||MapGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+9)=(unsigned)&MapGlobal;*(unsigned*)(code+47)=(unsigned)(code+96)-(unsigned)(code+51);*(unsigned*)(code+56)=(unsigned)DestroyString-(unsigned)(code+60);
    typedef void (__fastcall *Call)(void*,void*,long);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];index=p[1];MapGlobal=arena+p[2];
        *(void**)(arena+0xd04)=(void*)GetInfo;entry(arena,0,(long)index-1);record(3,0,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_GAME_MAP_LOAD_PASS cases=%u\n",cases);return 0;
}
