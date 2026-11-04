#include <stdlib.h>
#include "fable_game_text_bank.h"
extern "C" void UnusedRegistrationInsertBoundary() { abort(); }
int main(int argc,char**argv)
{
    CBankFileAsyncData* value=reinterpret_cast<CBankFileAsyncData*>(argc);
    _STL::set<CBankFileAsyncData*>::_Rep_type* tree=
        reinterpret_cast<_STL::set<CBankFileAsyncData*>::_Rep_type*>(argv);
    return tree->insert_unique(value).second;
}
#pragma comment(linker,"/alternatename:?_M_insert@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@PAU_Rb_tree_node_base@2@0ABQAVCBankFileAsyncData@@0@Z=_UnusedRegistrationInsertBoundary")
