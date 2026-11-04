#include <new>
#include "fable_string.h"
#include "fable_small_vector.h"
template<> void CSmallVector<CCharString, 8>::Clear()
{
    delete[] Data;
    Data = 0;
    Size = 0;
}
