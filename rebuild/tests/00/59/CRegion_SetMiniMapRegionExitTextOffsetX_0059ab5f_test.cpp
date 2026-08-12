#include <cstdio>

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

// local stub for the extern helper: doubles the value
int __cdecl RegionHelper_4292c0(int a) { return a * 2 + 1; }

int main()
{
    CRegion r;
    r.f0 = 21;
    CRegion* ret = r.SetMiniMapRegionExitTextOffsetX();
    if (ret == &r && r.f0 == 43)
        printf("OFFSETX_OK\n");
    else
        printf("FAIL %d\n", r.f0);
    return (ret == &r && r.f0 == 43) ? 0 : 1;
}