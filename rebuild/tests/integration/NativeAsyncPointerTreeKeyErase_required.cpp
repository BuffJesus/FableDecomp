#include "fable_game_text_bank.h"
typedef _STL::_Rb_tree<CBankFileAsyncData*,CBankFileAsyncData*,
    _STL::_Identity<CBankFileAsyncData*>,_STL::less<CBankFileAsyncData*>,
    _STL::allocator<CBankFileAsyncData*> > AsyncTree;
size_t RequireAsyncKeyErase(AsyncTree* tree, CBankFileAsyncData* const& key)
{ return tree->erase(key); }
_STL::pair<AsyncTree::iterator,AsyncTree::iterator>
RequireAsyncEqualRange(AsyncTree* tree, CBankFileAsyncData* const& key)
{ return tree->equal_range(key); }
int main() { return 0; }
