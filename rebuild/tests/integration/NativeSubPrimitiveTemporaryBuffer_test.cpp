#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subprimitive_temp_aliases.h"
#include "subprimitive_temp_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Allocate(void*,void*);
static unsigned char *arena,initial[65536],expected[65536];static unsigned cases,snapshots,total,cursor,offset,mode,fail;
struct Event{unsigned bytes,result;unsigned char state[65536];};static Event events[32];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned call){a[65535]^=(unsigned char)(call+7);if(call==0){if(mode==1)W(a,offset+4)=17;else if(mode==2)W(a,offset+4)=0xfffffffd;else if(mode==3){W(a,offset)=0xdeadbeef;W(a,offset+8)=(unsigned)(arena+0x2000);}}}
extern "C" void* __cdecl NativeTempMalloc(unsigned bytes){REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(bytes==e.bytes&&!memcmp(arena,e.state,sizeof(expected)));++snapshots;Mutate(arena,cursor++);return (void*)e.result;}
static void Model(){W(expected,offset)=W(expected,offset+4);W(expected,offset+8)=0;if((int)W(expected,offset+4)>0xfffffff)W(expected,offset+4)=0xfffffff;total=0;
 while((int)W(expected,offset+4)>0){REQUIRE(total<32);Event&e=events[total];e.bytes=W(expected,offset+4)*8;e.result=total>=fail?(unsigned)(arena+0x2000+total*64):0;memcpy(e.state,expected,sizeof(expected));Mutate(expected,total++);W(expected,offset+8)=e.result;if(e.result)break;W(expected,offset+4)=(unsigned)((int)W(expected,offset+4)/2);}}
int main(){arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+44)=(unsigned)&NativeTempMalloc-(unsigned)(code+48);
 const int lengths[]={(-2147483647-1),-3,-1,0,1,2,3,15,16,17,255,256,257,0xfffffff,0x10000000,2147483647};const unsigned failures[]={0,1,3,100};
 for(unsigned n=0;n<16;++n)for(unsigned policy=0;policy<4;++policy)for(mode=0;mode<4;++mode)for(unsigned location=0;location<2;++location){offset=location?0x100:0;fail=failures[policy];for(unsigned i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+n*29);W(arena,offset+4)=lengths[n];memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));Model();
  for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));cursor=0;typedef void(__fastcall*Fn)(void*,void*);(run?Allocate:(Fn)code)(arena+offset,0);REQUIRE(cursor==total&&!memcmp(arena,expected,sizeof(expected)));++snapshots;}++cases;
 }printf("NATIVE_SUBPRIMITIVE_TEMP_BUFFER_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
