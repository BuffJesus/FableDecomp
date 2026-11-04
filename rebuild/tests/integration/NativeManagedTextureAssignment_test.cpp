#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_managed_texture.h"
#include "managed_texture_assignment_retail.h"
#include "managed_texture_assignment_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" const CManagedTexture* __fastcall ManagedAssign(void*,void*,const CManagedTexture*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,step,count,snapshots,location,releaseMutation,deleteMutation,sourceObject,calls;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void snapshot(){if(!mode){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(step<count&&!memcmp(images[step++],arena,sizeof(arena)));++snapshots;}}
bool CManagedTexture::Release(){require((unsigned)this==ptr(0x100));++calls;word(ptr(0x1000),0)=calls;word(ptr(0x1000),4)=0;word(ptr(0x1000),8)=(unsigned)this;
 if(releaseMutation==1)word(ptr(0x100),4)=0;
 if(releaseMutation==2)word(ptr(0x100),4)=word(sourceObject,4);
 if(releaseMutation==3)word(ptr(0x100),4)=ptr(0x800);
 if(releaseMutation==4)word(sourceObject,4)=0;
 if(releaseMutation==5)word(sourceObject,4)=ptr(0x800);
 if(releaseMutation==6){word(ptr(0x100),4)=0;word(sourceObject,4)=ptr(0xc00);}
 if(releaseMutation==7)word(sourceObject,4)=word(ptr(0x100),4);
 snapshot();return false;}
class Boundary{public:void Delete()const;};
void Boundary::Delete()const{require((unsigned)this==ptr(0x400)||(unsigned)this==ptr(0x800)||(unsigned)this==ptr(0xc00));++calls;word(ptr(0x1000),0)=calls;word(ptr(0x1000),4)=1;word(ptr(0x1000),8)=(unsigned)this;if(deleteMutation==1)word(sourceObject,4)=ptr(0xc00);if(deleteMutation==2)word(ptr(0x100),4)=ptr(0xc00);if(deleteMutation==3){word(sourceObject,4)=0;word(ptr(0x100),4)=ptr(0xc00);}snapshot();word((unsigned)this,20)=0xdededede;}
static unsigned model(){unsigned target=ptr(0x100);if(word(target,4)!=word(sourceObject,4)){((CManagedTexture*)target)->Release();unsigned old=word(target,4);if(old!=word(sourceObject,4)){if(old){if(--word(old,4)==0)((Boundary*)old)->Delete();word(target,4)=0;}unsigned next=word(sourceObject,4);word(target,4)=next;if(next)++word(next,4);}}return target;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(CManagedTexture::*release)()=&CManagedTexture::Release;unsigned releaseAddress=0;memcpy(&releaseAddress,&release,4);*(unsigned*)(code+19)=releaseAddress-(unsigned)(code+23);void(Boundary::*d)()const=&Boundary::Delete;unsigned table[2]={0,0};memcpy(table+1,&d,4);const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};const unsigned nodes[4]={0,0x400,0x800,0xc00};unsigned cases=0;
for(unsigned old=0;old<4;++old)for(unsigned next=0;next<4;++next)for(unsigned ref=0;ref<6;++ref)for(unsigned altRef=0;altRef<3;++altRef)for(releaseMutation=0;releaseMutation<8;++releaseMutation)for(deleteMutation=0;deleteMutation<4;++deleteMutation)for(unsigned alias=0;alias<2;++alias)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;sourceObject=ptr(alias?0x100:0x200);memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),4)=old?ptr(nodes[old]):0;word(ptr(0x200),4)=next?ptr(nodes[next]):0;for(unsigned node=1;node<4;++node){word(ptr(nodes[node]),0)=(unsigned)table;word(ptr(nodes[node]),4)=node==1?refs[ref]:node==2?refs[altRef]:refs[(ref+2)%6];}memset((void*)ptr(0x1000),0,16);memcpy(initial,arena,sizeof(arena));count=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));step=calls=0;unsigned result=!mode?model():(unsigned)(mode==1?((const CManagedTexture*(__fastcall*)(void*,void*,const CManagedTexture*))code):ManagedAssign)((void*)ptr(0x100),0,(const CManagedTexture*)sourceObject);require(result==ptr(0x100));word(ptr(0x1000),12)=result;snapshot();if(mode)require(step==count);}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_ASSIGNMENT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
