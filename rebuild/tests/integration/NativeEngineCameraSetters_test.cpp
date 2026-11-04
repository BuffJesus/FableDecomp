#include <stdio.h>
#include "engine_camera_setters_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SetCamera(void*,void*,unsigned);
extern "C" void __fastcall SetWindow(void*,void*,unsigned);
extern "C" void __fastcall SetGameCamera(void*,void*,unsigned);
extern "C" void __fastcall SetGameWindow(void*,void*,unsigned);
extern "C" void* CameraGlobal;
static unsigned char* arena;
static unsigned mode;
static FILE* output;
static void put(unsigned char* p,unsigned v){*(unsigned*)p=v;}
static void event(unsigned kind,void* self=0,unsigned arg=0){unsigned h[]={kind,(unsigned)self,arg,(unsigned)CameraGlobal};fwrite(h,4,4,output);fwrite(arena,1,2048,output);}
extern "C" void __fastcall ProcessBoundary(unsigned char* self,void*,unsigned force){
    if(force!=1)throw 1;
    event(2,self,force);
    if(mode&2){put(arena+0x14,0xabcdef01);put(self+0x10,0x76543210);CameraGlobal=arena+0x500;}
}
#ifndef COMPOSED
extern "C" void __fastcall CameraBoundary(unsigned char* self,void*,unsigned arg){
    event(1,self,arg);put(self+12,arg);
    if(mode&1){CameraGlobal=arena+0x500;put(arena+0x14,0x12345678);}
}
extern "C" void __fastcall WindowBoundary(unsigned char* self,void*,unsigned arg){
    event(3,self,arg);put(self+16,arg);
    if(mode&1){CameraGlobal=arena+0x500;put(arena+0x14,0x12345678);}
}
#endif
int main(int argc,char** argv){
    if(argc!=3||CameraGlobal)return 2;
    FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned p[3],cases=0;
    while(fread(p,4,3,input)==3){
        if(fread(arena,1,2048,input)!=2048)return 5;
        mode=p[0];CameraGlobal=(void*)p[1];
        SetCamera(arena,0,p[2]);event(4);SetWindow(arena,0,p[2]);event(5);
#ifdef COMPOSED
        SetGameCamera(arena+0x500,0,p[2]);event(6);SetGameWindow(arena+0x500,0,p[2]);event(7);
#endif
        ++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_ENGINE_CAMERA_SETTERS_PASS cases=%u\n",cases);return 0;
}
