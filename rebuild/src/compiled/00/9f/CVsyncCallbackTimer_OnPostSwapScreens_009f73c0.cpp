#include "fable_display_viewport.h"
#include "fable_d3d9_query.h"
#include "fable_game.h"
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
void CVsyncCallbackTimer::OnPostSwapScreens(){
 unsigned long flags=D3DGETDATA_FLUSH;
 bool failed=false;
 while(PendingQueories.size()>0){
  _STL::_List_node<CPendingQuery>* head=PendingQueories._M_node;
  _STL::list<CPendingQuery>::iterator back(head);--back;
  IDirect3DQuery9* query=static_cast<_STL::_List_node<CPendingQuery>*>(back._M_node)->_M_data.Query;
  HRESULT result=query->GetData(0,0,flags);
  if(result==0){
   unsigned pendingCount=PendingQueories.size();unsigned freeCount=FreeQueoryObjects.size();
   _STL::_List_node<CPendingQuery>* frameHead=PendingQueories._M_node;
   _STL::list<CPendingQuery>::iterator frameBack(frameHead);--frameBack;
   FinishedFrame=static_cast<_STL::_List_node<CPendingQuery>*>(frameBack._M_node)->_M_data.Frame;
   _STL::_List_node<CPendingQuery>* valueHead=PendingQueories._M_node;
   _STL::list<CPendingQuery>::iterator valueBack(valueHead);--valueBack;
   FreeQueoryObjects.push_back(static_cast<_STL::_List_node<CPendingQuery>*>(valueBack._M_node)->_M_data);
   PendingQueories.pop_back();
  } else if(result==1){Sleep(0);}
  else {ClearAllQueries();failed=true;break;}
 }
 if(!failed){
  if(FreeQueoryObjects.size()==0){
   CPendingQuery created;
   HRESULT result=reinterpret_cast<FableD3DQueryDevice*>(Device)->VTable->CreateQuery(Device,D3DQUERYTYPE_EVENT,&created.Query);
   if(result==0)FreeQueoryObjects.push_back(created);
  }
  if(FreeQueoryObjects.size()>0){
   _STL::_List_node<CPendingQuery>* head=FreeQueoryObjects._M_node;
   _STL::list<CPendingQuery>::iterator back(head);--back;
   CPendingQuery query=static_cast<_STL::_List_node<CPendingQuery>*>(back._M_node)->_M_data;
   FreeQueoryObjects.pop_back();
   HRESULT result=query.Query->Issue(D3DISSUE_END);
   PendingQueories.push_front(query);
  }
 }
 ++PendingFrame;
 GPUBlocks[LatestBlockIndex].GPUPrevTimeStamp=GPUBlocks[LatestBlockIndex].GPUTimeStamp;
 GPUBlocks[LatestBlockIndex].GPUTimeStamp=GFGetTime();
 ++GPUBlocks[LatestBlockIndex].GPUFrameCount;
 CPUPrevTimeStamp=CPUTimeStamp;CPUTimeStamp=GFGetTime();++CPUFrameCount;
}
