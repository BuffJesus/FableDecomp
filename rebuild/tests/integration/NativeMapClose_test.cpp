#include <stdio.h>
#include <string.h>
#include "map_close_aliases.h"
#include "map_close_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* WaterGlobal;
extern "C" void __fastcall Entry(void*);
extern "C" void __fastcall World(void*);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self){unsigned h[]={kind,(unsigned)self,(unsigned)WaterGlobal};fwrite(h,4,3,output);fwrite(arena,1,8192,output);}
extern "C" void __fastcall Landscape(void* self){
    record(1,self);if(mode==1)*(void**)(owner+4)=arena+0xb00;
    if(mode==2)*(void**)(owner+4)=0;
}
extern "C" void __fastcall Detail(void* self){record(2,self);if(mode==3)*(void**)(owner+0x40)=arena+0xe40;}
extern "C" void __fastcall DeleteData(void* self){record(3,self);if(mode==4)*(void**)(owner+0x40)=arena+0xe40;}
extern "C" void __cdecl FreeInfo(void* self){record(4,self);if(self)memset(self,0x94,12);if(mode==5)memset(owner+0x20,0x95,0x24);}
void __cdecl operator delete(void* self){FreeInfo(self);}
extern "C" void __fastcall WaterClose(void* self){record(5,self);}
extern "C" void __fastcall BankClose(void* self){record(6,self);*(unsigned*)(arena+0x1a8)=2;}
int main(int argc,char** argv){
    if(argc!=4||WaterGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+17)=(unsigned)Landscape-(unsigned)(code+21);*(unsigned*)(code+29)=(unsigned)Detail-(unsigned)(code+33);*(unsigned*)(code+61)=(unsigned)FreeInfo-(unsigned)(code+65);
    *(unsigned*)(code+171)=(unsigned)code-(unsigned)(code+175);*(unsigned*)(code+191)=(unsigned)&WaterGlobal;*(unsigned*)(code+201)=(unsigned)WaterClose-(unsigned)(code+205);
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;Call world=argv[3][0]=='r'?(Call)(code+128):World;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];owner=arena+p[1];WaterGlobal=0;
        *(void**)(arena+0xe04)=(void*)DeleteData;*(void**)(arena+0xe44)=(void*)DeleteData;*(void**)(arena+0x314)=(void*)BankClose;
        for(unsigned i=0;i<2;++i){if(p[2])world(arena);else entry(owner);record(7,0);}++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_MAP_CLOSE_PASS cases=%u\n",cases);return 0;
}
