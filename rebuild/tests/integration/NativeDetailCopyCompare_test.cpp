#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "detail_copy_aliases.h"
#include "detail_copy_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Equivalent(void*,void*,const void*);
extern "C" void* __fastcall Copy(void*,void*,const void*);
extern "C" unsigned TypeTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static void record(unsigned kind,unsigned value){unsigned h[]={kind,value};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" void __fastcall BaseCtor(void* self){*(unsigned*)self=0;record(1,(unsigned)self);if(mode==1)arena[0x114]^=0x55;if(mode==2)arena[0x14]^=0xaa;if(mode==3)arena[0x153]^=0x7f;}
extern "C" void __fastcall BaseDtor(void*){exit(20);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+copyCallFixup)=(unsigned)BaseCtor-(unsigned)(code+copyCallFixup+4);*(unsigned*)(code+copyTableFixup)=(unsigned)TypeTable;
 typedef bool (__fastcall *Eq)(void*,void*,const void*);typedef void* (__fastcall *Ctor)(void*,void*,const void*);Eq eq=argv[3][0]=='r'?(Eq)code:Equivalent;Ctor copy=argv[3][0]=='r'?(Ctor)(code+64):Copy;
 unsigned p,cases=0;while(fread(&p,4,1,in)==1){if(fread(arena,1,2048,in)!=2048)return 5;mode=p;record(0,eq(arena,0,arena+0x100));if(!eq(arena,0,arena))return 6;if(copy(arena,0,arena+0x100)!=arena)return 7;record(2,eq(arena,0,arena+0x100));++cases;}
 fclose(in);fclose(output);printf("NATIVE_DETAIL_COPY_COMPARE_PASS cases=%u\n",cases);return 0;
}
