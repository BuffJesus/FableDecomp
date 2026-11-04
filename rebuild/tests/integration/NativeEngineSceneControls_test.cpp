#include <stdio.h>
#include <stdlib.h>
#include "engine_scene_controls_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall StartScene(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Preload(void*,void*,bool,bool);
extern "C" unsigned char __fastcall IsPreloaded(void*);
extern "C" void __fastcall Enable3D(void*,void*,bool);
extern "C" void __fastcall Enable2D(void*,void*,bool);
extern "C" unsigned char __fastcall Is3D(void*);
extern "C" unsigned char __fastcall Is2D(void*);
static unsigned char* arena;static FILE* output;
static void event(unsigned kind,unsigned value=0){unsigned h[]={kind,value};fwrite(h,4,2,output);fwrite(arena,1,512,output);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(arena,1,512,input)!=512)return 5;bool fast=p[0]!=0,unused=p[1]!=0;event(1,IsPreloaded(arena));Preload(arena,0,fast,unused);event(2);event(3,IsPreloaded(arena));StartScene(arena,0,p[2],p[3]);event(4);Preload(arena,0,!fast,!unused);event(5);event(6,IsPreloaded(arena));Enable3D(arena,0,fast);event(7,Is3D(arena));Enable2D(arena,0,unused);event(8,Is2D(arena));Enable3D(arena,0,!fast);event(9,Is3D(arena));Enable2D(arena,0,!unused);event(10,Is2D(arena));++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_SCENE_CONTROLS_PASS cases=%u\n",cases);return 0;}
