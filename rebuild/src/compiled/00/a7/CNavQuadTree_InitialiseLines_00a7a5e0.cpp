#include "candidates/nav_quad_tree_initialise_lines_compiled.h"

void CNavQuadTree::InitialiseLines(
    std::vector<std::list<C2DLineF> >& lineLists,
    const std::vector<C2DLineF>& lines) const
{
    // Operand order is intentional: VC7.1 emits the retail x87 load/multiply
    // sequence when the global scale is written first.
    const long mapWidthCells = static_cast<long>(DAT_0123095c * MapWidth);
    const long mapHeightCells = static_cast<long>(DAT_0123095c * MapHeight);

    reinterpret_cast<TreeLineVectorAdapter*>(&lineLists)->EnsureCapacity(
        mapWidthCells * mapHeightCells);

    C2DBoxI area;
    area.MinX = 0;
    area.MinY = 0;
    area.MaxX = mapWidthCells;
    area.MaxY = mapHeightCells;
    UpdateLines(lineLists, lines, area);
}
