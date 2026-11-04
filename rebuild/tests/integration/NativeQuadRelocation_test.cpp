#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quad_relocation_aliases.h"
#include "quad_relocation_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Relocate(void*,void*,long,void*,void*,long);
extern "C" void __fastcall Post(void*);
extern "C" void __fastcall SetOwner(void*,void*,void*,void*);
extern "C" void* Renderer;
static unsigned char* arena;static FILE* output;
static void record(unsigned kind){unsigned h[]={kind,(unsigned)Renderer};if(fwrite(h,4,2,output)!=2||fwrite(arena,1,8192,output)!=8192)exit(9);}
int main(int argc,char** argv){
 if(argc!=4||Renderer)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],target=retailFixups[i][1],v=0;
  switch(target){case 0xbddcb0:v=(unsigned)(code+192);break;case 0xbd9ab0:v=(unsigned)(code+288);break;case 0x1436ea8:v=(unsigned)&Renderer;break;default:return 8;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Move)(void*,void*,long,void*,void*,long);typedef void (__fastcall *PostCall)(void*);typedef void (__fastcall *Set)(void*,void*,void*,void*);
 Move move=argv[3][0]=='r'?(Move)code:Relocate;PostCall post=argv[3][0]=='r'?(PostCall)(code+192):Post;Set set=argv[3][0]=='r'?(Set)(code+288):SetOwner;
 unsigned p[5],cases=0;
 while(fread(p,4,5,in)==5){if(fread(arena,1,8192,in)!=8192)return 5;Renderer=arena+0x1000;
  for(unsigned j=0;j<2;++j){if(p[0])post(arena);else move(arena,0,(long)p[3],arena+p[1],arena+p[2],(long)p[4]);record(j);}
  set(0,0,(void*)p[3],(void*)p[4]);record(2);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_QUAD_RELOCATION_PASS cases=%u\n",cases);return 0;
}
