#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_graphic_info.h"
#include "graphic_info_frame_release_data_retail.h"
#include "graphic_info_frame_release_data_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall FrameReleaseData(void*,void*);
static unsigned arena[4096],initial[4096],images[2][4096],mode,step,count,snapshots,location,mutation,node;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
class Boundary{public:void Delete()const;};
void Boundary::Delete()const{require((unsigned)this==node);word(ptr(0x1000),0)=(unsigned)this;if(mutation&1)word(ptr(0x100),8)=ptr(0x800);if(mutation&2)word(ptr(0x100),8)=0;snapshot();word(node,20)=0xdededede;}
static void model(){unsigned data=word(ptr(0x100),8);if(data){if(--word(data,4)==0)((Boundary*)data)->Delete();word(ptr(0x100),8)=0;}word(ptr(0x100),8)=0;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));void(Boundary::*d)()const=&Boundary::Delete;unsigned table[2]={0,0};memcpy(table+1,&d,4);const unsigned raw[8]={0,1,2,3,255,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0;
for(unsigned present=0;present<2;++present)for(unsigned ref=0;ref<8;++ref)for(mutation=0;mutation<4;++mutation)for(unsigned pos=0;pos<2;++pos)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;node=ptr(pos?0x600:0x400);memset(arena,fill?0xa9:0,sizeof(arena));word(node,0)=(unsigned)table;word(node,4)=raw[ref];word(ptr(0x100),8)=present?node:0;memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):FrameReleaseData)((void*)ptr(0x100),0);snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_GRAPHIC_INFO_FRAME_RELEASE_DATA_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
