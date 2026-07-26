#include <stdio.h>
#include <string.h>

#include "candidates/nav_quad_tree_initialise_lines_compiled.h"

extern const float DAT_0123095c = 0.5f;

static int gUpdateCalls;
static const void* gLineLists;
static const void* gLines;
static C2DBoxI gArea;

void TreeLineVectorAdapter::EnsureCapacity(int count)
{
    reinterpret_cast<std::vector<std::list<C2DLineF> >*>(this)->resize(count);
}

void CNavQuadTree::UpdateLines(
    std::vector<std::list<C2DLineF> >& lineLists,
    const std::vector<C2DLineF>& lines,
    const C2DBoxI& area) const
{
    ++gUpdateCalls;
    gLineLists = &lineLists;
    gLines = &lines;
    gArea = area;
}

static int CheckEmptyLists(const std::vector<std::list<C2DLineF> >& lists)
{
    for (size_t index = 0; index < lists.size(); ++index)
    {
        if (!lists[index].empty()) return 0;
    }
    return 1;
}

int main()
{
    CNavQuadTree tree;
    std::vector<std::list<C2DLineF> > lineLists;
    std::vector<C2DLineF> lines;
    memset(&tree, 0, sizeof(tree));

    tree.MapWidth = 6.0f;
    tree.MapHeight = 4.0f;
    tree.InitialiseLinesForTest(lineLists, lines);

    if (lineLists.size() != 6 || !CheckEmptyLists(lineLists)) return 1;
    if (gUpdateCalls != 1 || gLineLists != &lineLists || gLines != &lines) return 2;
    if (gArea.MinX != 0 || gArea.MinY != 0 || gArea.MaxX != 3 || gArea.MaxY != 2) return 3;

    lineLists.resize(9);
    tree.MapWidth = 4.0f;
    tree.MapHeight = 2.0f;
    tree.InitialiseLinesForTest(lineLists, lines);
    if (lineLists.size() != 2 || !CheckEmptyLists(lineLists)) return 4;
    if (gUpdateCalls != 2 || gArea.MaxX != 2 || gArea.MaxY != 1) return 5;

    tree.MapWidth = 0.0f;
    tree.MapHeight = 0.0f;
    tree.InitialiseLinesForTest(lineLists, lines);
    if (!lineLists.empty()) return 6;
    if (gUpdateCalls != 3 || gArea.MaxX != 0 || gArea.MaxY != 0) return 7;

    printf("NAV_QUAD_TREE_INITIALISE_LINES_TEST PASS\n");
    return 0;
}
