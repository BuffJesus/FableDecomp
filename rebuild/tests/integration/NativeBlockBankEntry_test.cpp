#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block_bank_entry_aliases.h"
#include "block_bank_entry_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SetPool(void*,void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self){if(++events>30)exit(20);unsigned h[]={kind,(unsigned)self};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,2048,output)!=2048)exit(9);}
extern "C" void __fastcall Release(void* data){record(1,data);if(mode==1)at(4)=(unsigned)(arena+0x340);if(mode==2)at(0x300)=9;if(mode==3)at(0x320)=0;}
void __cdecl operator delete(void* p){record(2,p);if(mode==4)at(0)=0xabcdef;}
extern "C" void __cdecl Delete(void*);
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<2;++i){unsigned off=retailFixups[i];*(unsigned*)(code+off)=(unsigned)Delete-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:SetPool;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];events=0;for(unsigned j=0x300;j<=0x340;j+=0x20){at(j+4)=(unsigned)Release;at(j+8)=(unsigned)(arena+j+0x10);}
 fn(arena,0,(void*)p[1],(void*)p[2]);record(3,arena);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_BLOCK_BANK_ENTRY_PASS cases=%u\n",cases);return 0;
}
