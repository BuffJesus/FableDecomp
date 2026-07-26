#include <cstddef>
#include <cstdint>

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

    struct CNavQuadTreeJoinNavigationLayersOverlay
    {
        std::byte Pad00[0x28];
        CNavigationTransitionPointOverlay* TransitionPointsFirst; // +0x28
        CNavigationTransitionPointOverlay* TransitionPointsLast;  // +0x2C
    };
    static_assert(offsetof(CNavQuadTreeJoinNavigationLayersOverlay, TransitionPointsFirst) == 0x28);
    static_assert(offsetof(CNavQuadTreeJoinNavigationLayersOverlay, TransitionPointsLast) == 0x2C);

    struct CNavQuadTreeNodeJoinOverlay
    {
        std::byte Pad00[0x0F];
        std::uint8_t Byte0F; // +0x0F
    };
    static_assert(offsetof(CNavQuadTreeNodeJoinOverlay, Byte0F) == 0x0F);

    template <typename T, std::size_t N>
    struct CSmallVector;

    template <>
    struct CSmallVector<CANavQuadTreeNode const*, 8>
    {
        void __thiscall push_back(CANavQuadTreeNode const* const* value);
    };

    CANavQuadTreeNode* __thiscall GetNodeAt(
        CNavQuadTree* self,
        const C2DVector* position,
        long layerIndex);
}

void CNavQuadTree::JoinNavigationLayers()
{
    auto* const self = reinterpret_cast<CNavQuadTreeJoinNavigationLayersOverlay*>(this);

    CNavigationTransitionPointOverlay* transitionPoint = self->TransitionPointsFirst;
    CANavQuadTreeNode* local_4 = reinterpret_cast<CANavQuadTreeNode*>(this);

    if (transitionPoint != self->TransitionPointsLast)
    {
        do
        {
            CANavQuadTreeNode* const node0 =
                GetNodeAt(this, &transitionPoint->Position, transitionPoint->LayerIndex0);
            CANavQuadTreeNode* const node1 =
                GetNodeAt(this, &transitionPoint->Position, transitionPoint->LayerIndex1);

            if ((((node0 != nullptr) && (node1 != nullptr)) &&
                 (reinterpret_cast<const CNavQuadTreeNodeJoinOverlay*>(node0)->Byte0F == 0)) &&
                (reinterpret_cast<const CNavQuadTreeNodeJoinOverlay*>(node1)->Byte0F == 0))
            {
                local_4 = node1;
                reinterpret_cast<CSmallVector<CANavQuadTreeNode const*, 8>*>(
                    reinterpret_cast<std::byte*>(node0) + 0x14)
                    ->push_back(&local_4);

                local_4 = node0;
                reinterpret_cast<CSmallVector<CANavQuadTreeNode const*, 8>*>(
                    reinterpret_cast<std::byte*>(node1) + 0x14)
                    ->push_back(&local_4);
            }

            transitionPoint = transitionPoint + 1;
        } while (transitionPoint != self->TransitionPointsLast);
    }
}