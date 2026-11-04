#include <stdio.h>
#include "shader_camera_position_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SetPosition(void*,void*,const void*);
int main(int argc,char** argv){
    if(argc!=3)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned offset,cases=0;
    while(fread(&offset,4,1,in)==1){if(fread(arena,1,2048,in)!=2048)return 5;SetPosition(arena,0,arena+offset);fwrite(arena,1,2048,out);++cases;}
    fclose(in);fclose(out);printf("NATIVE_SHADER_CAMERA_POSITION_PASS cases=%u\n",cases);return 0;
}
