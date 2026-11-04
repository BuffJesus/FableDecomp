#include <stdio.h>
#include <string.h>
#include "water_close_aliases.h"
#include "water_close_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* WaterGlobal;
extern "C" void __fastcall Entry(void*);
extern "C" void __fastcall World(void*);
extern "C" void __fastcall Wait(void*,void*,void*,float);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode;
static void put(unsigned off,unsigned value){*(unsigned*)(owner+off)=value;}
static void record(unsigned kind,void* self,unsigned arg=0,unsigned extra=0){unsigned h[]={kind,(unsigned)self,arg,extra,(unsigned)WaterGlobal};fwrite(h,4,5,output);fwrite(arena,1,8192,output);}
extern "C" void* __fastcall System(){record(1,0);if(mode==1)put(0x5b8,0);if(mode==2)put(0x5b8,(unsigned)(arena+0x1200));return arena+0x1c00;}
extern "C" unsigned __stdcall ComRelease(void* self){record(2,self);if(mode==3)put(0x5bc,(unsigned)(arena+0x1880));return 7;}
extern "C" void __fastcall Discard(void* self,void*,void* slot){record(3,self,(unsigned)slot);if(mode==4){put(0x5bc,(unsigned)(arena+0x1880));put(0x5b4,(unsigned)(arena+0x18c0));}}
extern "C" void __fastcall DeleteData(void* self){record(4,self);if(mode==5){if(self==arena+0x1200)put(0x5bc,(unsigned)(arena+0x1880));else put(0x5b4,(unsigned)(arena+0x18c0));}}
extern "C" void __cdecl FreeInfo(void* self){record(5,self);if(self)memset(self,0x95,12);if(mode==6){put(0x5a8,0xdeadbeef);owner[0x276]=1;}}
void __cdecl operator delete(void* self){FreeInfo(self);}
extern "C" void __fastcall MapClose(void* self){record(6,self);}
extern "C" void __fastcall BankClose(void* self){record(7,self);}
int main(int argc,char** argv){
    if(argc!=4||WaterGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;owner=arena+0x400;
    unsigned* table=(unsigned*)VirtualAlloc((void*)0x25000000,4096,0x3000,4);if(table!=(void*)0x25000000)return 7;table[2]=(unsigned)ComRelease;table[9]=(unsigned)BankClose;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned fix=retailFixups[i][0],target=retailFixups[i][1],value=0;
        switch(target){
        case 0xa63200:value=(unsigned)(code+384);break;
        case 0x9a4ec0:value=(unsigned)System;break;
        case 0xa0aa70:value=(unsigned)(code+448);break;
        case 0xac1ee0:value=(unsigned)(code+192);break;
        case 0x9d68a0:value=(unsigned)(code+224);break;
        case 0xbfe9bc:value=(unsigned)FreeInfo;break;
        case 0x9d67e0:value=(unsigned)Discard;break;
        case 0xb3ef40:value=(unsigned)MapClose;break;
        case 0xb6db80:value=(unsigned)code;break;
        case 0x1436e54:value=(unsigned)&WaterGlobal;break;
        default:return 8;
        }
        *(unsigned*)(code+fix)=target==0x1436e54?value:value-(unsigned)(code+fix+4);
    }
    typedef void (__fastcall *Call)(void*);Call entry=argv[3][0]=='r'?(Call)code:Entry;Call world=argv[3][0]=='r'?(Call)(code+256):World;
    typedef void (__fastcall *WaitCall)(void*,void*,void*,float);WaitCall wait=argv[3][0]=='r'?(WaitCall)(code+448):Wait;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];WaterGlobal=owner;
        for(unsigned i=0;i<4;++i)*(void**)(arena+0x1804+i*0x40)=(void*)DeleteData;
        float timeout;memcpy(&timeout,owner+0xb0,4);wait(arena+0x1e00,0,(void*)(0x1000+mode),timeout);
        for(unsigned i=0;i<2;++i){if(p[1])world(arena);else entry(owner);record(8,0,i);}++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_WATER_CLOSE_PASS cases=%u\n",cases);return 0;
}
