#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "surface_pixel_aliases.h"
#include "surface_pixel_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u kind %u\n",__LINE__,cases,p[0]);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned __fastcall ByteDepth(void*,void*);
extern "C" void __fastcall InitRGBA(void*,void*,long,long,long,long,bool);
extern "C" void __fastcall SetPixel(void*,void*,long,long,void*,void*);
extern "C" const unsigned char FormatTable[1692];
static unsigned char *arena,expected[1024],immutable[1692];
static unsigned p[14],cases,events,index;static FILE* output;
static unsigned& W(unsigned char*a,unsigned n){return *(unsigned*)(a+n);}
static long T(unsigned row,unsigned field){return *(const long*)(FormatTable+36*row+field);}
static void Record(){REQUIRE(!memcmp(arena,expected,1024)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,1024,output);}
static long __stdcall GetDesc(void*self,void*desc){
 REQUIRE(events++==0 && self==arena+0x180);
 memset(desc,0x77,32);*(unsigned*)desc=(unsigned)T(p[8],0);
 index=0;while(T(index,0)!=T(p[8],0)){++index;REQUIRE(index<46);}
 if(p[12]==1){W(arena,p[1]+4)=W(expected,p[1]+4)=(unsigned)(arena+0x1c0);W(arena,0x208)=W(expected,0x208)=16;W(arena,0x240)=W(expected,0x240)=0x76543210;}
 if(p[12]==2){W(arena,0x20c)=W(expected,0x20c)=(unsigned)(arena+0x350);W(arena,0x240)=W(expected,0x240)=0xfedcba98;}
 Record();return (long)p[13];
}
extern "C" void __fastcall Convert(void*self,void*,void*colour,unsigned char*dest){
 REQUIRE(events++==1 && *(unsigned*)self==index && colour==arena+0x240);
 unsigned offset=p[9]*(unsigned)(T(index,8)/8)+p[10]*W(expected,0x208);
 unsigned address=W(expected,0x20c)+offset;
 REQUIRE(dest==(unsigned char*)address && address>=0x24000280 && address<=0x240003fc);
 unsigned value=W(expected,0x240)^index;
 *(unsigned*)dest=value;W(expected,address-0x24000000)=value;
 Record();
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned k=0;k<sizeof(retailFixups)/sizeof(retailFixups[0]);++k){unsigned off=retailFixups[k][0],target=retailFixups[k][1],relative=retailFixups[k][2],resolved=0;
  if(target>=0x129ba40 && target<0x129c0dc)resolved=(unsigned)FormatTable+target-0x129ba40;
  else if(target==0x9e3830)resolved=(unsigned)(code+224);
  else if(target==0x9e3800)resolved=(unsigned)code;
  else if(target==0x9e3f60)resolved=(unsigned)&Convert;
  else return 6;
  *(unsigned*)(code+off)=relative?resolved-(unsigned)(code+off+4):resolved;
 }
 static void*vt[13];vt[12]=(void*)&GetDesc;memcpy(immutable,FormatTable,1692);
 typedef unsigned(__fastcall *BF)(void*,void*);
 typedef void(__fastcall *IF)(void*,void*,long,long,long,long,bool);
 typedef void(__fastcall *SF)(void*,void*,long,long,void*,void*);
 bool native=strcmp(argv[3],"retail")==0;BF bf=native?(BF)code:&ByteDepth;IF init=native?(IF)(code+32):&InitRGBA;SF set=native?(SF)(code+128):&SetPixel;
 while(fread(p,4,14,in)==14){
  memset(arena,0x95,1024);W(arena,p[1]+64)=p[2];W(arena,p[1]+4)=(unsigned)(arena+0x180);
  W(arena,0x180)=W(arena,0x1c0)=(unsigned)vt;W(arena,0x208)=p[11];W(arena,0x20c)=(unsigned)(arena+0x340);W(arena,0x240)=0x12345678;
  memcpy(expected,arena,1024);events=0;
  if(p[0]==0){unsigned value=bf(arena+p[1]+64,0);REQUIRE(value==(unsigned)(T(p[2],8)/8));}
  else if(p[0]==1){
   unsigned want=p[2];for(unsigned j=0;j<46;++j)if(T(j,4)==2 && T(j,16)==(long)p[3] && T(j,20)==(long)p[4] && T(j,24)==(long)p[5] && T(j,12)==(long)p[6]){want=j;break;}
   W(expected,p[1]+64)=want;init(arena+p[1]+64,0,p[3],p[4],p[5],p[6],p[7]!=0);
  }else {set(arena+p[1],0,p[9],p[10],arena+0x240,arena+0x200);REQUIRE(events==2);}
  Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SURFACE_PIXEL_PASS cases=%u\n",cases);return 0;
}
