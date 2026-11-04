#include "fable_display_viewport.h"
namespace _STL {
 template<> __forceinline list<CVsyncCallbackTimer::CPendingQuery>::list() : _M_node(0) {
  _Node* node=allocator<_Node>().allocate(1);
  node->_M_next=node;node->_M_prev=node;_M_node=node;
 }
}
CVsyncCallbackTimer::CVsyncCallbackTimer(){
 CPUFrameCount=0;LatestBlockIndex=0;
 GPUBlocks[0].GPUTimeStamp=0.0;GPUBlocks[0].GPUPrevTimeStamp=0.0;
 GPUBlocks[1].GPUTimeStamp=0.0;GPUBlocks[1].GPUPrevTimeStamp=0.0;
 CPUTimeStamp=0.0;CPUPrevTimeStamp=0.0;
 GPUBlocks[0].GPUFrameCount=0;GPUBlocks[0].Locked=0;
 GPUBlocks[1].GPUFrameCount=0;GPUBlocks[1].Locked=0;
 PendingFrame=1;FinishedFrame=0;
}
