#include "engine/CEngineStaticMapFileBlockManager.h"
namespace _STL {
template<> void CStaticMapBlockTree::_M_erase(_Rb_tree_node<map<long,CEngineStaticMapFileBlockManager::CFileBlock>::value_type>* node) {
    while(node) {
        _M_erase(static_cast<_Rb_tree_node<map<long,CEngineStaticMapFileBlockManager::CFileBlock>::value_type>*>(node->_M_right));
        _Rb_tree_node<map<long,CEngineStaticMapFileBlockManager::CFileBlock>::value_type>* left=static_cast<_Rb_tree_node<map<long,CEngineStaticMapFileBlockManager::CFileBlock>::value_type>*>(node->_M_left);
        _Destroy(&node->_M_value_field);
        free(node);
        node=left;
    }
}
}

