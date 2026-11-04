#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "fable_display_engine.h"
#include "display_engine_update_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall UpdateDisplayEngine(void*);
extern "C" unsigned TipText=0,TipTitle=0;
#define FadeRange (*(unsigned*)&CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio)
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};unsigned globals[]={TipText,TipTitle,FadeRange};fwrite(h,4,4,output);fwrite(globals,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall StopProgressBoundary(bool a,bool b){event(1,a,b);if(mode==1)word(0xe8)=2;else if(mode==2)word(0x2c)=0;else if(mode==3)word(0xe8)=0xffffffff;}
extern "C" void* __fastcall AssignWideBoundary(void* self,void*,const unsigned short* text){if(*text)exit(10);unsigned label=self==&TipText?1:self==&TipTitle?2:0;if(!label)exit(11);event(2,label);*(unsigned*)self=(unsigned)(arena+0xa00+label*16);if(mode==4)word(0xe8)=label==1?2:0;else if(mode==5&&label==2)word(0x2c)=(unsigned)(arena+0x440);return self;}
extern "C" void __fastcall AboutToRunBoundary(void* self){event(3,(unsigned)self);if(mode==6)word(0x2c)=0;else if(mode==7){word(0xe8)=0;word(0x2c)=(unsigned)(arena+0x440);}}
extern "C" float __fastcall DrawDistanceBoundary(void* self){event(4,(unsigned)self);return *(float*)(arena+(self==arena+0x440?0x604:0x600));}
int main(int argc,char** argv){
 if(!GInitialFadeAllowedToBeActive || FadeRange!=0)return 12;
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x804c)=(unsigned)&DrawDistanceBoundary;
 unsigned p[2],g[3],cases=0;while(fread(p,4,2,input)==2){if(fread(g,4,3,input)!=3||fread(arena,1,4096,input)!=4096)return 5;mode=p[0];TipText=g[0];TipTitle=g[1];FadeRange=g[2];_clear87();_control87(_MCW_EM | _RC_NEAR | (p[1]==0x27f?_PC_53:_PC_64),_MCW_EM|_MCW_RC|_MCW_PC);UpdateDisplayEngine(arena);event(5);++cases;}
 fclose(input);fclose(output);printf("NATIVE_DISPLAY_ENGINE_UPDATE_PASS cases=%u\n",cases);return 0;
}
