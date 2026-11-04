#include "fable_symbol_map.h"
#include "fable_stlport_pair_swap.h"
namespace _STL {
template<class I,class T,class C>
I FABLE_FASTCALL __unguarded_partition(I first,I last,T pivot,C comp) {
 for(;;) {
  while(comp(*first,pivot))++first;
  --last;
  while(comp(pivot,*last))--last;
  if(!(first<last))return first;
  _STL::swap(*first,*last);
  ++first;
 }
}
}
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
template Entry* FABLE_FASTCALL _STL::__unguarded_partition(Entry*,Entry*,Entry,CCompareString<CSymbolMap::CSymbolInfo>);
