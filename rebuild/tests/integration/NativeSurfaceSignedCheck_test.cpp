#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "surface_signed_check_aliases.h"
#include "surface_signed_check_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u/%u\n",__LINE__,cases,events,expectedEvents);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Check(void*,void*,void*,void*,void*);
extern "C" const unsigned char FormatTable[1692];
static unsigned char *arena,expected[1024],snapshots[8][1024],immutable[1692];static unsigned p[7],cases,events,expectedEvents,receivers[8];static FILE*output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static unsigned T(unsigned i,unsigned off){return *(const unsigned*)(FormatTable+36*i+off);}
static void Mutate(unsigned char*a,unsigned event){
 if(event!=0)return;
 if(p[6]==1)W(a,p[0]+4)=(unsigned)(arena+0x240);
 if(p[6]==2){W(a,0x300)=1;W(a,0x320)=0;W(a,0x324)=0;W(a,0x328)=256;W(a,0x32c)=128;}
 if(p[6]==3){W(a,0x20c)=60;W(a,0x204)=257;W(a,0x208)=129;}
}
struct Description{unsigned width,height,format;};
static Description ModelDesc(){
 REQUIRE(expectedEvents<8);unsigned ptr=W(expected,p[0]+4),off=ptr-(unsigned)arena;receivers[expectedEvents]=ptr;
 Description d={W(expected,off+4),W(expected,off+8),W(expected,off+12)};
 Mutate(expected,expectedEvents);memcpy(snapshots[expectedEvents],expected,1024);++expectedEvents;return d;
}
static unsigned Index(unsigned format){for(unsigned i=0;i<46;++i)if(T(i,0)==format)return i;REQUIRE(false);return 0;}
static bool IsSigned(unsigned i){return T(i,4)==6;}
static bool Box(unsigned off){
 if(!off)return true;if(W(expected,off)||W(expected,off+4))return false;
 Description d=ModelDesc();if(W(expected,off+8)!=d.width)return false;
 d=ModelDesc();return W(expected,off+12)==d.height;
}
static bool Model(){
 Description d=ModelDesc();if(!IsSigned(Index(d.format))&&!IsSigned(W(expected,0x300)))return false;
 if(!Box(p[3]?0x320:0)||!Box(p[4]?0x340:0))return false;
 unsigned source=W(expected,0x300);if(IsSigned(source)||(long)T(source,8)/8!=4)return false;
 d=ModelDesc();if(!IsSigned(Index(d.format)))return false;
 d=ModelDesc();return (long)T(Index(d.format),8)/8==2;
}
static long __stdcall GetDesc(void*self,void*out){
 REQUIRE(events<expectedEvents&&(unsigned)self==receivers[events]);unsigned off=(unsigned char*)self-arena;
 memset(out,0x73,32);*(unsigned*)out=W(arena,off+12);((unsigned*)out)[6]=W(arena,off+4);((unsigned*)out)[7]=W(arena,off+8);
 Mutate(arena,events);REQUIRE(!memcmp(arena,snapshots[events],1024)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,1024,output);++events;return (long)p[5];
}
static void SetBox(unsigned off,unsigned kind){W(arena,off)=W(arena,off+4)=0;W(arena,off+8)=256;W(arena,off+12)=128;if(kind==2)W(arena,off)=1;if(kind==3)W(arena,off+4)=1;if(kind==4)W(arena,off+8)=255;if(kind==5)W(arena,off+12)=127;}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned target=retailFixups[i][1],value=0;
  switch(target){case 0x415bd0:value=(unsigned)(code+288);break;case 0x415bf0:value=(unsigned)(code+320);break;case 0x9f2f90:value=(unsigned)(code+352);break;case 0x9f2f60:value=(unsigned)(code+400);break;case 0x9e3970:value=(unsigned)(code+448);break;case 0x9e3800:value=(unsigned)(code+480);break;case 0x9e3830:value=(unsigned)(code+512);break;default:REQUIRE(target>=0x129ba40&&target<0x129c0dc);value=(unsigned)FormatTable+target-0x129ba40;}
  if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;
 }
 static void*vt[13];vt[12]=(void*)&GetDesc;memcpy(immutable,FormatTable,1692);typedef bool(__fastcall *Fn)(void*,void*,void*,void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Check;
 while(fread(p,4,7,in)==7){
  memset(arena,0x95,1024);W(arena,p[0]+4)=(unsigned)(arena+0x200);W(arena,0x200)=W(arena,0x240)=(unsigned)vt;
  W(arena,0x204)=W(arena,0x244)=256;W(arena,0x208)=W(arena,0x248)=128;W(arena,0x20c)=T(p[1],0);W(arena,0x24c)=60;W(arena,0x300)=p[2];SetBox(0x320,p[3]);SetBox(0x340,p[4]);
  memcpy(expected,arena,1024);events=expectedEvents=0;bool want=Model();bool result=fn(arena+p[0],0,p[3]?arena+0x320:0,p[4]?arena+0x340:0,arena+0x300);
  REQUIRE(result==want&&events==expectedEvents&&!memcmp(arena,expected,1024)&&!memcmp(immutable,FormatTable,1692));fwrite(arena,1,1024,output);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SURFACE_SIGNED_CHECK_PASS cases=%u\n",cases);return 0;
}
