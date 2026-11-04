#include "fable_threaded_file_object.h"
// Retail 0x009902D0: the uid map's find, out of line with _M_find inlined (same recipe as
// CCodeSectionTree::find 0x009F0D90, whose bytes it shares).
namespace _STL {
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    __forceinline typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k)
    {
        _Base_ptr __header = this->_M_header._M_data;          // end()
        _Base_ptr __y = this->_M_header._M_data;               // Last node which is not less than __k.
        _Base_ptr __x = this->_M_header._M_data->_M_parent;    // Current node (the root).

        while (__x != 0)
            if (!_M_key_compare(static_cast<_Rb_tree_node<_Value>*>(__x)->_M_value_field.first, __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        if (__y == __header ||
            _M_key_compare(__k, static_cast<_Rb_tree_node<_Value>*>(__y)->_M_value_field.first))
            __y = __header;
        return __y;
    }
typedef _Rb_tree<unsigned long,
    pair<const unsigned long,CThreadedFileManager::CFileObject*>,
    _Select1st<pair<const unsigned long,CThreadedFileManager::CFileObject*> >,
    less<unsigned long>,
    allocator<pair<const unsigned long,CThreadedFileManager::CFileObject*> > > CThreadedFileUidTree;
template<> CThreadedFileUidTree::iterator CThreadedFileUidTree::find(const unsigned long& key) { return iterator(_M_find(key)); }
template CThreadedFileUidTree::iterator CThreadedFileUidTree::find(const unsigned long&);
}
