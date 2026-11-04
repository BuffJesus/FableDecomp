#include "fable_symbol_map.h"
template<> _STL::set<CCharString>::~set() {
 if(_M_t._M_node_count){
  _M_t._M_erase(static_cast<_STL::_Rb_tree_node<CCharString>*>(_M_t._M_header._M_data->_M_parent));
  _M_t._M_header._M_data->_M_left=_M_t._M_header._M_data;
  _M_t._M_header._M_data->_M_parent=0;
  _M_t._M_header._M_data->_M_right=_M_t._M_header._M_data;
  _M_t._M_node_count=0;
 }
 if(_M_t._M_header._M_data)free(_M_t._M_header._M_data);
}
template<> CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >::~CVectorMap() {}

template _STL::set<CCharString>::~set();
template CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >::~CVectorMap();

typedef _STL::set<CCharString>::_Rep_type NamesTree;
template<> void NamesTree::_M_erase(_STL::_Rb_tree_node<CCharString>* node) {
 while(node){
  _M_erase(static_cast<_STL::_Rb_tree_node<CCharString>*>(node->_M_right));
  _STL::_Rb_tree_node<CCharString>* next=static_cast<_STL::_Rb_tree_node<CCharString>*>(node->_M_left);
  node->_M_value_field.~CCharString();free(node);node=next;
 }
}
template void NamesTree::_M_erase(_STL::_Rb_tree_node<CCharString>*);
