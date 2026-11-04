// Behaviour fixture for CIndexBuffer::RelocateData / OnPostRelocation (provider CIndexBuffer_Relocation_00ac1f50.cpp).
// The index data lives in a D3D buffer, so a movable-pool move must leave the buffer (raw storage laid out like retail)
// and both addresses untouched. RelocateData is called non-virtually (the destructor and vtable are not linked); a
// __stdcall-shaped call with four arguments must also return with the stack balanced (retail ret 0x10).
#include <stdio.h>
#include <string.h>
#include "fable_index_buffer.h"

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

int main()
{
    unsigned long raw[5] = { 0x11111111u, 0x22222222u, 0x300, 4, 0x55555555u };
    unsigned long before[5];
    memcpy(before, raw, sizeof(raw));
    char oldBlock[16] = "old", newBlock[16] = "new";
    CIndexBuffer* buffer = reinterpret_cast<CIndexBuffer*>(raw);

    for (int i = 0; i < 64; ++i)   // an unbalanced callee-pops convention would corrupt the stack here
        buffer->CIndexBuffer::RelocateData(7, oldBlock, newBlock, 16);
    buffer->OnPostRelocation();
    CHECK(memcmp(raw, before, sizeof(raw)) == 0);
    CHECK(strcmp(oldBlock, "old") == 0 && strcmp(newBlock, "new") == 0);

    if (g_fail) { printf("INDEX_BUFFER_RELOCATION_FAIL %d\n", g_fail); return 1; }
    printf("INDEX_BUFFER_RELOCATION_OK\n");
    return 0;
}
