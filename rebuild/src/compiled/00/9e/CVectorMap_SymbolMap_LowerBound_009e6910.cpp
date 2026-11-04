#include "fable_symbol_map.h"
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
typedef CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> > Map;
template<> const Entry* Map::LowerBound(const CCharString& key) const {
 const Entry* first=begin();long count=end()-first;
 while(count>0){long step=count/2;const Entry* middle=first+step;
  if(middle->first.FastCompare(key)){first=middle+1;count-=step+1;}
  else count=step;
 }
 return first;
}
template const Entry* Map::LowerBound(const CCharString&) const;
