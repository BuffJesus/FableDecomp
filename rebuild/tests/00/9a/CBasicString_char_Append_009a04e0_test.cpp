// Behaviour of CBasicString<char>::Append(const char*, unsigned long) 0x009A04E0:
// the buffer is first extended to the combined length, the text copied after the
// current end and terminated, the length updated, and the buffer zeroed from
// after the terminator up to the next 4-byte boundary (nothing beyond).
#include "fable_basic_string.h"
#include <stdio.h>
#include <string.h>

static int g_extends;
static unsigned long g_extendTo;
template<> void CBasicString<char>::ExtendData(unsigned long length) { ++g_extends; g_extendTo = length; }

int main()
{
    char buffer[32];
    memset(buffer, 0xCC, sizeof(buffer));
    memcpy(buffer, "abc", 4);
    unsigned char store[sizeof(CBasicString<char>)];
    memset(store, 0, sizeof(store));
    CBasicString<char>* s = reinterpret_cast<CBasicString<char>*>(store);
    s->PData = buffer;
    s->StringLength = 3;

    s->Append("defghXYZ", 5);
    if (g_extends != 1 || g_extendTo != 8) { puts("extend"); return 1; }
    if (s->StringLength != 8 || memcmp(buffer, "abcdefgh", 9) != 0) { puts("text"); return 1; }
    if (buffer[9] != 0 || buffer[10] != 0 || buffer[11] != 0 || (unsigned char)buffer[12] != 0xCC) { puts("pad"); return 1; }

    // Ending on a 4-byte boundary (terminator at 7): nothing past it is cleared.
    char second[16];
    memset(second, 0xCC, sizeof(second));
    memcpy(second, "abc", 4);
    s->PData = second;
    s->StringLength = 3;
    s->Append("defg", 4);
    if (s->StringLength != 7 || memcmp(second, "abcdefg", 8) != 0 || (unsigned char)second[8] != 0xCC) { puts("boundary"); return 1; }
    s->PData = buffer;
    s->StringLength = 8;

    s->Append("", 0);
    if (g_extendTo != 8 || s->StringLength != 8 || buffer[8] != 0) { puts("empty"); return 1; }
    puts("FABLETLC_CBASICSTRING_APPEND_009A04E0_PASS");
    return 0;
}
