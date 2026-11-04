#include "fable_symbol_map.h"
#include "fable_stlport_pair_swap.h"
namespace _STL {
template<class K,class V> void FABLE_FASTCALL swap(pair<K,V>& left,pair<K,V>& right) {
 pair<K,V> temporary=left;left=right;right=temporary;
}
}
template void FABLE_FASTCALL _STL::swap(_STL::pair<CCharString,CSymbolMap::CSymbolInfo>&,_STL::pair<CCharString,CSymbolMap::CSymbolInfo>&);
