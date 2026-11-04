#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "detail_constructors_aliases.h"
#include "detail_constructors_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructDetail(void*,void*,void*);
extern "C" void* __fastcall ConstructQuad(void*,void*,void*,long,long,long,long);
extern "C" void __fastcall DestroyQuad(void*);
extern "C" void __fastcall DestroyDetail(void*);
extern "C" unsigned QuadTable[4];
extern "C" void __fastcall FreeVirtual(void*,void*,void*){exit(12);}
static unsigned char* arena;static FILE* output;
static void record(unsigned kind,unsigned result){unsigned h[]={kind,result};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,2048,output)!=2048)exit(9);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+10)=(unsigned)QuadTable;*(unsigned*)(code+128+33)=(unsigned)code-(unsigned)(code+128+37);
 typedef void* (__fastcall *Detail)(void*,void*,void*);typedef void* (__fastcall *Quad)(void*,void*,void*,long,long,long,long);
 Detail detail=argv[3][0]=='r'?(Detail)(code+128):ConstructDetail;Quad quad=argv[3][0]=='r'?(Quad)code:ConstructQuad;
 unsigned p[6],cases=0;while(fread(p,4,6,in)==6){if(fread(arena,1,2048,in)!=2048)return 5;
  void* result=p[0]?quad(arena,0,(void*)p[1],p[2],p[3],p[4],p[5]):detail(arena,0,arena+0x400);if(result!=arena)return 6;record(1,(unsigned)result);
  if(p[0])DestroyQuad(arena);else DestroyDetail(arena);record(2,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_DETAIL_CONSTRUCTORS_PASS cases=%u\n",cases);return 0;
}
