#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display_format_checks_aliases.h"
#include "display_format_checks_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall TextureFormat(void*,void*,void*);
extern "C" bool __fastcall TargetFormat(void*,void*,void*,void*);
extern "C" const unsigned char FormatTable[1692];
static unsigned char *arena,expected[2048],immutable[1692];static unsigned p[5],cases,events,stage;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static unsigned Format(unsigned at){unsigned index=W(arena,at);REQUIRE(index<46);return *(const unsigned*)(FormatTable+36*index);}
static void Record(){REQUIRE(!memcmp(arena,expected,2048)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,2048,output);}
static long Call(void*self,unsigned adapter,unsigned device,unsigned adapterFormat,unsigned colour,unsigned depth,unsigned kind){
 REQUIRE(kind==stage&&events++==stage);REQUIRE((unsigned)self==W(arena,p[0]+0x54)&&adapter==W(arena,p[0]+0x60)&&device==W(arena,p[0]+0x5c)&&adapterFormat==W(arena,p[0]+0x1c4)&&colour==Format(p[2])&&(kind==0||depth==Format(p[3])));
 ++W(arena,0x6c0);++W(expected,0x6c0);
 if(p[4]==1){W(arena,p[0]+0x54)=W(expected,p[0]+0x54)=(unsigned)(arena+0x640);}
 if(p[4]==2){W(arena,p[2])=W(expected,p[2])=45-W(expected,p[2]);if(p[2]!=p[3])W(arena,p[3])=W(expected,p[3])=45-W(expected,p[3]);W(arena,p[0]+0x60)=W(expected,p[0]+0x60)=0x12345678;}
 Record();return (long)p[1];
}
static long __stdcall CheckTexture(void*self,unsigned adapter,unsigned device,unsigned mode,unsigned usage,unsigned type,unsigned format){REQUIRE(usage==0&&type==3);return Call(self,adapter,device,mode,format,0,0);}
static long __stdcall CheckTarget(void*self,unsigned adapter,unsigned device,unsigned mode,unsigned colour,unsigned depth){return Call(self,adapter,device,mode,colour,depth,1);}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<3;++i)*(unsigned*)(code+retailFixups[i])=(unsigned)FormatTable;
 static void* table[13];table[10]=(void*)&CheckTexture;table[12]=(void*)&CheckTarget;memcpy(immutable,FormatTable,1692);
 typedef bool(__fastcall *TFn)(void*,void*,void*);typedef bool(__fastcall *RFn)(void*,void*,void*,void*);bool native=strcmp(argv[3],"retail")==0;TFn tf=native?(TFn)code:&TextureFormat;RFn rf=native?(RFn)(code+64):&TargetFormat;
 while(fread(p,4,5,in)==5){
  if(fread(arena,1,2048,in)!=2048)return 6;W(arena,0x600)=W(arena,0x640)=(unsigned)table;memcpy(expected,arena,2048);events=0;
  for(stage=0;stage<2;++stage){bool result=stage?rf(arena+p[0],0,arena+p[2],arena+p[3]):tf(arena+p[0],0,arena+p[2]);REQUIRE(result==((long)p[1]>=0));Record();}
  REQUIRE(events==2);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_DISPLAY_FORMAT_CHECKS_PASS cases=%u\n",cases);return 0;
}
