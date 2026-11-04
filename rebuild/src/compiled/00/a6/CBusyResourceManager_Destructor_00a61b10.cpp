// PDB CBusyResourceManager ownership; retail STLport list specializations.
#include "fable_busy_resource_manager.h"
namespace _STL {
template<> void list<CBusyResourceManager::CPendingQuery>::clear() {
    _List_node_base* current=_M_node->_M_next;
    while(current!=_M_node) {
        _List_node_base* node=current;current=current->_M_next;free(node);
    }
    _M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;
}
template<> __declspec(noinline) list<CBusyResourceManager::CPendingQuery>::~list() {
    clear();
    _Node* node=_M_node;if(node)free(node);
}
}
CBusyResourceManager::~CBusyResourceManager() { ClearAllQueries(); }
