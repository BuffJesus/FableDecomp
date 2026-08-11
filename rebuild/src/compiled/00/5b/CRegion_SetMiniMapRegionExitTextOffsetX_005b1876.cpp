#pragma optimize("s",on)
// Retail 0x005b1876 CRegion::SetMiniMapRegionExitTextOffsetX
// Despite the manifest name, the body swaps this->p through a cdecl transform,
// returning the old pointer via an out-param. arg1 (CCharString*) is unused.

extern "C" void * __cdecl RegionXformPtr(void *p);

struct CRegion
{
    void *p;
};

void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(
    CRegion *self, int /*edx*/, void **out, void * /*unused*/)
{
    void *old = self->p;
    self->p = RegionXformPtr(old);
    *out = old;
}