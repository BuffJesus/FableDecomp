#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "engine_world_frames_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall WorldFrame(void*,void*,bool);
extern "C" void __fastcall PausedFrame(void*);
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned self=0,unsigned arg=0){unsigned h[]={kind,self,arg};fwrite(h,4,3,output);fwrite(arena,1,2048,output);}
static unsigned index(void* self){if(self<arena+0x400||self>=arena+0x560)exit(10);return (((unsigned)self-(unsigned)arena)&255)/32;}
extern "C" unsigned __fastcall PeekFlagsBoundary(void* self){unsigned idx=index(self);event(1,(unsigned)self);unsigned value=((unsigned*)self)[1];if(mode==1){word(0xb8)^=1;word(0x600+idx*4)=(unsigned)(arena+0x500+idx*32);}if(mode==3){arena[0x84]=0;word(0x88)=0;}return value;}
static void update(void* self,bool paused,unsigned first){unsigned idx=index(self);event(paused?3:2,(unsigned)self,first);if(mode==2){word(0xb0)=2;word(paused?0x90:0x8c)=0xffffffff;word(paused?0xa0:0x98)=0;word(paused?0xa4:0x9c)=0x3ff00000;word(0x16c)=(unsigned)(arena+0x600+(idx+1)*4);}}
extern "C" void __fastcall WorldBoundary(void* self,void*,bool first){update(self,false,first);}
extern "C" void __fastcall PausedBoundary(void* self){update(self,true,0);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8028)=(unsigned)&PeekFlagsBoundary;word(0x801c)=(unsigned)&WorldBoundary;word(0x8020)=(unsigned)&PausedBoundary;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(arena,1,2048,input)!=2048)return 5;mode=p[0];_clear87();_control87(_MCW_EM|_RC_NEAR|(p[3]==0x27f?_PC_53:_PC_64),_MCW_EM|_MCW_RC|_MCW_PC);if(p[1]==0)WorldFrame(arena,0,p[2]!=0);else PausedFrame(arena);event(4);++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_WORLD_FRAMES_PASS cases=%u\n",cases);return 0;}
