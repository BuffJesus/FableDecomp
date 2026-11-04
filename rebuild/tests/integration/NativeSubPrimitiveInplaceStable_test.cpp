#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_merge.h"
#include "fable_stlport_tempbuf.h"
#include "subprimitive_inplace_stable_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
typedef CEngineSubPrimitiveRenderer::CSortElement E;typedef _STL::less<E> Compare;
static unsigned char *arena,initial[65536],expected[65536];static unsigned cases,snapshots;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Stable(unsigned offset,unsigned count){for(unsigned i=1;i<count;++i){unsigned key=W(expected,offset+i*8),payload=W(expected,offset+i*8+4),j=i;while(j&&W(expected,offset+(j-1)*8)>key){W(expected,offset+j*8)=W(expected,offset+(j-1)*8);W(expected,offset+j*8+4)=W(expected,offset+(j-1)*8+4);--j;}W(expected,offset+j*8)=key;W(expected,offset+j*8+4)=payload;}}
int main(){arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=(unsigned)(code+f[1])-(unsigned)(code+f[0]+4);}
 const unsigned sizes[]={0,1,2,14,15,16,17,31,32,255,257,1025};
 for(unsigned n=0;n<12;++n)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned location=0;location<2;++location)for(unsigned mode=0;mode<5;++mode)for(unsigned variant=0;variant<(mode==0?1:mode==1||mode==2?5:mode==3?6:2);++variant){
  unsigned count=sizes[n],offset=location?0x4000:0x1000,split=variant==0?0:variant==1?(count?1:0):variant==2?count/2:variant==3?(count?count-1:0):count,random=0x87654321;
  for(i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+pattern*29);
  for(i=0;i<count;++i){random=random*1664525+1013904223;unsigned key;switch(pattern){case 0:key=0;break;case 1:key=0xffffffff;break;case 2:key=i;break;case 3:key=count-i;break;case 4:key=(i%4)*0x40100401;break;case 5:key=random;break;case 6:key=(i&1)?0x7fffffff:0x80000000;break;default:key=random%7;}W(arena,offset+i*8)=key;W(arena,offset+i*8+4)=i;}
  memcpy(expected,arena,sizeof(expected));if(mode==1){Stable(offset,split);Stable(offset+split*8,count-split);memcpy(arena,expected,sizeof(expected));}else if(mode==3){Stable(offset,count);memcpy(arena,expected,sizeof(expected));}
  memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));unsigned answer=0,out=variant?0xc000:0x8000;E value;value.SortValue=variant==0?0:variant==1?1:variant==2?0x7fffffff:variant==3?0xffffffff:count?W(initial,offset+(variant==4?0:count-1)*8):17;value.SubPrimitive=0;
  if(mode==0||mode==1)Stable(offset,count);
  else if(mode==2){for(i=0;i<count;++i)memcpy(expected+offset+i*8,initial+offset+((i+split)%count)*8,8);answer=offset+(count-split)*8;}
  else if(mode==4){for(i=0;i<count*8;++i){expected[offset+i]=initial[out+i];expected[out+i]=initial[offset+i];}answer=out+count*8;}
  for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));E*first=(E*)(arena+offset),*last=first+count,*result=0;
   if(mode==0){typedef void(__fastcall*Fn)(E*,E*,Compare);if(run)_STL::__inplace_stable_sort(first,last,Compare());else ((Fn)(code+0))(first,last,Compare());}
   else if(mode==1){typedef void(__fastcall*Fn)(E*,E*,E*,ptrdiff_t,ptrdiff_t,Compare);if(run)_STL::__merge_without_buffer(first,first+split,last,ptrdiff_t(split),ptrdiff_t(count-split),Compare());else ((Fn)(code+112))(first,first+split,last,ptrdiff_t(split),ptrdiff_t(count-split),Compare());}
   else if(mode==2){typedef E*(__fastcall*Fn)(E*,E*,E*,ptrdiff_t*,_STL::random_access_iterator_tag);result=run?_STL::__rotate(first,first+split,last,(ptrdiff_t*)0,_STL::random_access_iterator_tag()):((Fn)(code+528))(first,first+split,last,(ptrdiff_t*)0,_STL::random_access_iterator_tag());REQUIRE(result==(E*)(arena+answer));}
   else if(mode==3){typedef E*(__fastcall*Fn)(E*,E*,const E&,Compare,ptrdiff_t*);unsigned lo=0,hi=0;while(lo<count&&W(initial,offset+lo*8)<value.SortValue)++lo;while(hi<count&&W(initial,offset+hi*8)<=value.SortValue)++hi;result=run?_STL::__lower_bound(first,last,value,Compare(),(ptrdiff_t*)0):((Fn)(code+400))(first,last,value,Compare(),(ptrdiff_t*)0);REQUIRE(result==first+lo);result=run?_STL::__upper_bound(first,last,value,Compare(),(ptrdiff_t*)0):((Fn)(code+464))(first,last,value,Compare(),(ptrdiff_t*)0);REQUIRE(result==first+hi);}
   else{typedef E*(__fastcall*Fn)(E*,E*,E*);result=run?_STL::swap_ranges(first,last,(E*)(arena+out)):((Fn)(code+896))(first,last,(E*)(arena+out));REQUIRE(result==(E*)(arena+answer));}
   REQUIRE(!memcmp(arena,expected,sizeof(expected)));++snapshots;
  }++cases;
 }printf("NATIVE_SUBPRIMITIVE_INPLACE_STABLE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
