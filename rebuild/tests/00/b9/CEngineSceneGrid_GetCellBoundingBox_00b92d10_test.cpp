#include <cstdio>

struct C3DBoundingBox { float lo[3]; float hi[3]; };

struct CEngineSceneGrid
{
    int cellW;
    bool BuildCellBounds(C3DBoundingBox* bbox, unsigned long* pOut,
                         unsigned long a, unsigned long b);
    bool GetCellBoundingBox(C3DBoundingBox* bbox, unsigned long a, unsigned long b);
};

// Local definition of the forwarded-to helper for the standalone test.
bool CEngineSceneGrid::BuildCellBounds(C3DBoundingBox* bbox, unsigned long* pOut,
                                       unsigned long a, unsigned long b)
{
    // Record what we received; write back through the out-slot.
    if (!bbox || !pOut) return false;
    bbox->lo[0] = (float)a;
    bbox->hi[0] = (float)b;
    *pOut = a + b + this->cellW;
    return true;
}

bool CEngineSceneGrid::GetCellBoundingBox(C3DBoundingBox* bbox,
                                          unsigned long a, unsigned long b)
{
    return BuildCellBounds(bbox, &b, a, b);
}

int main()
{
    CEngineSceneGrid g;
    g.cellW = 100;
    C3DBoundingBox bb;
    bool r = g.GetCellBoundingBox(&bb, 7, 9);

    bool ok = r && bb.lo[0] == 7.0f && bb.hi[0] == 9.0f;
    // Null bbox -> false
    ok = ok && (g.GetCellBoundingBox(0, 1, 2) == false);

    if (ok) printf("GETCELLBBOX_OK\n");
    else    printf("GETCELLBBOX_FAIL r=%d lo=%f hi=%f\n", (int)r, bb.lo[0], bb.hi[0]);
    return ok ? 0 : 1;
}