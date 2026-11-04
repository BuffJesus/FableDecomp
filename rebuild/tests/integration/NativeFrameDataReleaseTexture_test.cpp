#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_graphic_info.h"
#include "frame_data_release_texture_retail.h"
#include "frame_data_release_texture_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall ReleaseFrameTexture(void*,void*);
static unsigned arena[4096],initial[4096],images[2][4096],mode,step,count,snapshots,location,frameNum,calls;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<2);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
void CGraphicInfo::UnloadFrame(unsigned long frame){require((unsigned)this==ptr(0x800)&&frame==frameNum&&!calls);calls=1;word(ptr(0x2000),0)=1;word(ptr(0x2000),4)=frame;word(ptr(0x2000),8)=(unsigned)this;unsigned mutation=frameNum&3;if(mutation==1)word(ptr(0x100),48)=ptr(0x1100);if(mutation==2)word(ptr(0x100),64)=ptr(0x1200);if(mutation==3){word(ptr(0x400),12)=ptr(0xc00);*(unsigned short*)ptr(0x404)=0xffff;}snapshot();}
static void model(){unsigned self=ptr(0x100);if(!word(self,48)){unsigned owner=word(self,64),info=word(owner,12),frame=*(unsigned short*)(owner+4);((CGraphicInfo*)info)->UnloadFrame(frame);}snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));void(CGraphicInfo::*unload)(unsigned long)=&CGraphicInfo::UnloadFrame;unsigned unloadAddress=0;memcpy(&unloadAddress,&unload,4);*(unsigned*)(code+fixupSite)=unloadAddress-(unsigned)(code+fixupSite+4);unsigned cases=0;
for(frameNum=0;frameNum<65536;++frameNum)for(unsigned live=0;live<2;++live)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),48)=live?ptr(0x1000):0;word(ptr(0x100),64)=ptr(0x400);word(ptr(0x400),12)=ptr(0x800);*(unsigned short*)ptr(0x404)=(unsigned short)frameNum;memset((void*)ptr(0x2000),0,12);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;if(!mode)model();else{(mode==1?((void(__fastcall*)(void*,void*))code):ReleaseFrameTexture)((void*)ptr(0x100),0);snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_FRAME_DATA_RELEASE_TEXTURE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
