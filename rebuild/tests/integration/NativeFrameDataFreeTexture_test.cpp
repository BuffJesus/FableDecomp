#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_graphic_info.h"
#include "engine/CTextureManager.h"
#include "frame_data_free_texture_retail.h"
#include "frame_data_free_texture_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall FreeFrameTexture(void*,void*,bool);
static unsigned arena[4096],initial[4096],images[8][4096],mode,step,count,snapshots,location,mutation,frameBytes,managedBytes;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static signed char& byte(unsigned p,unsigned o){return *(signed char*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<8);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
static void event(unsigned object,unsigned kind){word(ptr(0x2000),0)=object;word(ptr(0x2000),4)=kind;
 if(kind==0){byte(ptr(0x100),56)=99;if(mutation==2)word(ptr(0x100),28)=0;if(mutation==3)word(ptr(0x100),44)=ptr(0x1100);}
 if(kind==1&&mutation==3)word(ptr(0x100),44)=0;
 if(kind==3&&mutation==1)word(ptr(0x100),64)=ptr(0x500);
 if(kind==5&&mutation==2)word(word(ptr(0x100),64),8)=ptr(0x1200);
 snapshot();}
bool CTextureManager::ReduceMipmapLevel(CGraphicFrame& frame,CManagedTexture& managed,unsigned long levels){require((unsigned)this==ptr(0x1c00)&&(unsigned)&frame==ptr(0x11c)&&(unsigned)&managed==ptr(0x120)&&levels==1);event((unsigned)this,0);return false;}
unsigned long CGraphicFrameData::Sizeof()const{event((unsigned)this,1);return frameBytes;}
unsigned long CManagedTexture::Sizeof()const{require((unsigned)this==ptr(0x128));event((unsigned)this,2);word(ptr(0x100),44)=0;return managedBytes;}
bool CManagedTexture::Release(){require((unsigned)this==ptr(0x120));event((unsigned)this,3);word(ptr(0x100),36)=0;return (mutation&1)!=0;}
void CGraphicFrame::UnInitialise(){require((unsigned)this==ptr(0x11c));event((unsigned)this,4);word(ptr(0x100),28)=0;}
class Boundary{public:void Delete()const;};
void Boundary::Delete()const{require((unsigned)this==ptr(0x100));event((unsigned)this,5);word(ptr(0x100),20)=0xdededede;}
static unsigned age(unsigned node){unsigned list=word(node,8);return list?word(list,44)-word(node,24):0xffffffff;}
static unsigned sizeModel(){unsigned size=72,frame=word(ptr(0x100),28);if(frame){event(frame,1);size+=frameBytes;}if(word(ptr(0x100),44)){event(ptr(0x128),2);word(ptr(0x100),44)=0;size+=managedBytes;}return size;}
static void setSize(unsigned size){unsigned list=word(ptr(0x100),8);if(list){word(list,36)-=word(ptr(0x100),20);word(list,36)+=size;}word(ptr(0x100),20)=size;}
static bool model(bool discard){unsigned self=ptr(0x100);if(word(self,28)&&word(word(word(word(self,64),12),32),0x270)<=age(word(self,36))){unsigned unused=age(self),owner=word(self,64),info=word(owner,12),bank=word(info,32);if(unused<=word(bank,0x26c))return false;unsigned level=(int)byte(self,56)+1;if(!(byte(info,18)&1)&&level<=(unsigned)(int)byte(owner,6)){event(ptr(0x1c00),0);byte(self,56)=(signed char)level;setSize(sizeModel());return true;}if(discard){event(ptr(0x120),3);word(self,36)=0;event(ptr(0x11c),4);word(self,28)=0;byte(self,56)=-1;byte(self,58)=127;unsigned slot=word(self,64)+8,data=word(slot,0);if(data){if(--word(data,4)==0)((Boundary*)data)->Delete();word(slot,0)=0;}word(slot,0)=0;return true;}}setSize(sizeModel());return false;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(CTextureManager::*r)(CGraphicFrame&,CManagedTexture&,unsigned long)=&CTextureManager::ReduceMipmapLevel;unsigned long(CGraphicFrameData::*f)()const=&CGraphicFrameData::Sizeof;unsigned long(CManagedTexture::*s)()const=&CManagedTexture::Sizeof;bool(CManagedTexture::*m)()=&CManagedTexture::Release;void(CGraphicFrame::*u)()=&CGraphicFrame::UnInitialise;unsigned targets[5];memcpy(targets,&r,4);memcpy(targets+1,&f,4);memcpy(targets+2,&s,4);memcpy(targets+3,&m,4);memcpy(targets+4,&u,4);for(unsigned fix=0;fix<sizeof(fixups)/sizeof(fixups[0]);++fix){unsigned target=fixups[fix].target>=0x80000000?targets[fixups[fix].target-0x80000000]:(unsigned)(code+fixups[fix].target);*(unsigned*)(code+fixups[fix].site)=target-(unsigned)(code+fixups[fix].site+4);}void(Boundary::*d)()const=&Boundary::Delete;unsigned table[2]={0,0};memcpy(table+1,&d,4);const unsigned raw[6]={0,1,2,0x7fffffff,0x80000000,0xffffffff};const signed char levels[12][2]={{-128,-128},{-128,127},{-1,-1},{-1,0},{0,0},{0,1},{1,1},{1,2},{126,127},{127,127},{127,-128},{-2,-1}};unsigned cases=0;
for(unsigned frame=0;frame<2;++frame)for(unsigned textureAge=0;textureAge<6;++textureAge)for(unsigned ownAge=0;ownAge<6;++ownAge)for(unsigned pair=0;pair<12;++pair)for(unsigned flat=0;flat<2;++flat)for(unsigned discard=0;discard<2;++discard)for(mutation=0;mutation<4;++mutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;frameBytes=raw[textureAge];managedBytes=raw[ownAge];memset(arena,fill?0xa9:0,sizeof(arena));unsigned self=ptr(0x100);word(self,0)=(unsigned)table;word(self,4)=raw[ownAge];word(self,8)=ownAge==5?0:ptr(0x1500);word(self,20)=raw[(ownAge+1)%6];word(self,24)=0-raw[ownAge];word(self,28)=frame?ptr(0x600):0;word(self,36)=ptr(0x1000);word(self,44)=(textureAge&1)?ptr(0x1100):0;word(self,64)=ptr(0x400);byte(self,56)=levels[pair][0];word(ptr(0x1000),8)=textureAge==5?0:ptr(0x1400);word(ptr(0x1000),24)=0-raw[textureAge];word(ptr(0x1400),44)=0;word(ptr(0x1500),44)=0;word(ptr(0x1500),36)=raw[(textureAge+1)%6];for(unsigned o=0x400;o<=0x500;o+=0x100){word(ptr(o),8)=self;word(ptr(o),12)=ptr(0x800);byte(ptr(o),6)=levels[pair][1];}byte(ptr(0x800),18)=(signed char)flat;word(ptr(0x800),32)=ptr(0x1800);word(ptr(0x1800),0x270)=raw[(textureAge+1)%6];word(ptr(0x1800),0x26c)=raw[(ownAge+1)%6];word(ptr(0x1800),0x280)=ptr(0x1c00);memcpy(initial,arena,sizeof(arena));count=0;bool expected=false;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;bool result=!mode?model(discard!=0):(mode==1?((bool(__fastcall*)(void*,void*,bool))code):FreeFrameTexture)((void*)self,0,discard!=0);snapshot();if(!mode)expected=result;else require(result==expected&&step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_FREE_TEXTURE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
