// CEngineSceneGrid::GetCellBoundingBox @ 0x00b92d10
// bool __fastcall (CEngineSceneGrid*, C3DBoundingBox*, ulong, ulong)
// Forwards to a helper (0xb919c0) passing bbox, &b(out-slot), a, b.

struct C3DBoundingBox;

struct CEngineSceneGrid
{
    // helper method the game forwards to (0x00b919c0), __fastcall
    bool BuildCellBounds(C3DBoundingBox* bbox, unsigned long* pOut,
                         unsigned long a, unsigned long b);

    bool GetCellBoundingBox(C3DBoundingBox* bbox, unsigned long a, unsigned long b);
};

bool CEngineSceneGrid::GetCellBoundingBox(C3DBoundingBox* bbox,
                                          unsigned long a, unsigned long b)
{
    return BuildCellBounds(bbox, &b, a, b);
}