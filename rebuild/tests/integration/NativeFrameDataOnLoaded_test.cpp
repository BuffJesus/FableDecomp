#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_graphic_info.h"
#include "frame_data_on_loaded_retail.h"
#include "frame_data_on_loaded_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall OnLoaded(void*,void*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,uploadMutation,deleteMutation,uploadResult,uploaded,deleted,cachedSlot;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
bool CGraphicInfoFrame::CFrameData::UploadFrame(bool wait){require((unsigned)this==ptr(0x100)&&!wait&&!uploaded);uploaded=1;word(ptr(0x2000),0)=1;if(uploadMutation==1)word(ptr(0x100),64)=ptr(0x800);if(uploadMutation==2)word(ptr(0x100),28)=0;if(uploadMutation==3)word(ptr(0x100),28)=ptr(0x1400);if(uploadMutation==4)word(word(ptr(0x100),64),8)=ptr(0xa00);if(uploadMutation==5)word(ptr(0x100),48)=0;snapshot();return uploadResult!=0;}
class Boundaries{public:void Delete()const;};
void Boundaries::Delete()const{require(((unsigned)this==ptr(0x600)||(unsigned)this==ptr(0xa00))&&!deleted);deleted=1;word(ptr(0x2000),4)=1;word((unsigned)this,8)=0xdededede;if(deleteMutation==1)word(ptr(0x100),64)=ptr(0x400);if(deleteMutation==2)word(cachedSlot,0)=ptr(0xa00);if(deleteMutation==3){word(ptr(0x100),64)=ptr(0x800);word(ptr(0x100),28)=ptr(0x1400);}snapshot();}
static void model(){unsigned self=ptr(0x100);if(word(self,48)){if(!((CGraphicInfoFrame::CFrameData*)self)->UploadFrame(false)&&!word(self,28)){cachedSlot=word(self,64)+8;unsigned data=word(cachedSlot,0);if(data){if(--word(data,4)==0)((Boundaries*)data)->Delete();word(cachedSlot,0)=0;}word(cachedSlot,0)=0;}}snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(CGraphicInfoFrame::CFrameData::*upload)(bool)=&CGraphicInfoFrame::CFrameData::UploadFrame;unsigned uploadAddress=0;memcpy(&uploadAddress,&upload,4);*(unsigned*)(code+fixupSite)=uploadAddress-(unsigned)(code+fixupSite+4);void(Boundaries::*d)()const=&Boundaries::Delete;unsigned table[2]={0,0};memcpy(&table[1],&d,4);const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};unsigned cases=0;
for(unsigned live=0;live<2;++live)for(unsigned frame=0;frame<2;++frame)for(uploadResult=0;uploadResult<2;++uploadResult)for(unsigned owner=0;owner<2;++owner)for(unsigned dataLive=0;dataLive<2;++dataLive)for(unsigned ref=0;ref<6;++ref)for(uploadMutation=0;uploadMutation<6;++uploadMutation)for(deleteMutation=0;deleteMutation<4;++deleteMutation)for(unsigned alias=0;alias<2;++alias)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),48)=live?ptr(0x1000):0;word(ptr(0x100),28)=frame?ptr(0x1400):0;word(ptr(0x100),64)=alias?ptr(0x128):owner?ptr(0x800):ptr(0x400);word(ptr(0x400),8)=word(ptr(0x800),8)=dataLive?ptr(0x600):0;if(alias)word(ptr(0x130),0)=dataLive?ptr(0x600):0;word(ptr(0x600),0)=word(ptr(0xa00),0)=(unsigned)table;word(ptr(0x600),4)=refs[ref];word(ptr(0xa00),4)=refs[(ref+1)%6];memset((void*)ptr(0x2000),0,8);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=uploaded=deleted=0;cachedSlot=0;if(!mode)model();else{cachedSlot=word(ptr(0x100),64)+8;/* callbacks may replace the owner before it is captured */if(uploadMutation==1&&word(ptr(0x100),48))cachedSlot=ptr(0x808);(mode==1?((void(__fastcall*)(void*,void*))code):OnLoaded)((void*)ptr(0x100),0);snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_ON_LOADED_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
