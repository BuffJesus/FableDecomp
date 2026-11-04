#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "world_grid_aliases.h"
#include "world_grid_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Build(void*);
extern "C" void __fastcall Link(void*,void*,void*,long);
extern "C" void __fastcall Unlink(void*,void*,void*,long);
static unsigned char* arena;static FILE* output;static unsigned mode;
static void record(unsigned kind,unsigned arg){unsigned h[]={kind,arg};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,65536,output)!=65536)exit(8);}
void* __cdecl operator new[](unsigned n){
 if(n>60000)exit(9);record(1,n);
 if(mode==1)*(unsigned*)(arena+0x24)=*(unsigned*)(arena+0x20)+4;
 if(mode==2){*(void**)(arena+0x20)=arena+0x500;*(void**)(arena+0x24)=arena+0x510;}
 if(mode==3){*(unsigned*)(arena+0x10)=3;*(unsigned*)(arena+0x14)=3;*(unsigned*)(arena+0x24)=*(unsigned*)(arena+0x20);}
 return arena+0x1000;
}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+264)=(unsigned)&operator new[]-(unsigned)(code+268);*(unsigned*)(code+335)=(unsigned)(code+384)-(unsigned)(code+339);
 typedef void (__fastcall *Builder)(void*);typedef void (__fastcall *Linker)(void*,void*,void*,long);
 Builder build=argv[3][0]=='r'?(Builder)code:Build;Linker link=argv[3][0]=='r'?(Linker)(code+384):Link;Linker unlink=argv[3][0]=='r'?(Linker)(code+544):Unlink;unsigned p[3],cases=0;
 while(fread(p,4,3,in)==3){if(fread(arena,1,65536,in)!=65536)return 5;mode=p[0];build(arena);record(2,0);build(arena);record(3,0);
  if(p[1]){link(arena,0,arena+0x600,(long)p[2]);record(4,p[2]);unlink(arena,0,arena+0x600,(long)(p[2]^0x5555));record(5,p[2]^0x5555);unlink(arena,0,arena+0x600,(long)p[2]);record(6,p[2]);}++cases;
 }
 fclose(in);fclose(output);output=0;printf("NATIVE_WORLD_GRID_PASS cases=%u\n",cases);return 0;
}
