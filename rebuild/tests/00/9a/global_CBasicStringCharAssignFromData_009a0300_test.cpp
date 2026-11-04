// Behaviour of CBasicString<char>::SetString(const char*, unsigned long)
// 0x009A0300: an old buffer is freed with delete[]; the new one comes from
// new[] sized (length + 4) & ~3, holds exactly `length` characters (not
// stopping at embedded NULs), a terminator and zeroed slack; StringLength and
// the 31-bit DataLength are set and the UseFastExtend byte and top bit kept.
#include "fable_basic_string.h"
#include <new>
#include <stdio.h>
#include <string.h>

static unsigned char g_heap[2][32];
static int g_next;
static unsigned int g_newSize;
static void* g_deleted;
void* operator new[](unsigned int size)
{
    g_newSize = size;
    memset(g_heap[g_next], 0xEE, sizeof(g_heap[g_next]));
    return g_heap[g_next++];
}
void operator delete[](void* p) { g_deleted = p; }

// Exposes the protected member to the fixture.
struct Probe : CBasicString<char>
{
    void Set(const char* text, unsigned long length) { SetString(text, length); }
};

static unsigned long U32(const unsigned char* p, int o) { return *reinterpret_cast<const unsigned long*>(p + o); }

int main()
{
    unsigned char storage[sizeof(CBasicString<char>)];
    memset(storage, 0, sizeof(storage));
    storage[8 + 3] = 0x80;           // spare top bit of the size dword
    storage[12] = 0x01;              // UseFastExtend
    Probe* s = reinterpret_cast<Probe*>(storage);

    s->Set("abcde", 5);
    char* first = s->PData;
    if (first != (char*)g_heap[0] || g_newSize != 8 || g_deleted != 0) { puts("alloc"); return 1; }
    if (memcmp(first, "abcde\0\0\0", 8) != 0 || first[8] != (char)0xEE) { puts("text"); return 1; }
    if (U32(storage, 4) != 5 || U32(storage, 8) != (0x80000000 | 8) || storage[12] != 0x01) { puts("fields"); return 1; }

    static const char kEmbedded[] = { 'x', 0, 'y', 'z' };
    s->Set(kEmbedded, 4);
    if (g_deleted != first || s->PData != (char*)g_heap[1] || g_newSize != 8) { puts("realloc"); return 1; }
    if (memcmp(s->PData, "x\0yz\0\0\0\0", 8) != 0 || U32(storage, 4) != 4) { puts("embedded"); return 1; }
    puts("FABLETLC_CBASICSTRING_CHAR_SETSTRING_009A0300_PASS");
    return 0;
}
