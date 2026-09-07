#ifndef FABLETLC_NAV_QUAD_TREE_INITIALISE_LINES_COMPILED_H
#define FABLETLC_NAV_QUAD_TREE_INITIALISE_LINES_COMPILED_H

#include <list>
#include <vector>

#include "rebuild_abi.h"

struct C2DVector
{
    union { float X; float x; };
    union { float Y; float y; };
};

class C2DBoxF;

class C2DLineF
{
public:
    bool IntersectsWith(const C2DBoxF* box) const;
    C2DVector Start;
    C2DVector End;
};

class C2DBoxF
{
public:
    float MinX;
    float MinY;
    float MaxX;
    float MaxY;
};

class C2DBoxI
{
public:
    long MinX;
    long MinY;
    long MaxX;
    long MaxY;
};

struct CNavLineCellLinks
{
    CNavLineCellLinks* Next;
    CNavLineCellLinks* Prev;
};

struct CNavLineCell
{
    CNavLineCellLinks* Head;
};

struct CNavLineCellVector
{
    CNavLineCell* First;
    CNavLineCell* Last;
    CNavLineCell* End;
};

#pragma pack(push, 1)
class CNavQuadTree
{
private:
    void InitialiseLines(
        std::vector<std::list<C2DLineF> >& lineLists,
        const std::vector<C2DLineF>& lines) const;
    void UpdateLines(
        CNavLineCellVector& lineLists,
        const std::vector<C2DLineF>& lines,
        const C2DBoxI& area) const;

public:
    void InitialiseLinesForTest(
        std::vector<std::list<C2DLineF> >& lineLists,
        const std::vector<C2DLineF>& lines) const
    {
        InitialiseLines(lineLists, lines);
    }

    void UpdateLinesForTest(
        CNavLineCellVector& lineLists,
        const std::vector<C2DLineF>& lines,
        const C2DBoxI& area) const
    {
        UpdateLines(lineLists, lines, area);
    }

    fable_u8 Unknown00[0x07];
    C2DVector MapPos;
    fable_u8 Pad0F;
    volatile float MapWidth;
    volatile float MapHeight;
};
#pragma pack(pop)

extern const float DAT_0123095c;
extern const float DAT_0122dcb4;
extern const float DAT_0125916c;

class TreeLineVectorAdapter
{
public:
    void EnsureCapacity(int count);
};

FABLE_STATIC_ASSERT(sizeof(C2DVector) == 0x08);
FABLE_STATIC_ASSERT(sizeof(C2DLineF) == 0x10);
FABLE_STATIC_ASSERT(sizeof(C2DBoxI) == 0x10);
FABLE_STATIC_ASSERT(sizeof(C2DBoxF) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CNavLineCell) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CNavLineCellVector) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapPos) == 0x07);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapWidth) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapHeight) == 0x14);

#endif
