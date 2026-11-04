#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "surface_attachment_aliases.h"
#include "surface_attachment_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall Assign(void*,void*,void*);
extern "C" void __fastcall Attach(void*,void*,void*);
extern "C" void __fastcall AttachZ(void*,void*,void*);
static unsigned char *arena,expected[4096],states[2][4096];static unsigned p[4],cases,dst,src,events,count,receivers[2],kinds[2];static FILE* output;
static unsigned& W(unsigned char* a,unsigned off){return *(unsigned*)(a+off);}
static void mutate(unsigned char* a,unsigned receiver,unsigned kind){
 ++W(a,receiver+4);
 if(kind==0 && p[3]==1){W(a,src+4)=(unsigned)(arena+0xc40);W(a,src+8)=2;W(a,src+12)=0xdeadbeef;}
 if(kind==0 && p[3]==2){W(a,dst+4)=(unsigned)(arena+0xc40);W(a,dst+8)=0xabc;}
 if(kind==1 && p[3]==3){W(a,dst+4)=0;W(a,dst+8)=0x1234;W(a,dst+12)=0x76543210;}
}
static void expectedEvent(unsigned receiver,unsigned kind){
 REQUIRE(count<2);receivers[count]=receiver;kinds[count]=kind;mutate(expected,receiver,kind);memcpy(states[count++],expected,4096);
}
static unsigned event(void* self,unsigned kind){
 unsigned receiver=(unsigned char*)self-arena;REQUIRE(events<count && receiver==receivers[events] && kind==kinds[events]);
 mutate(arena,receiver,kind);REQUIRE(!memcmp(arena,states[events],4096));fwrite(arena,1,4096,output);++events;return 17;
}
static unsigned __stdcall Release(void*self){return event(self,0);}
static unsigned __stdcall AddRef(void*self){return event(self,1);}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i)*(unsigned*)(code+retailFixups[i][0])=(unsigned)(code+retailFixups[i][1])-(unsigned)(code+retailFixups[i][0]+4);
 static void* table[3];table[1]=(void*)&AddRef;table[2]=(void*)&Release;bool native=strcmp(argv[3],"retail")==0;
 typedef void*(__fastcall *AssignFn)(void*,void*,void*);typedef void(__fastcall *AttachFn)(void*,void*,void*);
 while(fread(p,4,4,in)==4){
  if(fread(arena,1,4096,in)!=4096)return 6;for(i=0;i<3;++i)W(arena,0xc00+32*i)=(unsigned)table;
  dst=p[0]+(p[1]==2?0x14:4);src=p[2];events=count=0;memcpy(expected,arena,4096);
  if(W(expected,dst+4)){expectedEvent(W(expected,dst+4)-(unsigned)arena,0);W(expected,dst+4)=W(expected,dst+8)=0;}
  W(expected,dst+4)=W(expected,src+4);W(expected,dst+8)=W(expected,src+8);if(W(expected,dst+8)==2)W(expected,dst+12)=W(expected,src+12);
  if(W(expected,dst+4))expectedEvent(W(expected,dst+4)-(unsigned)arena,1);
  if(p[1]==0){AssignFn fn=native?(AssignFn)(code+96):&Assign;REQUIRE(fn(arena+dst,0,arena+src)==arena+dst);}
  else{AttachFn fn=native?(AttachFn)(code+(p[1]==1?0:48)):(p[1]==1?&Attach:&AttachZ);fn(arena+p[0],0,arena+src);}
  REQUIRE(events==count&&!memcmp(arena,expected,4096));fwrite(arena,1,4096,output);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SURFACE_ATTACHMENT_PASS cases=%u\n",cases);return 0;
}
