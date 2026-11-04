#include "fable_checksum_cache_file.h"
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::_Rep_type CacheTree;
typedef _STL::map<CWideString, CChecksumCacheFile::CEntry>::value_type CacheValue;
typedef _STL::_Rb_tree_node<CacheValue> CacheNode;
template<> void CacheTree::_M_erase(CacheNode* node)
{
    while (node) {
        _M_erase(static_cast<CacheNode*>(node->_M_right));
        CacheNode* next = static_cast<CacheNode*>(node->_M_left);
        node->_M_value_field.~CacheValue();
        free(node);
        node = next;
    }
}
template void CacheTree::_M_erase(CacheNode*);
