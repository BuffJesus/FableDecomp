#include "fable_string.h"
#include "fable_stlport_tree.h"
namespace _STL {
template<class K,class V,class KeyOfValue,class Compare,class Alloc>
typename _Rb_tree<K,V,KeyOfValue,Compare,Alloc>::iterator _Rb_tree<K,V,KeyOfValue,Compare,Alloc>::insert_equal(const V& value) {
_Link_type y=(_Link_type)this->_M_header._M_data;
_Link_type x=_M_root();
while(x!=0){y=x;x=this->_M_key_compare(KeyOfValue()(value),_S_key(x))?_S_left(x):_S_right(x);}
return _M_insert(x,y,value);
}
template _Rb_tree<CCharString,pair<const CCharString,unsigned long>,_Select1st<pair<const CCharString,unsigned long> >,less<CCharString>,allocator<pair<const CCharString,unsigned long> > >::iterator _Rb_tree<CCharString,pair<const CCharString,unsigned long>,_Select1st<pair<const CCharString,unsigned long> >,less<CCharString>,allocator<pair<const CCharString,unsigned long> > >::insert_equal(const pair<const CCharString,unsigned long>&);
}
