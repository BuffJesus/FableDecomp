#pragma optimize("s",on)
// CRegion::SetMiniMapRegionExitTextOffsetX @ 0x005ba51f
// Retail: swaps a pointer-sized member (offset 0) through a cdecl helper,
// writing the OLD value out through the first stack argument.
//   edi = this->m0;
//   this->m0 = helper(edi);
//   *out = edi;

int __cdecl RegionExitTextHelper(int p);  // 0x4292c0, cdecl, one arg, returns int

struct CRegion_MiniMap {
    int m0;  // offset 0x00: pointer/handle that gets swapped
};

// this-in-ecx setter. ret 8 => two dwords of stack args (out + one unused/ignored).
void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(
        CRegion_MiniMap* self, int /*edx*/, int* out, int /*ignored*/)
{
    int old = self->m0;
    self->m0 = RegionExitTextHelper(old);
    *out = old;
}