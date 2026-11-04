#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_device_reset_callback.h"
typedef _STL::_List_node<CDeviceResetCallback*> ResetCallbackNode;
void CDisplayManager::PreDeviceReset()
{
    if(PRenderManager.get()) PRenderManager->Uninit();
    VsyncCallbackTimer->OnPreDeviceReset();
    for(_STL::_List_node_base* node=DeviceResetCallbacks._M_node->_M_next;node!=DeviceResetCallbacks._M_node;node=node->_M_next)
        static_cast<ResetCallbackNode*>(node)->_M_data->OnPreDeviceReset();
    SystemManager->OnPreDeviceReset();
}
bool CDisplayManager::PostDeviceReset()
{
    if(UsingRenderManager) PRenderManager->Init(D3DDevice);
    for(_STL::_List_node_base* node=DeviceResetCallbacks._M_node->_M_next;node!=DeviceResetCallbacks._M_node;node=node->_M_next)
        if(!static_cast<ResetCallbackNode*>(node)->_M_data->OnPostDeviceReset()) return false;
    SystemManager->OnPostDeviceReset();
    return true;
}
