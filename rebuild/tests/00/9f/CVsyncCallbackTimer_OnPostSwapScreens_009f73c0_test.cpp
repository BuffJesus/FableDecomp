#include "fable_display_viewport.h"
#include "fable_d3d9_query.h"
#include "fable_game.h"
#include "test_support/import_redirect.h"
#include <stdio.h>
#include <string.h>
#pragma comment(lib,"kernel32.lib")
volatile CVsyncCallbackTimer::CGPUInfoBlock CVsyncCallbackTimer::GPUBlocks[2];
double CVsyncCallbackTimer::CPUTimeStamp;
double CVsyncCallbackTimer::CPUPrevTimeStamp;
long CVsyncCallbackTimer::CPUFrameCount;
volatile long CVsyncCallbackTimer::LatestBlockIndex;
typedef _STL::_List_node<CVsyncCallbackTimer::CPendingQuery> Node;
static Node pendingHead,freeHead;
static unsigned long storage[5],queries[32][2],queryTable[8],deviceTable[119],device;
static unsigned errors,pendingCount,freeCount,mode,createResult,issueResult,startIndex,mutation;
static unsigned polls[32],sleeps,clears,creates,issues,times,lastIssued;
static CVsyncCallbackTimer* Object(){return reinterpret_cast<CVsyncCallbackTimer*>(storage);}
struct CVsyncCallbackTimerTestAccess {
 static void Fill(){
  Object()->Device=reinterpret_cast<IDirect3DDevice9*>(&device);Object()->PendingQueories._M_node=&pendingHead;Object()->FreeQueoryObjects._M_node=&freeHead;Object()->PendingFrame=10;Object()->FinishedFrame=77;
  for(unsigned i=0;i<2;++i){CVsyncCallbackTimer::GPUBlocks[i].GPUTimeStamp=100.125+i*100;CVsyncCallbackTimer::GPUBlocks[i].GPUPrevTimeStamp=-3.5-i;CVsyncCallbackTimer::GPUBlocks[i].GPUFrameCount=100+i*100;CVsyncCallbackTimer::GPUBlocks[i].Locked=0x44+i;}
  CVsyncCallbackTimer::CPUTimeStamp=30.75;CVsyncCallbackTimer::CPUPrevTimeStamp=-7.0;CVsyncCallbackTimer::CPUFrameCount=23;CVsyncCallbackTimer::LatestBlockIndex=startIndex;
 }
 static void Clock(){if(times==0 && (mutation&1))CVsyncCallbackTimer::LatestBlockIndex^=1;if(times==1 && (mutation&2)){CVsyncCallbackTimer::CPUFrameCount=70;CVsyncCallbackTimer::CPUTimeStamp=99.0;}}
 static bool Check(bool failed){
  if(Object()->Device!=reinterpret_cast<IDirect3DDevice9*>(&device) || Object()->PendingFrame!=11 || Object()->FinishedFrame!=static_cast<long>(pendingCount && !failed?101:77))return false;
  unsigned target=startIndex^((mutation&1)!=0);
  for(unsigned i=0;i<2;++i)if(CVsyncCallbackTimer::GPUBlocks[i].GPUTimeStamp!=(i==target?10.25:100.125+i*100) || CVsyncCallbackTimer::GPUBlocks[i].GPUPrevTimeStamp!=(i==startIndex?100.125+i*100:-3.5-i) || CVsyncCallbackTimer::GPUBlocks[i].GPUFrameCount!=static_cast<long>(100+i*100+(i==target)) || CVsyncCallbackTimer::GPUBlocks[i].Locked!=static_cast<long>(0x44+i))return false;
  return CVsyncCallbackTimer::LatestBlockIndex==static_cast<long>(target) && CVsyncCallbackTimer::CPUTimeStamp==20.5 && CVsyncCallbackTimer::CPUPrevTimeStamp==30.75 && CVsyncCallbackTimer::CPUFrameCount==((mutation&2)?71:24);
 }
};
static void Empty(Node& head){while(head._M_next!=&head){Node* node=static_cast<Node*>(head._M_next);head._M_next=node->_M_next;free(node);}head._M_prev=&head;}
static void Add(Node& head,unsigned id){Node* node=static_cast<Node*>(malloc(sizeof(Node)));node->_M_next=&head;node->_M_prev=head._M_prev;head._M_prev->_M_next=node;head._M_prev=node;node->_M_data.Query=reinterpret_cast<IDirect3DQuery9*>(queries[id]);node->_M_data.Frame=100+id;node->_M_data.TimeStamp=id+0.25;}
#ifdef TIMER_POST_USE_NATIVE_NODES
void* __cdecl operator new(size_t,void* p) throw(){return p;}
#else
namespace _STL {
 template<> list<CVsyncCallbackTimer::CPendingQuery>::iterator list<CVsyncCallbackTimer::CPendingQuery>::insert(iterator position,const CVsyncCallbackTimer::CPendingQuery& value){
  _Node* node=static_cast<_Node*>(malloc(sizeof(_Node)));node->_M_data=value;node->_M_next=position._M_node;node->_M_prev=position._M_node->_M_prev;node->_M_prev->_M_next=node;node->_M_next->_M_prev=node;return iterator(node);
 }
}
#endif
void CVsyncCallbackTimer::ClearAllQueries(){if(this!=Object())++errors;++clears;Empty(pendingHead);Empty(freeHead);}
double FABLE_FASTCALL GFGetTime(){CVsyncCallbackTimerTestAccess::Clock();return times++==0?10.25:20.5;}
static void FABLE_STDCALL SleepMock(unsigned long milliseconds){if(milliseconds)++errors;++sleeps;}
static HRESULT FABLE_STDCALL GetData(IDirect3DQuery9* query,void* data,DWORD bytes,DWORD flags){
 unsigned id=reinterpret_cast<unsigned long*>(query)[1];if(id<1 || id>pendingCount || data || bytes || flags!=1 || static_cast<Node*>(pendingHead._M_prev)->_M_data.Query!=query)++errors;
 unsigned attempt=polls[id]++;if(mode==3)return static_cast<HRESULT>(0x80004005);return attempt<mode?1:0;
}
static HRESULT FABLE_STDCALL Issue(IDirect3DQuery9* query,DWORD flags){if(flags!=1 || pendingHead._M_next!=&pendingHead)++errors;++issues;lastIssued=reinterpret_cast<unsigned long*>(query)[1];return issueResult?static_cast<HRESULT>(0x80004005):0;}
static HRESULT FABLE_STDCALL Create(IDirect3DDevice9* input,D3DQUERYTYPE type,IDirect3DQuery9** result){if(input!=reinterpret_cast<IDirect3DDevice9*>(&device) || type!=D3DQUERYTYPE_EVENT)++errors;++creates;*result=reinterpret_cast<IDirect3DQuery9*>(queries[30]);return createResult==0?0:createResult==1?1:static_cast<HRESULT>(0x80004005);}
static unsigned Count(Node& head){unsigned n=0;_STL::_List_node_base* previous=&head;for(_STL::_List_node_base* node=head._M_next;node!=&head;node=node->_M_next){if(node->_M_prev!=previous || ++n>32){++errors;break;}previous=node;}if(head._M_prev!=previous)++errors;return n;}
int main(){FableTest::ImportRedirect sleep("Sleep",reinterpret_cast<void*>(&SleepMock));if(!sleep.Installed())return 2;
 queryTable[6]=reinterpret_cast<unsigned long>(&Issue);queryTable[7]=reinterpret_cast<unsigned long>(&GetData);deviceTable[118]=reinterpret_cast<unsigned long>(&Create);device=reinterpret_cast<unsigned long>(deviceTable);
 for(unsigned q=0;q<32;++q){queries[q][0]=reinterpret_cast<unsigned long>(queryTable);queries[q][1]=q;}
 unsigned scenarios=0;
 for(pendingCount=0;pendingCount<4;++pendingCount)for(freeCount=0;freeCount<3;++freeCount)for(mode=0;mode<4;++mode)for(createResult=0;createResult<3;++createResult)for(issueResult=0;issueResult<2;++issueResult)for(startIndex=0;startIndex<2;++startIndex)for(mutation=0;mutation<4;++mutation){
  pendingHead._M_next=pendingHead._M_prev=&pendingHead;freeHead._M_next=freeHead._M_prev=&freeHead;for(unsigned i=1;i<=pendingCount;++i)Add(pendingHead,i);for(unsigned j=1;j<=freeCount;++j)Add(freeHead,j+10);
  memset(polls,0,sizeof(polls));errors=sleeps=clears=creates=issues=times=lastIssued=0;CVsyncCallbackTimerTestAccess::Fill();Object()->OnPostSwapScreens();++scenarios;
  bool failed=pendingCount && mode==3;bool made=!failed && !pendingCount && !freeCount;unsigned issued=failed?0:pendingCount?1:freeCount?freeCount+10:createResult==0?30:0;
  if(clears!=(failed?1U:0U) || creates!=(made?1U:0U) || issues!=(issued?1U:0U) || lastIssued!=issued || sleeps!=(failed?0:pendingCount*mode) || times!=2 || !CVsyncCallbackTimerTestAccess::Check(failed))++errors;
  if(Count(pendingHead)!=(issued?1U:0U) || Count(freeHead)!=(failed?0:pendingCount+freeCount+(made && createResult==0)-(issued!=0)))++errors;
  if(issued){Node* node=static_cast<Node*>(pendingHead._M_next);if(node->_M_data.Query!=reinterpret_cast<IDirect3DQuery9*>(queries[issued]))++errors;if(issued!=30 && (node->_M_data.Frame!=static_cast<long>(100+issued) || node->_M_data.TimeStamp!=issued+0.25))++errors;}
  for(unsigned k=1;k<=pendingCount;++k)if(polls[k]!=(failed?(k==pendingCount?1U:0U):mode+1))++errors;
  Empty(pendingHead);Empty(freeHead);if(errors){printf("TIMER_POST_SWAP_FAIL scenario=%u errors=%u\n",scenarios,errors);return 1;}
 }
 printf("TIMER_POST_SWAP_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
