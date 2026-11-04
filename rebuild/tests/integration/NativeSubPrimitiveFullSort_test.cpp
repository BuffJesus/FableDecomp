#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_heap.h"
#include "subprimitive_full_sort_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
typedef CEngineSubPrimitiveRenderer::CSortElement E;typedef _STL::less<E> Compare;
static unsigned char *arena,initial[65536],nativeState[65536],midState[65536];static unsigned cases,snapshots,count,offset;
static unsigned keys[1100],sorted[1100],seen[1100];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Verify(unsigned length){
 REQUIRE(!memcmp(arena,initial,offset)&&!memcmp(arena+offset+count*8,initial+offset+count*8,65536-offset-count*8));memset(seen,0,sizeof(seen));
 for(unsigned i=0;i<count;++i){unsigned key=W(arena,offset+i*8),id=W(arena,offset+i*8+4);REQUIRE(id<count&&!seen[id]&&key==keys[id]);seen[id]=1;if(i<length)REQUIRE(key==sorted[i]);}
}
int main(){
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=(unsigned)(code+f[1])-(unsigned)(code+f[0]+4);}
 const unsigned sizes[]={0,1,2,15,16,17,31,32,255,256,257,1025},depths[]={0,1,2,64};
 for(unsigned n=0;n<12;++n)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned location=0;location<2;++location)for(unsigned mode=0;mode<7;++mode){
  count=sizes[n];offset=location?0x4000:0x1000;unsigned random=0x87654321;
  for(i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+pattern*29);
  for(i=0;i<count;++i){random=random*1664525+1013904223;unsigned key;
   switch(pattern){case 0:key=0;break;case 1:key=0xffffffff;break;case 2:key=i;break;case 3:key=count-i;break;case 4:key=(i%4)*0x40100401;break;case 5:key=random;break;case 6:key=(i&1)?0x7fffffff:0x80000000;break;default:key=random%7;}
   keys[i]=sorted[i]=W(arena,offset+i*8)=key;W(arena,offset+i*8+4)=i;
  }
  for(i=1;i<count;++i){unsigned key=sorted[i],j=i;while(j&&sorted[j-1]>key){sorted[j]=sorted[j-1];--j;}sorted[j]=key;}
  memcpy(initial,arena,sizeof(initial));unsigned middle=(count+1)/2;
  for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));E*first=(E*)(arena+offset),*last=first+count;
   if(mode==0){typedef void(__fastcall*Fn)(E*,E*);if(run)_STL::sort(first,last);else ((Fn)(code+0))(first,last);}
   else if(mode<5){typedef void(__fastcall*Fn)(E*,E*,E*,ptrdiff_t,Compare);if(run)_STL::__introsort_loop(first,last,(E*)0,ptrdiff_t(depths[mode-1]),Compare());else ((Fn)(code+128))(first,last,(E*)0,ptrdiff_t(depths[mode-1]),Compare());if(run)REQUIRE(!memcmp(arena,midState,sizeof(midState)));else memcpy(midState,arena,sizeof(midState));++snapshots;typedef void(__fastcall*Ins)(E*,E*,Compare);if(run)_STL::__insertion_sort(first,last,Compare());else ((Ins)(code+944))(first,last,Compare());}
   else if(mode==5){typedef void(__fastcall*Fn)(E*,E*,E*,E*,Compare);if(run)_STL::__partial_sort(first,first+middle,last,(E*)0,Compare());else ((Fn)(code+832))(first,first+middle,last,(E*)0,Compare());}
   else{typedef void(__fastcall*Make)(E*,E*,Compare,E*,ptrdiff_t*);if(run)_STL::__make_heap(first,last,Compare(),(E*)0,(ptrdiff_t*)0);else ((Make)(code+624))(first,last,Compare(),(E*)0,(ptrdiff_t*)0);for(unsigned child=1;child<count;++child)REQUIRE(first[(child-1)/2].SortValue>=first[child].SortValue);if(run)REQUIRE(!memcmp(arena,midState,sizeof(midState)));else memcpy(midState,arena,sizeof(midState));++snapshots;typedef void(__fastcall*Sort)(E*,E*,Compare);if(run)_STL::sort_heap(first,last,Compare());else ((Sort)(code+736))(first,last,Compare());}
   Verify(mode==5?middle:count);if(run)REQUIRE(!memcmp(arena,nativeState,sizeof(nativeState)));else memcpy(nativeState,arena,sizeof(nativeState));++snapshots;
  }++cases;
 }
 printf("NATIVE_SUBPRIMITIVE_FULL_SORT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
