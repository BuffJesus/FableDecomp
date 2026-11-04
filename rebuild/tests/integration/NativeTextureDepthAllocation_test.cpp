#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_depth_allocation_aliases.h"
#include "texture_depth_allocation_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall TextureInit(void*,void*,void*,long,void*,unsigned,unsigned,unsigned,unsigned);
extern "C" bool __fastcall DepthInit(void*,void*,void*,void*,unsigned,unsigned);
extern "C" const unsigned char FormatTable[1692];
static unsigned char *arena,expected[8192],immutable[1692];static unsigned p[12],cases,releases,roots,creates,calculations,initialPointer,expectedLevels;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,8192)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,8192,output);}
static unsigned Mips(unsigned x,unsigned y){unsigned v=x<y?x:y,bits=0;for(;v;v>>=1)++bits;return bits>=4?bits-2:1;}
static unsigned __stdcall Release(void*self){
 REQUIRE((unsigned)self==initialPointer&&++releases==1&&roots==0);unsigned off=(unsigned char*)self-arena;++W(arena,off+4);++W(expected,off+4);Record();return 0;
}
extern "C" void* __fastcall GetSystem(){
 REQUIRE(++roots==1&&releases==(initialPointer?1:0));
 if(initialPointer){if(p[0]==0){W(expected,p[1])=0;W(expected,p[1]+4)&=0x0fffffff;}else W(expected,p[1]+4)=W(expected,p[1]+8)=0;}
 REQUIRE(!memcmp(arena,expected,8192));expectedLevels=(long)p[4]>0?p[4]:Mips(W(arena,p[2]),W(arena,p[2]+4));
 if(p[10]==1)W(arena,0x1060)=W(expected,0x1060)=(unsigned)(arena+0x1400);
 if(p[10]==2){W(arena,p[2])=W(expected,p[2])=W(expected,p[2])+17;W(arena,p[2]+4)=W(expected,p[2]+4)=W(expected,p[2]+4)^31;W(arena,p[3])=W(expected,p[3])=(W(expected,p[3])+5)%46;}
 Record();return arena+0x1000;
}
static long Create(void*self,unsigned x,unsigned y,unsigned levels,unsigned flags,unsigned format,unsigned pool,void** out,void* shared){
 REQUIRE(++creates==1&&roots==1&&shared==0);unsigned display=W(arena,0x1060)-(unsigned)arena;
 REQUIRE((unsigned)self==W(arena,display+0x58)&&x==W(arena,p[2])&&y==W(arena,p[2]+4));
 unsigned index=W(arena,p[3]);REQUIRE(index<46&&format==*(const unsigned*)(FormatTable+36*index));
 REQUIRE(out==(void**)(arena+p[1]+(p[0]?4:0)));
 if(p[0]==0)REQUIRE(levels==expectedLevels&&flags==p[5]&&pool==p[6]);else REQUIRE(levels==0&&flags==0&&pool==p[6]);
 REQUIRE(!memcmp(arena,expected,8192));unsigned ptr=p[11]?(unsigned)(arena+0x1800):0;*out=(void*)ptr;W(expected,p[1]+(p[0]?4:0))=ptr;
 if(p[10]==3&&p[0]==0)W(arena,p[1]+4)=W(expected,p[1]+4)=0xa1234567;
 Record();return (long)p[9];
}
static long __stdcall CreateTexture(void*self,unsigned x,unsigned y,unsigned levels,unsigned flags,unsigned format,unsigned pool,void** out,void* shared){REQUIRE(p[0]==0);return Create(self,x,y,levels,flags,format,pool,out,shared);}
static long __stdcall CreateDepth(void*self,unsigned x,unsigned y,unsigned format,unsigned multi,unsigned quality,unsigned discard,void** out,void* shared){REQUIRE(p[0]==1&&quality==0&&discard==0);return Create(self,x,y,0,0,format,multi,out,shared);}
extern "C" void __fastcall CalcLength(void*self,void*){
 REQUIRE(self==arena+p[1]&&p[0]==0&&(long)p[9]>=0&&++calculations==1);W(expected,p[1]+4)=(W(expected,p[1]+4)&0x0fffffff)|0x10000000;REQUIRE(!memcmp(arena,expected,8192));
 unsigned size=(W(arena,p[2])^W(arena,p[2]+4))&0x0fffffff;W(arena,p[1]+4)=W(expected,p[1]+4)=(W(expected,p[1]+4)&0xf0000000)|size;Record();
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned val=retailFixups[i][1]==0x129ba40?(unsigned)FormatTable:retailFixups[i][1]==0x9a4ec0?(unsigned)&GetSystem:(unsigned)&CalcLength;if(retailFixups[i][2])val-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=val;}
 static void* table[30];table[2]=(void*)&Release;table[23]=(void*)&CreateTexture;table[29]=(void*)&CreateDepth;memcpy(immutable,FormatTable,1692);
 typedef bool(__fastcall *TFn)(void*,void*,void*,long,void*,unsigned,unsigned,unsigned,unsigned);typedef bool(__fastcall *DFn)(void*,void*,void*,void*,unsigned,unsigned);bool native=strcmp(argv[3],"retail")==0;TFn tf=native?(TFn)(code+112):&TextureInit;DFn df=native?(DFn)code:&DepthInit;
 while(fread(p,4,12,in)==12){
  if(fread(arena,1,8192,in)!=8192)return 6;W(arena,0x1600)=W(arena,0x1640)=W(arena,0x1800)=W(arena,0x1840)=(unsigned)table;memcpy(expected,arena,8192);initialPointer=W(arena,p[1]+(p[0]?4:0));releases=roots=creates=calculations=0;
  bool result=p[0]?df(arena+p[1],0,arena+p[2],arena+p[3],p[6],p[7]):tf(arena+p[1],0,arena+p[2],(long)p[4],arena+p[3],p[5],p[6],p[7],p[8]);
  if(p[0])W(expected,p[1]+8)=1;
  REQUIRE(result==((long)p[9]>=0)&&creates==1&&roots==1&&calculations==(p[0]==0&&(long)p[9]>=0?1:0));Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_TEXTURE_DEPTH_ALLOCATION_PASS cases=%u\n",cases);return 0;
}
