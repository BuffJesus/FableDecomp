#include <cstdio>

extern "C" int __cdecl SetMiniMapRegionExitTextOffsetX_helper(int p);

struct CRegion { int m0; };
struct CCharString { int h0; };

void __fastcall CRegion_SetMiniMapRegionExitTextOffsetX(CRegion* self, int edx, CCharString* out, float x);

// Stand-in for retail 0x4292c0: return old+1 so we can observe the swap.
extern "C" int __cdecl SetMiniMapRegionExitTextOffsetX_helper(int p) { return p + 100; }

int main()
{
    CRegion r; r.m0 = 42;
    CCharString out; out.h0 = -1;
    CRegion_SetMiniMapRegionExitTextOffsetX(&r, 0, &out, 3.5f);
    // old value (42) must land in out; member becomes helper(42)=142
    if (out.h0 == 42 && r.m0 == 142) {
        printf("OFFSETX_SWAP_OK\n");
        return 0;
    }
    printf("FAIL m0=%d out=%d\n", r.m0, out.h0);
    return 1;
}