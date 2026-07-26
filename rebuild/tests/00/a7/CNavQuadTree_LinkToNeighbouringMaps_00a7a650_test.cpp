#include <stdio.h>
#include <string.h>
#include "candidates/nav_quad_tree_transition_points_compiled.h"

class CNavigatorManager {};

#pragma pack(push, 1)
struct CNavQuadTreeLinkTestOverlay
{
    fable_u8 Unknown00[0x06];
    fable_u8 Initialising;
    float MapPosX;
    float MapPosY;
    fable_u8 Unknown0F;
    float MapWidth;
    float MapHeight;
    long MapIndex;
    fable_u32 NavigationLayersFirst;
    fable_u32 NavigationLayersLast;
    fable_u8 Unknown24[0x54];
    fable_u32 HeapFirst;
    fable_u32 HeapLast;
    fable_u8 Unknown80[0x04];
    fable_u8 HeapOrder;
    fable_u8 HeapDirty;
};
#pragma pack(pop)

struct EdgeCall
{
    float ThisMap;
    float AdjacentMap;
    const CNavigatorManager* Manager;
    fable_u8 Initialising;
};

float FableNavEdgeEpsilon = 0.5f;
static EdgeCall gEdgeCalls[4];
static int gEdgeCallCount;
static int gHeapCallCount;
static fable_u32 gHeapFirst;
static fable_u32 gHeapLast;
static unsigned int gHeapOrder;

void CNavQuadTree::ConnectVerticalMapEdge(
    float thisMapCoordinate,
    float adjacentMapCoordinate,
    const CNavigatorManager& navigatorManager)
{
    CNavQuadTreeLinkTestOverlay* self =
        reinterpret_cast<CNavQuadTreeLinkTestOverlay*>(this);
    if (gEdgeCallCount < 4)
    {
        gEdgeCalls[gEdgeCallCount].ThisMap = thisMapCoordinate;
        gEdgeCalls[gEdgeCallCount].AdjacentMap = adjacentMapCoordinate;
        gEdgeCalls[gEdgeCallCount].Manager = &navigatorManager;
        gEdgeCalls[gEdgeCallCount].Initialising = self->Initialising;
    }
    ++gEdgeCallCount;
}

void FABLE_FASTCALL Vector_MakeHeap_SoundPair(
    fable_u32 first,
    fable_u32 last,
    unsigned int order)
{
    ++gHeapCallCount;
    gHeapFirst = first;
    gHeapLast = last;
    gHeapOrder = order;
}

int main()
{
    CNavQuadTree tree;
    CNavigatorManager manager;
    CNavQuadTreeLinkTestOverlay* self =
        reinterpret_cast<CNavQuadTreeLinkTestOverlay*>(&tree);
    memset(&tree, 0, sizeof(tree));

    self->Initialising = 1;
    tree.LinkToNeighbouringMaps(manager);
    if (gEdgeCallCount != 0 || gHeapCallCount != 0) return 1;
    if (self->Initialising != 0) return 2;

    self->MapPosX = 10.0f;
    self->MapPosY = 20.0f;
    self->MapWidth = 100.0f;
    self->MapHeight = 200.0f;
    self->NavigationLayersFirst = 0x1000;
    self->NavigationLayersLast = 0x1028;
    self->HeapFirst = 0x11111111;
    self->HeapLast = 0x22222222;
    self->HeapOrder = 7;
    self->HeapDirty = 1;
    tree.LinkToNeighbouringMaps(manager);

    if (gEdgeCallCount != 4) return 3;
    if (gEdgeCalls[0].ThisMap != 20.5f || gEdgeCalls[0].AdjacentMap != 19.5f) return 4;
    if (gEdgeCalls[1].ThisMap != 219.5f || gEdgeCalls[1].AdjacentMap != 220.5f) return 5;
    if (gEdgeCalls[2].ThisMap != 10.5f || gEdgeCalls[2].AdjacentMap != 9.5f) return 6;
    if (gEdgeCalls[3].ThisMap != 109.5f || gEdgeCalls[3].AdjacentMap != 110.5f) return 7;
    for (int index = 0; index < 4; ++index)
    {
        if (gEdgeCalls[index].Manager != &manager) return 8;
        if (gEdgeCalls[index].Initialising != 1) return 9;
    }
    if (gHeapCallCount != 1) return 10;
    if (gHeapFirst != 0x11111111 || gHeapLast != 0x22222222 || gHeapOrder != 7) return 11;
    if (self->HeapDirty != 0 || self->Initialising != 0) return 12;

    printf("NAV_QUAD_TREE_LINK_NEIGHBOURING_MAPS_TEST PASS\n");
    return 0;
}
