#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_visibility_aliases.h"
#include "map_visibility_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,void*,bool);
extern "C" void __fastcall OwnerOne(void*,void*,long,bool);
extern "C" void __fastcall OwnerAll(void*,void*,bool);
static unsigned char* arena;
static unsigned char* map;
static FILE* output;
static unsigned mode,allocations;
static void record(unsigned kind,void* self,unsigned argument){
    unsigned h[]={kind,(unsigned)self,argument,allocations};fwrite(h,4,4,output);fwrite(arena,1,4096,output);
}
extern "C" void* __cdecl MapMalloc(unsigned n){
    record(1,0,n);if(n!=12||allocations>=4)exit(8);
    void* result=arena+0xa00+32*allocations++;memset(result,0x9b,n);return result;
}
extern "C" void __cdecl MapFree(void* p){record(2,p,0);memset(p,0xdd,12);}
extern "C" void __fastcall Landscape(void* self,void*,bool visible){
    record(3,self,visible);memset(self,0x83+visible,16);
    if(mode==1)*(unsigned*)(map+4)=(unsigned)(arena+0xd00);
    if(mode==2)*(unsigned*)(map+4)=0;
}
extern "C" void __fastcall Detail(void* self,void*,bool visible){record(4,self,visible);memset(self,0x93+visible,16);}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    void* targets[]={(void*)MapMalloc,(void*)MapFree,(void*)Landscape,(void*)Detail};unsigned offsets[]={33,95,123,136};
    for(unsigned i=0;i<4;++i)*(unsigned*)(code+offsets[i])=(unsigned)targets[i]-(unsigned)(code+offsets[i]+4);
    *(unsigned*)(code+256+20)=(unsigned)code-(unsigned)(code+256+24);
    *(unsigned*)(code+320+40)=(unsigned)code-(unsigned)(code+320+44);
    typedef void (__fastcall *Call)(void*,void*,void*,bool);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    typedef void (__fastcall *One)(void*,void*,long,bool);One one=argv[3][0]=='r'?(One)(code+256):OwnerOne;
    typedef void (__fastcall *All)(void*,void*,bool);All all=argv[3][0]=='r'?(All)(code+320):OwnerAll;
    unsigned p[4],cases=0;
    while(fread(p,4,4,in)==4){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];map=arena+p[1];allocations=0;
        for(unsigned j=0;j<3;++j){
            bool visible=((p[2]>>j)&1)!=0;
            if(p[3]==0)entry(map,0,arena+0x300,visible);
            else if(p[3]==1)one(arena+0x2d4,0,1,visible);
            else all(arena+0x2d4,0,visible);
            record(5,0,j);
        }
        ++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_MAP_VISIBILITY_PASS cases=%u\n",cases);return 0;
}
