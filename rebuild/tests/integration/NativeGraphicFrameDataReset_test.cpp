#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_frame_data_reset_retail.h"
#include "graphic_frame_data_reset_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall FrameReset(void*,void*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,count,step,snapshots,location,releaseMutation,frameMutation,releaseResult;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
class Boundaries {public:bool Release();void UnInitialise();};
bool Boundaries::Release(){require((unsigned)this==ptr(0x120));word(ptr(0x300),0)=1;if(releaseMutation&1)word(ptr(0x100),28)=0;if(releaseMutation&2)word(ptr(0x100),36)=ptr(0x800);if(releaseMutation&4)word(ptr(0x100),56)=0xdeadbeef;snapshot();return releaseResult!=0;}
void Boundaries::UnInitialise(){require((unsigned)this==ptr(0x11c));require(word(ptr(0x300),0)==1);word(ptr(0x300),4)=1;if(frameMutation&1)word(ptr(0x100),28)=ptr(0x900);if(frameMutation&2)word(ptr(0x100),44)=0;if(frameMutation&4)word(ptr(0x100),56)=0x12345678;snapshot();}
static void model(){((Boundaries*)ptr(0x120))->Release();((Boundaries*)ptr(0x11c))->UnInitialise();*(unsigned char*)ptr(0x138)=255;*(unsigned char*)ptr(0x13a)=127;snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(Boundaries::*r)()=&Boundaries::Release;void(Boundaries::*u)()=&Boundaries::UnInitialise;unsigned targets[2];memcpy(&targets[0],&r,4);memcpy(&targets[1],&u,4);for(unsigned f=0;f<2;++f)*(unsigned*)(code+fixups[f].site)=targets[fixups[f].target]-(unsigned)(code+fixups[f].site+4);unsigned cases=0;
for(releaseMutation=0;releaseMutation<8;++releaseMutation)for(frameMutation=0;frameMutation<8;++frameMutation)for(releaseResult=0;releaseResult<2;++releaseResult)for(unsigned byte=0;byte<256;++byte)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),56)=byte*0x01010101;word(ptr(0x300),0)=word(ptr(0x300),4)=0;memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;if(!mode)model();else{(mode==1?((void(__fastcall*)(void*,void*))code):FrameReset)((void*)ptr(0x100),0);snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_GRAPHIC_FRAME_DATA_RESET_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
