#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "long_vector_aliases.h"
#include "long_vector_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Grow(void*,void*,void*,const void*,const void*,unsigned,bool);

static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,unsigned value){unsigned h[]={kind,value};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void* __cdecl Allocate(unsigned n){if(n>128)exit(20);record(1,n);if(mode==1)at(0x600)^=0x55;if(mode==2)at(0x200)^=0x77;return arena+0x800;}
extern "C" void __cdecl Free(void* p){record(2,(unsigned)p);if(mode==3)at(4)=0;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<4;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0xbfea0e:v=(unsigned)Allocate;break;case 0xbfea14:v=(unsigned)Free;break;case 0xbfeae6:v=(unsigned)memmove;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*,const void*,const void*,unsigned,bool);Fn funcs[1]={argv[3][0]=='r'?(Fn)code:Grow};
 unsigned p[6],cases=0;while(fread(p,4,6,in)==6){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];funcs[p[1]](arena,0,(void*)(at(0)+p[2]*4),arena+p[5],arena+0x604,p[3],p[4]!=0);record(3,0);++cases;}
 fclose(in);fclose(output);printf("NATIVE_LONG_VECTOR_PASS cases=%u\n",cases);return 0;
}
