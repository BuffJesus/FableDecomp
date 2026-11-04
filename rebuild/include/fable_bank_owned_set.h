#pragma once
#include "fable_stlport_tree.h"
_STL::_Rb_tree_node_base* __cdecl FableBankEraseTreeNode(_STL::_Rb_tree_node_base*, _STL::_Rb_tree_node_base*&, _STL::_Rb_tree_node_base*&, _STL::_Rb_tree_node_base*&);
template<class K> class FableBankOwnedSet : public _STL::set<K> {
public:
    ~FableBankOwnedSet() {
        this->_M_t.clear();
        typename _STL::set<K>::_Rep_type::_Base_ptr header=this->_M_t._M_header._M_data;
        if(header) free(header);
    }
    bool empty() const { return this->_M_t._M_node_count==0; }
    unsigned long erase(const K& key) { return this->_M_t.erase(key); }
    typename _STL::set<K>::iterator begin() { return typename _STL::set<K>::iterator(this->_M_t._M_header._M_data->_M_left); }
    void erase(typename _STL::set<K>::iterator position) {
        _STL::_Rb_tree_node_base* h=this->_M_t._M_header._M_data;
        _STL::_Rb_tree_node_base* n=FableBankEraseTreeNode(position._M_node,h->_M_parent,h->_M_left,h->_M_right);
        if(n) free(n);
        --this->_M_t._M_node_count;
    }
};
