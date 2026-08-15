#include <cstdio>

// Self-contained behaviour model of CRegionMinimap::InitialiseRegionMinimapSize.
struct CRegion { int id; };

struct CWorldMap {
    virtual char TestRegion(CRegion* r) { return (char)(r->id % 2); } // accept odd ids
    virtual int  GetRegionExtent(CRegion* r) { return r->id * 10; }
};

struct CRegionMinimap {
    int accum;
    void SetRegionMapExtents(int v) { accum += v; }
    void InitialiseRegionMinimapSize(CWorldMap* worldMap, CRegion** begin, CRegion** end);
};

static bool g_freed = false;
static void FreeArrayBuf(void* p) { g_freed = true; }

// Model the loop body identically (slots resolved as the two virtuals above).
void CRegionMinimap::InitialiseRegionMinimapSize(CWorldMap* worldMap, CRegion** begin, CRegion** end)
{
    int count = (int)(end - begin);
    for (int i = 0; i < count; ++i) {
        CRegion* r = begin[i];
        if (worldMap->TestRegion(r)) {
            int v = worldMap->GetRegionExtent(r);
            this->SetRegionMapExtents(v);
        }
    }
    if (begin) FreeArrayBuf(begin);
}

int main() {
    CRegion nodes[5] = {{0},{1},{2},{3},{4}};
    CRegion* items[5];
    for (int k = 0; k < 5; ++k) items[k] = nodes + k;
    CWorldMap wm;
    CRegionMinimap mm; mm.accum = 0;
    mm.InitialiseRegionMinimapSize(&wm, items, items + 5);
    // odd ids 1,3 accepted -> extents 10 + 30 = 40
    if (mm.accum == 40 && g_freed) printf("PARITYOK_66cc40\n");
    else printf("FAIL accum=%d freed=%d\n", mm.accum, (int)g_freed);
    return 0;
}