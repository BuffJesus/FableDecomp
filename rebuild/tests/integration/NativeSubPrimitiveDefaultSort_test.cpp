#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_stlport_algo.h"
#include "fable_stlport_tree.h"
#include "subprimitive_default_sort_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char *arena,initial[65536],expected[65536];
static unsigned cases,snapshots,count,offset,mode,cursor,total;
struct Event{unsigned kind,first,last,depth;unsigned char state[65536];};static Event events[3];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u event %u\n",__LINE__,cases,cursor);exit(70);}}while(0)
static void Mutate(unsigned char*a,unsigned kind,unsigned first,unsigned last){W(a,0x100)^=0x1234567*kind;W(a,first)^=kind+mode*0x9876543;if(last>first)W(a,last-4)^=kind*0x10203;}
static void Call(unsigned kind,void*first,void*last,unsigned depth){REQUIRE(cursor<total);Event&e=events[cursor];REQUIRE(kind==e.kind&&first==arena+e.first&&last==arena+e.last&&depth==e.depth&&!memcmp(arena,e.state,sizeof(expected)));++snapshots;Mutate(arena,kind,e.first,e.last);++cursor;}
static void Emit(unsigned kind,unsigned first,unsigned last,unsigned depth=0){REQUIRE(total<3);Event&e=events[total++];e.kind=kind;e.first=first;e.last=last;e.depth=depth;memcpy(e.state,expected,sizeof(expected));Mutate(expected,kind,first,last);}
extern "C" void __fastcall Intro(void*first,void*last,void*type,int depth,unsigned){REQUIRE(!type);Call(1,first,last,depth);}
extern "C" void __fastcall Insert(void*first,void*last,unsigned){Call(2,first,last,0);}
extern "C" void __fastcall Aux(void*first,void*last,void*type,unsigned){REQUIRE(!type);Call(3,first,last,0);}
namespace _STL {
typedef CEngineSubPrimitiveRenderer::CSortElement E;
template<> void FABLE_FASTCALL __introsort_loop(E*first,E*last,E*type,ptrdiff_t depth,less<E>){Intro(first,last,type,depth,0);}
template<> void FABLE_FASTCALL __insertion_sort(E*first,E*last,less<E>){Insert(first,last,0);}
template<> void FABLE_FASTCALL __unguarded_insertion_sort_aux(E*first,E*last,E*type,less<E>){Aux(first,last,type,0);}
}
int main(){
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[]={(unsigned)&Intro,(unsigned)&Insert,(unsigned)&Aux};
 for(unsigned i=0;i<4;++i){const unsigned*f=retailFixups[i];*(unsigned*)(code+f[0])=targets[f[1]]-(unsigned)(code+f[0]+4);}
 const unsigned sizes[]={0,1,2,15,16,17,31,32,255,256,1024,1025};
 for(unsigned n=0;n<12;++n)for(unsigned pattern=0;pattern<4;++pattern)for(unsigned location=0;location<2;++location)for(mode=0;mode<4;++mode){
  count=sizes[n];offset=location?0x4000:0x1000;
  for(i=0;i<65536;++i)arena[i]=(unsigned char)(i*13+pattern*29);
  for(i=0;i<count;++i){W(arena,offset+i*8)=pattern?count-i:i;W(arena,offset+i*8+4)=i;}
  memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));total=0;
  if(count){unsigned depth=0;for(unsigned power=1;power<=count/2;power*=2)++depth;Emit(1,offset,offset+count*8,depth*2);if(count>16){Emit(2,offset,offset+128);Emit(3,offset+128,offset+count*8);}else Emit(2,offset,offset+count*8);}
  for(unsigned run=0;run<2;++run){memcpy(arena,initial,sizeof(initial));cursor=0;if(run)_STL::sort((CEngineSubPrimitiveRenderer::CSortElement*)(arena+offset),(CEngineSubPrimitiveRenderer::CSortElement*)(arena+offset+count*8));else{typedef void(__fastcall*Fn)(void*,void*);((Fn)code)(arena+offset,arena+offset+count*8);}REQUIRE(cursor==total&&!memcmp(arena,expected,sizeof(expected)));++snapshots;}++cases;
 }
 printf("NATIVE_SUBPRIMITIVE_DEFAULT_SORT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
