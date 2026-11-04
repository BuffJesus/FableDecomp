#include <stdio.h>
#include <string.h>
#include <float.h>
#include "lighting_setters_aliases.h"
#include "lighting_setters_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall EngineGlobal(void*,void*,const void*,const void*,const void*,const void*);
extern "C" void __fastcall Engine2D(void*,void*,const void*,const void*,const void*,const void*);
extern "C" void __fastcall OwnerGlobal(void*,void*,const void*,const void*,const void*,const void*);
extern "C" void __fastcall Owner2D(void*,void*,const void*,const void*,const void*,const void*);
extern "C" void* __fastcall Normalize(void*);
extern "C" void* LightingGlobal;
extern "C" void* RendererGlobal;
static unsigned char* code;
static unsigned resolve(unsigned target){
    if(target==0x1436e9c)return(unsigned)&LightingGlobal;
    if(target==0x1436e7c)return(unsigned)&RendererGlobal;
    if(target==0xa14440)return(unsigned)Normalize;
    if(target==0xb47280)return(unsigned)(code+512);
    if(target==0xb4a5f0)return(unsigned)(code+768);
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||LightingGlobal||RendererGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;
    memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0],target=resolve(retailFixups[i][1]);
        if(retailFixups[i][2])target-=(unsigned)(code+offset+4);
        *(unsigned*)(code+offset)=target;
    }
    typedef void (__fastcall *Setter)(void*,void*,const void*,const void*,const void*,const void*);
    Setter nativeFunctions[]={(Setter)(code+512),(Setter)code,(Setter)(code+768),(Setter)(code+256)};
    Setter compiledFunctions[]={OwnerGlobal,EngineGlobal,Owner2D,Engine2D};
    unsigned p[7],cases=0;
    while(fread(p,4,7,in)==7){
        if(fread(arena,1,49152,in)!=49152)return 5;
        LightingGlobal=RendererGlobal=arena+p[1];
        _clearfp();_control87(p[6]?_PC_64:_PC_53,_MCW_PC);
        Setter f=argv[3][0]=='r'?nativeFunctions[p[0]]:compiledFunctions[p[0]];
        f((p[0]&1)?(void*)0x1badb002:arena+p[1],0,arena+p[2],arena+p[3],arena+p[4],arena+p[5]);
        fwrite(&LightingGlobal,4,1,out);fwrite(&RendererGlobal,4,1,out);fwrite(arena,1,49152,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_LIGHTING_SETTERS_PASS cases=%u\n",cases);return 0;
}
