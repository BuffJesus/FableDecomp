// CTexture::GetRefCount (0x009F9D60): AddRef, then return Release's count; 0 without a texture.
// Drives the landed member through a fake COM vtable (slot 1 AddRef, slot 2 Release).
#include "fable_surface_types.h"
#include <stdio.h>
#include <string.h>

static int addRefs;
static unsigned long __stdcall FakeAddRef(void*) { ++addRefs; return 7; }
static unsigned long __stdcall FakeRelease(void*) { return 42; }

int main()
{
    void* table[32] = {0};
    table[1] = (void*)FakeAddRef;
    table[2] = (void*)FakeRelease;
    struct { void** vtbl; } com = { table };
    union { double align; unsigned char bytes[sizeof(CTexture)]; } storage;
    memset(&storage, 0, sizeof(storage));
    CTexture* texture = reinterpret_cast<CTexture*>(storage.bytes);
    if (texture->GetRefCount() != 0 || addRefs != 0) { printf("FAIL null\n"); return 1; }
    texture->PD3DTexture = reinterpret_cast<IDirect3DTexture9*>(&com);
    if (texture->GetRefCount() != 42 || addRefs != 1) { printf("FAIL count\n"); return 1; }
    printf("GETREFCOUNT_OK\n");
    return 0;
}
