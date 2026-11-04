#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "graphic_frame_loading_init_retail.h"
#include "graphic_frame_loading_init_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall FrameLoading(void*,void*,unsigned long,unsigned long);
extern "C" unsigned FrameVTable[],ManagedVTable[],ResourceVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned arena[16384],initial[16384],images[4][16384],mode,count,step,snapshots,location,selected,allocationMutation,deleteMutation,loadMutation,failAllocation;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<4);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
void* __cdecl operator new(size_t bytes){require(bytes==68);++word(ptr(0x3000),0);if(allocationMutation==1)word(selected,8)=ptr(0x2400);if(allocationMutation==2)word(selected,8)=ptr(0x2500);if(allocationMutation==3)word(selected,12)=ptr(0x1e00);snapshot();return failAllocation?0:(void*)ptr(0x2600);}
class Boundaries {public:void Delete()const;void Load(unsigned long,unsigned long);};
void Boundaries::Delete()const{unsigned p=(unsigned)this;require(p==ptr(0x2400)||p==ptr(0x2500));++word(ptr(0x3000),8);word(p,12)=0xdededede;if(deleteMutation)word(selected,12)=ptr(0x1e00);snapshot();}
void Boundaries::Load(unsigned long a,unsigned long b){require(a==0xfedcba98&&b==0x87654321&&(unsigned)this==word(selected,8));++word(ptr(0x3000),12);word(ptr(0x3000),16)=(unsigned)this;if(loadMutation)word(selected,8)=ptr(0x2500);snapshot();}
static void modelCtor(){unsigned self=ptr(0x2600),bank=word(word(selected,12),32),list=bank+0x168,head=list+4;word(self,0)=(unsigned)FrameVTable;word(self,4)=1;for(unsigned o=8;o<28;o+=4)word(self,o)=0;word(self,28)=0;word(self,32)=word(self,40)=(unsigned)ManagedVTable;word(self,36)=word(self,44)=word(self,48)=word(self,52)=0;word(self,64)=selected;*(unsigned char*)(self+56)=255;*(unsigned char*)(self+58)=127;word(self,60)=word(list,44)-1;word(self,20)=72;word(self,8)=list;word(self,24)=word(list,44);word(self,12)=word(head,12);word(self,16)=head;word(word(self,12),16)=self;word(word(self,16),12)=self;++word(list,32);word(list,36)+=word(self,20);}
static void model(){unsigned frame=ptr(0x100);while(*(unsigned short*)(frame+4)>0&&(*(unsigned char*)(word(frame,12)+18)&1))frame=word(word(frame,12),40);require(frame==selected);if(!word(frame,8)){unsigned fresh=(unsigned)operator new(68);if(fresh)modelCtor();unsigned old=word(frame,8);if(old){if(--word(old,4)==0)((Boundaries*)old)->Delete();word(frame,8)=0;}word(frame,8)=fresh;}((Boundaries*)word(frame,8))->Load(0xfedcba98,0x87654321);snapshot();}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));void(Boundaries::*d)()const=&Boundaries::Delete;void(Boundaries::*l)(unsigned long,unsigned long)=&Boundaries::Load;unsigned table[2]={0,0},loadAddress=0;memcpy(&table[1],&d,4);memcpy(&loadAddress,&l,4);unsigned targets[8]={(unsigned)FrameVTable,(unsigned)ManagedVTable,(unsigned)ResourceVTable,(unsigned)BaseVTable,(unsigned)&operator new,loadAddress,(unsigned)&AbortBoundary,(unsigned)&AbortBoundary};
for(unsigned f=0;f<sizeof(fixups)/sizeof(fixups[0]);++f){unsigned target=fixups[f].target<0x80000000?(unsigned)(code+fixups[f].target):targets[fixups[f].target-0x80000000];*(unsigned*)(code+fixups[f].site)=fixups[f].kind==20?target-(unsigned)(code+fixups[f].site+4):target;}
const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff},flags[6]={0,1,2,3,0x80,0xff};unsigned cases=0;
for(unsigned depth=0;depth<4;++depth)for(unsigned flag=0;flag<6;++flag)for(unsigned live=0;live<2;++live)for(unsigned ref=0;ref<6;++ref)for(allocationMutation=0;allocationMutation<4;++allocationMutation)for(deleteMutation=0;deleteMutation<2;++deleteMutation)for(loadMutation=0;loadMutation<2;++loadMutation)for(failAllocation=0;failAllocation<2;++failAllocation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x4000:0;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<4;++i){unsigned fr=ptr(0x100+i*0x100),info=ptr(0x1000+i*0x100);*(unsigned short*)(fr+4)=i<depth?7:0;word(fr,12)=info;word(fr,8)=0;*(unsigned char*)(info+18)=(unsigned char)flags[flag];word(info,40)=ptr(0x100+(i+1)*0x100);word(info,32)=ptr(0x800);}word(ptr(0x1e00),32)=ptr(0x1800);selected=ptr(0x100+((flags[flag]&1)?depth:0)*0x100);word(selected,8)=live?ptr(0x2400):0;
for(unsigned k=0;k<2;++k){word(ptr(0x2400+k*0x100),0)=(unsigned)table;word(ptr(0x2400+k*0x100),4)=refs[ref];unsigned bank=ptr(k?0x1800:0x800),list=bank+0x168,head=list+4;word(head,12)=ptr(0x3200+k*0x100);word(head,16)=head;word(list,44)=refs[(ref+k)%6];word(list,32)=refs[(ref+2)%6];word(list,36)=refs[(ref+3)%6];}memset((void*)ptr(0x3000),0,20);memcpy(initial,arena,sizeof(arena));count=0;
for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=0;if(!mode)model();else{(mode==1?((void(__fastcall*)(void*,void*,unsigned long,unsigned long))code):FrameLoading)((void*)ptr(0x100),0,0xfedcba98,0x87654321);snapshot();require(step==count);}}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_GRAPHIC_FRAME_LOADING_INIT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
