#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world_map_visibility_aliases.h"
#include "world_map_visibility_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall EntryOne(void*,void*,long,bool);
extern "C" void __fastcall EntryAll(void*,void*,bool);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode,calls;
static void record(void* self,void* list,unsigned visible){
    unsigned h[]={(unsigned)self,(unsigned)list,visible,calls};
    fwrite(h,4,4,output);fwrite(arena,1,4096,output);
}
extern "C" void __fastcall Boundary(void* self,void*,void* list,bool visible){
    ++calls;if(calls>12)exit(8);record(self,list,visible);
    memset(self,visible?0x51:0x93,16);
    if(calls==1){
        if(mode==1)*(unsigned*)(owner+0x24)=*(unsigned*)(owner+0x20);
        if(mode==2){*(unsigned*)(owner+0x20)=(unsigned)(arena+0x600);*(unsigned*)(owner+0x24)=(unsigned)(arena+0x610);}
        if(mode==3)*(unsigned*)(owner+0x24)=*(unsigned*)(owner+0x20)+24;
    }
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+20)=(unsigned)Boundary-(unsigned)(code+24);
    *(unsigned*)(code+64+40)=(unsigned)Boundary-(unsigned)(code+64+44);
    typedef void (__fastcall *One)(void*,void*,long,bool);typedef void (__fastcall *All)(void*,void*,bool);
    unsigned p[5],cases=0;
    while(fread(p,4,5,in)==5){
        if(fread(arena,1,4096,in)!=4096)return 5;
        mode=p[1];owner=arena+p[2];calls=0;
        if(p[0]){All f=argv[3][0]=='r'?(All)(code+64):EntryAll;f(owner,0,p[4]!=0);}
        else {One f=argv[3][0]=='r'?(One)code:EntryOne;f(owner,0,(long)p[3],p[4]!=0);}
        record(0,0,p[4]);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_WORLD_MAP_VISIBILITY_PASS cases=%u\n",cases);return 0;
}
