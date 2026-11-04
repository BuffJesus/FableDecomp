#pragma once
#include "fable_stlport_tree.h"
namespace _STL {
// Pair overload retains retail's fastcall swap boundary for nontrivial entries.
template<class K,class V> void FABLE_FASTCALL swap(pair<K,V>& left,pair<K,V>& right);
}
