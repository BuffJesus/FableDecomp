// Behaviour of vector<char>::resize(n, x) 0x004122D0: a smaller n moves the
// end back to begin + n (storage kept); an equal or larger n inserts n - size
// copies of x at the end through _M_fill_insert.
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <string.h>

typedef _STL::vector<char> V;
static char* g_pos; static size_t g_n; static const char* g_x; static int g_calls;
template<> void V::_M_fill_insert(char* pos, size_t n, const char& x) { ++g_calls; g_pos = pos; g_n = n; g_x = &x; }

int main()
{
    static unsigned char store[sizeof(V)];
    V* v = reinterpret_cast<V*>(store);
    static char bytes[16] = "abcdefghij";
    v->_M_start = bytes; v->_M_finish = bytes + 10; v->_M_end_of_storage._M_data = bytes + 16;
    char x = 'z';
    v->resize(4, x);
    if (v->_M_finish != bytes + 4 || v->_M_start != bytes || v->_M_end_of_storage._M_data != bytes + 16 ||
        g_calls != 0 || memcmp(bytes, "abcdefghij", 10) != 0) { puts("shrink"); return 1; }
    v->resize(9, x);
    if (g_calls != 1 || g_pos != bytes + 4 || g_n != 5 || g_x != &x) { puts("grow"); return 1; }
    v->resize(4, x);
    if (g_calls != 2 || g_n != 0) { puts("same size"); return 1; }
    puts("FABLETLC_STLVECTOR_CHAR_RESIZE_004122D0_PASS");
    return 0;
}