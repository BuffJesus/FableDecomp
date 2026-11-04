// Behaviour fixture for CIndexBuffer::Uninitialise (provider CIndexBuffer_Uninitialise_00ac1ee0.cpp). The destructor is
// not linked, so the buffer is raw storage laid out like retail; the D3D buffer is a fake COM object whose Release
// (IUnknown slot 2) counts calls. A held buffer must be released exactly once and cleared; an empty one left alone;
// the other members untouched.
#include <stdio.h>
#include "fable_index_buffer.h"

static int g_fail, g_release;
static void* g_released;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static long __stdcall FakeQueryInterface(void*, const void*, void**) { return -1; }
static unsigned long __stdcall FakeAddRef(void*) { return 2; }
static unsigned long __stdcall FakeRelease(void* self) { ++g_release; g_released = self; return 0; }

int main()
{
    static void* vtable[3] = { (void*)&FakeQueryInterface, (void*)&FakeAddRef, (void*)&FakeRelease };
    void* fake[2] = { vtable, 0 };
    unsigned long raw[5] = { 0x11111111u, 0, 0x300, 4, 0x55555555u };
    raw[1] = reinterpret_cast<unsigned long>(fake);
    CIndexBuffer* buffer = reinterpret_cast<CIndexBuffer*>(raw);

    buffer->Uninitialise();
    CHECK(g_release == 1 && g_released == fake);
    CHECK(raw[1] == 0);
    CHECK(raw[0] == 0x11111111u && raw[2] == 0x300 && raw[3] == 4 && raw[4] == 0x55555555u);
    buffer->Uninitialise();
    CHECK(g_release == 1 && raw[1] == 0);

    if (g_fail) { printf("INDEX_BUFFER_UNINITIALISE_FAIL %d\n", g_fail); return 1; }
    printf("INDEX_BUFFER_UNINITIALISE_OK\n");
    return 0;
}
