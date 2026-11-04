#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "external_texture_aliases.h"
#include "external_texture_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Allocate(void*,void*,unsigned,void*,long,void*);
extern "C" void __fastcall Free(void*,void*,void*);
static unsigned char *arena,expected[1024];static unsigned p[7],cases,allocations,inits,releases,deletes;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,1024));fwrite(arena,1,1024,output);}
void* __cdecl operator new(size_t n){if(!arena)return malloc(n);REQUIRE(n==8&&++allocations==1);if(p[5]==1){W(arena,0x20)=W(expected,0x20)=123;W(arena,0x28)=W(expected,0x28)=(p[3]+1)%46;}Record();return arena+p[0];}
void __cdecl operator delete(void* ptr){if(!arena){free(ptr);return;}REQUIRE(ptr==arena+p[0]&&++deletes==1&&releases==p[6]);if(p[6]){W(expected,p[0])=0;W(expected,p[0]+4)&=0x0fffffff;}Record();}
extern "C" bool __fastcall Initialise(void*self,void*,void*d,long levels,void*f,unsigned usage,unsigned pool,unsigned dynamic,unsigned automatic){
 REQUIRE(self==arena+p[0]&&++inits==1&&allocations==1&&d==arena+0x20&&f==arena+0x28&&(unsigned)levels==p[2]&&usage==0&&pool==1&&dynamic==0&&automatic==0);
 W(expected,p[0])=W(expected,p[0]+4)=0;Record();
 W(arena,p[0])=W(expected,p[0])=p[6]?(unsigned)(arena+0x80):0;
 W(arena,p[0]+4)=W(expected,p[0]+4)=p[5]==2?0xf1234567:0x10000080;
 if(p[5]==3){W(arena,0x20)=W(expected,0x20)=456;W(arena,0x28)=W(expected,0x28)=7;}Record();return p[4]!=0;
}
static unsigned __stdcall Release(void*self){REQUIRE(self==arena+0x80&&++releases==1&&deletes==0);++W(arena,0x84);++W(expected,0x84);Record();return 0;}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<4;++i){unsigned v;switch(retailFixups[i][1]){case 0xbfea1a:v=(unsigned)(void*(__cdecl *)(size_t))&operator new;break;case 0xbfe9bc:v=(unsigned)(void(__cdecl *)(void*))&operator delete;break;case 0x9fa280:v=(unsigned)&Initialise;break;case 0x9f9f70:v=(unsigned)(code+128);break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=v-(unsigned)(code+retailFixups[i][0]+4);}
 static void* table[3];table[2]=(void*)&Release;
 typedef void*(__fastcall *AF)(void*,void*,unsigned,void*,long,void*);typedef void(__fastcall *FF)(void*,void*,void*);bool native=!strcmp(argv[3],"retail");AF a=native?(AF)code:&Allocate;FF f=native?(FF)(code+80):&Free;
 while(fread(p,4,7,in)==7){if(fread(arena,1,1024,in)!=1024)return 6;W(arena,0x20)=17;W(arena,0x24)=32;W(arena,0x28)=p[3];W(arena,0x80)=(unsigned)table;memcpy(expected,arena,1024);allocations=inits=releases=deletes=0;
  void* result=a(arena,0,p[1],arena+0x20,(long)p[2],arena+0x28);REQUIRE(result==arena+p[0]&&inits==1);Record();f(arena,0,result);REQUIRE(deletes==1);Record();f(arena,0,0);REQUIRE(deletes==1);Record();++cases;
 }fclose(in);fclose(output);printf("NATIVE_EXTERNAL_TEXTURE_PASS cases=%u\n",cases);return 0;
}
