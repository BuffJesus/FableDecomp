// CVectorMap Sort / Update for the navigation quad tree's 64-bit-keyed maps (retail 0x00A84590 / 0x00A84B10 for
// <unsigned __int64, bool>, 0x00A845E0 / 0x00A84B60 for <unsigned __int64, CANavQuadTreeNode*>; FableWin names). The
// sorts are STLport introsorts over the 16-byte pairs (0x00A84160 / 0x00A841F0).
#include "fable_vector_map.h"

class CANavQuadTreeNode;

template void CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<unsigned __int64, bool> >::Sort();
template void CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<unsigned __int64, bool> >::Update();
template void CVectorMap<unsigned __int64, CANavQuadTreeNode*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> >::Sort();
template void CVectorMap<unsigned __int64, CANavQuadTreeNode*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> >::Update();
