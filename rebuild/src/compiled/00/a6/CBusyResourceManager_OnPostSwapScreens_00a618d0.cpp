// Retail A618D0; FableWin CBusyResourceManager::OnPostSwapScreens.
#include "fable_busy_resource_manager.h"
#include <new>
namespace _STL {
template<> inline list<CBusyResourceManager::CPendingQuery>::iterator list<CBusyResourceManager::CPendingQuery>::insert(iterator position,const CBusyResourceManager::CPendingQuery& value) {
    _Node* node=static_cast<_Node*>(malloc(sizeof(_Node)));
    new (&node->_M_data) CBusyResourceManager::CPendingQuery(value);
    _List_node_base* following=position._M_node;
    _List_node_base* previous=following->_M_prev;
    node->_M_next=following;node->_M_prev=previous;
    previous->_M_next=node;following->_M_prev=node;
    return iterator(node);
}
}
void CBusyResourceManager::OnPostSwapScreens()
{
    while(PendingQueories.size()>0) {
        HRESULT result=PendingQueories.back().Query->GetData(0,0,D3DGETDATA_FLUSH);
        if(result!=0 && PendingQueories.size()<9) break;
        unsigned pendingCount=PendingQueories.size();
        unsigned freeCount=FreeQueoryObjects.size();
        FinishedFrame=PendingQueories.back().Frame;
        FreeQueoryObjects.push_back(PendingQueories.back());
        PendingQueories.pop_back();
    }
    if(FreeQueoryObjects.size()==0) {
        CPendingQuery created;
        HRESULT result=reinterpret_cast<FableD3DQueryDevice*>(D3DDevice)->VTable->CreateQuery(D3DDevice,D3DQUERYTYPE_EVENT,&created.Query);
        if(result!=0) { FinishedFrame=PendingFrame-2;return; }
        FreeQueoryObjects.push_back(created);
    }
    CPendingQuery query=FreeQueoryObjects.back();
    FreeQueoryObjects.pop_back();
    query.Query->Issue(D3DISSUE_END);
    PendingQueories.push_front(query);
    ++PendingFrame;
}
