// Behaviour fixture for CVertexStreamConverter::CVertexStreamConverter (provider
// CVertexStreamConverter_CVertexStreamConverter_00ac44c0.cpp): constructing the stateless converter in place must not
// write its storage and must hand back the object (placement new returns the constructor's this).
#include <stdio.h>
#include <new>
#include "fable_vertex_stream_converter.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

int main()
{
    unsigned char storage[4] = { 0xA1, 0xB2, 0xC3, 0xD4 };
    CVertexStreamConverter* converter = new (storage) CVertexStreamConverter;
    CHECK(static_cast<void*>(converter) == static_cast<void*>(storage));
    CHECK(storage[0] == 0xA1 && storage[1] == 0xB2 && storage[2] == 0xC3 && storage[3] == 0xD4);
    CVertexStreamConverter onStack;
    (void)onStack;

    if (g_fail) { printf("VERTEX_STREAM_CONVERTER_CTOR_FAIL %d\n", g_fail); return 1; }
    printf("VERTEX_STREAM_CONVERTER_CTOR_OK\n");
    return 0;
}
