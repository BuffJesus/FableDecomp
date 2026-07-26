#include <cmath>
#include <cstddef>
#include <list>
#include <vector>

namespace
{
    struct C2DBoxIOverlay
    {
        int MinX;
        int MinY;
        int MaxX;
        int MaxY;
    };
    static_assert(sizeof(C2DBoxIOverlay) == 0x10);
    static_assert(offsetof(C2DBoxIOverlay, MinX) == 0x0);
    static_assert(offsetof(C2DBoxIOverlay, MinY) == 0x4);
    static_assert(offsetof(C2DBoxIOverlay, MaxX) == 0x8);
    static_assert(offsetof(C2DBoxIOverlay, MaxY) == 0xC);

    [[nodiscard]] static int Ftol2(const float value)
    {
        return static_cast<int>(std::nearbyint(value));
    }

    void __thiscall Vector_EnsureCapacity_TreeLines(
        std::vector<std::list<C2DLineF>>* self,
        int capacity);
}

void CNavQuadTree::InitialiseLines(
    std::vector<std::list<C2DLineF>>& lineLists,
    const std::vector<C2DLineF>& lines) const
{
    const int mapWidthCells = Ftol2(MapWidth * DAT_0123095c);
    const int mapHeightCells = Ftol2(MapHeight * DAT_0123095c);

    Vector_EnsureCapacity_TreeLines(&lineLists, mapHeightCells * mapWidthCells);

    C2DBoxIOverlay area{};
    area.MaxX = mapWidthCells;
    area.MaxY = mapHeightCells;

    UpdateLines(
        lineLists,
        lines,
        reinterpret_cast<const C2DBoxI&>(area));
}