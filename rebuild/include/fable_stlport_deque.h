#pragma once

// Retail STLport deque / stack (0x28 bytes): two four-pointer iterators,
// then the map pointer and map size. The base owns the 128-byte element
// blocks; reconstructed initialization lives in the 0x0059AA43/AC9F/AEF9
// units. stack's inline constructor delegates to deque (0x0059B310).
// top() 0x0059B039 steps a copy of _M_finish back and returns *_M_cur.

#include "fable_stlport_vector.h"

namespace _STL
{
    template <class _Tp>
    struct _Deque_iterator_layout
    {
        _Tp* _M_cur;
        _Tp* _M_first;
        _Tp* _M_last;
        _Tp** _M_node;
        void _M_set_node(_Tp** p){_M_node=p;_M_first=*p;_M_last=_M_first+(sizeof(_Tp)<128?128/sizeof(_Tp):1);}
        _Deque_iterator_layout() : _M_cur(0),_M_first(0),_M_last(0),_M_node(0) {}
    };

    template <class _Tp, class _Alloc = allocator<_Tp> >
    class _Deque_base
    {
    public:
        _Deque_base(const _Alloc&, size_t);
        void _M_initialize_map(size_t);
        void _M_create_nodes(_Tp**, _Tp**);
        typedef _Tp value_type;
        typedef _Tp& reference;
        typedef const _Tp& const_reference;

    protected:
        _Deque_iterator_layout<_Tp> _M_start;     // +0x00
        _Deque_iterator_layout<_Tp> _M_finish;    // +0x10
        _Tp** _M_map;                              // +0x20
        unsigned int _M_map_size;                  // +0x24
    };

    template <class T,class A=allocator<T> > class deque : public _Deque_base<T,A> {
    public:
        deque();
    };

    template <class _Tp, class _Sequence = deque<_Tp> >
    class stack
    {
    public:
        typedef typename _Sequence::value_type value_type;
        typedef typename _Sequence::reference reference;

        stack() : c() {}
        reference top();

    protected:
        _Sequence c;
    };
}

typedef char FableStlportDequeSizeMustBe28[
    sizeof(_STL::deque<void*>) == 0x28 ? 1 : -1];
