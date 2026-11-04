// _STL::_Rb_tree<unsigned long, pair<const unsigned long, CCodeSectionManager::CSectionDesc>, ...>::clear
// @ 0x009F0F30 (STLport 4.6 in-class clear; out of line in the retail CodeSectionManager unit,
// referenced by the tree's destructor; _M_erase is the recursive node erase at 0x009F0D40).
#include "engine/CCodeSectionManager.h"
namespace _STL {
template void CCodeSectionTree::clear();
}
