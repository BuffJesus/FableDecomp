#include "fable_threaded_file_object.h"
typedef _STL::map<unsigned long,CThreadedFileManager::CFileObject*>::_Rep_type UidTree;
typedef _STL::_Rb_tree_node<_STL::pair<const unsigned long,CThreadedFileManager::CFileObject*> > UidNode;
template<> void UidTree::_M_erase(UidNode*);

namespace _STL {
template<class K,class V,class KeyOf,class Compare,class Alloc>
void _Rb_tree<K,V,KeyOf,Compare,Alloc>::_M_erase(_Rb_tree_node<V>* node)
{
    while(node) {
        _M_erase(static_cast<_Rb_tree_node<V>*>(node->_M_right));
        _Rb_tree_node<V>* left=static_cast<_Rb_tree_node<V>*>(node->_M_left);
        _Destroy(&node->_M_value_field);
        free(node);
        node=left;
    }
}

template<> inline map<unsigned long,CThreadedFileManager::CFileObject*>::~map()
{
    _M_t.clear();
    if(_M_t._M_header._M_data)
        _M_t._M_header.deallocate(_M_t._M_header._M_data,1);
}

template<> inline map<CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*>::~map()
{
    _M_t.clear();
    if(_M_t._M_header._M_data)
        _M_t._M_header.deallocate(_M_t._M_header._M_data,1);
}
}

CThreadedFileManager::~CThreadedFileManager()
{
    Uninitialise();
}
