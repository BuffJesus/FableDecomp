#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory_string_aliases.h"
#include "memory_string_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ReadString(void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,queries,first,second,length,nullString;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" unsigned __fastcall Position(void* self){record(1,self,queries);unsigned result=queries++?second:first;if(queries==1&&mode==1)at(24)=(unsigned)(arena+0x420)-first;if(queries==2&&mode==2)at(4)=5;return result;}
extern "C" void* __fastcall Construct(void* self,void*,const char* text,long n){if(n!=-1||text!=(char*)arena+(mode==1?0x420:0x400))exit(20);record(2,(void*)text,n);at(0x700)=1;at(0x704)=length;*(unsigned*)self=nullString?0:(unsigned)(arena+0x700);if(mode==3){at(4)=6;at(12)=(unsigned)(arena+0x550);}return self;}
extern "C" void* __fastcall Copy(void* self,void*,const void* other){record(3,self,*(unsigned*)other);*(unsigned*)self=*(unsigned*)other;if(*(unsigned*)other)++*(unsigned*)*(unsigned*)other;if(mode==4)at(4)=0x1234;return self;}
extern "C" void __fastcall Destroy(void* self){record(4,0,*(unsigned*)self);if(*(unsigned*)self)--*(unsigned*)*(unsigned*)self;if(mode==5)at(20)=17;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<3;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0x99ebf0:v=(unsigned)Construct;break;case 0x99ec30:v=(unsigned)Copy;break;case 0x99eae0:v=(unsigned)Destroy;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void* (__fastcall *Fn)(void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:ReadString;
 unsigned p[5],cases=0;while(fread(p,4,5,in)==5){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];first=p[1];second=p[2];length=p[3];nullString=p[4];queries=0;at(0)=(unsigned)(arena+0x600);at(0x608)=(unsigned)Position;at(24)=(unsigned)(arena+0x400)-first;if(fn(arena,0,arena+0x100)!=arena+0x100)return 6;record(5,0,queries);++cases;}
 fclose(in);fclose(output);printf("NATIVE_MEMORY_READ_STRING_PASS cases=%u\n",cases);return 0;
}
