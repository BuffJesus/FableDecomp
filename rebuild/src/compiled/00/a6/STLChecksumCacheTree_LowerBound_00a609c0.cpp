#include "fable_checksum_cache_file.h"
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::_Rep_type Tree;
template<> Tree::_Link_type Tree::_M_lower_bound(const CWideString& key) const {
    _Link_type result=this->_M_header._M_data;
    _Link_type node=_M_root();
    while(node) {
        if(!_M_key_compare(_S_key(node),key)) {result=node;node=_S_left(node);}
        else node=_S_right(node);
    }
    return result;
}
