#include <stdio.h>
#include "shader_matrices_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall World(void*,void*,const void*);
extern "C" void __fastcall View(void*,void*,const void*);
extern "C" void __fastcall Projection(void*,void*,const void*);
extern "C" void __fastcall Transpose(void*);
int main(int argc,char** argv){
    if(argc!=3)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,2048,in)!=2048)return 5;
        if(p[0]==0)World(arena,0,arena+p[1]);
        else if(p[0]==1)View(arena,0,arena+p[1]);
        else if(p[0]==2)Projection(arena,0,arena+p[1]);
        else Transpose(arena+p[1]);
        fwrite(arena,1,2048,out);
        if(p[0]==3){Transpose(arena+p[1]);fwrite(arena,1,2048,out);}
        ++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_SHADER_MATRICES_PASS cases=%u\n",cases);return 0;
}
