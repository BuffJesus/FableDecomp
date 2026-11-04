#include <stdio.h>
#include <string.h>
#include "world_map_close_aliases.h"
#include "world_map_close_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* WaterGlobal;
extern "C" void __fastcall Entry(void*);
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void put(unsigned offset,unsigned value){*(unsigned*)(arena+offset)=value;}
static void record(unsigned kind,void* self){unsigned h[]={kind,(unsigned)self,(unsigned)WaterGlobal};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall MapClose(void* self){
    record(1,self);memset(self,0x91,16);
    if(mode==1)put(0x24,*(unsigned*)(arena+0x20)+4);
    if(mode==2)put(0x24,*(unsigned*)(arena+0x20)+16);
    if(mode==3){unsigned n=*(unsigned*)(arena+0x24)-*(unsigned*)(arena+0x20);put(0x20,(unsigned)(arena+0x700));put(0x24,(unsigned)(arena+0x700)+n);}
    if(mode==4)WaterGlobal=0;
    if(mode==5)WaterGlobal=arena+0xe00;
}
extern "C" void __fastcall WaterClose(void* self){record(2,self);memset(self,0x92,16);if(mode==5)put(0x34,(unsigned)(arena+0xf40));}
extern "C" void __fastcall BankClose(void* self){record(3,self);put(0x1a8,0xffffffff);put(0x1b0,0xabcdef01);}
extern "C" void __fastcall AltBankClose(void* self){record(4,self);put(0x1a8,1);put(0x1b0,2);}
int main(int argc,char** argv){
    if(argc!=4||WaterGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+43)=(unsigned)MapClose-(unsigned)(code+47);*(unsigned*)(code+63)=(unsigned)&WaterGlobal;*(unsigned*)(code+73)=(unsigned)WaterClose-(unsigned)(code+77);
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];WaterGlobal=p[1]?arena+p[1]:0;
        put(0xf14,(unsigned)BankClose);put(0xf54,(unsigned)AltBankClose);
        entry(arena);record(5,0);entry(arena);record(6,0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_WORLD_MAP_CLOSE_PASS cases=%u\n",cases);return 0;
}
