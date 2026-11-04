#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
static void require(bool ok,int line){if(!ok){fprintf(stderr,"POINTER_FILL_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
struct Block{unsigned char* base;size_t size;bool live;};static Block blocks[8];static unsigned allocations,releases;
extern "C" void* __cdecl TextFixtureMalloc(size_t size){
 unsigned slot=0;while(slot<8 && blocks[slot].live)++slot;CHECK(slot<8);unsigned char* p=(unsigned char*)malloc(size+32);CHECK(p);memset(p,0x6c,size+32);blocks[slot].base=p;blocks[slot].size=size;blocks[slot].live=true;++allocations;return p+16;
}
extern "C" void __cdecl TextFixtureFree(void* value){
 if(!value)return;unsigned slot=0;while(slot<8 && (!blocks[slot].live || blocks[slot].base+16!=value))++slot;CHECK(slot<8);Block& block=blocks[slot];for(unsigned i=0;i<16;++i)CHECK(block.base[i]==0x6c && block.base[16+block.size+i]==0x6c);block.live=false;++releases;free(block.base);
}
int main(){
 unsigned cases=0,grows=0,aliasCases=0,zeroCases=0;
 for(unsigned initial=0;initial<16;++initial)for(unsigned spare=0;spare<9;++spare)for(unsigned position=0;position<=initial;++position)for(unsigned count=0;count<9;++count)for(unsigned mode=0;mode<3;++mode){
  typedef NGameText::CDataBankEntry Entry;_STL::vector<Entry*> values;unsigned capacity=initial+spare;Entry* expected[64];Entry* external=(Entry*)(0x100000+cases*4);Entry* chosen=mode==0?0:external;
  if(capacity){values._M_start=(Entry**)TextFixtureMalloc(capacity*4);values._M_finish=values._M_start+initial;values._M_end_of_storage._M_data=values._M_start+capacity;}
  for(unsigned i=0;i<initial;++i)values._M_start[i]=(Entry*)(0x200000+i*0x100);
  bool alias=mode==2 && initial;unsigned selected=initial/2;if(alias){chosen=values._M_start[selected];++aliasCases;}
  for(unsigned i=0;i<position;++i)expected[i]=values._M_start[i];for(unsigned i=0;i<count;++i)expected[position+i]=chosen;for(unsigned i=position;i<initial;++i)expected[i+count]=values._M_start[i];
  Entry** at=values._M_start?values._M_start+position:0;
  values._M_fill_insert(at,count,alias?values._M_start[selected]:chosen);
  CHECK(values.size()==initial+count);for(unsigned i=0;i<initial+count;++i)CHECK(values._M_start[i]==expected[i]);
  unsigned expectedCapacity=capacity;if(count && spare<count){expectedCapacity=initial+(initial>count?initial:count);++grows;}CHECK(values.capacity()==expectedCapacity);if(!count)++zeroCases;
  TextFixtureFree(values._M_start);values._M_start=values._M_finish=values._M_end_of_storage._M_data=0;for(unsigned i=0;i<8;++i)CHECK(!blocks[i].live);++cases;
 }
 CHECK(cases==33048 && allocations==releases);printf("NATIVE_POINTER_FILL_PASS cases=%u grows=%u aliases=%u zero=%u allocations=%u\n",cases,grows,aliasCases,zeroCases,allocations);return 0;
}
