#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shadow_lookup_aliases.h"
#include "shadow_lookup_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u pixel %u\n",__LINE__,cases,pixels);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Build(void*,void*);extern "C" void* SurfaceVtable[];
static unsigned char*arena;static unsigned char expected[0xc0000];static unsigned p[4],cases,pixels,phase,activeBuffer,activePitch;static void*surface;static void*rectangle;static void*colourAddress;static FILE*output;
static unsigned& A(void*self,unsigned off){return *(unsigned*)((unsigned char*)self+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,0xc0000));fwrite(arena,1,0xc0000,output);}
extern "C" void __fastcall InitFormat(void*self,void*,long r,long g,long b,long a,bool sign){REQUIRE(phase++==0&&r==8&&g==8&&b==8&&a==8&&!sign);A(self,0)=7;Record();}
extern "C" bool __fastcall TextureInit(void*self,void*,const long*size,long levels,const unsigned*format,unsigned usage,unsigned pool,bool dynamic,bool mipmaps){
 REQUIRE(phase++==1&&self==arena+p[0]+0x32c&&size[0]==256&&size[1]==256&&levels==1&&*format==7&&usage==0&&pool==1&&!dynamic&&!mipmaps);
 A(self,0)=A(expected+p[0]+0x32c,0)=(unsigned)(arena+0xd00);A(self,4)=A(expected+p[0]+0x32c,4)=0x10040000;Record();return p[2]!=0;
}
extern "C" void* __fastcall GetLevel(void*self,void*,void*out,long level){
 REQUIRE(phase++==2&&self==arena+p[0]+0x32c&&level==0);surface=out;A(out,0)=(unsigned)SurfaceVtable;A(out,4)=(unsigned)(arena+0xc00);A(out,8)=3;A(out,12)=0x12345678;Record();return out;
}
extern "C" void* __fastcall Lock(void*self,void*,void*out,unsigned flags){
 REQUIRE(phase++==3&&self==surface&&flags==0);rectangle=out;A(out,0)=A(out,4)=256;A(out,8)=activePitch=p[1];A(out,12)=(unsigned)(arena+(activeBuffer=0x10000));Record();return out;
}
extern "C" void __fastcall SetPixel(void*self,void*,long x,long y,void*colour,const void*rect){
 REQUIRE(phase==4&&self==surface&&rect==rectangle&&pixels<65536&&x==(long)(pixels%256)&&y==(long)(pixels/256));
 if(!pixels)colourAddress=colour;REQUIRE(colourAddress==colour&&A((void*)rect,0)==256&&A((void*)rect,4)==256&&A((void*)rect,8)==activePitch&&A((void*)rect,12)==(unsigned)(arena+activeBuffer));
 unsigned value=((unsigned)x<<16)|(((unsigned)y&0xfe)<<8);REQUIRE(A(colour,0)==value);unsigned off=activeBuffer+(unsigned)y*activePitch+4*(unsigned)x;A(arena,off)=A(expected,off)=value;
 if(p[3]==1&&pixels==31){activeBuffer=0x60000;A(rectangle,12)=(unsigned)(arena+activeBuffer);}
 if(p[3]==2&&pixels==255){activePitch+=16;A(rectangle,8)=activePitch;}
 if(p[3]==3)A(colour,0)=0xffffffff;
 ++pixels;
}
static long __stdcall Unlock(void*self){REQUIRE(phase++==4&&self==arena+0xc00&&pixels==65536);Record();return (long)0x80004005;}
static unsigned __stdcall Release(void*self){REQUIRE(phase++==5&&self==arena+0xc00);++A(arena,0xc04);++A(expected,0xc04);Record();return 1;}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x100000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned address=retailFixups[i][1],value=0;switch(address){case 0x9e3790:value=(unsigned)&InitFormat;break;case 0x9fa280:value=(unsigned)&TextureInit;break;case 0x9f9e00:value=(unsigned)&GetLevel;break;case 0x9f33e0:value=(unsigned)&Lock;break;case 0x9f31c0:value=(unsigned)&SetPixel;break;case 0x9f2dc0:value=(unsigned)(code+240);break;case 0x9f2e20:value=(unsigned)(code+256);break;case 0x122f84c:value=(unsigned)SurfaceVtable;break;default:abort();}
  if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;
 }
 static void*table[15];table[2]=(void*)&Release;table[14]=(void*)&Unlock;typedef void(__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Build;
 while(fread(p,4,4,in)==4){
  memset(arena,0x95,0xc0000);A(arena,0xc00)=(unsigned)table;memcpy(expected,arena,0xc0000);pixels=phase=0;surface=rectangle=colourAddress=0;
  fn(arena+p[0],0);REQUIRE(pixels==65536&&phase==6);Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SHADOW_LOOKUP_PASS cases=%u pixels=%u\n",cases,cases*65536);return 0;
}
