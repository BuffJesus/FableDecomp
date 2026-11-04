#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shadow_cleanup_aliases.h"
#include "shadow_cleanup_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,caseNumber);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Cleanup(void*,void*);
extern "C" void __fastcall ReleaseResources(void*,void*);
static unsigned char* arena;static unsigned p[3],caseNumber,eventCount,expectedCount;
static unsigned char expected[4096],eventStates[6][4096];static unsigned expectedReceivers[6];static FILE* output;
static const unsigned slots[6]={0x2cc,0x2dc,0x30c,0x2fc,0x318,0x2e8};
static unsigned& Word(unsigned char* data,unsigned offset){return *(unsigned*)(data+offset);}
static void mutate(unsigned char* data,unsigned receiver,unsigned slot){
 ++Word(data,receiver+4);data[p[0]+0x2af]=(unsigned char)(0xa0+slot);
 if(p[1]==1 && slot<5)Word(data,p[0]+slots[slot+1])=0;
 if(p[1]==2 && slot<5)Word(data,p[0]+slots[slot+1])=(unsigned)(arena+0xc80+16*(slot+1));
 if(p[1]==3)Word(data,p[0]+slots[slot])=(unsigned)(arena+0xc80+16*slot);
}
static unsigned __stdcall Release(void* self){
 REQUIRE(eventCount<expectedCount);unsigned off=(unsigned char*)self-arena;REQUIRE(off==expectedReceivers[eventCount]);
 unsigned slot=Word(arena,off+8);mutate(arena,off,slot);REQUIRE(!memcmp(arena,eventStates[eventCount],4096));
 fwrite(arena,1,4096,output);++eventCount;return 7;
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i)*(unsigned*)(code+retailFixups[i][0])=(unsigned)(code+retailFixups[i][1])-(unsigned)(code+retailFixups[i][0]+4);
 static void* table[3];table[2]=(void*)&Release;typedef void(__fastcall *Fn)(void*,void*);bool native=strcmp(argv[3],"retail")==0;
 while(fread(p,4,3,in)==3){
  if(fread(arena,1,4096,in)!=4096)return 6;
  for(i=0;i<6;++i)for(unsigned bank=0;bank<2;++bank){unsigned off=0xc00+0x80*bank+16*i;Word(arena,off)=(unsigned)table;Word(arena,off+8)=i;}
  memcpy(expected,arena,4096);expectedCount=eventCount=0;
  if(expected[p[0]+0x2af]){
   for(i=0;i<6;++i){unsigned at=p[0]+slots[i];unsigned value=Word(expected,at);if(!value)continue;
    unsigned receiver=value-(unsigned)arena;REQUIRE(receiver>=0xc00&&receiver<0xce0);expectedReceivers[expectedCount]=receiver;
    mutate(expected,receiver,i);memcpy(eventStates[expectedCount++],expected,4096);
    Word(expected,at)=0;if(i<4)Word(expected,at+4)=0;else Word(expected,at+4)&=0x0fffffff;
   }
   expected[p[0]+0x2af]=0;
  }
  Fn fn=native?(Fn)(code+(p[2]?80:0)):(p[2]?&ReleaseResources:&Cleanup);fn(arena+p[0],0);
  REQUIRE(eventCount==expectedCount&&!memcmp(arena,expected,4096));fwrite(arena,1,4096,output);++caseNumber;
 }
 fclose(in);fclose(output);printf("NATIVE_SHADOW_CLEANUP_PASS cases=%u\n",caseNumber);return 0;
}
