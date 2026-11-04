// _STL::_Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&)
// @ 0x0042971D (STLport 4.6, cdecl static; /Os unit, see 0x004294E2): restores
// the red-black invariants after _M_insert (0x004316CF) links a node; the
// rotations stay out of line (0x0042951B / 0x0042955B).
#pragma optimize("s", on)
#include "fable_stlport_tree.h"

template void __cdecl _STL::_Rb_global<bool>::_Rebalance(_STL::_Rb_tree_node_base* __x,
                                                         _STL::_Rb_tree_node_base*& __root);