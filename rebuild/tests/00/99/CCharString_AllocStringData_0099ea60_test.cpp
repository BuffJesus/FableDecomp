// Behaviour of CCharString::AllocStringData(const char*, long) 0x0099EA60,
// reached through the reconstructed CCharString(const char*, long) 0x0099EBF0:
// length -1 builds the data with CBasicString(const char*) (whole C string);
// any other length starts an empty, fast-extending CBasicString (null buffer,
// length 0, size 0 with the spare top bit kept) and SetString(text, length).
// Either way the block holds one reference.
#include "fable_string.h"
#include <new>
#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp"

fable_i32 g_CCharStringInstanceCount_013BD800;
void CCharString::UnassignString() {}

// Recording stand-ins (fixture scaffolding) for the CBasicString<char> bodies
// (retail 0x009A0590 and 0x009A0300).
static int g_textCtor;
static const void* g_textCtorThis;
static const char* g_textCtorText;
static int g_setString;
static const char* g_setText;
static unsigned long g_setLength;
static unsigned char g_stateAtSet[13];
template<> CBasicString<char>::CBasicString(const char* text)
{
    ++g_textCtor;
    g_textCtorThis = this;
    g_textCtorText = text;
    PData = const_cast<char*>(text);
}
template<> void CBasicString<char>::SetString(const char* text, unsigned long length)
{
    ++g_setString;
    g_setText = text;
    g_setLength = length;
    memcpy(g_stateAtSet, this, sizeof(g_stateAtSet));
}

// Allocations come from a 0xDA-filled pool so untouched bytes are visible.
static unsigned char g_pool[2][0x20];
static int g_next;
static unsigned int g_newSize;
void* operator new(unsigned int size)
{
    g_newSize = size;
    memset(g_pool[g_next], 0xDA, sizeof(g_pool[g_next]));
    return g_pool[g_next++];
}

static unsigned long U32(const unsigned char* p, int o) { return *reinterpret_cast<const unsigned long*>(p + o); }

int main()
{
    static const char kText[] = "definition";
    unsigned char a[sizeof(CCharString)];
    new (a) CCharString(kText, -1);
    const unsigned char* data = g_pool[0];
    if (U32(a, 0) != (unsigned long)data || g_newSize != 0x11) { puts("whole alloc"); return 1; }
    if (g_textCtor != 1 || g_textCtorThis != data || g_textCtorText != kText || g_setString != 0) { puts("whole ctor"); return 1; }
    if (U32(data, 0x0D) != 1) { puts("whole refs"); return 1; }

    unsigned char b[sizeof(CCharString)];
    new (b) CCharString(kText, 3);
    data = g_pool[1];
    if (U32(b, 0) != (unsigned long)data || g_textCtor != 1 || g_setString != 1 || g_setText != kText || g_setLength != 3) { puts("part"); return 1; }
    // Before SetString: null buffer, length 0, DataLength 0 with the spare top
    // bit of the pool's 0xDA kept, UseFastExtend (bit 0) set in the pool's 0xDA byte.
    if (U32(g_stateAtSet, 0) != 0 || U32(g_stateAtSet, 4) != 0 || U32(g_stateAtSet, 8) != 0x80000000 ||
        g_stateAtSet[12] != 0xDB) { printf("part state %08lx %02x\n", U32(g_stateAtSet, 8), g_stateAtSet[12]); return 1; }
    if (U32(data, 0x0D) != 1) { puts("part refs"); return 1; }

    puts("FABLETLC_CCHARSTRING_ALLOCSTRINGDATA_0099EA60_PASS");
    return 0;
}
