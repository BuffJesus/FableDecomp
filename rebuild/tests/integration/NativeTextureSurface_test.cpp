#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texture_surface_aliases.h"
#include "texture_surface_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* SurfaceVtable[];
extern "C" void* __fastcall Copy(void*,void*,void*);
extern "C" void __fastcall Init(void*,void*,void*);
extern "C" void* __fastcall GetLevel(void*,void*,void*,long);
static unsigned char *arena,expected[4096],states[3][4096];static unsigned p[7],cases,events,count,receivers[3],kinds[3];static FILE* output;
static unsigned& W(unsigned char* a,unsigned off){return *(unsigned*)(a+off);}
static void mutate(unsigned char* a,unsigned receiver,unsigned kind){
 ++W(a,receiver+4);
 if(kind==2 && p[3]==1){W(a,p[1]+12)=0xabcdef01;W(a,0x100)=0;}
 if(kind==1 && p[3]==2){W(a,p[1]+4)=0;W(a,p[1]+8)=2;W(a,p[1]+12)=0x12345678;}
 if(kind==0 && p[3]==3){W(a,p[1]+4)=(unsigned)(arena+0xc40);W(a,p[1]+8)=0xffffffff;W(a,p[1]+12)=0x87654321;}
}
static void expectedEvent(unsigned receiver,unsigned kind){
 REQUIRE(count<3);receivers[count]=receiver;kinds[count]=kind;mutate(expected,receiver,kind);memcpy(states[count++],expected,4096);
}
static unsigned event(void* self,unsigned kind){
 unsigned receiver=(unsigned char*)self-arena;REQUIRE(events<count && receiver==receivers[events] && kind==kinds[events]);
 mutate(arena,receiver,kind);REQUIRE(!memcmp(arena,states[events],4096));fwrite(arena,1,4096,output);++events;return 17;
}
static unsigned __stdcall Release(void*self){return event(self,0);}
static unsigned __stdcall AddRef(void*self){return event(self,1);}
static long __stdcall Mipmap(void*self,unsigned level,void** out){
 REQUIRE(level==p[5]&&((unsigned)out<0x24000000||(unsigned)out>=0x24010000));
 *out=(void*)p[4];event(self,2);return (long)p[6];
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned val=retailFixups[i][2]?(unsigned)(code+retailFixups[i][1])-(unsigned)(code+retailFixups[i][0]+4):(unsigned)SurfaceVtable;*(unsigned*)(code+retailFixups[i][0])=val;}
 static void* table[19];table[1]=(void*)&AddRef;table[2]=(void*)&Release;table[18]=(void*)&Mipmap;bool native=strcmp(argv[3],"retail")==0;
 typedef void*(__fastcall *CopyFn)(void*,void*,void*);typedef void(__fastcall *InitFn)(void*,void*,void*);typedef void*(__fastcall *GetFn)(void*,void*,void*,long);
 while(fread(p,4,7,in)==7){
  if(fread(arena,1,4096,in)!=4096)return 6;for(i=0;i<4;++i)W(arena,0xc00+32*i)=(unsigned)table;
  events=count=0;memcpy(expected,arena,4096);unsigned dst=p[1],src=p[2];
  if(p[0]==0){
   W(expected,dst)=(unsigned)SurfaceVtable;W(expected,dst+4)=W(expected,src+4);W(expected,dst+8)=W(expected,src+8);
   if(W(expected,dst+8)==2)W(expected,dst+12)=W(expected,src+12);
   if(W(expected,dst+4))expectedEvent(W(expected,dst+4)-(unsigned)arena,1);
   CopyFn fn=native?(CopyFn)code:&Copy;REQUIRE(fn(arena+dst,0,arena+src)==arena+dst);
  }else if(p[0]==1){
   if(W(expected,dst+4)){expectedEvent(W(expected,dst+4)-(unsigned)arena,0);W(expected,dst+4)=W(expected,dst+8)=0;}
   W(expected,dst+4)=p[4];W(expected,dst+8)=3;
   InitFn fn=native?(InitFn)(code+64):&Init;fn(arena+dst,0,(void*)p[4]);
  }else{
   expectedEvent(W(expected,0x100)-(unsigned)arena,2);
   W(expected,dst)=(unsigned)SurfaceVtable;W(expected,dst+4)=p[4];W(expected,dst+8)=3;
   if(p[4]){expectedEvent(p[4]-(unsigned)arena,1);expectedEvent(p[4]-(unsigned)arena,0);}
   GetFn fn=native?(GetFn)(code+144):&GetLevel;REQUIRE(fn(arena+0x100,0,arena+dst,(long)p[5])==arena+dst);
  }
  REQUIRE(events==count&&!memcmp(arena,expected,4096));fwrite(arena,1,4096,output);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_TEXTURE_SURFACE_PASS cases=%u\n",cases);return 0;
}
