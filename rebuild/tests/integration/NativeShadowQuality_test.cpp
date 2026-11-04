#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "shadow_quality_aliases.h"
#include "shadow_quality_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Quality(void*,void*,long,unsigned);
static unsigned char *arena,*object,expected[4096];
static unsigned p[6],step;static FILE* output;
static void snapshot(unsigned result){
 if(memcmp(arena,expected,4096))abort();
 unsigned status=_statusfp();if(status)abort();
 fwrite(arena,1,4096,output);fwrite(&status,4,1,output);fwrite(&result,4,1,output);
}
extern "C" void __fastcall Uninitialise(void* self,void*){
 if(self!=object||step++!=0)abort();snapshot(0);
 if(p[3]&1){*(unsigned*)(object+0x338)=*(unsigned*)(expected+p[0]+0x338)=0xdeadbeef;*(unsigned*)(object+0x33c)=*(unsigned*)(expected+p[0]+0x33c)=0x7f800001;}
 object[0x2af]=expected[p[0]+0x2af]=0;
}
extern "C" bool __fastcall Initialise(void* self,void*){
 if(self!=object||step++!=1)abort();snapshot(0);
 if(p[3]&2){*(unsigned*)(object+0x338)=*(unsigned*)(expected+p[0]+0x338)=0x12345678;*(unsigned*)(object+0x33c)=*(unsigned*)(expected+p[0]+0x33c)=0xff800000;}
 object[0x2af]=expected[p[0]+0x2af]=(unsigned char)p[4];return p[4]!=0;
}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+26)=(unsigned)&Uninitialise-(unsigned)(code+30);*(unsigned*)(code+33)=(unsigned)&Initialise-(unsigned)(code+37);
 typedef bool (__fastcall *Fn)(void*,void*,long,unsigned);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Quality;unsigned cases=0;
 while(fread(p,4,6,input)==6){
  if(fread(arena,1,4096,input)!=4096)return 6;memcpy(expected,arena,4096);object=arena+p[0];step=0;
  *(unsigned*)(expected+p[0]+0x338)=p[1];*(unsigned*)(expected+p[0]+0x33c)=p[2];
  _controlfp(p[5],_MCW_PC);_clearfp();bool result=fn(object,0,(long)p[1],p[2]);
  if(step!=2||result!=(p[4]!=0))abort();snapshot(result);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_SHADOW_QUALITY_PASS cases=%u\n",cases);return 0;
}
