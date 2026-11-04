#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_manager_global_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned EngineManagerOwner[];
extern "C" unsigned char ManagerTable[],SingletonTable[];
extern "C" void* __fastcall GetEngineManager();
extern "C" void __fastcall DeleteManagerData(void*);
extern "C" void __fastcall AddEngine(void*,void*,void*);
extern "C" void __fastcall RemoveEngine(void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mallocs;
static void event(unsigned kind,unsigned arg=0){
 unsigned h[]={kind,arg};unsigned char copy[4096];memcpy(copy,arena,4096);unsigned& table=*(unsigned*)(copy+0x100);if(table==(unsigned)ManagerTable)table=0x1231584;else if(table==(unsigned)SingletonTable)table=0x123156c;unsigned& fn=*(unsigned*)(copy+0x704);if(fn==(unsigned)&DeleteManagerData)fn=0x4361e0;
 fwrite(h,4,2,output);fwrite(EngineManagerOwner,4,2,output);fwrite(copy,1,4096,output);
}
void* operator new(size_t n){if(n!=20&&n!=12)exit(10);event(2,n);return arena+(n==20?0x100:0x700);}
extern "C" void* __cdecl FableEngineGlobalMalloc(unsigned n){if(n!=12)exit(11);event(3,n);return arena+0x300+mallocs++*0x20;}
extern "C" void __cdecl FableEngineGlobalFree(void* p){event(4,(unsigned)p);}
void operator delete(void* p){event(5,(unsigned)p);}
static void finalObservation(){event(9);fclose(output);}
#pragma init_seg(lib)
struct FinalObserver{FinalObserver(){atexit(finalObservation);}};
static FinalObserver observer;
int main(int argc,char** argv){
 if(argc!=2)return 2;output=fopen(argv[1],"wb");if(!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;event(0);
 void* manager=GetEngineManager();event(1,(unsigned)manager);event(1,(unsigned)GetEngineManager());
 for(unsigned i=0;i<2;++i){void* engine=arena+0x900+i*16;AddEngine(manager,0,engine);event(6,(unsigned)engine);}
 RemoveEngine(manager,0,arena+0x900);event(7);for(i=0;i<3;++i)*(unsigned*)(arena+0x108+i*4)=(unsigned)(arena+0x800+i*16);event(8);printf("NATIVE_ENGINE_MANAGER_GLOBAL_PASS\n");return 0;
}
