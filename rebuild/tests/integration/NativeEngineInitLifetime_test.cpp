#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_init_lifetime_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructInit(void*);
extern "C" void __fastcall DestroyInit(void*);
extern "C" unsigned WideCount;
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned label=0){unsigned h[]={kind,label,WideCount};fwrite(h,4,3,output);fwrite(arena,1,256,output);}
extern "C" void* __fastcall WideConstruct(void* self){unsigned label=((unsigned char*)self-arena-0x20)/4;if(label>=5)exit(10);event(1,label);*(unsigned*)self=(unsigned)(arena+0x300+label*16);if(mode==1){word(0x34)=0x12345678;word(0x60)=0x88776655;}else if(mode==2)word(label*4)=(unsigned)(arena+0x700+label*16);else if(mode==3&&label<4)*((unsigned*)self+1)=0xabcdef01;return self;}
extern "C" void __fastcall WideDestroy(void* self){unsigned label=((unsigned char*)self-arena-0x20)/4;if(label>=5)exit(11);event(2,label);*(unsigned*)self=0;if(mode==1&&label<4)*((unsigned*)self+1)=0x12345678;else if(mode==2)word(0x5c)=0xdeadbeef;else if(mode==3&&label>0)*((unsigned*)self-1)=(unsigned)(arena+0x600+label*16);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[3],cases=0;while(fread(p,4,3,input)==3){memset(arena,p[0],256);mode=p[1];WideCount=p[0];if(p[2]!=1){if(ConstructInit(arena)!=arena)return 12;event(3);}if(p[2]!=0){DestroyInit(arena);event(4);}++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_INIT_LIFETIME_PASS cases=%u\n",cases);return 0;}
