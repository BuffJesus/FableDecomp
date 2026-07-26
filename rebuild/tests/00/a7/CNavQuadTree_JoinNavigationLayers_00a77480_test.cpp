#include <stdio.h>
#include <string.h>
#include "candidates/nav_quad_tree_transition_points_compiled.h"

struct LinkCall
{
    const void* Vector;
    const CANavQuadTreeNode* Value;
};

static CANavQuadTreeNode* gLayerNodes[8];
static int gLookupCount;
static LinkCall gLinkCalls[8];
static int gLinkCallCount;

CANavQuadTreeNode* CNavQuadTree::GetNodeAt(
    const C2DVector*,
    long layerIndex) const
{
    ++gLookupCount;
    if (layerIndex < 0 || layerIndex >= 8) return 0;
    return gLayerNodes[layerIndex];
}

template <>
void CSmallVector<const CANavQuadTreeNode*, 8>::push_back(
    const CANavQuadTreeNode* const& value)
{
    if (gLinkCallCount < 8)
    {
        gLinkCalls[gLinkCallCount].Vector = this;
        gLinkCalls[gLinkCallCount].Value = value;
    }
    ++gLinkCallCount;
}

int main()
{
    CNavQuadTree tree;
    CNavigationTransitionPoint points[3];
    CANavQuadTreeNode nodes[4];
    memset(&tree, 0, sizeof(tree));
    memset(points, 0, sizeof(points));
    memset(nodes, 0, sizeof(nodes));
    memset(gLayerNodes, 0, sizeof(gLayerNodes));

    tree.TransitionPointsFirst = points;
    tree.TransitionPointsLast = points;
    tree.JoinNavigationLayers();
    if (gLookupCount != 0 || gLinkCallCount != 0) return 1;

    points[0].LayerIndex0 = 0;
    points[0].LayerIndex1 = 1;
    points[1].LayerIndex0 = 2;
    points[1].LayerIndex1 = 7;
    points[2].LayerIndex0 = 2;
    points[2].LayerIndex1 = 3;
    gLayerNodes[0] = &nodes[0];
    gLayerNodes[1] = &nodes[1];
    gLayerNodes[2] = &nodes[2];
    gLayerNodes[3] = &nodes[3];
    nodes[2].LinkDisabled = 1;
    tree.TransitionPointsLast = points + 3;
    tree.JoinNavigationLayers();

    if (gLookupCount != 6) return 2;
    if (gLinkCallCount != 2) return 3;
    if (gLinkCalls[0].Vector != &nodes[0].Links || gLinkCalls[0].Value != &nodes[1]) return 4;
    if (gLinkCalls[1].Vector != &nodes[1].Links || gLinkCalls[1].Value != &nodes[0]) return 5;

    printf("NAV_QUAD_TREE_JOIN_NAVIGATION_LAYERS_TEST PASS\n");
    return 0;
}
