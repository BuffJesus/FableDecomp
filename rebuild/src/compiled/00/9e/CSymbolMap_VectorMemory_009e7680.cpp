#include "fable_symbol_map.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> inline pair<CCharString,CSymbolMap::CSymbolInfo>* vector<pair<CCharString,CSymbolMap::CSymbolInfo> >::erase(pair<CCharString,CSymbolMap::CSymbolInfo>* first,pair<CCharString,CSymbolMap::CSymbolInfo>* last){
 pair<CCharString,CSymbolMap::CSymbolInfo>* result=__copy(last,_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
 _Destroy(result,_M_finish);_M_finish=result;return first;
}
}
template<class T> void CArrayVector<T>::DeallocateMemory() {
 if(this->capacity()>0){this->clear();_STL::vector<T> empty;this->swap(empty);}
}
template void CArrayVector<_STL::pair<CCharString,CSymbolMap::CSymbolInfo> >::DeallocateMemory();

template<class K,class V,class C> void CVectorMap<K,V,C>::DeallocateMemory() {
 this->clear();_STL::vector<_STL::pair<K,V> > empty;this->swap(empty);Dirty=false;
}
template void CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >::DeallocateMemory();
