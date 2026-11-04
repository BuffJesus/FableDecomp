#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_engine.h"
#include "display_about_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall AboutToRun(void*);
#define FadeAllowed GInitialFadeAllowedToBeActive
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0){unsigned h[]={kind,a,b,FadeAllowed};fwrite(h,4,4,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall FadeUpdate(void* self,void*,float elapsed){unsigned bits=*(unsigned*)&elapsed;event(1,(unsigned)self,bits);if(self!=arena+0xbc||bits!=0x3dcccccd)exit(10);if(mode==1)memset(arena+0xbc,0xa5,28);else if(mode==2){arena[0xd8]=1;FadeAllowed=false;}else if(mode==3){word(0xf0)=0x7fc12345;word(0xf4)=0x12345678;arena[0xec]=0;}}
extern "C" void __fastcall EndProfile(long id){event(2,id);if(id!=1)exit(11);if(mode==4){arena[0xec]=0;word(0xf4)=0x87654321;}}
int main(int argc,char** argv){
 if(!FadeAllowed || CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio!=0.0f)return 12;
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[2],cases=0;while(fread(p,4,2,input)==2){if(fread(arena,1,4096,input)!=4096)return 5;mode=p[0];FadeAllowed=p[1]!=0;AboutToRun(arena);event(3);++cases;}
 fclose(input);fclose(output);printf("NATIVE_DISPLAY_ABOUT_TO_RUN_PASS cases=%u\n",cases);return 0;
}
