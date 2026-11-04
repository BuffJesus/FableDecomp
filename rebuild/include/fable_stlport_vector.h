#pragma once

// Retail links STLport 4.x, not VC7.1's Dinkumware library. Its vector is
// 12 bytes: start, finish and an end-of-storage proxy that carries the
// (empty) allocator through the empty-base optimisation. Only the pieces a
// reconstruction needs are modelled; the construction path (vector ->
// _Vector_base -> _STLP_alloc_proxy, with a default-argument allocator
// temporary) is kept because it decides retail's instruction scheduling
// (CNewFrontendGameComponent ctor 0x0042EA8F).

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "rebuild_abi.h"

namespace _STL
{
    struct input_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template <class _Tp>
    inline void swap(_Tp& __a, _Tp& __b)
    {
        _Tp __tmp = __a;
        __a = __b;
        __b = __tmp;
    }

    inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
    {
        return (__last == __first)
            ? __result
            : ((char*)memmove(__result, __first, (const char*)__last - (const char*)__first))
                + ((const char*)__last - (const char*)__first);
    }

    // Type-trait tags, passed by const reference. Retail never stores into a
    // tag temporary (push_back 0x009E7AB0 and _M_insert_overflow 0x009E77E0
    // overlay them on dead argument slots); VC7.1 zero-fills `T()` of an
    // empty POD, so the tags carry an empty constructor.
    struct __true_type
    {
#ifndef FABLE_STLP_ZERO_TRUE_TAG
        __true_type()
        {
        }
#endif
    };
    struct __false_type
    {
        __false_type()
        {
        }
    };

    // STLport _Construct: copy-construct in place. Units that instantiate it
    // include <new> for the throw() placement new (which is why VC7.1
    // null-checks the result, as retail does); the header does not, since
    // fixtures define their own placement new.
    template <class _T1, class _T2>
    FABLE_UNIT_INLINE void _Construct(_T1* __p, const _T2& __val)
    {
        new (__p) _T1(__val);
    }

    // STLport _Destroy: run the destructor in place.
    template <class _Tp>
    inline void _Destroy(_Tp* __pointer)
    {
        __pointer->~_Tp();
    }

    // STLport's __type_traits, reduced to has_trivial_destructor: scalars and
    // pointers are trivial, class types are not (STLport's default without
    // compiler support), so ~vector of a class type calls __destroy_aux.
    template <class _Tp>
    struct __type_traits
    {
        typedef __false_type has_trivial_destructor;
        typedef __false_type is_POD_type;
    };
    template <class _Tp>
    struct __type_traits<_Tp*>
    {
        typedef __true_type has_trivial_destructor;
        typedef __true_type is_POD_type;
    };
#define FABLE_STLP_TRIVIAL_TYPE(T) \
    template <> \
    struct __type_traits<T> \
    { \
        typedef __true_type has_trivial_destructor; \
        typedef __true_type is_POD_type; \
    };
    FABLE_STLP_TRIVIAL_TYPE(bool)
    FABLE_STLP_TRIVIAL_TYPE(char)
    FABLE_STLP_TRIVIAL_TYPE(signed char)
    FABLE_STLP_TRIVIAL_TYPE(unsigned char)
    FABLE_STLP_TRIVIAL_TYPE(short)
    FABLE_STLP_TRIVIAL_TYPE(unsigned short)
    FABLE_STLP_TRIVIAL_TYPE(int)
    FABLE_STLP_TRIVIAL_TYPE(unsigned int)
    FABLE_STLP_TRIVIAL_TYPE(long)
    FABLE_STLP_TRIVIAL_TYPE(unsigned long)
    FABLE_STLP_TRIVIAL_TYPE(float)
    FABLE_STLP_TRIVIAL_TYPE(double)
#undef FABLE_STLP_TRIVIAL_TYPE

    template <class _Tp>
    inline const _Tp&(max)(const _Tp& __a, const _Tp& __b)
    {
        return __a < __b ? __b : __a;
    }
    // (min), also by const reference: a caller that keeps the bound in a local
    // selects between the two addresses (CAFile::CompareFiles 0x00998F60:
    // `lea eax,[bound]; jg; lea eax,[count]`).
    template <class _Tp>
    inline const _Tp&(min)(const _Tp& __a, const _Tp& __b)
    {
        return __b < __a ? __b : __a;
    }

    // STLport's non-POD range helpers: loops VC7.1 keeps out of line. They
    // are declared here (bodies: fable_stlport_uninitialized.h, which callers
    // such as _M_insert_overflow 0x009E77E0 must see) and instantiated in the
    // landings that reconstruct them (<pair<CCharString, CSymbolMap::CSymbolInfo>*>:
    // __uninitialized_copy 0x009E6270, __uninitialized_fill_n 0x009E62E0,
    // __destroy_aux 0x006FBCC0). Free functions are __fastcall in retail.
    template <class _InputIter, class _ForwardIter>
    _ForwardIter FABLE_FASTCALL __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                     _ForwardIter __result, const __false_type&);
    template <class _ForwardIter, class _Size, class _Tp>
    _ForwardIter FABLE_FASTCALL __uninitialized_fill_n(_ForwardIter __first, _Size __n,
                                                       const _Tp& __x, const __false_type&);
    template <class _ForwardIter>
    void FABLE_FASTCALL __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&);

    template <class _ForwardIter>
    inline void __destroy_aux(_ForwardIter, _ForwardIter, const __true_type&)
    {
    }

    // POD elements copy as raw memory (STLport's __uninitialized_copy with
    // __true_type, down to __copy_trivial). Fastcall like the other free
    // helpers: a size-optimised unit keeps it out of line (<unsigned long>:
    // 0x0045BCC1, from vector::operator= 0x0045BC09).
    template <class _Tp>
    inline _Tp* FABLE_FASTCALL __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result,
                                                    const __true_type&)
    {
        return (_Tp*)__copy_trivial(__first, __last, __result);
    }

    // STLport _Destroy(first, last): destroy a range in place.
    template <class _Tp>
    FABLE_UNIT_INLINE void _Destroy(_Tp* __first, _Tp* __last)
    {
        __destroy_aux(__first, __last, typename __type_traits<_Tp>::has_trivial_destructor());
    }

    template <class _Tp>
    class allocator
    {
    public:
        allocator() {}
        allocator(const allocator<_Tp>&) {}
        ~allocator() {}

        // Retail's STLport allocator is malloc/free backed.
        _Tp* allocate(size_t __n)
        {
            return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0;
        }

        void deallocate(_Tp* __p, size_t)
        {
            free(__p);
        }
    };

    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc
    {
    public:
        _Value _M_data;

        _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p)
            : _MaybeReboundAlloc(__a), _M_data(__p)
        {
        }
    };

    template <class _Tp, class _Alloc>
    class _Vector_base
    {
    public:
        _Vector_base(const _Alloc& __a)
            : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
        {
        }

        // Allocate room for exactly n elements (the copy constructor's base).
        _Vector_base(size_t __n, const _Alloc& __a)
            : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
        {
            _M_start = _M_end_of_storage.allocate(__n);
            _M_finish = _M_start;
            _M_end_of_storage._M_data = _M_start + __n;
        }

        // STLport passes the capacity too; the malloc allocator ignores it,
        // and leaving it out keeps vectors of incomplete types declarable.
        ~_Vector_base()
        {
            if (_M_start != 0)
                _M_end_of_storage.deallocate(_M_start, 0);
        }

        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : public _Vector_base<_Tp, _Alloc>
    {
    public:
        typedef _Tp value_type;
        explicit vector(const _Alloc& __a = _Alloc())
            : _Vector_base<_Tp, _Alloc>(__a)
        {
        }

        // STLport _Destroy(first, last) then ~_Vector_base. Element type
        // must be complete where a vector is destroyed. Out of line under
        // /Os for vector<CWideString> (0x00412130); inlined to the bare
        // free() for trivially destructible elements (vector<CDateAndTime>
        // in ~CNewFrontendGameComponent 0x0042F50B); for pair<CCharString,
        // CSymbolInfo> the __destroy_aux call stays out of line (0x006FBCC0,
        // from CVectorMap::DeallocateUnusedMemory 0x009E7C70).
        ~vector()
        {
            _Destroy(this->_M_start, this->_M_finish);
        }

        // Out of line in retail (vector<unsigned long>: 0x00535470, copying a
        // CDefinitionManager::CDefClassInfo in AddDefClass 0x009B0AC0); the
        // body is in fable_stlport_vector_impl.h.
        vector(const vector<_Tp, _Alloc>& __x);
        // STLport's out-of-line assignment (vector<unsigned long>: 0x0045BC09,
        // from __linear_insert 0x009B8C00 assigning a CDefClassInfo).
        vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);

        // Zero-filled; retail calls it (vector<unsigned char>: 0x411910).
        explicit vector(size_t __n);

        _Tp* begin() { return this->_M_start; }
        _Tp* end() { return this->_M_finish; }
        const _Tp* begin() const { return this->_M_start; }
        const _Tp* end() const { return this->_M_finish; }
        size_t size() const { return size_t(this->_M_finish - this->_M_start); }
        size_t capacity() const { return size_t(this->_M_end_of_storage._M_data - this->_M_start); }
        _Alloc get_allocator() const { return _Alloc(); }
        bool empty() const { return this->_M_start == this->_M_finish; }
        // STLport's front/back (inline; C3DMeshFileGroupChunk::ReadFromFile 0x00ABCC80).
        _Tp& front() { return *this->_M_start; }
        const _Tp& front() const { return *this->_M_start; }
        _Tp& back() { return *(this->_M_finish - 1); }
        const _Tp& back() const { return *(this->_M_finish - 1); }
        _Tp& operator[](size_t __n) { return *(this->_M_start + __n); }
        const _Tp& operator[](size_t __n) const { return *(this->_M_start + __n); }

        // Out of line in retail; byte-identical instantiations fold
        // (vector<float>/vector<unsigned int>: 0x00442770).
        void resize(size_t __new_size, const _Tp& __x);

        // STLport's push_back is inline. VC7.1 keeps it out of line for class
        // elements (vector<pair<CCharString, CSymbolMap::CSymbolInfo> >:
        // 0x009E7AB0, from CSymbolMap::AddSymbolDirty) and inlines it for
        // pointers and scalars (CDefinitionManager::InstantiateDef 0x009ADC90).
        // The inline body is in fable_stlport_vector_impl.h (it needs the
        // placement new of <new>, which this header leaves to its users).
        void push_back(const _Tp& __x);
        void pop_back()
        {
            --this->_M_finish;
            _Destroy(this->_M_finish);
        }

        // STLport's reallocating insert (out of line; <pair<CCharString,
        // CSymbolInfo> >: 0x009E77E0).
        void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                size_t __fill_len, bool __atend = false);
        // The POD form (out of line; <CDefinitionManager::CInstantiatedDefInfo*>:
        // 0x009B4BD0, <unsigned long>: 0x00464ACE).
        void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                size_t __fill_len, bool __atend = false);
        // Per-unit: retail keeps the one-argument resize out of line for some class elements (vector<C3DGroup2>:
        // 0x00AE8540, from CParticleProgram::ReadGroups 0x00AE3A60; Ego_r too). A unit that defines
        // FABLE_VECTOR_RESIZE_OUT_OF_LINE before its first include gets only the declaration (the instantiation is
        // another unit's); everywhere else the inline forwarder below is unchanged.
#ifdef FABLE_VECTOR_RESIZE_OUT_OF_LINE
        void resize(size_t __new_size);
#else
        void resize(size_t __new_size) { resize(__new_size, _Tp()); }
#endif

        // Out of line in retail (<pair<CCharString, CSymbolInfo> >: 0x009E72C0).
        void reserve(size_t __n);

        void swap(vector<_Tp, _Alloc>& __x)
        {
            _STL::swap(this->_M_start, __x._M_start);
            _STL::swap(this->_M_finish, __x._M_finish);
            _STL::swap(this->_M_end_of_storage._M_data, __x._M_end_of_storage._M_data);
        }

        // Range insert for pointer ranges; STLport's dispatch is inline and
        // lands on _M_range_insert (out of line; <pair<CCharString,
        // CSymbolInfo> >: 0x009E73F0).
        // Fill insert (STLport's inline forwarder; _M_fill_insert is out of line:
        // <char> 0x004110D0, from resize 0x004122D0).
        void insert(_Tp* __pos, size_t __n, const _Tp& __x)
        {
            _M_fill_insert(__pos, __n, __x);
        }
        void _M_fill_insert(_Tp* __pos, size_t __n, const _Tp& __x);

        void insert(_Tp* __pos, const _Tp* __first, const _Tp* __last)
        {
            _M_range_insert(__pos, __first, __last, forward_iterator_tag());
        }
        void _M_range_insert(_Tp* __pos, const _Tp* __first, const _Tp* __last, const forward_iterator_tag&);

        // STLport's private helpers (inline); the out-of-line members that
        // use them are defined in fable_stlport_vector_impl.h.
        FABLE_UNIT_INLINE _Tp* _M_allocate_and_copy(size_t __n, const _Tp* __first, const _Tp* __last)
        {
            _Tp* __result = this->_M_end_of_storage.allocate(__n);
            __uninitialized_copy(__first, __last, __result, typename __type_traits<_Tp>::is_POD_type());
            return __result;
        }
        FABLE_UNIT_INLINE void _M_clear()
        {
            _Destroy(this->_M_start, this->_M_finish);
            if (this->_M_start != 0)
                this->_M_end_of_storage.deallocate(this->_M_start, 0);
        }
        void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
        {
            this->_M_start = __s;
            this->_M_finish = __f;
            this->_M_end_of_storage._M_data = __e;
        }

        // STLport's inline body below dispatches on the element's traits; a
        // unit that retail keeps out of line supplies an explicit
        // specialization (vector<CInputEvent>: 0x009F67E0).
        _Tp* erase(_Tp* __first, _Tp* __last);
        // STLport's single-element erase, inlined in retail: trivially assignable elements move as raw memory (scratch
        // resource release), class elements through __copy (CVertexPairContraction::CleanUp 0x00AC7B00 expands it).
        _Tp* erase(_Tp* __position)
        {
            if(__position+1!=this->_M_finish)
                __copy_ptrs(__position+1,this->_M_finish,__position,typename __type_traits<_Tp>::is_POD_type());
            --this->_M_finish;
            _Destroy(this->_M_finish);
            return __position;
        }
        void clear() { erase(begin(), end()); }
    };
}

namespace _STL
{
    // STLport's counted random-access copy (body: fable_stlport_algobase.h).
    template <class _RandomAccessIter, class _OutputIter, class _Distance>
    _OutputIter FABLE_FASTCALL __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                      _OutputIter __result, const random_access_iterator_tag&, _Distance*);

    // STLport __copy_ptrs: trivially assignable elements move as raw memory
    // (__copy_trivial keeps its empty-range test, which retail emits even for
    // erase(begin(), end())); class elements are assigned one by one through
    // the out-of-line __copy (<CLipSyncDataBankEntry*>: 0x00A438E0).
    template <class _Tp>
    inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
    {
        return (_Tp*)__copy_trivial(__first, __last, __result);
    }
    template <class _Tp>
    inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
    {
        return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
    }

    // STLport's range erase: copy the tail down, destroy what is left over
    // (nothing for trivial elements; CLipSyncDataBank::Close 0x00A42DA0 calls
    // __destroy_aux 0x00A43A00), then cut the end.
    template <class _Tp, class _Alloc>
    inline _Tp* vector<_Tp, _Alloc>::erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, typename __type_traits<_Tp>::is_POD_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
}

FABLE_STATIC_ASSERT(sizeof(_STL::vector<int>) == 0x0C);

// STLport's headers carry the helper bodies with the declarations. A fixture
// of one of those helpers defines FABLE_STLPORT_DECLARATIONS_ONLY so that it
// calls the landed out-of-line instantiation instead of compiling its own.
#ifndef FABLE_STLPORT_DECLARATIONS_ONLY
#include "fable_stlport_uninitialized.h"
#endif
