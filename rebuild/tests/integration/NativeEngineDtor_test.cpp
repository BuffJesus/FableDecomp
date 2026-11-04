#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "engine_dtor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall DestroyEngine(void*);
extern "C" void __fastcall ReleaseEngine(void*);
extern "C" void* __fastcall DeleteEngine(void*,void*,unsigned);
extern "C" unsigned char EngineTable[],EngineDeviceTable[],IEngineTable[],PoolTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned self,unsigned a=0){unsigned h[]={kind,self,a};fwrite(h,4,3,output);unsigned p=word(0),s=word(12),pool=word(0x130);if(p==(unsigned)EngineTable)word(0)=0x12a0f3c;else if(p==(unsigned)IEngineTable)word(0)=0x12a0d24;if(s==(unsigned)EngineDeviceTable)word(12)=0x12a0f30;if(pool==(unsigned)PoolTable)word(0x130)=0x129a064;fwrite(arena,1,4096,output);word(0)=p;word(12)=s;word(0x130)=pool;}
extern "C" void __fastcall UninitialiseBoundary(void* self){if(self!=arena)exit(10);event(1,(unsigned)self);if(mode&1){word(0x168)=0;word(0x15c)=(unsigned)(arena+0xe00);}}
extern "C" void __cdecl EngineDtorFree(void* self){if(self<arena+0xa00||self>=arena+0xf00)exit(11);event(2,(unsigned)self);*(unsigned*)self^=0x12345678;if(mode&2)word(0x14c)=0;}
extern "C" void* __fastcall BlockDeleteBoundary(void* self,void*,unsigned flags){if(self<arena+0x500||self>=arena+0x600||flags!=1)exit(12);event(3,(unsigned)self,flags);if(mode&1)((unsigned*)self)[5]=0;return self;}
extern "C" void __fastcall PoolBaseDtorBoundary(void* self){if(self!=arena+0x130)exit(13);event(4,(unsigned)self);*(unsigned*)self=0x129a7c4;if(mode&1)word(0x118)=0;}
extern "C" void __fastcall DefDeleteBoundary(void* self){if(self<arena+0x800||self>=arena+0x900)exit(14);event(5,(unsigned)self);if(mode&2)word(0x118)=0;}
extern "C" void __fastcall InitBaseDtorBoundary(void* self){if(self!=arena)exit(15);event(6,(unsigned)self);word(0)=0x129a7cc;word(0xf00)=0x87654321;}
void __cdecl operator delete(void* self){if(!arena){free(self);return;}if(self!=arena)exit(16);event(7,(unsigned)self);word(0xf04)=0x76543210;}
extern "C" void UnusedEngineDtorBoundary(){exit(99);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8000)=(unsigned)&BlockDeleteBoundary;word(0xa004)=(unsigned)&DefDeleteBoundary;
 unsigned p[2],cases=0;while(fread(p,4,2,input)==2){if(fread(arena,1,4096,input)!=4096)return 5;mode=p[0];unsigned op=p[1],value=0;if(op>=5)word(0)=(unsigned)EngineTable;if(op>=5)ReleaseEngine(op==5?0:arena);else if(op==0)DestroyEngine(arena);else{unsigned flags[]={0,0,1,2,0xffffffff};value=(unsigned)DeleteEngine(arena,0,flags[op]);}event(8,value);++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_DTOR_PASS cases=%u\n",cases);return 0;}
