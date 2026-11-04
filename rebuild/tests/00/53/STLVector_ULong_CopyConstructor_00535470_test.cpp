// Behaviour of _STL::vector<unsigned long>::vector(const vector&) 0x00535470:
// the copy owns a fresh block of exactly the source's size holding the same
// values (end of storage = end); an empty source gives an empty copy with no
// block.
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

typedef _STL::vector<unsigned long> Vector;

int main()
{
    unsigned long values[5] = { 3, 1, 4, 1, 5 };
    unsigned char srcStore[sizeof(Vector)], dstStore[sizeof(Vector)];
    Vector* src = reinterpret_cast<Vector*>(srcStore);
    src->_M_start = values;
    src->_M_finish = values + 4;
    src->_M_end_of_storage._M_data = values + 5;
    Vector* copy = new (dstStore) Vector(*src);
    if (copy->_M_start == 0 || copy->_M_start == values || copy->_M_finish != copy->_M_start + 4 ||
        copy->_M_end_of_storage._M_data != copy->_M_start + 4 || memcmp(copy->_M_start, values, 4 * sizeof(long)) != 0)
    { puts("copy"); return 1; }
    free(copy->_M_start);

    src->_M_finish = values;
    memset(dstStore, 0xCC, sizeof(dstStore));
    copy = new (dstStore) Vector(*src);
    if (copy->_M_start != 0 || copy->_M_finish != 0 || copy->_M_end_of_storage._M_data != 0) { puts("empty"); return 1; }
    puts("FABLETLC_VECTOR_ULONG_COPY_00535470_PASS");
    return 0;
}