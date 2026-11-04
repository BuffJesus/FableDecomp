#include <stdio.h>
#include "engine_environment_flag_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SetEnvironmentFlag(void*,void*,bool);
extern "C" void* EnvironmentGlobal;
int main(int argc,char** argv){
    if(argc!=3||EnvironmentGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    void* arena=VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;
        EnvironmentGlobal=(void*)p[1];SetEnvironmentFlag((void*)p[0],0,p[2]!=0);
        fwrite(&EnvironmentGlobal,4,1,out);fwrite(arena,1,4096,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_ENGINE_ENVIRONMENT_FLAG_PASS cases=%u\n",cases);return 0;
}
