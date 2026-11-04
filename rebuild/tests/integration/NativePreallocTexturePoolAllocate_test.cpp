#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prealloc_texture_pool_allocate_retail.h"
#include "prealloc_texture_pool_allocate_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall PoolAllocate(void*,void*);
extern "C" unsigned char ReuseBusyByte;
static unsigned arena[4096],initial[4096],images[6][4096];
static unsigned location,mode,steps,count,snapshots,callbackMode;static unsigned owners[11];
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned pointer,unsigned offset){return *(unsigned*)(pointer+offset);}
static unsigned char& byte(unsigned pointer,unsigned offset){return *(unsigned char*)(pointer+offset);}
static unsigned node(unsigned i){return ptr(0x400+i*0x300);}
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void snapshot(){word(ptr(0x2000),60)=ReuseBusyByte;if(!mode){require(count<6);memcpy(images[count++],arena,sizeof(arena));}else{require(steps<count&&!memcmp(images[steps++],arena,sizeof(arena)));++snapshots;}}
extern "C" void __fastcall OwnerReleaseBoundary(void* self,void*){
 unsigned owner=(unsigned)self;require(owner>=ptr(0x1400)&&owner<=ptr(0x1600)&&(owner-ptr(0x1400))%0x100==0);unsigned n=node((owner-ptr(0x1400))/0x100),pool=ptr(0x100);require(byte(n,0x49)==1&&byte(n,0x48)!=0);unsigned call=word(ptr(0x2000),0)++;require(call<5);word(ptr(0x2000),4+call*8)=n;word(ptr(0x2000),8+call*8)=owner;
 if(callbackMode==0)byte(n,0x48)=0;
 if(callbackMode==2){byte(n,0x48)=0;word(pool,0x3c)=0;word(pool,0x14)=pool+4;}
 if(callbackMode==3){word(n,0x30)=ptr(0x1600);byte(n,0x48)=0;}
 if(callbackMode==4){byte(n,0x48)=0;byte(n,0x49)=0xff;byte(n,0x4a)=0xff;}
 if(callbackMode==5)word(pool,0x3c)=1;
 if(callbackMode==6){byte(n,0x48)=0;word(pool,0x14)=0;ReuseBusyByte=ReuseBusyByte?0:1;}
 if(callbackMode==7){unsigned next=word(pool,0x14);if(next&&next!=pool+4){byte(next,0x48)=0;byte(next,0x49)=0;byte(next,0x4a)=0xff;}}
 snapshot();
}
static void setMRU(unsigned object){unsigned list=word(object,8);if(!list)return;unsigned next=word(object,16);if(next){unsigned prev=word(object,12);if(word(prev,16)==object&&word(next,12)==object){word(prev,16)=next;word(next,12)=prev;}word(object,12)=word(object,16)=0;}word(object,16)=list+4;word(object,12)=word(list+4,12);word(word(object,12),16)=object;word(word(object,16),12)=object;}
static unsigned model(){unsigned pool=ptr(0x100);for(unsigned i=0;i<word(pool,0x3c);++i){unsigned current=word(pool,0x14);if(!current||current==pool+4)continue;setMRU(current);if(byte(current,0x4a))byte(current,0x4a)=0;if(byte(current,0x49))continue;byte(current,0x49)=1;if(byte(current,0x48))OwnerReleaseBoundary((void*)word(current,0x30),0);byte(current,0x49)=0;if(!byte(current,0x48))return current;}return 0;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));require(sizeof(fixups)/sizeof(*fixups)==3);for(unsigned i=0;i<3;++i){const Fixup& fix=fixups[i];unsigned target=fix.target==0x80000000?(unsigned)&ReuseBusyByte:(unsigned)(code+fix.target);*(unsigned*)(code+fix.site)=fix.kind==20?target-(unsigned)(code+fix.site+4):target;}memset(owners,0,sizeof(owners));owners[10]=(unsigned)OwnerReleaseBoundary;const unsigned char values[4]={0,1,0x80,0xff};const unsigned counts[5]={0,1,2,3,5};unsigned cases=0;
for(unsigned shape=0;shape<6;++shape)for(unsigned limit=0;limit<5;++limit)for(unsigned busy=0;busy<4;++busy)for(unsigned locked=0;locked<4;++locked)for(unsigned allocated=0;allocated<4;++allocated)for(callbackMode=0;callbackMode<8;++callbackMode)for(unsigned reuse=0;reuse<2;++reuse)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 location=place?0x1800:0;unsigned pool=ptr(0x100),head=pool+4;memset(arena,fill?0xa9:0,sizeof(arena));word(pool,0x3c)=counts[limit];word(head,12)=word(head,16)=head;
 for(unsigned n=0;n<3;++n){unsigned current=node(n);for(unsigned j=0;j<19;++j)word(current,j*4)=pattern(n+j+busy);word(current,8)=pool;word(current,12)=n?node(n-1):head;word(current,16)=n==2?head:node(n+1);word(current,0x20)=pattern(busy+n);word(current,0x30)=ptr(0x1400+n*0x100);byte(current,0x4a)=values[(busy+n)%4];byte(current,0x49)=values[(locked+n)%4];byte(current,0x48)=values[(allocated+n)%4];word(ptr(0x1400+n*0x100),0)=(unsigned)owners;}
 if(shape==1)word(head,16)=0;
 if(shape==2||shape==4){word(head,16)=word(head,12)=node(0);word(node(0),12)=word(node(0),16)=head;if(shape==4){word(node(0),8)=0;word(node(0),12)=word(node(0),16)=0;}}
 if(shape==3||shape==5){word(head,16)=node(0);word(head,12)=node(2);if(shape==5)word(node(0),12)=node(1);}
 memset((void*)ptr(0x2000),0,64);memcpy(initial,arena,sizeof(arena));count=0;unsigned expected=0;
 for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));ReuseBusyByte=(unsigned char)reuse;steps=0;unsigned returned=mode==0?model():(unsigned)(mode==1?((void*(__fastcall*)(void*,void*))code):PoolAllocate)((void*)pool,0);if(!mode)expected=returned;else require(returned==expected);word(ptr(0x2000),56)=returned;snapshot();if(mode)require(steps==count);}++cases;
}VirtualFree(code,0,0x8000);printf("NATIVE_PREALLOC_TEXTURE_POOL_ALLOCATE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
