#include "candidates/nav_quad_tree_transition_points_compiled.h"

void CNavQuadTree::JoinNavigationLayers()
{
    CNavigationTransitionPoint* transitionPoint = TransitionPointsFirst;
    CANavQuadTreeNode* localNode;
    if (transitionPoint != TransitionPointsLast)
    {
        do
        {
            CANavQuadTreeNode* node0 =
                GetNodeAt(&transitionPoint->Position, transitionPoint->LayerIndex0);
            CANavQuadTreeNode* node1 =
                GetNodeAt(&transitionPoint->Position, transitionPoint->LayerIndex1);
            if (node0 != 0 && node1 != 0 &&
                node0->LinkDisabled == 0 && node1->LinkDisabled == 0)
            {
                localNode = node1;
                node0->Links.push_back(localNode);
                localNode = node0;
                node1->Links.push_back(localNode);
            }
            ++transitionPoint;
        } while (transitionPoint != TransitionPointsLast);
    }
}
