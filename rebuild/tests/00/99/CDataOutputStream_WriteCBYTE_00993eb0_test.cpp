// Behaviour of CDataOutputStream::WriteSLONG 0x00993EB0: one Write call (vtable slot 4)
// on the same stream with the value's four bytes.
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

struct Fake { void** vptr; };
static void* g_vtable[8];
static const Fake* g_this;
static unsigned char g_bytes[8];
static long g_size;
static int g_calls;
static void FABLE_FASTCALL FakeWrite(Fake* self, void*, const void* data, long size)
{
    g_this = self; g_size = size; ++g_calls;
    memcpy(g_bytes, data, size > 8 ? 8 : size);
}

int main()
{
    Fake f = { g_vtable };
    g_vtable[4] = (void*)&FakeWrite;
    CDataOutputStream* s = reinterpret_cast<CDataOutputStream*>(&f);
    const long values[3] = { (long)0x12345678, (long)-1, 0 };
    for (int i = 0; i < 3; ++i)
    {
        g_calls = 0;
        s->WriteSLONG(values[i]);
        if (g_calls != 1 || g_this != &f || g_size != 4 || memcmp(g_bytes, &values[i], 4) != 0) { printf("value %d\n", i); return 1; }
    }
    puts("FABLETLC_CDATAOUTPUTSTREAM_WRITESLONG_00993EB0_PASS");
    return 0;
}
