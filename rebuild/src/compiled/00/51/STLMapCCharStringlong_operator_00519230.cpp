// _STL::map<CCharString, long>::operator[](const CCharString&) @ 0x00519230
// (STLport; CDefStringTable::AddString 0x009D4F90 records each new string's
// index with it; _M_lower_bound 0x0047547D, the hinted insert_unique 0x00516AF0).
#include "fable_stlport_tree_impl.h"
#include "fable_string.h"

template long& _STL::map<CCharString, long>::operator[](const CCharString& __k);