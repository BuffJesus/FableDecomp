// Pending unique insertion00990900 exact263B; UID00990AD0 exact180B.
#include "fable_threaded_file_object.h"
namespace _STL
{
    // Walk down to the leaf where __v belongs; unless its key is already
    // there (the in-order predecessor of that slot is not below it), link a
    // new node in.
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = this->_M_header._M_data->_M_parent;   // The root.
        bool __comp = true;
        while (__x != 0)
        {
            __y = __x;
            __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
            __x = __comp ? __x->_M_left : __x->_M_right;
        }
        iterator __j = iterator(__y);
        if (__comp)
            if (__j == iterator(this->_M_header._M_data->_M_left))   // begin()
                return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
            else
                --__j;
        if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
            return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
        return pair<iterator, bool>(__j, false);
    }


typedef map<unsigned long,CThreadedFileManager::CFileObject*> UidMap;
typedef map<CThreadedFileManager::CPendingObjectSortKey,CThreadedFileManager::CFileObject*> PendingMap;
template pair<UidMap::_Rep_type::iterator,bool> UidMap::_Rep_type::insert_unique(const UidMap::value_type&);
template pair<PendingMap::_Rep_type::iterator,bool> PendingMap::_Rep_type::insert_unique(const PendingMap::value_type&);
}
