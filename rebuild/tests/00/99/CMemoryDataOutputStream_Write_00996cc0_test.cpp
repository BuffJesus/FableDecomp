// Behaviour of CMemoryDataOutputStream::Write 0x00996CC0 (and PeekData
// 0x00995E70, inlined into it): PeekData is the array object itself while
// Data is empty and Data's first byte otherwise; Write ignores sizes <= 0 and
// writes that would pass 2 GB, grows Data (through resize) only when the
// bytes run past its end, copies them in at the position and advances it.
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

static char g_buffer[64];
static int g_resizeCalls;
static size_t g_resizeTo;

namespace _STL
{
    // Stub: grow into g_buffer, zero filling like STLport's resize(n, char()).
    template <>
    void vector<char>::resize(size_t n, const char& fill)
    {
        ++g_resizeCalls; g_resizeTo = n;
        if (this->_M_start == 0) this->_M_start = g_buffer;
        for (char* p = this->_M_finish ? this->_M_finish : this->_M_start; p < this->_M_start + n; ++p) *p = fill;
        this->_M_finish = this->_M_start + n;
        this->_M_end_of_storage._M_data = g_buffer + sizeof(g_buffer);
    }
}

int main()
{
    static unsigned char store[sizeof(CMemoryDataOutputStream)];
    memset(store, 0, sizeof(store));
    CMemoryDataOutputStream* s = reinterpret_cast<CMemoryDataOutputStream*>(store);
    memset(g_buffer, 0x55, sizeof(g_buffer));

    if (s->PeekData() != (const void*)&s->Data) { puts("peek empty"); return 1; }

    s->CMemoryDataOutputStream::Write("xyz", 0);
    s->CMemoryDataOutputStream::Write("xyz", -3);
    if (g_resizeCalls != 0 || s->CurrentPosition != 0) { puts("non-positive size"); return 1; }

    s->CMemoryDataOutputStream::Write("abcd", 4);
    if (g_resizeCalls != 1 || g_resizeTo != 4 || s->CurrentPosition != 4 || memcmp(g_buffer, "abcd", 4) != 0)
    { puts("first write"); return 1; }
    if (s->PeekData() != (const void*)g_buffer) { puts("peek data"); return 1; }

    // Overwrite inside the buffer, ending exactly at its end: no resize.
    s->CurrentPosition = 1;
    s->CMemoryDataOutputStream::Write("XYZ", 3);
    if (g_resizeCalls != 1 || s->CurrentPosition != 4 || memcmp(g_buffer, "aXYZ", 4) != 0)
    { puts("in-place write"); return 1; }

    // Past the end: grows to position + size.
    s->CurrentPosition = 6;
    s->CMemoryDataOutputStream::Write("pq", 2);
    if (g_resizeCalls != 2 || g_resizeTo != 8 || s->CurrentPosition != 8 || memcmp(g_buffer, "aXYZ\0\0pq", 8) != 0)
    { puts("grow write"); return 1; }

    // A write that would pass 2 GB is dropped.
    s->CurrentPosition = 0x7FFFFFF0;
    s->CMemoryDataOutputStream::Write("0123456789abcdef0123", 20);
    if (g_resizeCalls != 2 || s->CurrentPosition != 0x7FFFFFF0) { puts("2 GB limit"); return 1; }

    puts("FABLETLC_CMEMORYDATAOUTPUTSTREAM_WRITE_00996CC0_PASS");
    return 0;
}