#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_manager_owner_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned EngineManagerOwner[2]={0,0};
extern "C" void* __fastcall GetEngineManager();
extern "C" void __fastcall ResetEngineManager(void*,void*,void*);
extern "C" void* __fastcall DeleteEngineManager(void*,void*,unsigned);
extern "C" void __fastcall DestroyEngineList(void*);
extern "C" void __fastcall DeleteManagerData(void*);
extern "C" unsigned char ManagerTable[],SingletonTable[];
static unsigned char* arena;static FILE* output;static unsigned mode,failure;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0){
 unsigned h[]={kind,a,b};unsigned char copy[4096];memcpy(copy,arena,4096);unsigned offsets[]={0x100,0x200,0x240};for(unsigned i=0;i<3;++i){unsigned& v=*(unsigned*)(copy+offsets[i]);if(v==(unsigned)ManagerTable)v=0x1231584;else if(v==(unsigned)SingletonTable)v=0x123156c;}
 unsigned callbacks[]={0x604,0x704,0x744};for(i=0;i<3;++i){unsigned& v=*(unsigned*)(copy+callbacks[i]);if(v==(unsigned)&DeleteManagerData)v=0x4361e0;}
 fwrite(h,4,3,output);fwrite(EngineManagerOwner,4,2,output);fwrite(copy,1,4096,output);
}
void* operator new(size_t n){if(!output)return malloc(n);if(n!=20&&n!=12)exit(10);event(1,n);if(mode==1)EngineManagerOwner[0]=(unsigned)(arena+(n==20?0x100:0x240));return failure==n?0:arena+(n==20?0x200:0x600);}
extern "C" void* __cdecl FableEngineOwnerMalloc(unsigned n){if(n!=12)exit(11);event(2,n);if(mode==2){word(0x208)=(unsigned)(arena+0x820);EngineManagerOwner[1]=(unsigned)(arena+0x740);}return arena+0x400;}
extern "C" void __cdecl FableEngineOwnerFree(void* p){event(3,(unsigned)p);if(mode==1&&p>=arena+0x320&&p<arena+0x3e0)*(unsigned*)p=0;if(mode==3&&p==arena+0x320)word(0x104)=*(unsigned*)p;}
void operator delete(void* p){if(!output){free(p);return;}event(4,(unsigned)p);if(mode==3&&p==arena+0x100)EngineManagerOwner[1]=(unsigned)(arena+0x740);}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[5],cases=0;while(fread(p,4,5,input)==5){if(fread(EngineManagerOwner,4,2,input)!=2||fread(arena,1,4096,input)!=4096)return 5;mode=p[1];failure=p[2];word(0x100)=word(0x200)=word(0x240)=(unsigned)ManagerTable;word(0x704)=word(0x744)=(unsigned)&DeleteManagerData;unsigned result=0;
  if(p[0]==0)result=(unsigned)GetEngineManager();else if(p[0]==1)ResetEngineManager(EngineManagerOwner,0,p[4]?arena+0x200:0);else if(p[0]==2)result=(unsigned)DeleteEngineManager(arena+0x100,0,p[3]);else if(p[0]==3)DestroyEngineList(arena+0x104);else DeleteManagerData(p[4]?arena+0x100:0);event(5,p[0],result);++cases;}
 fclose(input);fclose(output);output=0;printf("NATIVE_ENGINE_MANAGER_OWNER_PASS cases=%u\n",cases);return 0;
}
