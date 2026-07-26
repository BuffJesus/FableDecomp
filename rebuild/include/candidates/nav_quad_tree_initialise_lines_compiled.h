#ifndef FABLETLC_NAV_QUAD_TREE_INITIALISE_LINES_COMPILED_H
#define FABLETLC_NAV_QUAD_TREE_INITIALISE_LINES_COMPILED_H

#include <list>
#include <vector>

#include "rebuild_abi.h"

struct C2DVector
{
    float X;
    float Y;
};

class C2DLineF
{
public:
    C2DVector Start;
    C2DVector End;
};

class C2DBoxI
{
public:
    long MinX;
    long MinY;
    long MaxX;
    long MaxY;
};

class CNavQuadTree
{
private:
    void InitialiseLines(
        std::vector<std::list<C2DLineF> >& lineLists,
        const std::vector<C2DLineF>& lines) const;
    void UpdateLines(
        std::vector<std::list<C2DLineF> >& lineLists,
        const std::vector<C2DLineF>& lines,
        const C2DBoxI& area) const;

public:
    void InitialiseLinesForTest(
        std::vector<std::list<C2DLineF> >& lineLists,
        const std::vector<C2DLineF>& lines) const
    {
        InitialiseLines(lineLists, lines);
    }

    fable_u8 Unknown00[0x10];
    volatile float MapWidth;
    volatile float MapHeight;
};

extern const float DAT_0123095c;

class TreeLineVectorAdapter
{
public:
    void EnsureCapacity(int count);
};

FABLE_STATIC_ASSERT(sizeof(C2DVector) == 0x08);
FABLE_STATIC_ASSERT(sizeof(C2DLineF) == 0x10);
FABLE_STATIC_ASSERT(sizeof(C2DBoxI) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapWidth) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapHeight) == 0x14);

#endif
