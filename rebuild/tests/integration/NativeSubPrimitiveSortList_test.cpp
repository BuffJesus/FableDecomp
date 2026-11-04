#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_tree.h"
#include "subprimitive_sort_list_aliases.h"
#include "subprimitive_sort_list_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SortList(void*,void*,void*,unsigned);
static unsigned char *arena,initial[65536],expected[65536];
static unsigned cases,snapshots,count,flag,calls,arrayOffset,entryOffset;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static unsigned Route(){return !count?0:flag?1:count<256?2:3;}
static void Mutation(unsigned char*a,unsigned route){W(a,0x10)^=route*0x7654321;W(a,arrayOffset+8)^=0x3456789;for(unsigned i=0;i<count;++i)W(a,entryOffset+i*8+4)^=route*0x10203;}
static void Visit(unsigned route,void*first,void*last){REQUIRE(calls++==0&&route==Route()&&first==arena+entryOffset&&last==arena+entryOffset+count*8&&!memcmp(arena,initial,sizeof(initial)));Mutation(arena,route);}
extern "C" void __fastcall Stable(void*first,void*last,void*type,void*distance,unsigned comparator){REQUIRE(!type&&!distance);Visit(1,first,last);}
extern "C" void __fastcall Ordinary(void*first,void*last){Visit(2,first,last);}
extern "C" void __fastcall Radix(void*owner,void*,void*array){REQUIRE(owner==arena&&array==arena+arrayOffset);Visit(3,arena+entryOffset,arena+entryOffset+count*8);}
namespace _STL {
template<> void FABLE_FASTCALL __stable_sort(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last,CEngineSubPrimitiveRenderer::CSortElement* type,ptrdiff_t* distance,less<CEngineSubPrimitiveRenderer::CSortElement> comparator){Stable(first,last,type,distance,0);}
template<> void FABLE_FASTCALL sort(CEngineSubPrimitiveRenderer::CSortElement* first,CEngineSubPrimitiveRenderer::CSortElement* last){Ordinary(first,last);}
}
int main(){
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[]={(unsigned)&Stable,(unsigned)&Ordinary,(unsigned)&Radix};
 for(unsigned i=0;i<3;++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=targets[f[1]]-(unsigned)(code+f[0]+4);}
 const unsigned sizes[]={0,1,2,15,16,17,254,255,256,257,1024,1025},flags[]={0,1,2,255};
 for(unsigned n=0;n<12;++n)for(unsigned f=0;f<4;++f)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned location=0;location<2;++location){
  count=sizes[n];flag=flags[f];arrayOffset=location?0x300:0x100;entryOffset=location?0x4000:0x1000;
  for(i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+pattern*29);
  W(arena,arrayOffset)=(unsigned)(arena+entryOffset);W(arena,arrayOffset+4)=(unsigned)(arena+entryOffset+count*8);W(arena,arrayOffset+8)=(unsigned)(arena+entryOffset+1200*8);
  for(i=0;i<count;++i){W(arena,entryOffset+i*8)=i*0x7654321+pattern;W(arena,entryOffset+i*8+4)=i;}
  memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));if(Route())Mutation(expected,Route());
  for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));calls=0;typedef void(__fastcall*Fn)(void*,void*,void*,unsigned);(run?SortList:(Fn)code)(arena,0,arena+arrayOffset,flag);REQUIRE(calls==(count?1:0)&&!memcmp(arena,expected,sizeof(expected)));++snapshots;}++cases;
 }
 printf("NATIVE_SUBPRIMITIVE_SORT_LIST_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
