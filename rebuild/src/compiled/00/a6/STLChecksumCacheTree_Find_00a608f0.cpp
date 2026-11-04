#include "fable_checksum_cache_file.h"
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::_Rep_type Tree;
template<> Tree::_Base_ptr Tree::_M_find(const CWideString& key) {
    _Base_ptr result=this->_M_header._M_data;
    _Base_ptr node=result->_M_parent;
    while(node) {
        if(!_M_key_compare(_S_key(node),key)) {result=node;node=node->_M_left;}
        else node=node->_M_right;
    }
    if(result==this->_M_header._M_data || _M_key_compare(key,_S_key(result)))result=this->_M_header._M_data;
    return result;
}
