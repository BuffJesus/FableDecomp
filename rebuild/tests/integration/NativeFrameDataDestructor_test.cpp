#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_graphic_info.h"
#include "frame_data_destructor_retail.h"
#include "frame_data_destructor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall FrameDataDestructor(void*,void*);
extern "C" unsigned FrameVTable[],ResourceVTable[],PointeeVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[4096],initial[4096],images[12][4096],mode,step,count,snapshots,location,mutation,calls;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<12);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(unsigned kind,unsigned object){++calls;word(ptr(0x2000),0)=calls;word(ptr(0x2000),4)=kind;word(ptr(0x2000),8)=object;
 if(kind==0&&mutation==1)word(ptr(0x100),28)=ptr(0x700);
 if(kind==2&&mutation==2){if(object==ptr(0xe00))word(ptr(0x100),52)=ptr(0xb00);else if(object==ptr(0xe80))word(ptr(0x600),12)=ptr(0xd00);else if(object==ptr(0xf80))word(ptr(0x700),12)=ptr(0xc00);}
 if(kind==3&&mutation==3)word(ptr(0x100),28)=ptr(0x700);
 if(kind==4&&mutation==4)word(ptr(0x100),28)=ptr(0x600);
 if(kind==4&&mutation==2)word(ptr(0x100),28)=ptr(0x600);
 if(kind==4&&mutation==6)word(ptr(0x100),28)=ptr(0x700);
 if(kind==5&&mutation==5)word(object,12)=ptr(0xd00);
 if(kind==6&&mutation==6)word(ptr(0x100),52)=0;
 if(kind==7&&mutation==7)word(ptr(0x100),8)=ptr(0x1500);
 snapshot();}
static void __fastcall deleteData(void* p){event(2,(unsigned)p);}
void __cdecl operator delete(void* p){event(6,(unsigned)p);}
class Boundary {public:bool ManagedRelease();void FrameUninit();void ManagedDtor();void GraphicUninit();void RemoveResource(CResource&);};
bool Boundary::ManagedRelease(){require((unsigned)this==ptr(0x120));event(0,(unsigned)this);return false;}
void Boundary::FrameUninit(){require((unsigned)this==ptr(0x11c));event(1,(unsigned)this);word(ptr(0x100),28)=mutation==5?ptr(0x600):0;}
void Boundary::ManagedDtor(){require((unsigned)this==ptr(0x120)||(unsigned)this==ptr(0x128));event((unsigned)this==ptr(0x128)?3:4,(unsigned)this);}
void Boundary::GraphicUninit(){require((unsigned)this==ptr(0x600)||(unsigned)this==ptr(0x700));event(5,(unsigned)this);}
void Boundary::RemoveResource(CResource& r){require((unsigned)&r==ptr(0x100)&&((unsigned)this==ptr(0x1400)||(unsigned)this==ptr(0x1500)));event(7,(unsigned)this);word(ptr(0x100),8)=0;}
static void pointerDtor(unsigned slot){unsigned info=word(slot,4);if(info){--word(info,0);info=word(slot,4);if(!word(info,0)){deleteData((void*)word(info,8));operator delete((void*)word(slot,4));}}word(slot,0)=word(slot,4)=0;}
static void model(){unsigned self=ptr(0x100);word(self,0)=(unsigned)FrameVTable;((Boundary*)(self+32))->ManagedRelease();((Boundary*)(self+28))->FrameUninit();*(signed char*)(self+56)=-1;*(signed char*)(self+58)=127;pointerDtor(self+48);((Boundary*)(self+40))->ManagedDtor();((Boundary*)(self+32))->ManagedDtor();unsigned frame=word(self,28);if(frame){if(--word(frame,0)==0){if(*(unsigned char*)(frame+26)&16)((Boundary*)frame)->GraphicUninit();pointerDtor(frame+8);operator delete((void*)frame);}word(self,28)=0;}unsigned list=word(self,8);word(self,0)=(unsigned)ResourceVTable;if(list)((Boundary*)list)->RemoveResource(*(CResource*)self);word(self,0)=(unsigned)PointeeVTable;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(Boundary::*release)()=&Boundary::ManagedRelease;void(Boundary::*frame)()=&Boundary::FrameUninit;void(Boundary::*managed)()=&Boundary::ManagedDtor;void(Boundary::*graphic)()=&Boundary::GraphicUninit;void(Boundary::*remove)(CResource&)=&Boundary::RemoveResource;unsigned targets[9]={0,0,(unsigned)&operator delete,0,0,0,(unsigned)FrameVTable,(unsigned)ResourceVTable,(unsigned)PointeeVTable};memcpy(targets,&release,4);memcpy(targets+1,&frame,4);memcpy(targets+3,&managed,4);memcpy(targets+4,&graphic,4);memcpy(targets+5,&remove,4);for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target>=0x80000000?targets[fixups[f].target-0x80000000]:(unsigned)(code+fixups[f].target);*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};unsigned cases=0;
for(unsigned file=0;file<3;++file)for(unsigned frameState=0;frameState<3;++frameState)for(unsigned ref=0;ref<6;++ref)for(unsigned infoLive=0;infoLive<2;++infoLive)for(unsigned flag=0;flag<2;++flag)for(mutation=0;mutation<8;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));unsigned self=ptr(0x100);word(self,28)=frameState?ptr(frameState==1?0x600:0x700):0;word(self,48)=file==1?ptr(0x400):0;word(self,52)=file?ptr(0xa00):0;word(self,8)=flag?ptr(0x1400):0;for(unsigned fi=0;fi<2;++fi){unsigned node=ptr(fi?0x700:0x600);word(node,0)=refs[(ref+fi)%6];word(node,8)=ptr(0x800);word(node,12)=infoLive?ptr(fi?0xd00:0xc00):0;*(unsigned char*)(node+26)=(unsigned char)(flag?16:0);}for(unsigned i=0;i<4;++i){unsigned block=ptr(0xa00+i*256);word(block,0)=refs[(ref+i)%6];word(block,4)=(unsigned)&deleteData;word(block,8)=ptr(i==0?0xe00:i==1?0xf00:i==2?0xe80:0xf80);}memset((void*)ptr(0x2000),0,12);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;if(!mode)model();else(mode==1?((void(__fastcall*)(void*,void*))code):FrameDataDestructor)((void*)self,0);snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_DESTRUCTOR_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
