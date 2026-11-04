// Behaviour of vector<char>::_M_fill_insert(pos, n, x) 0x004110D0: inserts n
// copies of x before pos, keeping the order of the other bytes; in place when
// the spare capacity holds n (tail longer or not longer than n), otherwise
// through the growth path (new storage of size + max(size, n)); n == 0 does
// nothing.
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef _STL::vector<char> V;

static int Check(const V& v, const char* want, size_t cap)
{
    size_t len = strlen(want);
    if ((size_t)(v._M_finish - v._M_start) != len || memcmp(v._M_start, want, len) != 0 ||
        (size_t)(v._M_end_of_storage._M_data - v._M_start) != cap)
    {
        printf("got '%.*s' cap %u want '%s' cap %u\n", (int)(v._M_finish - v._M_start), v._M_start,
               (unsigned)(v._M_end_of_storage._M_data - v._M_start), want, (unsigned)cap);
        return 1;
    }
    return 0;
}

static void Set(V& v, const char* text, size_t cap)
{
    v._M_start = (char*)malloc(cap);
    memcpy(v._M_start, text, strlen(text));
    v._M_finish = v._M_start + strlen(text);
    v._M_end_of_storage._M_data = v._M_start + cap;
}

int main()
{
    static unsigned char store[sizeof(V)];
    V& v = *reinterpret_cast<V*>(store);
    char x = '*';

    Set(v, "abcdef", 16);
    char* start = v._M_start;
    v._M_fill_insert(v._M_start + 2, 0, x);
    if (Check(v, "abcdef", 16)) return 1;
    v._M_fill_insert(v._M_start + 2, 3, x);            // tail of 4 > 3
    if (Check(v, "ab***cdef", 16) || v._M_start != start) return 1;
    v._M_fill_insert(v._M_start + 7, 4, x);            // tail of 2 <= 4
    if (Check(v, "ab***cd****ef", 16) || v._M_start != start) return 1;
    v._M_fill_insert(v._M_finish, 3, x);               // at the end, exactly fills
    if (Check(v, "ab***cd****ef***", 16) || v._M_start != start) return 1;
    x = '#';
    v._M_fill_insert(v._M_start + 1, 2, x);            // no room: grows to 16 + 16
    if (Check(v, "a##b***cd****ef***", 32) || v._M_start == start) return 1;
    puts("FABLETLC_STLVECTOR_CHAR_FILLINSERT_004110D0_PASS");
    return 0;
}