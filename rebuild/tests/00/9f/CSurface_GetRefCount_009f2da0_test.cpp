// CSurface::GetRefCount (0x009F2DA0): AddRef, then return Release's count; 0 without a surface.
// Drives the landed member through a fake COM vtable (slot 1 AddRef, slot 2 Release).
#include "fable_surface.h"
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
    union { double align; unsigned char bytes[sizeof(CSurface)]; } storage;
    memset(&storage, 0, sizeof(storage));
    CSurface* surface = reinterpret_cast<CSurface*>(storage.bytes);
    if (surface->GetRefCount() != 0 || addRefs != 0) { printf("FAIL null\n"); return 1; }
    surface->PD3DSurface = reinterpret_cast<IDirect3DSurface9*>(&com);
    if (surface->GetRefCount() != 42 || addRefs != 1) { printf("FAIL count\n"); return 1; }
    printf("PARITY_OK\n");
    return 0;
}
