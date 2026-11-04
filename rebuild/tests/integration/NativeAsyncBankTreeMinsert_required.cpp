#include <stdlib.h>
#include <new>
#include "fable_stlport_tree.h"
class CBankFileAsyncData;
typedef _STL::set<CBankFileAsyncData*>::_Rep_type Tree;
extern "C" void UnusedPointerTreeRebalance(){abort();}
int main(int argc,char**argv){
    CBankFileAsyncData* value=(CBankFileAsyncData*)argv;
    ((Tree*)argv)->_M_insert(0,(_STL::_Rb_tree_node_base*)argv,value,
                            argc?0:(_STL::_Rb_tree_node_base*)(argv+1));
    return 0;
}
#pragma comment(linker,"/alternatename:?_Rebalance@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z=_UnusedPointerTreeRebalance")
