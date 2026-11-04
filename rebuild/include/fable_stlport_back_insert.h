#pragma once

// STLport back_insert_iterator / back_inserter (stl/_iterator.h). Retail
// copies a byte range into CMemoryFile's buffer through it: the counted
// __copy over a back_insert_iterator<vector<unsigned char> > is out of line
// (0x009E9900, fastcall) and push_back expands inside its loop.

#include "fable_stlport_algobase.h"

namespace _STL
{
    template <class _Container>
    class back_insert_iterator
    {
    protected:
        _Container* container;

    public:
        explicit back_insert_iterator(_Container& __x) : container(&__x) {}
        back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
        {
            container->push_back(__value);
            return *this;
        }
        back_insert_iterator<_Container>& operator*() { return *this; }
        back_insert_iterator<_Container>& operator++() { return *this; }
        back_insert_iterator<_Container> operator++(int) { return *this; }
    };

    template <class _Container>
    inline back_insert_iterator<_Container> back_inserter(_Container& __x)
    {
        return back_insert_iterator<_Container>(__x);
    }
}
