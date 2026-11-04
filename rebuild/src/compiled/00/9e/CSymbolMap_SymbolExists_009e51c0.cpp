#include "fable_symbol_map.h"
bool CSymbolMap::SymbolExists(const CCharString& name)const{return SLONGMap.Find(name)!=SLONGMap.end();}