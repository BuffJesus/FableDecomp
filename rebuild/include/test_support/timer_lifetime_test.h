#include "fable_display_viewport.h"
#include <new>
#include <stdio.h>
#include <string.h>
volatile CVsyncCallbackTimer::CGPUInfoBlock CVsyncCallbackTimer::GPUBlocks[2];
double CVsyncCallbackTimer::CPUTimeStamp;
double CVsyncCallbackTimer::CPUPrevTimeStamp;
long CVsyncCallbackTimer::CPUFrameCount;
volatile long CVsyncCallbackTimer::LatestBlockIndex;
typedef _STL::_List_node<CVsyncCallbackTimer::CPendingQuery> Node;
static unsigned calls,errors,mutation;
static unsigned long storage[5],before[5];
static CVsyncCallbackTimer* Object(){return reinterpret_cast<CVsyncCallbackTimer*>(storage);}
struct CVsyncCallbackTimerTestAccess {
 static void Fill(unsigned char pattern){
  memset((void*)CVsyncCallbackTimer::GPUBlocks,pattern,sizeof(CVsyncCallbackTimer::GPUBlocks));
  CVsyncCallbackTimer::CPUTimeStamp=12.5;CVsyncCallbackTimer::CPUPrevTimeStamp=-18.25;
  CVsyncCallbackTimer::CPUFrameCount=19;CVsyncCallbackTimer::LatestBlockIndex=7;
 }
 static bool Zero(){
  for(unsigned i=0;i<2;++i)if(CVsyncCallbackTimer::GPUBlocks[i].GPUTimeStamp!=0 || CVsyncCallbackTimer::GPUBlocks[i].GPUPrevTimeStamp!=0 || CVsyncCallbackTimer::GPUBlocks[i].GPUFrameCount!=0 || CVsyncCallbackTimer::GPUBlocks[i].Locked!=0)return false;
  return CVsyncCallbackTimer::CPUTimeStamp==0 && CVsyncCallbackTimer::CPUPrevTimeStamp==0 && CVsyncCallbackTimer::CPUFrameCount==0 && CVsyncCallbackTimer::LatestBlockIndex==0;
 }
};
void CVsyncCallbackTimer::ClearAllQueries(){
 if(this!=Object() || calls++!=0 || memcmp(storage,before,sizeof(storage)))++errors;
}
namespace _STL {
 template<> void list<CVsyncCallbackTimer::CPendingQuery>::clear(){
  unsigned slot=calls==1?2:1;
  if(this!=reinterpret_cast<list<CVsyncCallbackTimer::CPendingQuery>*>(&storage[slot]) || calls<1 || calls>2)++errors;
  ++calls;
  if(mutation&(slot==2?1:2)){free(_M_node);_M_node=0;}
 }
}
int main(){unsigned scenarios=0;
 for(unsigned pattern=0;pattern<256;++pattern)for(mutation=0;mutation<4;++mutation){
  memset(storage,pattern,sizeof(storage));unsigned long first=storage[0];CVsyncCallbackTimerTestAccess::Fill(static_cast<unsigned char>(pattern));
  CVsyncCallbackTimer* value=new(storage) CVsyncCallbackTimer;
  if(value!=Object() || storage[0]!=first || storage[3]!=1 || storage[4]!=0 || !CVsyncCallbackTimerTestAccess::Zero())return 1;
  Node* freeHead=reinterpret_cast<Node*>(storage[1]);Node* pendingHead=reinterpret_cast<Node*>(storage[2]);
  if(!freeHead || !pendingHead || freeHead==pendingHead || freeHead->_M_next!=freeHead || freeHead->_M_prev!=freeHead || pendingHead->_M_next!=pendingHead || pendingHead->_M_prev!=pendingHead)return 2;
  memcpy(before,storage,sizeof(storage));calls=errors=0;value->~CVsyncCallbackTimer();++scenarios;
  if(errors || calls!=3 || storage[0]!=first || storage[3]!=1 || storage[4]!=0 || !CVsyncCallbackTimerTestAccess::Zero())return 3;
 }
 printf("TIMER_LIFETIME_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
