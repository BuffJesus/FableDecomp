// Behaviour fixture for CIndexBuffer::Unlock / GetSize / Sizeof (provider CIndexBuffer_Queries_00ac1f00.cpp), on raw
// storage whose PD3DIndexBuffer (+0x04) is a fake COM object (slot 12 Unlock, slot 13 GetDesc). Checked: Unlock unlocks
// that buffer once; GetSize returns the Size its description reports; Sizeof is that size plus 0x18; neither query
// unlocks or touches the object's own fields.
#include <stdio.h>
#include <string.h>
#include "fable_index_buffer.h"
#include "fable_d3d9_index_buffer.h"

static int g_fail, g_unlocks, g_descs; static const void* g_self; static UINT g_size;
static unsigned long bufferStorage[1]; static void* bufferMethods[16];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static HRESULT __stdcall BufferUnlock(IDirect3DIndexBuffer9* self) { ++g_unlocks; g_self = self; return 0; }
static HRESULT __stdcall BufferGetDesc(IDirect3DIndexBuffer9* self, D3DINDEXBUFFER_DESC* desc)
{
    ++g_descs; g_self = self; memset(desc, 0x7F, sizeof(*desc)); desc->Size = g_size; return 0;
}
static unsigned long Word(const void* p, unsigned o) { unsigned long v; memcpy(&v, (const unsigned char*)p + o, 4); return v; }

int main()
{
    bufferMethods[12] = (void*)BufferUnlock; bufferMethods[13] = (void*)BufferGetDesc;
    bufferStorage[0] = (unsigned long)bufferMethods;
    static unsigned char raw[0x14];
    memset(raw, 0x5A, sizeof(raw));
    void* buffer = bufferStorage; memcpy(raw + 4, &buffer, 4);
    CIndexBuffer* b = reinterpret_cast<CIndexBuffer*>(raw);

    b->Unlock();
    CHECK(g_unlocks == 1 && g_self == bufferStorage && g_descs == 0);
    g_size = 96;
    CHECK(b->GetSize() == 96 && g_descs == 1 && g_self == bufferStorage);
    g_size = 1000;
    CHECK(b->Sizeof() == 1000 + 0x18 && g_descs == 2);
    g_size = 0;
    CHECK(b->Sizeof() == 0x18 && b->GetSize() == 0 && g_descs == 4 && g_unlocks == 1);
    CHECK(Word(raw, 0) == 0x5A5A5A5Au && Word(raw, 8) == 0x5A5A5A5Au && Word(raw, 0x10) == 0x5A5A5A5Au);

    if (g_fail) { printf("INDEX_BUFFER_QUERIES_FAIL %d\n", g_fail); return 1; }
    printf("INDEX_BUFFER_QUERIES_OK\n");
    return 0;
}
