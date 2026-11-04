// Ego_r lib_vertex_group_block.cpp: CVertexGroupBlock, a set of vertex-group
// indices in insertion order plus a group-to-position map.
#include <new>
#include "fable_vertex_group_block.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_tree_impl.h"

// STLport 4.6's _M_find for the group map, inline here (retail expands the
// lower-bound walk into AddGroupsFrom 0x00AD9260 and the subset tests).
namespace _STL
{
    typedef _Rb_tree<unsigned long, pair<const unsigned long, long>, _Select1st<pair<const unsigned long, long> >,
                     less<unsigned long>, allocator<pair<const unsigned long, long> > > _GroupIndexTree;

    template <>
    inline _GroupIndexTree::_Base_ptr _GroupIndexTree::_M_find(const unsigned long& __k)
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = _M_root();
        while (__x != 0)
        {
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        }
        if (__y != this->_M_header._M_data)
        {
            if (_M_key_compare(__k, _S_key(__y)))
                __y = this->_M_header._M_data;
        }
        return __y;
    }

    // STLport's erase by key: the matching range, then the range erase (both
    // out of line); the unused count folds away.
    template <>
    inline _GroupIndexTree::size_type _GroupIndexTree::erase(const unsigned long& __x)
    {
        pair<iterator, iterator> __p = equal_range(__x);
        size_type __n = 0;
        for (iterator __i = __p.first; __i != __p.second; ++__i)
            ++__n;
        erase(__p.first, __p.second);
        return __n;
    }

    // The owning destructor (as for map<CCharString, long>): clear the nodes,
    // then free the header.
    template <>
    inline map<unsigned long, long>::~map()
    {
        _M_t.clear();
        if (_M_t._M_header._M_data != 0)
            _M_t._M_header.deallocate(_M_t._M_header._M_data, 1);
    }
}

CVertexGroupBlock::CVertexGroupBlock()
{
}

CVertexGroupBlock::~CVertexGroupBlock()
{
}

CVertexGroupBlock::CVertexGroupBlock(unsigned long capacity)
{
    Groups.reserve(capacity);
}

bool CVertexGroupBlock::HasGroup(unsigned long group) const
{
    // Our STLport model has no const find (its _M_find is out of line, non-const).
    _STL::map<unsigned long, long>& map = const_cast<_STL::map<unsigned long, long>&>(GroupToIndexMap);
    return map.find(group) != map.end();
}

void CVertexGroupBlock::AddGroup(unsigned long group)
{
    Groups.push_back(group);
    GroupToIndexMap.insert(_STL::pair<const unsigned long, long>(group, Groups.size() - 1));
}

void CVertexGroupBlock::AddGroupsFrom(const CVertexGroupBlock& other)
{
    for (unsigned long i = 0; i < other.Groups.size(); ++i)
    {
        if (!HasGroup(other.Groups[i]))
            AddGroup(other.Groups[i]);
    }
}

// Drops the last `count` groups.
void CVertexGroupBlock::RemoveLast(unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
    {
        GroupToIndexMap.erase(Groups.back());
        Groups.pop_back();
    }
}

void CVertexGroupBlock::Clear()
{
    Groups.clear();
    GroupToIndexMap.clear();
}
