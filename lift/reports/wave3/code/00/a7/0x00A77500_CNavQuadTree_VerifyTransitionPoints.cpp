#include <cstddef>

namespace
{
    struct CNavigationTransitionPointOverlay
    {
        C2DVector Position; // +0x00
        long LayerIndex0;   // +0x08
        long LayerIndex1;   // +0x0C
    };
    static_assert(sizeof(CNavigationTransitionPointOverlay) == 0x10);
    static_assert(offsetof(CNavigationTransitionPointOverlay, Position) == 0x00);
    static_assert(offsetof(CNavigationTransitionPointOverlay, LayerIndex0) == 0x08);
    static_assert(offsetof(CNavigationTransitionPointOverlay, LayerIndex1) == 0x0C);

    struct CNavQuadTreeVerifyTransitionPointsOverlay
    {
        std::byte Pad00[0x28];
        CNavigationTransitionPointOverlay* TransitionPointsFirst; // +0x28
        CNavigationTransitionPointOverlay* TransitionPointsLast;  // +0x2C
    };
    static_assert(offsetof(CNavQuadTreeVerifyTransitionPointsOverlay, TransitionPointsFirst) == 0x28);
    static_assert(offsetof(CNavQuadTreeVerifyTransitionPointsOverlay, TransitionPointsLast) == 0x2C);

    CANavQuadTreeNode* __thiscall GetNodeAt(
        const CNavQuadTree* self,
        const C2DVector* position,
        long layerIndex);
}

void CNavQuadTree::VerifyTransitionPoints() const
{
    const auto* const self = reinterpret_cast<const CNavQuadTreeVerifyTransitionPointsOverlay*>(this);

    CNavigationTransitionPointOverlay* transitionPoint = self->TransitionPointsFirst;
    if (transitionPoint != self->TransitionPointsLast)
    {
        do
        {
            GetNodeAt(this, &transitionPoint->Position, transitionPoint->LayerIndex0);
            GetNodeAt(this, &transitionPoint->Position, transitionPoint->LayerIndex1);
            transitionPoint = transitionPoint + 1;
        } while (transitionPoint != self->TransitionPointsLast);
    }
}