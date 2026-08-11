#include <cstdio>

extern "C" void * __cdecl RegionXformPtr(void *p);

struct CRegion
{
    void *p;
};

void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(
    CRegion *self, int edx, void **out, void *unused);

// Local stub: pretend transform bumps the pointer by 0x10.
extern "C" void * __cdecl RegionXformPtr(void *p)
{
    return (char *)p + 0x10;
}

int main()
{
    int dummy = 0;
    CRegion r;
    r.p = (void *)&dummy;
    void *out = 0;

    CRegion_SetMiniMapRegionExitTextOffsetX(&r, 0, &out, (void *)0xABCD);

    bool ok = true;
    // old pointer returned via out
    if (out != (void *)&dummy) ok = false;
    // this->p advanced by 0x10
    if (r.p != (void *)((char *)&dummy + 0x10)) ok = false;

    if (ok) printf("SETX_SWAP_OK\n");
    else printf("SETX_SWAP_FAIL out=%p p=%p\n", out, r.p);
    return ok ? 0 : 1;
}