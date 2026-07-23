#include "candidates/nav_quad_tree_transition_points_compiled.h"

#pragma pack(push, 1)
struct CNavQuadTreeLinkOverlay
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

FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, Initialising) == 0x06);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, MapPosX) == 0x07);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, MapPosY) == 0x0B);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, MapWidth) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, MapHeight) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, NavigationLayersFirst) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, NavigationLayersLast) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, HeapFirst) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, HeapLast) == 0x7C);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, HeapOrder) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeLinkOverlay, HeapDirty) == 0x85);

extern float FableNavEdgeEpsilon;
void FABLE_FASTCALL Vector_MakeHeap_SoundPair(
    fable_u32 first,
    fable_u32 last,
    unsigned int order);

void CNavQuadTree::LinkToNeighbouringMaps(
    const CNavigatorManager& navigatorManager)
{
    CNavQuadTreeLinkOverlay* self =
        reinterpret_cast<CNavQuadTreeLinkOverlay*>(this);

    if (self->NavigationLayersFirst != self->NavigationLayersLast)
    {
        const float mapWidth = self->MapWidth;
        self->Initialising = 1;
        const float mapHeight = self->MapHeight;
        const float mapPosX = self->MapPosX;
        const float mapPosXCopy = self->MapPosX;
        const float mapPosY = self->MapPosY;

        ConnectVerticalMapEdge(
            self->MapPosY + FableNavEdgeEpsilon,
            self->MapPosY - FableNavEdgeEpsilon,
            navigatorManager);
        ConnectVerticalMapEdge(
            (mapHeight + mapPosY) - FableNavEdgeEpsilon,
            mapHeight + mapPosY + FableNavEdgeEpsilon,
            navigatorManager);
        ConnectVerticalMapEdge(
            mapPosX + FableNavEdgeEpsilon,
            mapPosX - FableNavEdgeEpsilon,
            navigatorManager);
        ConnectVerticalMapEdge(
            (mapWidth + mapPosXCopy) - FableNavEdgeEpsilon,
            mapWidth + mapPosXCopy + FableNavEdgeEpsilon,
            navigatorManager);

        if (self->HeapDirty != 0)
        {
            Vector_MakeHeap_SoundPair(
                self->HeapFirst,
                self->HeapLast,
                self->HeapOrder);
            self->HeapDirty = 0;
        }
    }

    self->Initialising = 0;
}
