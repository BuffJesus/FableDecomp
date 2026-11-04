#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
unsigned long g_NativeTextReadContext_0138DE48=0x12345678,g_NativeTextReadContext_0138E064=0x87654321;
extern "C" void ColdBoundary(){fprintf(stderr,"TAG_VECTOR_COLD\n");exit(80);}
static void require(bool ok,int line){if(!ok){fprintf(stderr,"TAG_VECTOR_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
struct Block{unsigned char* base;size_t size;bool live;};static Block blocks[8];static unsigned allocations,releases;
extern "C" void* __cdecl TagFixtureMalloc(size_t size){unsigned slot=0;while(slot<8 && blocks[slot].live)++slot;CHECK(slot<8);unsigned char* p=(unsigned char*)malloc(size+32);CHECK(p);memset(p,0xd6,size+32);blocks[slot].base=p;blocks[slot].size=size;blocks[slot].live=true;++allocations;return p+16;}
extern "C" void __cdecl TagFixtureFree(void* value){if(!value)return;unsigned slot=0;while(slot<8 && (!blocks[slot].live || blocks[slot].base+16!=value))++slot;CHECK(slot<8);Block& block=blocks[slot];for(unsigned i=0;i<16;++i)CHECK(block.base[i]==0xd6 && block.base[16+block.size+i]==0xd6);block.live=false;++releases;free(block.base);}
int main(){
 unsigned cases=0,aliases=0,grows=0;long chars=g_CCharStringInstanceCount_013BD800,wides=g_CWideStringInstanceCount_013BCA20;const char* names[6]={"A","BETA","GAMMA_LONG","D","ECHO","FOXTROT"};
 for(unsigned initial=0;initial<6;++initial)for(unsigned spare=0;spare<6;++spare)for(unsigned position=0;position<=initial;++position)for(unsigned count=0;count<6;++count)for(unsigned mode=0;mode<3;++mode){
  {typedef NGameText::CTextTag Tag;_STL::vector<Tag> values;unsigned capacity=initial+spare;if(capacity){values._M_start=(Tag*)TagFixtureMalloc(capacity*sizeof(Tag));values._M_finish=values._M_start+initial;values._M_end_of_storage._M_data=values._M_start+capacity;}
   for(unsigned i=0;i<initial;++i){new(values._M_start+i)Tag;values._M_start[i].Pos=(long)i*17-500;CCharString name(names[i]);values._M_start[i].Name=name;}
   Tag external;external.Pos=0x12345678;CCharString externalName(mode==0?"":"INSERTED");external.Name=externalName;bool alias=mode==2 && initial;unsigned selected=initial/2;const Tag& chosen=alias?values._M_start[selected]:external;if(alias)++aliases;
   long expectedPos[24];const char* expectedName[24];for(unsigned i=0;i<position;++i){expectedPos[i]=(long)i*17-500;expectedName[i]=names[i];}for(unsigned i=0;i<count;++i){expectedPos[position+i]=chosen.Pos;expectedName[position+i]=alias?names[selected]:(mode==0?"":"INSERTED");}for(unsigned i=position;i<initial;++i){expectedPos[i+count]=(long)i*17-500;expectedName[i+count]=names[i];}
   values._M_fill_insert(values._M_start?values._M_start+position:0,count,chosen);CHECK(values.size()==initial+count);for(unsigned i=0;i<initial+count;++i)CHECK(values[i].Pos==expectedPos[i] && !strcmp((const char*)values[i].Name,expectedName[i]));unsigned expectedCapacity=capacity;if(count && spare<count){expectedCapacity=initial+(initial>count?initial:count);++grows;}CHECK(values.capacity()==expectedCapacity);
   for(unsigned i=0;i<values.size();++i)values._M_start[i].~Tag();TagFixtureFree(values._M_start);values._M_start=values._M_finish=values._M_end_of_storage._M_data=0;
  }
  for(unsigned i=0;i<8;++i)CHECK(!blocks[i].live);CHECK(g_CCharStringInstanceCount_013BD800==chars && g_CWideStringInstanceCount_013BCA20==wides);++cases;
 }
 CHECK(cases==2268 && allocations==releases);printf("NATIVE_TAG_VECTOR_PASS cases=%u aliases=%u grows=%u allocations=%u\n",cases,aliases,grows,allocations);return 0;
}
#include "NativeFileInputStreamColdAliases.h"
#include "NativeBankLifetimeColdAliases.h"
#include "NativeTextMetadataColdAliases.h"
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetLastModifiedTime@CDataBank@NGameText@@MAE?AVCDateAndTime@@ABVCCharString@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?UnloadNextResource@CResourceList@@QAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?HandleFailedAllocation@CResourceList@@UAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_ColdBoundary")
