#include <new>
#include "fable_threaded_file_object.h"
#include "fable_stlport_tree_erase.h"
#include "fable_stlport_iterator_distance.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long,int);

namespace _STL {
    template <class _Tp, class _Alloc>
    inline void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data)
        {
            _Tp* finish = this->_M_finish;
            _Construct(finish, __x);
            this->_M_finish = finish + 1;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, typename __type_traits<_Tp>::is_POD_type(), 1UL, true);
    }

}
namespace _STL {
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    inline typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr
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
}


namespace _STL {
template<class K,class V,class KeyOf,class Compare,class Alloc>
pair<typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::iterator,
     typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::iterator>
_Rb_tree<K,V,KeyOf,Compare,Alloc>::equal_range(const K& key)
{
    return pair<iterator,iterator>(lower_bound(key),upper_bound(key));
}
template<class K,class V,class KeyOf,class Compare,class Alloc>
typename _Rb_tree<K,V,KeyOf,Compare,Alloc>::size_type
_Rb_tree<K,V,KeyOf,Compare,Alloc>::erase(const K& key)
{
    pair<iterator,iterator> range=equal_range(key);
    size_type count=static_cast<size_type>(distance(range.first,range.second));
    erase(range.first,range.second);
    return count;
}
}
namespace _STL {
template<class I>
inline ptrdiff_t __cdecl __distance(const I& first,const I& last,const input_iterator_tag&)
{
    ptrdiff_t count=0;
    I current(first);
    while (current!=last) {
        ++current;
        ++count;
    }
    return count;
}
}

// CThreadedFileManager::CloseAllStreamsForFile @ 0098E7D0.
// Snapshot IDs before callbacks: callbacks may remove existing requests or add
// new ones. Re-find after every unlocked wait/callback before dereferencing.
// Retail inlines the first two tree lookups and calls shared find after callback.
// NEAR_MATCH: only eight cursor-comparison scheduling bytes differ from retail.
// gate_native_threaded_close_streams.py checks the exact remaining difference.
void CThreadedFileManager::CloseAllStreamsForFile(CThreadedFile* file)
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
    EnterCriticalSection(section);
    ++BlockPendingObjectProcessingRefCount;
    typedef _STL::map<unsigned long,CFileObject*>::iterator Iterator;
    Iterator first=Objects.begin();
    _STL::vector<unsigned long> ids;
    for(;first!=Objects.end();++first)
        if(first->second->PFile==file) ids.push_back(first->first);
    unsigned long count=ids.size();
    if(count>0) for(unsigned long* current=ids.begin();count>0;--count,++current) {
        Iterator it(Objects._M_t._M_find(*current));
        while(it!=Objects.end() &&
              (it->second->LoadingStatus==FILE_LOADING_STATUS_PENDING_LOCKED ||
               it->second->LoadingStatus==FILE_LOADING_STATUS_READING ||
               it->second->LoadingStatus==FILE_LOADING_STATUS_READ_COMPLETED)) {
            LeaveCriticalSection(section);
            SetEvent(Event);
            SleepEx(0,1);
            EnterCriticalSection(section);
            it=Iterator(Objects._M_t._M_find(*current));
        }
        if(it!=Objects.end()) {
            CFileObject* object=it->second;
            if(object->LoadingStatus==FILE_LOADING_STATUS_PENDING) {
                {

                    const unsigned long device = object->PFile->deviceId_;
                    CPendingObjectSortKey key;
                    key.Object = object;
                    _STL::map<CPendingObjectSortKey, CFileObject*>::iterator& current =
                        DeviceQueue[device][object->Desc.Priority].CurrentPosition;
                    if (DeviceQueue[device][object->Desc.Priority].Objects.find(key) == current)
                        ++current;
                    DeviceQueue[device][object->Desc.Priority].Objects.erase(key);
                }
                if(object->Desc.PCallback) {
                    LeaveCriticalSection(section);
                    object->Desc.PCallback->OnFileReadCompletion(false);
                    EnterCriticalSection(section);
                    it=Objects.find(*current);
                }
            }
            if(it!=Objects.end()) {
                delete it->second;
                Objects.erase(it);
            }
        }
    }
    --BlockPendingObjectProcessingRefCount;
    LeaveCriticalSection(section);
}
