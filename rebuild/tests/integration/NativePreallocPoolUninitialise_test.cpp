#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pool_uninitialise_retail.h"
#include "pool_uninitialise_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall PoolUninitialise(void*,void*);
static unsigned arena[4096],initial[4096],expected[16][4096],captures[16][4096],captured,mutation,pool,node,base;
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void capture(){require(captured<16);memcpy(captures[captured++],arena,sizeof(arena));}
static void event(unsigned id,unsigned p){unsigned log=base+0x1800;unsigned n=word(log,0)++;require(n<14);word(log,4+n*8)=id;word(log,8+n*8)=p;capture();}
extern "C" void __fastcall RemoveBoundary(void* self,void*,void* resource){require((unsigned)self==pool);node=(unsigned)resource;event(1,node);if(mutation==1)word(pool,0x3c)=0;if(mutation==2)word(pool,0x48)=base+0x500;if(mutation==7){word(node,0x24)=base+0x680;word(node,0x34)=0xffffffff;}}
extern "C" void __fastcall DeleteDataBoundary(void* data){event(2,(unsigned)data);if(mutation==3)word(node,0x24)=base+0x680;if(mutation==7)word(node,0x20)^=0x100;}
void __cdecl operator delete(void* data){event(3,(unsigned)data);if(mutation==3)word(node,0x24)=base+0x6c0;}
extern "C" void __fastcall TextureBoundary(void* self,void*){require(self==0);require(!word(node,0x20)&&!word(node,0x24)&&!word(node,0x34)&&!word(node,0x3c)&&!word(node,0x40));event(4,0);if(mutation==4)word(pool,0x3c)=0;if(mutation==5)word(pool,0x48)=base+0x500;}
extern "C" void __fastcall VectorBoundary(void* self,void*){require((unsigned)self==pool+0x48);require(!word(pool,0x3c));event(5,(unsigned)self);word(pool,0x48)=0;word(pool,0x4c)=0;word(pool,0x50)=0;if(mutation==6)word(pool,0x3c)=0xffffffff;}
static unsigned pattern(unsigned n){const unsigned values[]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void model(){for(unsigned i=0;i<word(pool,0x3c);++i){unsigned data=word(word(pool,0x48),i*4);RemoveBoundary((void*)pool,0,(void*)data);unsigned info=word(data,0x24);if(info){--word(info,0);info=word(data,0x24);if(!word(info,0)){((void(__fastcall*)(void*))word(info,4))((void*)word(info,8));operator delete((void*)word(data,0x24));}}word(data,0x24)=0;word(data,0x20)=0;word(data,0x34)=0;word(data,0x3c)=0;word(data,0x40)=0;TextureBoundary(0,0);}word(pool,0x3c)=0;VectorBoundary((void*)(pool+0x48),0);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[]={(unsigned)RemoveBoundary,(unsigned)&operator delete,(unsigned)TextureBoundary,(unsigned)VectorBoundary};require(sizeof(fixups)/sizeof(*fixups)==4);for(unsigned f=0;f<4;++f){require(fixups[f].kind==20&&fixups[f].target<4);*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);}unsigned cases=0,snapshots=0;const unsigned counts[]={0,1,3};
 for(unsigned ci=0;ci<3;++ci)for(unsigned ref=0;ref<8;++ref)for(unsigned control=0;control<2;++control)for(mutation=0;mutation<8;++mutation)for(unsigned alias=0;alias<2;++alias)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){memset(arena,fill?0xa9:0,sizeof(arena));base=(unsigned)arena+(place?0x1800:0);pool=base+0x100;word(pool,0x3c)=counts[ci];word(pool,0x48)=base+0x400;for(unsigned i=0;i<3;++i){unsigned data=base+0x800+i*0x100;for(unsigned j=0;j<19;++j)word(data,j*4)=pattern(ref+j+i);word(data,0x24)=control?base+0x600+(alias?0:i*0x40):0;word(base+0x400,i*4)=data;word(base+0x500,i*4)=base+0x800+(2-i)*0x100;}for(unsigned b=0;b<4;++b){unsigned info=base+0x600+b*0x40;word(info,0)=pattern(ref);word(info,4)=(unsigned)DeleteDataBoundary;word(info,8)=pattern(ref+b);}memset((void*)(base+0x1800),0,128);memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0;
  for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;node=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):PoolUninitialise)((void*)pool,0);capture();if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}}++cases;}
 VirtualFree(code,0,0x8000);printf("NATIVE_PREALLOC_POOL_UNINITIALISE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
