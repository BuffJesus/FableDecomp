#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "engine_component_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructComponent(void*);
extern "C" void __fastcall DestroyComponent(void*);
extern "C" void __fastcall SetOwner(void*,void*,unsigned);
extern "C" void* __fastcall DeleteComponent(void*,void*,unsigned);
extern "C" void* __fastcall ConsoleName(void*,void*,void*);
extern "C" unsigned char ComponentTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned self=0,unsigned arg=0){unsigned h[]={kind,self,arg};fwrite(h,4,3,output);unsigned save=word(0);if(save==(unsigned)ComponentTable)word(0)=0x12a2544;fwrite(arena,1,1024,output);word(0)=save;}
void __cdecl operator delete(void* self){if(!arena){free(self);return;}if(self!=arena)exit(10);event(1,(unsigned)self);if(mode){word(4)=(unsigned)(arena+0x420);word(0x3fc)=0x11223344;}}
extern "C" void* __fastcall CharCtorBoundary(void* self,void*,const char* text,int length){if(self!=arena+0x200||*text||length!=-1)exit(11);event(2,(unsigned)self,(unsigned)length);*(unsigned*)self=(unsigned)(arena+0x300);if(mode)word(8)=0x87654321;return self;}
static void defaults(unsigned pattern);
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(arena,1,1024,input)!=1024)return 5;unsigned op=p[0],owner=p[2],parameter=p[3];mode=p[1];if(op==0)event(3,(unsigned)ConstructComponent(arena));else if(op==1){DestroyComponent(arena);event(4);}else if(op==2){SetOwner(arena,0,owner);event(5);}else if(op==3)event(6,(unsigned)DeleteComponent(arena,0,parameter));else if(op==4)event(7,(unsigned)ConsoleName(arena,0,arena+0x200));else if(op==5)defaults(parameter);else{event(3,(unsigned)ConstructComponent(arena));SetOwner(arena,0,owner);event(5);event(6,(unsigned)DeleteComponent(arena,0,parameter));}++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_COMPONENT_PASS cases=%u\n",cases);return 0;}

extern "C" void __fastcall Default0(void*,void*,unsigned);
extern "C" void __fastcall Default1(void*,void*,unsigned);
extern "C" unsigned char __fastcall Default2(void*,void*,unsigned);
extern "C" void __fastcall Default3(void*,void*,unsigned);
extern "C" void __fastcall Default4(void*,void*,unsigned);
extern "C" void __fastcall Default5(void*,void*,unsigned);
extern "C" void __fastcall Default6(void*,void*,unsigned);
extern "C" void __fastcall Default7(void*,void*);
extern "C" void __fastcall Default8(void*,void*);
extern "C" unsigned char __fastcall Default9(void*,void*);
static void defaults(unsigned pattern){unsigned args[]={(unsigned)(arena+0x300),0,0xffffffff,0x12345678};unsigned arg=args[pattern];
Default0(arena,0,arg);event(10);
Default1(arena,0,arg);event(11);
event(12,0,Default2(arena,0,arg));
Default3(arena,0,arg);event(13);
Default4(arena,0,arg);event(14);
Default5(arena,0,arg);event(15);
Default6(arena,0,arg);event(16);
Default7(arena,0);event(17);
Default8(arena,0);event(18);
event(19,0,Default9(arena,0));
}
