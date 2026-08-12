#pragma optimize("s",on)
// Retail 0x0059ab5f CRegion::SetMiniMapRegionExitTextOffsetX
// Body: this->f0 = helper(this->f0); return this;
// helper @0x4292c0 is a cdecl free function taking one arg (pop ecx cleanup).

extern int __cdecl RegionHelper_4292c0(int a);

struct CRegion
{
    int f0;
    CRegion* SetMiniMapRegionExitTextOffsetX();
};

CRegion* CRegion::SetMiniMapRegionExitTextOffsetX()
{
    this->f0 = RegionHelper_4292c0(this->f0);
    return this;
}