#include "fable_stlport_tree.h"
// Catalog fixtures are single translation units. Use the same generic tree
// algorithm under a fixture dummy type; the dedicated gate links and checks
// the real bool rebalance provider separately.
template<> void __cdecl _STL::_Rb_global<bool>::_Rebalance(_STL::_Rb_tree_node_base* node,_STL::_Rb_tree_node_base*& root) { _STL::_Rb_global<int>::_Rebalance(node,root); }
#include "../src/compiled/00/4a/STLSetString_InsertNode_004abc70.cpp"
#include "../tests/integration/StringSetInsertion_test.cpp"
