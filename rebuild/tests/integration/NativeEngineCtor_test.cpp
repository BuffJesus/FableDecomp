#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "engine_ctor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructEngine(void*);
extern "C" unsigned char EngineTable[],EngineDeviceTable[];
extern "C" void* __fastcall AllocateEngine();
static unsigned char* arena;static FILE* output;static unsigned mode,present;
static unsigned& word(unsigned off){return *(unsigned*)(arena+off);}
static void event(unsigned kind,unsigned self,unsigned a=0,unsigned b=0){unsigned h[]={kind,self,a,b};fwrite(h,4,4,output);unsigned p=word(0),s=word(12);if(p==(unsigned)EngineTable)word(0)=0x12a0f3c;if(s==(unsigned)EngineDeviceTable)word(12)=0x12a0f30;fwrite(arena,1,4096,output);word(0)=p;word(12)=s;}
#ifdef COMPOSE_ALLOCATION
void* __cdecl operator new(size_t size){if(!arena)return malloc(size);if(size!=0x178)exit(12);event(4,0,size);return present?arena:0;}
#endif
extern "C" void* __fastcall BaseCtorBoundary(void* self){if(self!=arena)exit(10);event(1,(unsigned)self);word(0)=0x129a7cc;arena[4]=0;if(mode&1)memset(arena+8,0xa5,0x170);if(mode&2)word(0x500)=0x87654321;return self;}
extern "C" void* __fastcall PoolCtorBoundary(void* self,void*,unsigned a,unsigned b){if(self!=arena+0x130||a!=0||b!=0x1000)exit(11);event(2,(unsigned)self,a,b);for(unsigned i=0;i<7;++i)((unsigned*)self)[i]=0x12340000+i;if(mode&1){word(0x14)=0xdeadbeef;word(0xc0)=0x87654321;}if(mode&2){word(0x14c)=0xabcdef01;word(0x84)=0xabcdef01;}if(mode&4){word(12)=0x65432100;word(0x600)=0x10293847;}return self;}
extern "C" void UnusedEngineCtorBoundary(){exit(99);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned cases=0;while(fread(&mode,4,1,input)==1){if(fread(&present,4,1,input)!=1||fread(arena,1,4096,input)!=4096)return 5;
#ifdef COMPOSE_ALLOCATION
 event(3,(unsigned)AllocateEngine());
#else
 event(3,(unsigned)ConstructEngine(arena));
#endif
 ++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_CTOR_PASS cases=%u\n",cases);return 0;}
