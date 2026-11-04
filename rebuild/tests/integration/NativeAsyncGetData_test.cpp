#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "async_get_data_retail.h"
#include "async_get_data_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall AsyncGetData(void*,void*);
static unsigned arena[4096],initial[4096],images[4][4096],mode,step,count,snapshots,location,mutation,entered,left,poolCalls;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static unsigned char& byte(unsigned o){return *(unsigned char*)ptr(o);}
static unsigned pattern(unsigned n){const unsigned p[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return p[n&7];}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<4);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void __stdcall enter(void* section){require((unsigned)section==ptr(0x114c)&&!entered&&!left);entered=1;word(ptr(0x2000),0)=1;word(ptr(0x2000),4)=(unsigned)section;if(mutation==1)word(ptr(0x400),40)=ptr(0x1400);if(mutation==2)word(ptr(0x400),28)=0;if(mutation==3)word(ptr(0x400),32)=0xffffffff;if(mutation==4)byte(0x445)=!byte(0x445);if(mutation==5)word(ptr(0x400),8)^=0xffffffff;snapshot();}
static void __stdcall leave(void* section){require((unsigned)section==ptr(0x114c)&&entered&&!left);left=1;word(ptr(0x2000),8)=1;if(mutation==7){word(ptr(0x400),8)^=0xffffffff;byte(0x445)=0x80;}snapshot();}
static int __stdcall tryEnter(void*){abort();return 0;}
extern "C" void __fastcall PoolLock(void* pool,void*,long handle){require(entered&&!left&&!poolCalls&&(unsigned)pool==word(ptr(0x400),28)&&(unsigned)handle==word(ptr(0x400),32));poolCalls=1;word(ptr(0x2000),12)=(unsigned)pool;word(ptr(0x2000),16)=(unsigned)handle;if(mutation==6){word(ptr(0x400),8)^=0xffffffff;word(ptr(0x400),28)=ptr(0xa00);byte(0x445)=0xff;}snapshot();}
extern "C"{unsigned EnterIAT=(unsigned)&enter,LeaveIAT=(unsigned)&leave,TryIAT=(unsigned)&tryEnter;}
static unsigned model(){unsigned self=ptr(0x400),section=word(self,40)+0x14c;enter((void*)section);unsigned pool=word(self,28),handle=word(self,32);if(pool&&handle!=0xffffffff&&!byte(0x445)){PoolLock((void*)pool,0,(long)handle);byte(0x445)=1;}unsigned data=word(self,8);leave((void*)section);return data;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[4]={(unsigned)&EnterIAT,(unsigned)&TryIAT,(unsigned)&LeaveIAT,(unsigned)&PoolLock};for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}const unsigned lockedBytes[5]={0,1,2,0x80,0xff},handles[4]={0xffffffff,0,7,0x80000000};unsigned cases=0;
for(unsigned pool=0;pool<3;++pool)for(unsigned handle=0;handle<4;++handle)for(unsigned locked=0;locked<5;++locked)for(unsigned data=0;data<8;++data)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x400),40)=ptr(0x1000);word(ptr(0x400),28)=pool?ptr(pool==1?0x800:0x900):0;word(ptr(0x400),32)=handles[handle];word(ptr(0x400),8)=pattern(data);byte(0x445)=(unsigned char)lockedBytes[locked];memset((void*)ptr(0x2000),0,32);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=entered=left=poolCalls=0;unsigned result=!mode?model():(unsigned)(mode==1?((void*(__fastcall*)(void*,void*))code):AsyncGetData)((void*)ptr(0x400),0);word(ptr(0x2000),24)=result;word(ptr(0x2000),28)=poolCalls;snapshot();if(mode)require(step==count&&entered&&left);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_ASYNC_GET_DATA_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
