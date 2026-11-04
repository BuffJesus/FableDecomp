#pragma once

// Retail STLport 4.x red-black tree behind map/set: 12 bytes (header-node
// proxy over the malloc allocator, node count, empty comparator). The
// construction and selected ownership paths are modelled: the base allocates the header node in
// its ctor and _M_empty_initialize links it to itself (CDefinitionManager
// 0x009B0470 inlines this for RecompiledDefs: malloc(0x18), colour byte 0,
// parent 0, left/right = header).

#include "fable_stlport_vector.h"
#include "fable_string.h"

namespace _STL
{
    template <class _T1, class _T2>
    struct pair
    {
        typedef _T1 first_type;
        typedef _T2 second_type;

        pair()
            : first(), second()
        {
        }

        pair(const _T1& __a, const _T2& __b)
            : first(__a), second(__b)
        {
        }

        // STLport's user-declared copy (a FABLE_UNIT_INLINE unit expands it
        // into _M_create_node 0x00431773; /Os units call it, 0x004297DB).
        FABLE_UNIT_INLINE pair(const pair& __o)
            : first(__o.first), second(__o.second)
        {
        }

        template<class _U1, class _U2>
        FABLE_UNIT_INLINE pair(const pair<_U1, _U2>& __o) : first(__o.first), second(__o.second) {}

        _T1 first;
        _T2 second;
    };

    template<class _T1,class _T2>
    inline pair<_T1,_T2> make_pair(const _T1& __a,const _T2& __b) { return pair<_T1,_T2>(__a,__b); }

    template <class _Tp>
    struct less
    {
        FABLE_UNIT_INLINE bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
    };

    typedef bool _Rb_tree_Color_type;
    const _Rb_tree_Color_type _S_rb_tree_red = false;
    const _Rb_tree_Color_type _S_rb_tree_black = true;

    struct _Rb_tree_node_base
    {
        typedef _Rb_tree_node_base* _Base_ptr;

        _Rb_tree_Color_type _M_color;
        _Base_ptr _M_parent;
        _Base_ptr _M_left;
        _Base_ptr _M_right;
        static _Base_ptr _S_minimum(_Base_ptr node);
        static _Base_ptr _S_maximum(_Base_ptr node);
    };

    template <class _Value>
    struct _Rb_tree_node : public _Rb_tree_node_base
    {
        _Value _M_value_field;
    };

    template <class _Tp, class _Alloc>
    class _Rb_tree_base
    {
    public:
        typedef _Rb_tree_node<_Tp> _Node;

        _Rb_tree_base(const _Alloc&)
            : _M_header(allocator<_Node>(), (_Node*)0)
        {
            _M_header._M_data = _M_header.allocate(1);
        }

        _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    };

    // STLport 4.6's in-order steps: static cdecl members of _Rb_global<bool>,
    // bodies below (0x004294E2 is _M_decrement, out of line from a /Os unit:
    // map<CCharString, long>::insert_unique 0x00431606).
    template <class _Dummy>
    class _Rb_global
    {
    public:
        static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node);
        static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* _M_node);
        static void __cdecl _Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);
        static void __cdecl _Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);
        // Recolours/rotates after a link (0x0042971D, from _M_insert 0x004316CF).
        static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);
        static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base*, _Rb_tree_node_base*&, _Rb_tree_node_base*&, _Rb_tree_node_base*&);
    };
    typedef _Rb_global<bool> _Rb_global_inst;

    // STLport 4.6 _Rb_tree_base_iterator: the node, stepped through
    // _Rb_global. /O2 units keep the steps out of line with _Rb_global's body
    // expanded (0x0050DEA0 / 0x0050DEE0, thiscall on the iterator; the hinted
    // insert_unique 0x00516AF0 calls both).
    struct _Rb_tree_base_iterator
    {
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
        void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }

        _Rb_tree_node_base* _M_node;
    };

    // One node pointer (STLport _Rb_tree_iterator over _Rb_tree_base_iterator).
    // It has constructors, so a function returning one uses a hidden result
    // slot (map<CCharString, long>'s insert_unique 0x00516AF0); the tree's
    // lookups return the bare node pointer instead (_M_find, _M_lower_bound).
    template <class _Value> struct _Rb_tree_const_iterator;
    template <class _Value>
    struct _Rb_tree_iterator : public _Rb_tree_base_iterator
    {
        typedef _Value value_type;
        typedef _Value* pointer;
        typedef _Value& reference;
        _Rb_tree_iterator() {}
        _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
        // STLport assigns in the body; being user-declared, the copy is built in
        // place when an iterator is passed by value (map operator[] 0x00519230).
        _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }

        _Value& operator*() const { return static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
        _Value* operator->() const { return &static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
        bool operator==(const _Rb_tree_iterator& __x) const { return _M_node == __x._M_node; }
        bool operator!=(const _Rb_tree_iterator& __x) const { return _M_node != __x._M_node; }
        bool operator==(const _Rb_tree_const_iterator<_Value>& __x) const;
        bool operator!=(const _Rb_tree_const_iterator<_Value>& __x) const;
        _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
        _Rb_tree_iterator operator++(int) { _Rb_tree_iterator old = *this; ++*this; return old; }
        _Rb_tree_iterator& operator--() { _M_decrement(); return *this; }
    };

    // Const view in this reduced model: same node pointer/hidden-return ABI,
    // const pair access, and unchanged existing nonconst type/mangling.
    template <class _Value>
    struct _Rb_tree_const_iterator : public _Rb_tree_base_iterator
    {
        typedef _Value value_type;
        typedef const _Value* pointer;
        typedef const _Value& reference;
        _Rb_tree_const_iterator() {}
        _Rb_tree_const_iterator(_Rb_tree_node_base* node) { _M_node=node; }
        _Rb_tree_const_iterator(const _Rb_tree_const_iterator& other) { _M_node=other._M_node; }
        _Rb_tree_const_iterator(const _Rb_tree_iterator<_Value>& other) { _M_node=other._M_node; }
        reference operator*() const { return static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
        pointer operator->() const { return &static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
        bool operator==(const _Rb_tree_const_iterator& other) const { return _M_node==other._M_node; }
        bool operator!=(const _Rb_tree_const_iterator& other) const { return _M_node!=other._M_node; }
        _Rb_tree_const_iterator& operator++() { _M_increment();return *this; }
        _Rb_tree_const_iterator operator++(int) { _Rb_tree_const_iterator old=*this;++*this;return old; }
        _Rb_tree_const_iterator& operator--() { _M_decrement();return *this; }
    };
    template<class V> inline bool _Rb_tree_iterator<V>::operator==(const _Rb_tree_const_iterator<V>& other) const { return _M_node==other._M_node; }
    template<class V> inline bool _Rb_tree_iterator<V>::operator!=(const _Rb_tree_const_iterator<V>& other) const { return _M_node!=other._M_node; }

    template <class _Key, class _Value, class _KeyOfValue, class _Compare,
              class _Alloc = allocator<_Value> >
    class _Rb_tree : public _Rb_tree_base<_Value, _Alloc>
    {
    public:
        typedef _Rb_tree_node_base* _Base_ptr;
        typedef _Rb_tree_iterator<_Value> iterator;
        typedef _Rb_tree_const_iterator<_Value> const_iterator;
        typedef size_t size_type;
        size_type erase(const _Key& key);
        typedef _Rb_tree_node<_Value>* _Link_type;
        _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
        static _Link_type& __cdecl _S_left(_Link_type node) { return (_Link_type&)node->_M_left; }
        static _Link_type& __cdecl _S_right(_Link_type node) { return (_Link_type&)node->_M_right; }
        static _Value& __cdecl _S_value(_Link_type node) { return node->_M_value_field; }
        static const _Key& __cdecl _S_key(_Link_type node) { return _KeyOfValue()(_S_value(node)); }
        _Link_type _M_upper_bound(const _Key& key) const;
        pair<iterator,iterator> equal_range(const _Key& key);
        iterator upper_bound(const _Key& key) { return iterator(_M_upper_bound(key)); }

        // Out of line in retail (set<CCharString>: 0x004AC380, called by
        // CSymbolMap::AddSymbolDirty 0x009E5420 with a hidden result slot).
        iterator insert_equal(const _Value& __v);
        pair<iterator, bool> insert_unique(const _Value& __v);

        // STLport 4.6's node lookups, out of line in retail and returning the
        // node in eax (map<CCharString, long>: _M_find 0x0042B467, called by
        // CDefStringTable::AddString 0x009D4F90; _M_lower_bound 0x0047547D and
        // the hinted insert_unique 0x00516AF0, called by operator[] 0x00519230).
        _Base_ptr _M_find(const _Key& __k);
        _Base_ptr _M_find(const _Key& __k) const;
        _Link_type _M_lower_bound(const _Key& __k) const;
        iterator insert_unique(iterator __position, const _Value& __v);
        // Links a new node for __v under __y (left when __x or __w is set, or the
        // key sorts first); out of line (map<CCharString, long>: 0x004316CF).
        iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const _Value& __v, _Base_ptr __w = 0);

        // Allocates a node and copy-constructs __x into it (map<CCharString,
        // long>: 0x00431773, out of line).
        _Rb_tree_node<_Value>* _M_create_node(const _Value& __x);

        void _M_erase(_Rb_tree_node<_Value>* __x);
        void erase(iterator position);
        void erase(iterator first, iterator last);
        void clear() {
            if (_M_node_count != 0) {
                _M_erase(static_cast<_Rb_tree_node<_Value>*>(this->_M_header._M_data->_M_parent));
                _M_leftmost() = this->_M_header._M_data;
                this->_M_header._M_data->_M_parent = 0;
                _M_rightmost() = this->_M_header._M_data;
                _M_node_count = 0;
            }
        }
        size_t size() const { return _M_node_count; }
        _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
        _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
        static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(static_cast<_Rb_tree_node<_Value>*>(__x)->_M_value_field); }

        iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
        const_iterator find(const _Key& __k) const { return const_iterator(_M_find(__k)); }
        iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
        iterator begin() { return iterator(_M_leftmost()); }
        iterator end() { return iterator(this->_M_header._M_data); }
        const_iterator begin() const { return const_iterator(_M_leftmost()); }
        const_iterator end() const { return const_iterator(this->_M_header._M_data); }
        _Compare key_comp() const { return _M_key_compare; }

        _Rb_tree(const _Compare& __comp = _Compare(), const _Alloc& __a = _Alloc())
            : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
        {
            _M_empty_initialize();
        }

        size_t _M_node_count;
        _Compare _M_key_compare;

    private:
        void _M_empty_initialize()
        {
            // Colour marks the header (red) apart from the root (black).
            this->_M_header._M_data->_M_color = _S_rb_tree_red;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
        }
    };

    template <class _Pair>
    struct _Select1st
    {
        const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
    };

    template <class _Tp>
    struct _Identity
    {
        const _Tp& operator()(const _Tp& value) const { return value; }
    };

    template <class _Key, class _Tp, class _Compare = less<_Key>,
              class _Alloc = allocator<pair<const _Key, _Tp> > >
    class map
    {
    public:
        typedef pair<const _Key, _Tp> value_type;
        typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
        typedef _Rb_tree_iterator<value_type> iterator;
        typedef _Rb_tree_const_iterator<value_type> const_iterator;

        map()
            : _M_t(_Compare(), _Alloc())
        {
        }

        ~map() {}

        void clear() { _M_t.clear(); }
        void erase(iterator position) { _M_t.erase(position); }
        typename _Rep_type::size_type erase(const _Key& __x) { return _M_t.erase(__x); }
        iterator find(const _Key& __k) { return _M_t.find(__k); }
        const_iterator find(const _Key& __k) const { return _M_t.find(__k); }
        iterator lower_bound(const _Key& __k) { return _M_t.lower_bound(__k); }
        iterator begin() { return _M_t.begin(); }
        iterator end() { return _M_t.end(); }
        const_iterator begin() const { return _M_t.begin(); }
        const_iterator end() const { return _M_t.end(); }
        iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
        pair<iterator,bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
        _Compare key_comp() const { return _M_t.key_comp(); }

        // STLport's in-class inline operator[], which VC7.1 keeps out of line
        // (map<CCharString, long>: 0x00519230, from CDefStringTable::AddString
        // 0x009D4F90). The body is in fable_stlport_tree_impl.h.
        _Tp& operator[](const _Key& __k);

        _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
    };

    template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
    class set
    {
    public:
        typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
        typedef typename _Rep_type::iterator iterator;

        set()
            : _M_t(_Compare(), _Alloc())
        {
        }

        ~set() {}

        pair<iterator, bool> insert(const _Key& __x)
        {
            return _M_t.insert_unique(__x);
        }
        size_t size() const { return _M_t.size(); }
        iterator end() { return _M_t.end(); }
        iterator upper_bound(const _Key& __x) { return _M_t.upper_bound(__x); }

        _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
    };
}

namespace _STL
{
    template <class _Dummy>
    _Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_color == _S_rb_tree_red && _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;   // The header: end() - 1 is the rightmost.
        else if (_M_node->_M_left != 0)
        {
            _Rb_tree_node_base* __y = _M_node->_M_left;
            while (__y->_M_right != 0)
                __y = __y->_M_right;
            _M_node = __y;
        }
        else
        {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left)
            {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
        return _M_node;
    }

    template <class _Dummy>
    void __cdecl _Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
    {
        _Rb_tree_node_base* __y = __x->_M_right;
        __x->_M_right = __y->_M_left;
        if (__y->_M_left != 0)
            __y->_M_left->_M_parent = __x;
        __y->_M_parent = __x->_M_parent;

        if (__x == __root)
            __root = __y;
        else if (__x == __x->_M_parent->_M_left)
            __x->_M_parent->_M_left = __y;
        else
            __x->_M_parent->_M_right = __y;
        __y->_M_left = __x;
        __x->_M_parent = __y;
    }

    template <class _Dummy>
    void __cdecl _Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
    {
        _Rb_tree_node_base* __y = __x->_M_left;
        __x->_M_left = __y->_M_right;
        if (__y->_M_right != 0)
            __y->_M_right->_M_parent = __x;
        __y->_M_parent = __x->_M_parent;

        if (__x == __root)
            __root = __y;
        else if (__x == __x->_M_parent->_M_right)
            __x->_M_parent->_M_right = __y;
        else
            __x->_M_parent->_M_left = __y;
        __y->_M_right = __x;
        __x->_M_parent = __y;
    }

    // The new red node __x may have a red parent: recolour up the tree while
    // the uncle is red, then rotate once or twice; the root ends up black.
    template <class _Dummy>
    void __cdecl _Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
    {
        __x->_M_color = _S_rb_tree_red;
        while (__x != __root && __x->_M_parent->_M_color == _S_rb_tree_red)
        {
            if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left)
            {
                _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
                if (__y && __y->_M_color == _S_rb_tree_red)
                {
                    __x->_M_parent->_M_color = _S_rb_tree_black;
                    __y->_M_color = _S_rb_tree_black;
                    __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                    __x = __x->_M_parent->_M_parent;
                }
                else
                {
                    if (__x == __x->_M_parent->_M_right)
                    {
                        __x = __x->_M_parent;
                        _Rotate_left(__x, __root);
                    }
                    __x->_M_parent->_M_color = _S_rb_tree_black;
                    __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                    _Rotate_right(__x->_M_parent->_M_parent, __root);
                }
            }
            else
            {
                _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
                if (__y && __y->_M_color == _S_rb_tree_red)
                {
                    __x->_M_parent->_M_color = _S_rb_tree_black;
                    __y->_M_color = _S_rb_tree_black;
                    __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                    __x = __x->_M_parent->_M_parent;
                }
                else
                {
                    if (__x == __x->_M_parent->_M_left)
                    {
                        __x = __x->_M_parent;
                        _Rotate_right(__x, __root);
                    }
                    __x->_M_parent->_M_color = _S_rb_tree_black;
                    __x->_M_parent->_M_parent->_M_color = _S_rb_tree_red;
                    _Rotate_left(__x->_M_parent->_M_parent, __root);
                }
            }
        }
        __root->_M_color = _S_rb_tree_black;
    }

    template <class _Dummy>
    _Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0)
        {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else
        {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right)
            {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)   // The root has the header as parent.
                _M_node = __y;
        }
        return _M_node;
    }
}

FABLE_STATIC_ASSERT(sizeof(_STL::map<int, int>) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(_STL::_Rb_tree_node_base) == 0x10);

// Retail CDefStringTable and CProfileManager use this owning specialization.
// Other tree families still require their separately recovered lifetime model.
namespace _STL {
template<> inline map<CCharString,long>::~map()
{
    _M_t.clear();
    if (_M_t._M_header._M_data != 0)
        _M_t._M_header.deallocate(_M_t._M_header._M_data, 1);
}
}
