#include <stdio.h>
#include <stdlib.h>
#include "engine_manager_list_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall AddEngine(void*,void*,void*);
extern "C" void __fastcall RemoveEngine(void*,void*,void*);
extern "C" void __fastcall RemoveEnginePointer(void*,void*,const void*);
static unsigned char* arena;static FILE* output;static unsigned mode;static unsigned* valuePointer;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned p=0){unsigned h[]={kind,p};fwrite(h,4,2,output);fwrite(arena,1,4096,output);}
extern "C" void* __cdecl FableEngineListMalloc(unsigned n){if(n!=12)exit(10);event(1,n);if(mode==1)word(4)=(unsigned)(arena+0x140);else if(mode==2)word(0x104)=(unsigned)(arena+0x100);else if(mode==3)word(0x108)=0x10293847;return arena+0x500;}
extern "C" void __cdecl FableEngineListFree(void* p){event(2,(unsigned)p);if(mode==1)*(unsigned*)p=0;else if(mode==2)*valuePointer=(unsigned)(arena+0x710);else if(mode==3){word(4)=(unsigned)(arena+0x140);((unsigned*)p)[2]=0;}}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(arena,1,4096,input)!=4096)return 5;mode=p[2];valuePointer=(unsigned*)(arena+(p[0]==3&&p[1]?0x208:0x800));if(p[0]==0)AddEngine(arena,0,(void*)p[3]);else if(p[0]==1)RemoveEngine(arena,0,(void*)p[3]);else RemoveEnginePointer(arena+4,0,valuePointer);event(3);++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_MANAGER_LIST_PASS cases=%u\n",cases);return 0;
}
