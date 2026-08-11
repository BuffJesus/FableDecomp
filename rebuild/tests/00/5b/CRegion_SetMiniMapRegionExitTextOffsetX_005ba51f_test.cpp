#include <cstdio>

int __cdecl RegionExitTextHelper(int p) { return p + 100; }

struct CRegion_MiniMap { int m0; };

void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(
        CRegion_MiniMap* self, int, int* out, int);

int main() {
    CRegion_MiniMap r;
    r.m0 = 7;
    int old = -1;
    CRegion_SetMiniMapRegionExitTextOffsetX(&r, 0, &old, 0);
    // old field returned out; member replaced by helper(old)
    if (old == 7 && r.m0 == 107) {
        std::printf("OFFSETX_SWAP_OK\n");
        return 0;
    }
    std::printf("FAIL old=%d m0=%d\n", old, r.m0);
    return 1;
}