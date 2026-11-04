#pragma once
#include "fable_stlport_tree.h"
// Scoped owner for the recovered bank maps; the shared map model currently
// omits destruction for other, independently recovered users.
template<class K, class V> class FableBankOwnedMap : public _STL::map<K, V> {
public:
    ~FableBankOwnedMap() {
        this->clear();
        typename _STL::map<K, V>::_Rep_type::_Base_ptr header = this->_M_t._M_header._M_data;
        if (header) free(header);
    }
};

template<class K,class V> class FableBankOwnedMultiMap : public FableBankOwnedMap<K,V> {
public:
typedef typename _STL::map<K,V>::value_type value_type;
typedef typename _STL::map<K,V>::iterator iterator;
iterator insert(const value_type& value){return this->_M_t.insert_equal(value);}
};
