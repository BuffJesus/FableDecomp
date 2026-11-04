#include <new>
#include "fable_display_viewport.h"

typedef _STL::_List_node<CDeviceResetCallback*> ResetCallbackNode;
void CDisplayManager::AddDeviceResetCallback(CDeviceResetCallback* callback)
{
    ResetCallbackNode* sentinel=DeviceResetCallbacks._M_node;
    ResetCallbackNode* node=static_cast<ResetCallbackNode*>(malloc(sizeof(ResetCallbackNode)));
    _STL::_Construct(&node->_M_data,callback);
    _STL::_List_node_base* previous=sentinel->_M_prev;
    node->_M_next=sentinel;
    node->_M_prev=previous;
    previous->_M_next=node;
    sentinel->_M_prev=node;
}
void CDisplayManager::RemoveDeviceResetCallback(CDeviceResetCallback* callback)
{
    _STL::_List_node_base* node=DeviceResetCallbacks._M_node->_M_next;
    while(node!=DeviceResetCallbacks._M_node && static_cast<ResetCallbackNode*>(node)->_M_data!=callback)
        node=node->_M_next;
    if(node!=DeviceResetCallbacks._M_node) {
        node->_M_unhook();
        free(node);
    }
}
