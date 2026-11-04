#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame_data_is_loaded_retail.h"
#include "frame_data_is_loaded_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall IsLoaded(void*,void*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,mutation,entered,left;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void __stdcall enter(void* section){require((unsigned)section==ptr(0x114c)&&!entered&&!left);entered=1;word(ptr(0x2000),0)=1;word(ptr(0x2000),4)=(unsigned)section;if(mutation==1)*(unsigned char*)ptr(0x444)=!*(unsigned char*)ptr(0x444);if(mutation==2){word(ptr(0x100),48)=ptr(0x600);word(ptr(0x400),40)=ptr(0x1400);}if(mutation==3)*(unsigned char*)ptr(0x444)=0;snapshot();}
static void __stdcall leave(void* section){require((unsigned)section==ptr(0x114c)&&entered&&!left);left=1;word(ptr(0x2000),8)=1;if(mutation==3)*(unsigned char*)ptr(0x444)=1;if(mutation==4)*(unsigned char*)ptr(0x446)=0x80;snapshot();}
static int __stdcall tryEnter(void*){abort();return 0;}
extern "C"{unsigned EnterIAT=(unsigned)&enter,LeaveIAT=(unsigned)&leave,TryIAT=(unsigned)&tryEnter;}
static bool model(){unsigned data=word(ptr(0x100),48);if(!data)return true;unsigned section=word(data,40)+0x14c;enter((void*)section);bool finished=*(unsigned char*)(data+68)!=0;if(finished)*(unsigned char*)(data+70)=0;leave((void*)section);return finished;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));unsigned targets[3]={(unsigned)&EnterIAT,(unsigned)&TryIAT,(unsigned)&LeaveIAT};for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}const unsigned finishedBytes[5]={0,1,2,0x80,0xff},restartBytes[6]={0,1,2,0x7f,0x80,0xff};unsigned cases=0;
for(unsigned live=0;live<2;++live)for(unsigned finished=0;finished<5;++finished)for(unsigned restart=0;restart<6;++restart)for(mutation=0;mutation<5;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),48)=live?ptr(0x400):0;word(ptr(0x100),52)=ptr(0x800);word(ptr(0x400),40)=ptr(0x1000);word(ptr(0x600),40)=ptr(0x1400);*(unsigned char*)ptr(0x444)=(unsigned char)finishedBytes[finished];*(unsigned char*)ptr(0x446)=(unsigned char)restartBytes[restart];*(unsigned char*)ptr(0x644)=!finishedBytes[finished];memset((void*)ptr(0x2000),0,16);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=entered=left=0;bool result=!mode?model():(mode==1?((bool(__fastcall*)(void*,void*))code):IsLoaded)((void*)ptr(0x100),0);word(ptr(0x2000),12)=result?1:0;snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_IS_LOADED_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
