#include <stdio.h>
#include "engine_camera_angle_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ChangeAngle(void*);
extern "C" void __fastcall StartScene(void*);
extern "C" void* OcclusionGlobal;
int main(int argc,char** argv){
    if(argc!=3||OcclusionGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;
        OcclusionGlobal=arena+p[2];
        if(p[0])ChangeAngle(arena+p[1]);else StartScene(arena+p[2]);
        fwrite(&OcclusionGlobal,4,1,out);fwrite(arena,1,4096,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_ENGINE_CAMERA_ANGLE_PASS cases=%u\n",cases);return 0;
}
