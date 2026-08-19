#pragma optimize("s",on)
struct CRegion { int field0; };
extern "C" int __cdecl RegionExitTextHelper(int v);
void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(CRegion* self, int edx_dummy, int* out, int unused)
{
    int old = self->field0;
    self->field0 = RegionExitTextHelper(old);
    *out = old;
}