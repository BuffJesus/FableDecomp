#include "candidates/nav_quad_tree_transition_points_compiled.h"

void CNavQuadTree::VerifyTransitionPoints() const
{
    CNavigationTransitionPoint* transitionPoint = TransitionPointsFirst;
    if (transitionPoint != TransitionPointsLast)
    {
        do
        {
            GetNodeAt(&transitionPoint->Position, transitionPoint->LayerIndex0);
            GetNodeAt(&transitionPoint->Position, transitionPoint->LayerIndex1);
            ++transitionPoint;
        } while (transitionPoint != TransitionPointsLast);
    }
}
