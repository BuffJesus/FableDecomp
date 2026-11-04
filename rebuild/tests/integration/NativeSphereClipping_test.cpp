#include <stdio.h>
#include <string.h>
#include <float.h>
#include "sphere_clipping_aliases.h"
#include "sphere_clipping_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall ClipSphere(void*,void*,const void*);
extern "C" void* CameraGlobal;
int main(int argc,char** argv){
    if(argc!=4||CameraGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+cameraFixup)=(unsigned)&CameraGlobal;
    typedef bool (__fastcall *Clipper)(void*,void*,const void*);
    Clipper clip=argv[3][0]=='r'?(Clipper)code:ClipSphere;unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;
        CameraGlobal=arena+p[0];_clearfp();_control87(p[2],_MCW_PC|_MCW_RC);
        unsigned result=clip((void*)0x1badb002,0,arena+p[1]);unsigned status=_statusfp(),control=_control87(0,0);
        fwrite(&result,4,1,out);fwrite(&status,4,1,out);fwrite(&control,4,1,out);fwrite(&CameraGlobal,4,1,out);fwrite(arena,1,4096,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_SPHERE_CLIPPING_PASS cases=%u\n",cases);return 0;
}
