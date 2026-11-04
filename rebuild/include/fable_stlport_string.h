#pragma once

// Retail STLport 4.x basic_string (12 bytes: start, finish and an
// end-of-storage proxy over the malloc allocator), as used by
// CWideStringData::Data (Ego_r: basic_string<wchar_t> Data; long NoRefs).
// wchar_t is unsigned short in retail (no /Zc:wchar_t). Only the pieces a
// reconstruction needs are modelled: the construction path from a
// C string (CWideString::AllocStringData 0x0099B3C0 inlines both ctors and
// calls the range initialiser 0x0099C670 out of line). The destructor is not
// modelled yet: landed code frees _M_start explicitly (the inlined
// ~basic_string) before deleting the CWideStringData block.

#include "fable_stlport_vector.h"

namespace _STL
{
    // Existing reduced-model fastcall helper shared with string vectors.
    template <class T> T* FABLE_FASTCALL uninitialized_fill_n(T* first, size_t count, const T& value);
    template <> wchar_t* FABLE_FASTCALL uninitialized_fill_n<wchar_t>(wchar_t* first, size_t count, const wchar_t& value);

    // The iterator tags live in fable_stlport_vector.h.

    // STLport's generic traits; retail's length is an inline loop, not wcslen.
    template <class _CharT>
    struct char_traits
    {
        static size_t length(const _CharT* __s)
        {
            size_t __i = 0;
            while (__s[__i] != _CharT())
                ++__i;
            return __i;
        }
    };

    template <class _Tp, class _Alloc>
    class _String_base
    {
    public:
        _String_base(const _Alloc& __a)
            : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0)
        {
        }

        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;

        size_t max_size() const { return (size_t(-1) / sizeof(_Tp)) - 1; }

        void _M_allocate_block(size_t __n)
        {
            if ((__n <= (max_size() + 1)) && (__n > 0))
            {
                _M_start = _M_end_of_storage.allocate(__n);
                _M_finish = _M_start;
                _M_end_of_storage._M_data = _M_start + __n;
            }
            else
                _M_throw_length_error();
        }

        // STLport _M_deallocate_block (inline): free the buffer.
        void _M_deallocate_block()
        {
            if (_M_start != 0)
                _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
        }

        // Out of line in retail (0x0099C550 for basic_string<wchar_t>).
        __declspec(noreturn) void _M_throw_length_error() const;
    };

    template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
    class basic_string : public _String_base<_CharT, _Alloc>
    {
    public:
        basic_string(const _CharT* __s, const _Alloc& __a = _Alloc())
            : _String_base<_CharT, _Alloc>(__a)
        {
            _M_range_initialize(__s, __s + _Traits::length(__s));
        }

        basic_string(const _CharT* __s, size_t __n, const _Alloc& __a = _Alloc())
            : _String_base<_CharT, _Alloc>(__a)
        {
            _M_range_initialize(__s, __s + __n);
        }

        basic_string(size_t count,_CharT character,const _Alloc& alloc=_Alloc());
        // STLport resize: out of line (basic_string<wchar_t>: 0x0099CD60, from
        // CWideString::SetLength 0x0099BC50); the one-argument form forwards.
        void resize(size_t __n, _CharT __c);
        void resize(size_t __n) { resize(__n, _CharT()); }

        template <class _InputIter>
        void _M_range_initialize(_InputIter __f, _InputIter __l)
        {
            _M_range_initialize(__f, __l, random_access_iterator_tag());
        }

        // STLport's forward-iterator range initialiser (out of line in retail:
        // basic_string<wchar_t> const wchar_t* instantiation 0x0099C670).
        template <class _ForwardIter>
        void _M_range_initialize(_ForwardIter __f, _ForwardIter __l, const forward_iterator_tag&)
        {
            ptrdiff_t __n = __l - __f;
            this->_M_allocate_block(__n + 1);
            this->_M_finish = (_CharT*)__copy_trivial(__f, __l, this->_M_start);
            _M_terminate_string();
        }

        void _M_terminate_string() { *this->_M_finish = _CharT(); }

        size_t size() const { return this->_M_finish - this->_M_start; }
        size_t capacity() const { return (this->_M_end_of_storage._M_data - this->_M_start) - 1; }

        // STLport's inline appends: a C string goes through the range form,
        // which dispatches on the iterator category.
        void reserve(size_t count);

        basic_string& append(size_t count, _CharT character)
        {
            if (count != 0)
            {
                const size_t old_size = size();
                if (count > this->max_size() || old_size > this->max_size() - count)
                    this->_M_throw_length_error();
                if (old_size + count > capacity())
                    reserve(old_size + (max)(old_size, count));
                uninitialized_fill_n(this->_M_finish + 1, count - 1, character);
                *(this->_M_finish + count) = _CharT();
                *this->_M_finish = character;
                this->_M_finish += count;
            }
            return *this;
        }

        basic_string& append(const _CharT* __s)
        {
            return append(__s, __s + _Traits::length(__s));
        }
        basic_string& append(const _CharT* __first, const _CharT* __last)
        {
            return append(__first, __last, random_access_iterator_tag());
        }

        // STLport's forward-iterator append (out of line in retail: the
        // basic_string<wchar_t> const wchar_t* instantiation 0x0099C7E0, from
        // CWideString::operator+= 0x0099B940). The new text goes after the
        // old in place when it fits (its first character last, over the old
        // terminator); otherwise both are copied into a buffer of
        // size + max(size, n) + 1.
        template <class _ForwardIter>
        basic_string& append(_ForwardIter __first, _ForwardIter __last, const forward_iterator_tag&)
        {
            if (__first != __last)
            {
                const size_t __old_size = size();
                ptrdiff_t __n = __last - __first;
                if ((size_t)__n > this->max_size() || __old_size > this->max_size() - (size_t)__n)
                    this->_M_throw_length_error();
                if (__old_size + __n > capacity())
                {
                    const size_t __len = __old_size + (max)(__old_size, (size_t)__n) + 1;
                    _CharT* __new_start = this->_M_end_of_storage.allocate(__len);
                    _CharT* __new_finish = __new_start;
                    __new_finish = __uninitialized_copy((const _CharT*)this->_M_start, (const _CharT*)this->_M_finish,
                                                        __new_start, __true_type());
                    __new_finish = __uninitialized_copy(__first, __last, __new_finish, __true_type());
                    *__new_finish = _CharT();
                    this->_M_deallocate_block();
                    this->_M_start = __new_start;
                    this->_M_finish = __new_finish;
                    this->_M_end_of_storage._M_data = __new_start + __len;
                }
                else
                {
                    _ForwardIter __f1 = __first;
                    ++__f1;
                    __uninitialized_copy(__f1, __last, this->_M_finish + 1, __true_type());
                    *(this->_M_finish + __n) = _CharT();
                    *this->_M_finish = *__first;
                    this->_M_finish += __n;
                }
            }
            return *this;
        }

        const _CharT* c_str() const { return this->_M_start; }
    };
}
