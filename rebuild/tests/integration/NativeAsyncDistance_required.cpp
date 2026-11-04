#include "fable_game_text_bank.h"
#include "fable_stlport_iterator_distance.h"
typedef _STL::_Rb_tree_iterator<CBankFileAsyncData*> AsyncDistanceIterator;
ptrdiff_t RequireAsyncDistance(const AsyncDistanceIterator& first,
                              const AsyncDistanceIterator& last,
                              const _STL::input_iterator_tag& category)
{
    return _STL::__distance(first, last, category);
}
int main() { return 0; } // Link-only fixture; never execute.

ptrdiff_t RequireAsyncCategoryDispatch(const AsyncDistanceIterator& first,
                                     const AsyncDistanceIterator& last)
{
    return _STL::distance(first, last);
}

typedef char AsyncIteratorLayoutUnchanged[sizeof(AsyncDistanceIterator) == 4 ? 1 : -1];
