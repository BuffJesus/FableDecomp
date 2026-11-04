// Retail 009C01C0; native owning-member destruction order.
#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"
#include "fable_stlport_uninitialized.h"
namespace _STL {
template<> void list<CDeviceResetCallback*>::clear() {
    _List_node_base* current=_M_node->_M_next;
    while(current!=_M_node) {
        _List_node_base* node=current;current=current->_M_next;free(node);
    }
    _M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;
}
// The pointer elements are not owned; only the nodes and sentinel are freed.
template<> __declspec(noinline) list<CDeviceResetCallback*>::~list() {
    clear();
    _Node* node=_M_node;if(node)free(node);
}
}
CDisplayManager::~CDisplayManager()
{
    if(UsingShaderRenderManager) FableShaderRenderManager_013bc470.Uninitialise();
    PRenderManager.reset();
    // DestroyDevice is inlined in the original translation unit.
    BackBuffer.Uninitialise();
    DepthBuffer.Uninitialise();
    if(D3DDevice) D3DDevice->Release();
}
