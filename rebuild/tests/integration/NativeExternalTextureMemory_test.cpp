#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "external_texture_memory_aliases.h"
#include "external_texture_memory_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Allocate(void*,void*,unsigned,void*,long,void*);
extern "C" void __fastcall Free(void*,void*,void*);
extern "C" const unsigned char FormatTable[1692];
static unsigned char *arena,expected[4096],immutable[1692];static unsigned p[9],cases,allocations,roots,creates,calculations,releases,deletes,expectedLevels,descriptions,total,textureX,textureY,textureFormat;static long count;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,4096)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,4096,output);}
void* __cdecl operator new(size_t n){if(!arena)return malloc(n);REQUIRE(n==8&&++allocations==1);if(p[5]==1){W(arena,0x20)=W(expected,0x20)=123;W(arena,0x28)=W(expected,0x28)=(p[3]+1)%46;}Record();return arena+p[0];}
void __cdecl operator delete(void* ptr){if(!arena){free(ptr);return;}REQUIRE(ptr==arena+p[0]&&++deletes==1&&releases==p[6]);if(p[6]){W(expected,p[0])=0;W(expected,p[0]+4)&=0x0fffffff;}Record();}
static unsigned Mips(unsigned x,unsigned y){unsigned v=x<y?x:y,bits=0;for(;v;v>>=1)++bits;return bits>=4?bits-2:1;}
extern "C" void* __fastcall GetSystem(){
 REQUIRE(++roots==1&&allocations==1);W(expected,p[0])=W(expected,p[0]+4)=0;
 expectedLevels=(long)p[2]>0?p[2]:Mips(W(arena,0x20),W(arena,0x24));Record();
 if(p[5]==2){W(arena,0x20)=W(expected,0x20)=11;W(arena,0x24)=W(expected,0x24)=19;W(arena,0x28)=W(expected,0x28)=(p[3]+5)%46;}
 if(p[5]==3)W(arena,0x560)=W(expected,0x560)=(unsigned)(arena+0x700);Record();return arena+0x500;
}
static long __stdcall CreateTexture(void*self,unsigned x,unsigned y,unsigned levels,unsigned flags,unsigned format,unsigned pool,void**out,void*shared){
 REQUIRE(++creates==1&&roots==1&&self==arena+(p[5]==3?0x840:0x800)&&x==W(arena,0x20)&&y==W(arena,0x24)&&levels==expectedLevels&&flags==0&&pool==1&&shared==0&&out==(void**)(arena+p[0]));
 REQUIRE(format==*(const unsigned*)(FormatTable+36*W(arena,0x28)));textureX=x;textureY=y;textureFormat=format;Record();
 *out=p[6]?arena+0x80:0;W(expected,p[0])=(unsigned)*out;
 if(p[5]==3)W(arena,p[0]+4)=W(expected,p[0]+4)=0xa1234567;Record();return (long)p[4];
}
static void Account(unsigned level){
 unsigned index=0;while(index<46&&*(const unsigned*)(FormatTable+36*index)!=textureFormat)++index;REQUIRE(index<46);
 unsigned bits=*(const unsigned*)(FormatTable+36*index+8);unsigned bytes=(bits*(textureX>>level)*(textureY>>level))>>3;total=(total+bytes)&0x0fffffff;W(expected,p[0]+4)=0x10000000|total;
}
static unsigned __stdcall GetLevelCount(void*self){
 REQUIRE(self==arena+0x80&&++calculations==1&&(long)p[4]>=0);W(expected,p[0]+4)=0x10000000;Record();return (unsigned)count;
}
static long __stdcall GetLevelDesc(void*self,unsigned level,unsigned*out){
 REQUIRE(self==arena+0x80&&(unsigned)out<(unsigned)arena);unsigned iteration=descriptions/2;REQUIRE(iteration<(unsigned)count&&level==(descriptions%2?0:iteration));
 if(descriptions&&descriptions%2==0)Account(iteration-1);Record();
 memset(out,0,32);out[0]=textureFormat;out[6]=textureX>>level;out[7]=textureY>>level;++descriptions;return (long)0x80004005;
}
static unsigned __stdcall Release(void*self){REQUIRE(self==arena+0x80&&++releases==1&&deletes==0);++W(arena,0x84);++W(expected,0x84);Record();return 0;}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned v;switch(retailFixups[i][1]){case 0xbfea1a:v=(unsigned)(void*(__cdecl *)(size_t))&operator new;break;case 0xbfe9bc:v=(unsigned)(void(__cdecl *)(void*))&operator delete;break;case 0x9fa280:v=(unsigned)(code+176);break;case 0x129ba40:v=(unsigned)FormatTable;break;case 0x9a4ec0:v=(unsigned)&GetSystem;break;case 0x9f9ee0:v=(unsigned)(code+368);break;case 0x9e3830:v=(unsigned)(code+512);break;case 0x9e3820:v=(unsigned)(code+576);break;case 0x129ba48:v=(unsigned)(FormatTable+8);break;case 0x9f9f70:v=(unsigned)(code+128);break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 static void* table[24];table[2]=(void*)&Release;table[23]=(void*)&CreateTexture;table[13]=(void*)&GetLevelCount;table[17]=(void*)&GetLevelDesc;memcpy(immutable,FormatTable,1692);
 typedef void*(__fastcall *AF)(void*,void*,unsigned,void*,long,void*);typedef void(__fastcall *FF)(void*,void*,void*);bool native=!strcmp(argv[3],"retail");AF a=native?(AF)code:&Allocate;FF f=native?(FF)(code+80):&Free;
 while(fread(p,4,9,in)==9){if(fread(arena,1,4096,in)!=4096)return 6;W(arena,0x20)=p[7];W(arena,0x24)=p[8];W(arena,0x28)=p[3];W(arena,0x80)=(unsigned)table;W(arena,0x560)=(unsigned)(arena+0x600);W(arena,0x658)=(unsigned)(arena+0x800);W(arena,0x758)=(unsigned)(arena+0x840);W(arena,0x800)=W(arena,0x840)=(unsigned)table;memcpy(expected,arena,4096);allocations=roots=creates=calculations=releases=deletes=descriptions=total=0;count=(long)(p[3]%5)-1;
  void* result=a(arena,0,p[1],arena+0x20,(long)p[2],arena+0x28);REQUIRE(result==arena+p[0]&&creates==1&&roots==1&&calculations==((long)p[4]>=0&&p[6]?1:0));if((long)p[4]>=0){W(expected,p[0]+4)=0x10000000|total;if(p[6]&&count>0){REQUIRE(descriptions==2*(unsigned)count);Account(count-1);}}Record();f(arena,0,result);REQUIRE(deletes==1);Record();f(arena,0,0);REQUIRE(deletes==1);Record();++cases;
 }fclose(in);fclose(output);printf("NATIVE_EXTERNAL_TEXTURE_MEMORY_PASS cases=%u\n",cases);return 0;
}
