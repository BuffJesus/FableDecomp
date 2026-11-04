// Behaviour of CBasicString<char>::ExtendData(unsigned long) 0x009A01C0: when the
// aligned size for length+1 exceeds the buffer, a new buffer is allocated
// (aligned to 4, or rounded up to a power of two when UseFastExtend is set),
// zeroed after the terminator to the next 4-byte boundary, the text copied and
// terminated, the old buffer freed; UseFastExtend survives the size update.
#include "fable_basic_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int g_lastNew;
static int g_deletes;
void* operator new[](unsigned int size) { g_lastNew = size; void* p = malloc(size + 8); memset(p, 0xCC, size + 8); return p; }
void operator delete[](void* p) { ++g_deletes; free(p); }

struct Probe : CBasicString<char>
{
    void Grow(unsigned long length) { ExtendData(length); }
};

static char* Text(const char* text, unsigned long size)
{
    char* p = new char[size];
    memset(p, 0, size);
    strcpy(p, text);
    return p;
}

int main()
{
    Probe s;
    // Plain extend: length 5 -> Align(6) = 8 bytes; "abcde", pad 6..7 zeroed.
    s.PData = Text("abcde", 8); s.StringLength = 5; s.DataLength = 4; s.UseFastExtend = false;
    g_deletes = 0; g_lastNew = 0;
    char* old = s.PData;
    s.Grow(5);
    if (s.PData == old || g_lastNew != 8 || s.DataLength != 8 || g_deletes != 1) { printf("plain %p %p %u %u %d\n", s.PData, old, g_lastNew, (unsigned)s.DataLength, g_deletes); return 1; }
    if (memcmp(s.PData, "abcde", 6) != 0 || s.PData[6] != 0 || s.PData[7] != 0) { puts("text"); return 1; }
    if (s.UseFastExtend || s.StringLength != 5) { puts("fields"); return 1; }

    // No growth when the aligned size fits.
    old = s.PData; g_lastNew = 0;
    s.Grow(7);
    if (s.PData != old || g_lastNew != 0 || s.DataLength != 8) { puts("fits"); return 1; }

    // Fast extend: length 9 -> Align(10) = 12 -> 16; length 12 -> 16; 100 -> 128.
    s.UseFastExtend = true;
    s.Grow(9);
    if (g_lastNew != 16 || s.DataLength != 16 || !s.UseFastExtend) { puts("fast16"); return 1; }
    if (memcmp(s.PData, "abcde", 6) != 0 || s.PData[6] != 0 || s.PData[7] != 0 || (unsigned char)s.PData[8] != 0xCC) { puts("fasttext"); return 1; }
    s.DataLength = 8;
    s.Grow(12);
    if (g_lastNew != 16 || s.DataLength != 16) { puts("exact16"); return 1; }
    s.Grow(100);
    if (g_lastNew != 128 || s.DataLength != 128 || !s.UseFastExtend) { puts("fast128"); return 1; }
    if (memcmp(s.PData, "abcde", 6) != 0) { puts("keep"); return 1; }

    // Null buffer: nothing to free.
    delete[] s.PData;
    s.PData = 0; s.StringLength = 0; s.DataLength = 0; s.UseFastExtend = false; g_deletes = 0;
    s.Grow(0);
    if (g_lastNew != 4 || s.DataLength != 4 || g_deletes != 0 || s.PData[0] != 0 || s.PData[1] != 0 || s.PData[3] != 0) { puts("null"); return 1; }
    puts("FABLETLC_CBASICSTRING_EXTENDDATA_009A01C0_PASS");
    return 0;
}