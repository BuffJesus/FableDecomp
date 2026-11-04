#include <stdio.h>
#include <string.h>
#include "bank_info_read_aliases.h"
#include "bank_info_read_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Read(void*,void*,unsigned,void*,unsigned);
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb"),*out=fopen(argv[2],"wb");if(!in||!out)return 3;
 unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 void* code=VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 typedef void (__fastcall *Fn)(void*,void*,unsigned,void*,unsigned);Fn fn=argv[3][0]=='r'?(Fn)code:Read;
 unsigned p[2],cases=0;while(fread(p,4,2,in)==2){if(fread(arena,1,2048,in)!=2048)return 5;fn(arena,0,p[0],arena+0x400,p[1]);if(fwrite(arena,1,2048,out)!=2048)return 6;++cases;}
 fclose(in);fclose(out);printf("NATIVE_BANK_INFO_READ_PASS cases=%u\n",cases);return 0;
}
