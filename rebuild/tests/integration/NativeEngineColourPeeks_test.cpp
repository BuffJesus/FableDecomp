#include <stdio.h>
#include "engine_colour_peeks_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Diffuse(void*,void*,void*);
extern "C" void* __fastcall Ambient(void*,void*,void*);
extern "C" void* LightingGlobal;
int main(int argc,char** argv){
    if(argc!=3||LightingGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,20480,in)!=20480)return 5;
        LightingGlobal=arena;
        void* result=p[0]?Ambient((void*)0x1badb002,0,arena+p[1]):Diffuse((void*)0x1badb002,0,arena+p[1]);
        fwrite(&result,4,1,out);fwrite(arena,1,20480,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_ENGINE_COLOUR_PEEKS_PASS cases=%u\n",cases);return 0;
}
