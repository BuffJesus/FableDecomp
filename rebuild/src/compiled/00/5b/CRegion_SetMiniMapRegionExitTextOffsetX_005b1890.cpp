#pragma optimize("s",on)
// CRegion::SetMiniMapRegionExitTextOffsetX @ 005b1890
// Body per retail disasm: swaps a pointer-sized member through a cdecl helper.
//   edi = self->m0; self->m0 = helper(edi); *out = edi;
// The float param is present in the ABI (ret 8) but unused by the body.

extern "C" int __cdecl SetMiniMapRegionExitTextOffsetX_helper(int p);

struct CRegion {
    int m0;
};

struct CCharString {
    int h0;
};

void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(CRegion* self, int edx, CCharString* out, float x)
{
    (void)edx;
    (void)x;
    int old = self->m0;
    self->m0 = SetMiniMapRegionExitTextOffsetX_helper(old);
    out->h0 = old;
}