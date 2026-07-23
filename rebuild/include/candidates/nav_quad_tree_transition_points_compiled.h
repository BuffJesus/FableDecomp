#ifndef FABLETLC_NAV_QUAD_TREE_TRANSITION_POINTS_COMPILED_H
#define FABLETLC_NAV_QUAD_TREE_TRANSITION_POINTS_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

template <typename T, int Capacity>
class CSmallVector
{
public:
    void push_back(const T& value);
};

class CANavQuadTreeNode
{
public:
    fable_u8 Unknown00[0x0F];
    fable_u8 LinkDisabled;
    fable_u8 Unknown10[0x04];
    CSmallVector<const CANavQuadTreeNode*, 8> Links;
};

class CNavigatorManager;

struct C2DVector
{
    float X;
    float Y;
};

struct CNavigationTransitionPoint
{
    C2DVector Position;
    long LayerIndex0;
    long LayerIndex1;
};

class CNavQuadTree
{
public:
    CANavQuadTreeNode* GetNodeAt(const C2DVector* position, long layerIndex) const;
    void ConnectVerticalMapEdge(
        float thisMapCoordinate,
        float adjacentMapCoordinate,
        const CNavigatorManager& navigatorManager);
    void JoinNavigationLayers();
    void LinkToNeighbouringMaps(const CNavigatorManager& navigatorManager);
    void VerifyTransitionPoints() const;

    fable_u8 Unknown00[0x28];
    CNavigationTransitionPoint* TransitionPointsFirst;
    CNavigationTransitionPoint* TransitionPointsLast;
};

FABLE_STATIC_ASSERT(sizeof(C2DVector) == 0x08);
FABLE_STATIC_ASSERT(sizeof(CNavigationTransitionPoint) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavigationTransitionPoint, LayerIndex0) == 0x08);
FABLE_STATIC_ASSERT(offsetof(CNavigationTransitionPoint, LayerIndex1) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, LinkDisabled) == 0x0F);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, Links) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, TransitionPointsFirst) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, TransitionPointsLast) == 0x2C);

#endif
