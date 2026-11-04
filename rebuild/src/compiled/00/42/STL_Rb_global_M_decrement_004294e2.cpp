// _STL::_Rb_global<bool>::_M_decrement(_Rb_tree_node_base*) @ 0x004294E2 (STLport
// 4.6, cdecl static): the in-order predecessor; the header (red, its parent's
// parent) steps to the rightmost node. Retail's unit is size-optimised (/Os),
// with _Rotate_left 0x0042951B, _Rotate_right 0x0042955B and _Rebalance 0x0042971D.
#pragma optimize("s", on)
#include "fable_stlport_tree.h"

template _STL::_Rb_tree_node_base* __cdecl _STL::_Rb_global<bool>::_M_decrement(_STL::_Rb_tree_node_base* _M_node);