#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display_engine_ctor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructDisplayEngine(void*,void*,const void*);
extern "C" unsigned char DisplayEngineTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned off){return *(unsigned*)(arena+off);}
static void event(unsigned kind,unsigned self,const unsigned* args=0){unsigned h[10]={kind,self,0,0,0,0,0,0,0,0};if(args)memcpy(h+2,args,32);fwrite(h,4,10,output);unsigned save=word(0);if(save==(unsigned)DisplayEngineTable)word(0)=0x1231574;fwrite(arena,1,4096,output);word(0)=save;}
extern "C" void* __fastcall BaseCtorBoundary(void* self){event(1,(unsigned)self);*(unsigned*)self=0x129a7c4;if(mode==1)word(0x500)=0x87654321;if(mode==2)word(0x68)=0x23456789;return self;}
extern "C" void* __fastcall CameraCtorBoundary(void* self,void*,const unsigned* pos,const unsigned* dir,unsigned fov,unsigned locked){
 unsigned args[]={pos[0],pos[1],pos[2],dir[0],dir[1],dir[2],fov,locked};event(2,(unsigned)self,args);
 for(unsigned i=0;i<14;++i)((unsigned*)self)[i]=args[i%8]^(0x12340000+i);
 if(mode==1)word(0x50c)=0xabcdef01;else if(mode==2){word(0x74)=0x76543210;word(0xc0)=0x10293847;}else if(mode==3){word(0x8)=0x55443322;word(0xf8)=0xdeadbeef;}return self;
}
extern "C" void UnusedDisplayEngineCtorBoundary(){exit(99);}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned cases=0;while(fread(&mode,4,1,input)==1){if(fread(arena,1,4096,input)!=4096)return 5;event(3,(unsigned)ConstructDisplayEngine(arena,0,arena+0x500));++cases;}
 fclose(input);fclose(output);printf("NATIVE_DISPLAY_ENGINE_CTOR_PASS cases=%u\n",cases);return 0;
}
