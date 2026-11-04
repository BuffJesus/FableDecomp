#include <stdio.h>
#include <float.h>
#include <string.h>
#include "camera_fov_getters_aliases.h"
#include "camera_fov_retail_oracle.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned __fastcall Flags(void*);
extern "C" float __fastcall Horizontal(void*);
extern "C" float __fastcall Vertical(void*);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,0x1000,0x3000,0x40);if(!code)return 6;
    memcpy(code,retailFlags,4);memcpy(code+16,retailHorizontal,4);memcpy(code+32,retailVertical,4);
    typedef unsigned (__fastcall *FlagsFunction)(void*);
    typedef float (__fastcall *FloatFunction)(void*);
    FlagsFunction flagsFunction=argv[3][0]=='r'?(FlagsFunction)code:Flags;
    FloatFunction horizontalFunction=argv[3][0]=='r'?(FloatFunction)(code+16):Horizontal;
    FloatFunction verticalFunction=argv[3][0]=='r'?(FloatFunction)(code+32):Vertical;
    unsigned control,cases=0;
    while(fread(&control,4,1,in)==1){
        if(fread(arena,1,256,in)!=256)return 5;
        _clearfp();_control87(control? _PC_64:_PC_53,_MCW_PC);
        unsigned flags=flagsFunction(arena+64);float h=horizontalFunction(arena+64),v=verticalFunction(arena+64);
        fwrite(&flags,4,1,out);fwrite(&h,4,1,out);fwrite(&v,4,1,out);fwrite(arena,1,256,out);++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_CAMERA_FOV_GETTERS_PASS cases=%u\n",cases);return 0;
}
