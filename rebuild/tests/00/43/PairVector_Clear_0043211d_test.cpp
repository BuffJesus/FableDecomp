#include "fable_def_class_base.h"
#include <stdio.h>
#include <string.h>

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> Pair;
typedef _STL::vector<Pair> PairVector;
template <> void PairVector::_M_clear();

static int g_freeCalls;
static void* g_freed;
extern "C" void __cdecl free(void* p)
{
    ++g_freeCalls;
    g_freed = p;
}

int main()
{
    unsigned char elements[16 * sizeof(Pair)];
    unsigned char bytes[sizeof(PairVector)];
    PairVector* vector = reinterpret_cast<PairVector*>(bytes);
    for (int i = 0; i < 16; ++i) {
        memset(bytes, 0, sizeof(bytes));
        Pair* start = (i & 1) ? reinterpret_cast<Pair*>(elements + i * sizeof(Pair)) : 0;
        vector->_M_start = start;
        vector->_M_finish = start;
        vector->_M_end_of_storage._M_data = start;
        g_freeCalls = 0;
        g_freed = 0;
        vector->PairVector::_M_clear();
        if (g_freeCalls != ((i & 1) != 0)) return 1;
        if ((i & 1) && g_freed != start) return 2;
        if (vector->_M_start != start || vector->_M_finish != start ||
            vector->_M_end_of_storage._M_data != start) return 3;
    }
    puts("PAIR_VECTOR_CLEAR_PASS cases=16");
    return 0;
}
