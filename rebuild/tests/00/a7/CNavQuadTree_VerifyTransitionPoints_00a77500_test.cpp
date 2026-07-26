#include <stdio.h>
#include <string.h>
#include "candidates/nav_quad_tree_transition_points_compiled.h"

struct NavCall
{
    const C2DVector* Position;
    long LayerIndex;
};

static NavCall gCalls[8];
static int gCallCount;

CANavQuadTreeNode* CNavQuadTree::GetNodeAt(
    const C2DVector* position,
    long layerIndex) const
{
    if (gCallCount < 8)
    {
        gCalls[gCallCount].Position = position;
        gCalls[gCallCount].LayerIndex = layerIndex;
    }
    ++gCallCount;
    return 0;
}

int main()
{
    CNavQuadTree tree;
    CNavigationTransitionPoint points[3];
    memset(&tree, 0, sizeof(tree));
    memset(points, 0, sizeof(points));

    tree.TransitionPointsFirst = points;
    tree.TransitionPointsLast = points;
    tree.VerifyTransitionPoints();
    if (gCallCount != 0) return 1;

    points[0].LayerIndex0 = 2;
    points[0].LayerIndex1 = 5;
    points[1].LayerIndex0 = -1;
    points[1].LayerIndex1 = 9;
    points[2].LayerIndex0 = 0;
    points[2].LayerIndex1 = 7;
    tree.TransitionPointsLast = points + 3;
    tree.VerifyTransitionPoints();

    if (gCallCount != 6) return 2;
    if (gCalls[0].Position != &points[0].Position || gCalls[0].LayerIndex != 2) return 3;
    if (gCalls[1].Position != &points[0].Position || gCalls[1].LayerIndex != 5) return 4;
    if (gCalls[2].Position != &points[1].Position || gCalls[2].LayerIndex != -1) return 5;
    if (gCalls[3].Position != &points[1].Position || gCalls[3].LayerIndex != 9) return 6;
    if (gCalls[4].Position != &points[2].Position || gCalls[4].LayerIndex != 0) return 7;
    if (gCalls[5].Position != &points[2].Position || gCalls[5].LayerIndex != 7) return 8;

    printf("NAV_QUAD_TREE_VERIFY_TRANSITION_POINTS_TEST PASS\n");
    return 0;
}
