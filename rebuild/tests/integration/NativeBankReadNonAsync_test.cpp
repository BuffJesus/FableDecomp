#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bank_nonasync_aliases.h"
#include "bank_nonasync_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ReadNonAsync(void*,void*,unsigned,void*,unsigned);
static unsigned char* arena;static FILE* output;static unsigned* current;static unsigned mode,expected[5];
extern "C" void __fastcall BankRead(void* self,void*,unsigned index,unsigned offset,void* dest,unsigned size){
 unsigned args[5]={(unsigned)self,index,offset,(unsigned)dest,size};if(memcmp(args,expected,sizeof(args)))exit(20);
 *(unsigned*)dest=index^offset^size;
 if(mode==1){current[1]=(unsigned)(arena+0x300);current[2]++;}
 if(mode==2){current[0]^=0x12345678;current[3]=offset;}
 if(fwrite(args,4,5,output)!=5||fwrite(arena,1,2048,output)!=2048)exit(9);
}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 *(unsigned*)(code+retailCallOffset)=(unsigned)BankRead-(unsigned)(code+retailCallOffset+4);
 typedef void (__fastcall *Fn)(void*,void*,unsigned,void*,unsigned);Fn fn=argv[3][0]=='r'?(Fn)code:ReadNonAsync;
 unsigned p[4],cases=0;while(fread(p,4,4,in)==4){if(fread(arena,1,2048,in)!=2048)return 5;mode=p[0];current=(unsigned*)(arena+p[1]);
  for(unsigned n=0;n<2;++n){expected[0]=current[1];expected[1]=current[2];expected[2]=p[2];expected[3]=(unsigned)(arena+0x500);expected[4]=p[3];fn(current,0,p[2],arena+0x500,p[3]);}++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_BANK_READ_NONASYNC_PASS cases=%u\n",cases);return 0;
}
