#include <stdio.h>
#include <string.h>
#include "resource_getters_aliases.h"
#include "resource_getters_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Loading(void*);
extern "C" void* __fastcall Animation(void*);
extern "C" void* __fastcall Scratch(void*);
extern "C" void* ResourceGlobal;
static unsigned char* arena;
static FILE* output;
static unsigned mode,owner,info;
static void put(unsigned offset,unsigned value){*(unsigned*)(arena+offset)=value;}
static void record(unsigned kind,void* p){
    unsigned values[]={kind,(unsigned)p,(unsigned)ResourceGlobal};
    fwrite(values,4,3,output);fwrite(arena,1,32768,output);
}
extern "C" void __fastcall ReleaseData(void* p){
    record(1,p);
    if(mode&1){
        put(owner+0x8e0,0x87654321);put(owner+0x8f0,0x12345678);
        put(info+8,0xabcdef01);ResourceGlobal=arena+(owner?0:0x4000);
    }
    if(mode&2){put(info,99);put(owner+0x8e4,0);put(owner+0x8f4,0);}
}
void __cdecl operator delete(void* p){
    record(2,p);if(mode&2)memset(p,0xe7,12);
}
static unsigned resolve(unsigned target){
    if(target==0x1436e98)return(unsigned)&ResourceGlobal;
    if(target==0xbfe9bc)return(unsigned)&operator delete;
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||ResourceGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0],target=resolve(retailFixups[i][1]);
        if(retailFixups[i][2])target-=(unsigned)(code+offset+4);
        *(unsigned*)(code+offset)=target;
    }
    typedef void* (__fastcall *Getter)(void*);
    Getter nativeFunctions[]={(Getter)code,(Getter)(code+256),(Getter)(code+512)};
    Getter compiledFunctions[]={Loading,Animation,Scratch};unsigned p[4],cases=0;
    while(fread(p,4,4,in)==4){
        if(fread(arena,1,32768,in)!=32768)return 5;
        mode=p[1];owner=p[2];info=p[3];ResourceGlobal=arena+owner;
        if(info)put(info+4,(unsigned)ReleaseData);
        Getter getter=argv[3][0]=='r'?nativeFunctions[p[0]]:compiledFunctions[p[0]];
        void* result=getter((void*)0x1badb002);record(3,result);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_ENGINE_RESOURCE_GETTERS_PASS cases=%u\n",cases);return 0;
}
