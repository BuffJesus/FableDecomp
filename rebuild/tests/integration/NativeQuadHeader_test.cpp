#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quad_header_aliases.h"
#include "quad_header_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ReadInfo(void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static void record(unsigned kind,unsigned size){if(++events>24)exit(20);unsigned h[]={kind,size};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,long size){if(size!=44)exit(21);unsigned* p=(unsigned*)self;record(1,size);memcpy(out,(void*)p[3],size);p[3]+=size;p[1]+=size;p[5]=mode==1?0:mode==2?32:128;if(mode==3)*(unsigned*)(arena+0x48)=0xabcdef12;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<1;++i){unsigned off=retailFixups[i];*(unsigned*)(code+off)=(unsigned)Overflow-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:ReadInfo;
 unsigned cases=0;while(fread(&mode,4,1,in)==1){if(fread(arena,1,2048,in)!=2048)return 5;events=0;fn(arena,0,arena+0x300);record(2,0);++cases;}
 fclose(in);fclose(output);printf("NATIVE_QUAD_HEADER_PASS cases=%u\n",cases);return 0;
}
