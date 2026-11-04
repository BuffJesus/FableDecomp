#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo.h"
namespace _STL { template<class I,class T,class C> void FABLE_FASTCALL __unguarded_linear_insert(I,T,C); }
#include "subprimitive_insertion_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
typedef CEngineSubPrimitiveRenderer::CSortElement E;typedef _STL::less<E> Compare;
static unsigned char *arena,initial[65536],expected[65536];static unsigned cases,snapshots;
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Stable(unsigned offset,unsigned count){for(unsigned i=1;i<count;++i){unsigned key=W(expected,offset+i*8),payload=W(expected,offset+i*8+4),j=i;while(j&&W(expected,offset+(j-1)*8)>key){W(expected,offset+j*8)=W(expected,offset+(j-1)*8);W(expected,offset+j*8+4)=W(expected,offset+(j-1)*8+4);--j;}W(expected,offset+j*8)=key;W(expected,offset+j*8+4)=payload;}}
int main(){
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<2;++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=(unsigned)(code+f[1])-(unsigned)(code+f[0]+4);}
 const unsigned sizes[]={0,1,2,15,16,17,31,32,255,256,257};
 for(unsigned kind=0;kind<4;++kind)for(unsigned n=0;n<11;++n)for(unsigned pattern=0;pattern<8;++pattern)for(unsigned location=0;location<2;++location){
  unsigned count=sizes[n],prefix=kind==1?16:kind>=2?count:0,offset=location?0x4000:0x1000;
  if(kind>=2&&!count)continue;
  unsigned length=kind==1?prefix+count:kind>=2?count+1:count,random=0x87654321;
  for(i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+pattern*29);
  for(i=0;i<length;++i){random=random*1664525+1013904223;unsigned key;
   switch(pattern){case 0:key=0;break;case 1:key=0xffffffff;break;case 2:key=i;break;case 3:key=length-i;break;case 4:key=(i%4)*0x40100401;break;case 5:key=random;break;case 6:key=(i&1)?0x7fffffff:0x80000000;break;default:key=random%7;}
   W(arena,offset+i*8)=key;W(arena,offset+i*8+4)=i;
  }
  if(prefix){W(arena,offset)=0;memcpy(expected,arena,sizeof(expected));Stable(offset,prefix);memcpy(arena,expected,sizeof(expected));}
  E incoming;memcpy(&incoming,arena+offset+count*8,8);
  memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));Stable(offset,length);
  for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));E*first=(E*)(arena+offset);
   if(kind==0){typedef void(__fastcall*Fn)(E*,E*,Compare);if(run)_STL::__insertion_sort(first,first+count,Compare());else ((Fn)(code+0))(first,first+count,Compare());}
   else if(kind==1){typedef void(__fastcall*Fn)(E*,E*,E*,Compare);if(run)_STL::__unguarded_insertion_sort_aux(first+prefix,first+length,(E*)0,Compare());else ((Fn)(code+64))(first+prefix,first+length,(E*)0,Compare());}
   else if(kind==2){typedef void(__fastcall*Fn)(E*,E*,E,Compare);if(run)_STL::__linear_insert(first,first+count,incoming,Compare());else ((Fn)(code+144))(first,first+count,incoming,Compare());}
   else{typedef void(__fastcall*Fn)(E*,E,Compare);if(run)_STL::__unguarded_linear_insert(first+count,incoming,Compare());else ((Fn)(code+240))(first+count,incoming,Compare());}
   REQUIRE(!memcmp(arena,expected,sizeof(expected)));++snapshots;
  }++cases;
 }
 printf("NATIVE_SUBPRIMITIVE_INSERTION_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
