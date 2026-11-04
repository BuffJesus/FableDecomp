#pragma once

// Retail STLport 4.x vector members that live in stl/_vector.c (template
// bodies the header includes). Kept opt-in: fixtures specialize these members
// as stubs, which must come before any implicit instantiation. Retail
// <pair<CCharString, CSymbolMap::CSymbolInfo> >: reserve 0x009E72C0,
// _M_range_insert 0x009E73F0 (both instantiated in SymbolMap.cpp);
// <unsigned long>: the copy constructor 0x00535470. push_back:
// <pair<CCharString, CSymbolInfo> > 0x009E7AB0, <pair<unsigned long,
// CDefinitionManager::CDefClassInfo> > 0x009B9170; _M_insert_overflow:
// 0x009E77E0 and 0x009B8DD0 respectively.

#include "fable_stlport_vector.h"
#include "fable_stlport_algobase.h"

namespace _STL
{
    // STLport's uninitialized_fill_n as _M_fill_insert sees it: trivial
    // elements inline to fill_n (rep stos for the byte types, <char> 0x004110D0);
    // class elements call the out-of-line fastcall instantiation, whose loop
    // body the landing unit supplies (<CLipSyncDataBankEntry>: 0x00A43670).
    template <class _Tp>
    inline _Tp* __fill_insert_uninitialized(_Tp* __first, size_t __n, const _Tp& __x, const __true_type&)
    {
        return __uninitialized_fill_n(__first, __n, __x, __true_type());
    }
    template <class _Tp>
    inline _Tp* __fill_insert_uninitialized(_Tp* __first, size_t __n, const _Tp& __x, const __false_type&)
    {
        return uninitialized_fill_n(__first, __n, __x);
    }

    // STLport's zero-filled vector(n): room for exactly n elements, then the
    // uninitialized fill (memset for the byte types). Retail expands it into
    // CDataInputStream::CopyTo 0x009964E0 and calls it elsewhere
    // (vector<unsigned char>: 0x00411910).
    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::vector(size_t __n)
        : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = __uninitialized_fill_n(this->_M_start, __n, _Tp(),
                                                 typename __type_traits<_Tp>::is_POD_type());
    }

    template <class _Tp, class _Alloc>
    vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                               typename __type_traits<_Tp>::is_POD_type());
    }

    // Copy the element into the spare slot, or grow the storage by one
    // (STLport's inline push_back; the overflow overload follows the type).
    template <class _Tp, class _Alloc>
    inline void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data)
        {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, typename __type_traits<_Tp>::is_POD_type(), 1UL, true);
    }

    // STLport's growth path for non-POD elements: new storage of at least double
    // the size, the elements before the insertion point copied over, the new
    // element(s) constructed, the tail copied unless inserting at the end, then
    // the old elements destroyed and their storage freed.
    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x,
                                                 const __false_type& /*DO NOT USE!!*/, size_t __fill_len,
                                                 bool __atend)
    {
        const size_t __old_size = size();
        const size_t __len = __old_size + (max)(__old_size, __fill_len);

        _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
        _Tp* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1)
        {
            _Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());

        __destroy_aux(this->_M_start, this->_M_finish, __false_type());
        if (this->_M_start != 0)
            this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
        this->_M_start = __new_start;
        this->_M_finish = __new_finish;
        this->_M_end_of_storage._M_data = __new_start + __len;
    }

    // The POD growth path: raw copies around the new element(s)
    // (<CDefinitionManager::CInstantiatedDefInfo*>: 0x009B4BD0).
    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x,
                                                 const __true_type& /*_IsPODType()*/, size_t __fill_len,
                                                 bool __atend)
    {
        const size_t __old_size = size();
        const size_t __len = __old_size + (max)(__old_size, __fill_len);

        _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
        _Tp* __new_finish = (_Tp*)__copy_trivial(this->_M_start, __position, __new_start);
        // handle insertion
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            // copy remainder
            __new_finish = (_Tp*)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }

    // STLport's fill insert (<char>: 0x004110D0; <CLipSyncDataBankEntry>:
    // 0x00A43CD0): room left -> move the tail up and fill the gap; otherwise
    // grow through _M_insert_overflow. The element's traits pick raw-memory or
    // element-wise helpers.
    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::_M_fill_insert(_Tp* __position, size_t __n, const _Tp& __x)
    {
        if (__n != 0)
        {
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                _Tp __x_copy = __x;
                const size_t __elems_after = this->_M_finish - __position;
                _Tp* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish,
                                         typename __type_traits<_Tp>::is_POD_type());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish,
                                         typename __type_traits<_Tp>::is_POD_type());
                    fill(__position, __position + __n, __x_copy);
                }
                else
                {
                    __fill_insert_uninitialized(this->_M_finish, __n - __elems_after, __x_copy,
                                                typename __type_traits<_Tp>::is_POD_type());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish,
                                         typename __type_traits<_Tp>::is_POD_type());
                    this->_M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            }
            else
                _M_insert_overflow(__position, __x, typename __type_traits<_Tp>::is_POD_type(), __n);
        }
    }

    // STLport's resize (an in-class inline VC7.1 keeps out of line: <char>
    // 0x004122D0; <unsigned int>/<float> 0x00442770): cut the tail, or append
    // copies of the value.
    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::resize(size_t __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::reserve(size_t __n)
    {
        if (capacity() < __n)
        {
            const size_t __old_size = size();
            _Tp* __tmp;
            if (this->_M_start)
            {
                __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
                _M_clear();
            }
            else
                __tmp = this->_M_end_of_storage.allocate(__n);
            _M_set(__tmp, __tmp + __old_size, __tmp + __n);
        }
    }

    template <class _Tp, class _Alloc>
    void vector<_Tp, _Alloc>::_M_range_insert(_Tp* __position, const _Tp* __first, const _Tp* __last,
                                              const forward_iterator_tag&)
    {
        if (__first != __last)
        {
            size_t __n = __last - __first;
            if (size_t(this->_M_end_of_storage._M_data - this->_M_finish) >= __n)
            {
                const size_t __elems_after = this->_M_finish - __position;
                _Tp* __old_finish = this->_M_finish;
                if (__elems_after > __n)
                {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    copy_backward(__position, __old_finish - __n, __old_finish);
                    copy(__first, __last, __position);
                }
                else
                {
                    const _Tp* __mid = __first + __elems_after;
                    __uninitialized_copy(__mid, __last, this->_M_finish, __false_type());
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                    this->_M_finish += __elems_after;
                    copy(__first, __mid, __position);
                }
            }
            else
            {
                const size_t __old_size = size();
                const size_t __len = __old_size + (max)(__old_size, __n);
                _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
                _Tp* __new_finish = __new_start;
                __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
                __new_finish = __uninitialized_copy(__first, __last, __new_finish, __false_type());
                __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
                _M_clear();
                _M_set(__new_start, __new_finish, __new_start + __len);
            }
        }
    }
}
