// Behaviour of CBasicString<char>::CBasicString(const char*) 0x009A0590:
// the fields start empty (null buffer, length 0, DataLength 0 keeping the spare
// top bit, UseFastExtend set) and SetString gets the text and its C length.
#include "fable_basic_string.h"
#include <new>
#include <stdio.h>
#include <string.h>

// Recording stand-in (fixture scaffolding) for SetString (retail 0x009A0300).
static int g_calls;
static const void* g_this;
static const char* g_text;
static unsigned long g_length;
static unsigned char g_state[13];
template<> void CBasicString<char>::SetString(const char* text, unsigned long length)
{
    ++g_calls;
    g_this = this;
    g_text = text;
    g_length = length;
    memcpy(g_state, this, sizeof(g_state));
}

static unsigned long U32(const unsigned char* p, int o) { return *reinterpret_cast<const unsigned long*>(p + o); }

int main()
{
    unsigned char storage[sizeof(CBasicString<char>)];
    memset(storage, 0xDA, sizeof(storage));
    static const char kText[] = "frontend";
    new (storage) CBasicString<char>(kText);
    if (g_calls != 1 || g_this != storage || g_text != kText || g_length != 8) { puts("call"); return 1; }
    if (U32(g_state, 0) != 0 || U32(g_state, 4) != 0 || U32(g_state, 8) != 0x80000000 || g_state[12] != 0xDB) { puts("fields"); return 1; }

    new (storage) CBasicString<char>("");
    if (g_calls != 2 || g_length != 0) { puts("empty"); return 1; }
    puts("FABLETLC_CBASICSTRING_CHAR_CTOR_009A0590_PASS");
    return 0;
}
