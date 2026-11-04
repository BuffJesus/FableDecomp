#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "surface_ownership_aliases.h"
#include "surface_ownership_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u kind %u\n",__LINE__,cases,p[0]);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Signed(void*,void*);
extern "C" void __fastcall Attach(void*,void*,void*,bool);
extern "C" bool __fastcall Image(void*,void*,void*,void*,bool);
extern "C" const unsigned char FormatTable[1692];
static unsigned char*arena,expected[8192],immutable[1692];static unsigned p[13],cases,releases,adds,roots,creates,width,height;static FILE*output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,8192)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,8192,output);}
static void CleanExpected(){if(p[2])W(expected,p[1]+4)=W(expected,p[1]+8)=0;}
static void AttachExpected(){CleanExpected();W(expected,p[1]+4)=p[3];W(expected,p[1]+8)=p[3]!=0;}
static unsigned __stdcall Release(void*self){
 REQUIRE((unsigned)self==p[2]&&++releases==1&&adds==0&&roots==0);unsigned off=(unsigned char*)self-arena;
 ++W(arena,off+4);++W(expected,off+4);
 if(p[9]==1){W(arena,p[1]+4)=W(expected,p[1]+4)=(unsigned)(arena+0x1840);W(arena,p[1]+8)=W(expected,p[1]+8)=0x76543210;W(arena,p[1]+12)=W(expected,p[1]+12)=7;}
 Record();return 0;
}
static unsigned __stdcall AddRef(void*self){
 REQUIRE(p[0]==1&&(unsigned)self==p[3]&&++adds==1&&releases==(p[2]?1:0));AttachExpected();unsigned off=(unsigned char*)self-arena;
 ++W(arena,off+8);++W(expected,off+8);
 if(p[9]==2){W(arena,p[1]+4)=W(expected,p[1]+4)=0;W(arena,p[1]+8)=W(expected,p[1]+8)=99;}
 Record();return 2;
}
extern "C" void* __fastcall GetSystem(){
 REQUIRE(p[0]==2&&++roots==1&&releases==(p[2]?1:0));CleanExpected();REQUIRE(!memcmp(arena,expected,8192));width=W(expected,p[11]);height=W(expected,p[11]+4);
 if(p[9]==1)W(arena,0x1060)=W(expected,0x1060)=(unsigned)(arena+0x1400);
 if(p[9]==2){W(arena,p[11])=W(expected,p[11])=123;W(arena,p[11]+4)=W(expected,p[11]+4)=456;W(arena,p[12])=W(expected,p[12])=(W(expected,p[12])+5)%46;}
 Record();return arena+0x1000;
}
static long __stdcall Create(void*self,unsigned x,unsigned y,unsigned format,unsigned pool,void**out,void*shared){
 REQUIRE(p[0]==2&&++creates==1&&roots==1&&x==width&&y==height&&pool==3&&shared==0&&out==(void**)(arena+p[1]+4));
 unsigned display=W(expected,0x1060)-(unsigned)arena;REQUIRE((unsigned)self==W(expected,display+0x58));unsigned idx=W(expected,p[12]);REQUIRE(idx<46&&format==*(const unsigned*)(FormatTable+36*idx));
 unsigned ptr=p[10]?(unsigned)(arena+0x1840):0;*out=(void*)ptr;W(expected,p[1]+4)=ptr;
 if(p[9]==3)W(arena,p[1]+8)=W(expected,p[1]+8)=0xabcdef12;
 Record();return (long)p[8];
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned target=retailFixups[i][1],value=target==0x9a4ec0?(unsigned)&GetSystem:(unsigned)FormatTable+target-0x129ba40;if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}
 static void*vt[37];vt[1]=(void*)&AddRef;vt[2]=(void*)&Release;vt[36]=(void*)&Create;memcpy(immutable,FormatTable,1692);
 typedef bool(__fastcall *SF)(void*,void*);typedef void(__fastcall *AF)(void*,void*,void*,bool);typedef bool(__fastcall *IF)(void*,void*,void*,void*,bool);
 bool native=strcmp(argv[3],"retail")==0;SF sf=native?(SF)code:&Signed;AF af=native?(AF)(code+32):&Attach;IF im=native?(IF)(code+112):&Image;
 while(fread(p,4,13,in)==13){
  memset(arena,0x95,8192);W(arena,p[1]+4)=p[2];W(arena,p[1]+8)=7;W(arena,p[1]+12)=0x87654321;W(arena,0x1800)=W(arena,0x1840)=W(arena,0x1600)=W(arena,0x1640)=(unsigned)vt;
  W(arena,0x1060)=(unsigned)(arena+0x1200);W(arena,0x1258)=(unsigned)(arena+0x1600);W(arena,0x1458)=(unsigned)(arena+0x1640);
  W(arena,p[11])=p[6];W(arena,p[11]+4)=p[7];W(arena,p[12])=p[5];memcpy(expected,arena,8192);releases=adds=roots=creates=0;
  if(p[0]==0){bool value=sf(arena+p[12],0);REQUIRE(value==(*(const unsigned*)(FormatTable+36*p[5]+4)==6));}
  else if(p[0]==1){af(arena+p[1],0,(void*)p[3],p[4]!=0);if(!p[4])AttachExpected();REQUIRE(adds==(p[4]?1:0)&&releases==(p[2]?1:0));}
  else {bool value=im(arena+p[1],0,arena+p[11],arena+p[12],p[4]!=0);W(expected,p[1]+8)=1;REQUIRE(value==((long)p[8]>=0)&&roots==1&&creates==1);}
  Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SURFACE_OWNERSHIP_PASS cases=%u\n",cases);return 0;
}
