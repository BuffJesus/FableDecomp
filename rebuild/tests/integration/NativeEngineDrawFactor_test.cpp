#include <stdio.h>
#include <string.h>
#include <float.h>
#include "engine_draw_factor_aliases.h"
#include "engine_draw_factor_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" float __fastcall DrawFactor(void*);
extern "C" void* DefinitionGlobal;
extern "C" void* RendererGlobal;
static const float one=1.0f;
static unsigned resolve(unsigned target){
    if(target==0x1436e20)return(unsigned)&DefinitionGlobal;
    if(target==0x1436e80)return(unsigned)&RendererGlobal;
    if(target==0x122ded8)return(unsigned)&one;
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||DefinitionGlobal||RendererGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0];*(unsigned*)(code+offset)=resolve(retailFixups[i][1]);
    }
    typedef float (__fastcall *Getter)(void*);
    Getter getter=argv[3][0]=='r'?(Getter)code:DrawFactor;unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;
        DefinitionGlobal=arena+p[0];RendererGlobal=arena+p[1];
        _clearfp();_control87(p[2],_MCW_PC|_MCW_RC);
        double result=getter((void*)0x1badb002);
        unsigned status=_statusfp(),control=_control87(0,0);
        fwrite(&result,8,1,out);fwrite(&status,4,1,out);fwrite(&control,4,1,out);
        fwrite(&DefinitionGlobal,4,1,out);fwrite(&RendererGlobal,4,1,out);fwrite(arena,1,4096,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_ENGINE_DRAW_FACTOR_PASS cases=%u\n",cases);return 0;
}
