#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animation_getter_aliases.h"
#include "animation_getter_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" const void* __fastcall Animation(void*,void*,unsigned);
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
 unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 typedef const void* (__fastcall *Fn)(void*,void*,unsigned);Fn funcs[1]={argv[3][0]=='r'?(Fn)code:Animation};unsigned p[3],cases=0;unsigned char before[4096];
 while(fread(p,4,3,in)==3){if(fread(arena,1,4096,in)!=4096)return 5;memcpy(before,arena,4096);for(unsigned i=0;i<1;++i){const void* value=funcs[i](arena+p[0],0,p[1]);if((unsigned)value!=p[2]||memcmp(before,arena,4096))return 6;if(fwrite(&value,4,1,out)!=1||fwrite(arena,1,4096,out)!=4096)return 7;}++cases;}
 fclose(in);fclose(out);printf("NATIVE_ANIMATION_GETTER_PASS cases=%u\n",cases);return 0;
}
