#include <cstddef>
#include <cstdint>

namespace
{
#pragma pack(push, 1)
    struct CANavQuadTreeNodeOverlay
    {
        std::byte Pad00[0x0C];
        std::uint8_t SizeIndex; // +0x0C
        std::byte Pad0D[0x02];
        std::uint8_t SplitFlag; // +0x0F
    };

    struct CNavConnectorNodeRefOverlay
    {
        std::int32_t RefCount;    // +0x00
        void (__cdecl* OnDie)();  // +0x04
        CNavConnectorNode* Value; // +0x08
    };

    struct CNavConnectorNodeEntryOverlay
    {
        std::uint32_t Key;                // +0x00
        CNavConnectorNode* ConnectorNode; // +0x04
        CNavConnectorNodeRefOverlay* Ref; // +0x08
    };

    struct CNavQuadTreeConnectVerticalMapEdgeOverlay
    {
        std::byte Pad00[0x07];
        float MapPosX;                      // +0x07
        float MapPosY;                      // +0x0B
        float MapWidth;                     // +0x10
        float MapHeight;                    // +0x14
        std::int32_t MapIndex;              // +0x18
        std::int32_t NavigationLayersFirst; // +0x1C
        std::int32_t NavigationLayersLast;  // +0x20
        std::byte Pad24[0x58];
        CNavConnectorNodeEntryOverlay* ConnectorEntriesCurrent; // +0x7C
        CNavConnectorNodeEntryOverlay* ConnectorEntriesEnd;     // +0x80
        std::uint8_t Byte84;                                     // +0x84
        std::uint8_t Byte85;                                     // +0x85
    };
#pragma pack(pop)

    static_assert(offsetof(CANavQuadTreeNodeOverlay, SizeIndex) == 0x0C);
    static_assert(offsetof(CANavQuadTreeNodeOverlay, SplitFlag) == 0x0F);

    static_assert(sizeof(CNavConnectorNodeRefOverlay) == 0x0C);
    static_assert(offsetof(CNavConnectorNodeRefOverlay, RefCount) == 0x00);
    static_assert(offsetof(CNavConnectorNodeRefOverlay, OnDie) == 0x04);
    static_assert(offsetof(CNavConnectorNodeRefOverlay, Value) == 0x08);

    static_assert(sizeof(CNavConnectorNodeEntryOverlay) == 0x0C);
    static_assert(offsetof(CNavConnectorNodeEntryOverlay, Key) == 0x00);
    static_assert(offsetof(CNavConnectorNodeEntryOverlay, ConnectorNode) == 0x04);
    static_assert(offsetof(CNavConnectorNodeEntryOverlay, Ref) == 0x08);

    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, MapPosX) == 0x07);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, MapPosY) == 0x0B);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, MapWidth) == 0x10);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, MapHeight) == 0x14);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, MapIndex) == 0x18);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, NavigationLayersFirst) == 0x1C);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, NavigationLayersLast) == 0x20);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, ConnectorEntriesCurrent) == 0x7C);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, ConnectorEntriesEnd) == 0x80);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, Byte84) == 0x84);
    static_assert(offsetof(CNavQuadTreeConnectVerticalMapEdgeOverlay, Byte85) == 0x85);

    extern float DAT_0123078c;
    extern float DAT_0122f59c;
    extern float DAT_0129dcac[];

    CANavQuadTreeNode* __thiscall GetNodeAt(
        const CNavQuadTree* self,
        const C2DVector* position,
        long layerIndex);

    void __thiscall Vector_InsertN_12byte(
        void* self,
        CNavConnectorNodeEntryOverlay* insertAt,
        const CNavConnectorNodeEntryOverlay* value,
        const float* adjacentMapXSource,
        int count,
        int unknown);
}

void __thiscall CNavQuadTree::ConnectVerticalMapEdge(
    float this_map_x,
    float adjacent_map_x,
    CNavigatorManager& nav_manager)
{
    auto* const self = reinterpret_cast<CNavQuadTreeConnectVerticalMapEdgeOverlay*>(this);

    const float scan_min = self->MapPosX;
    const float scan_max = self->MapWidth + self->MapPosX;
    const std::int32_t layer_bytes = self->NavigationLayersLast - self->NavigationLayersFirst;
    const std::int32_t layer_bias = layer_bytes >> 0x1F;

    std::uint32_t layer_index = 0;
    if (layer_bytes / 0x28 + layer_bias != layer_bias)
    {
        const float layer_scan_start = scan_min + DAT_0123078c;

        do
        {
            float scan = layer_scan_start;
            std::uint32_t current_layer = layer_index;

            if (scan < scan_max)
            {
                do
                {
                    C2DVector adjacent_position{};
                    adjacent_position.x = scan;
                    adjacent_position.y = adjacent_map_x;

                    CANavQuadTreeNode* const adjacent_node = CNavigatorManager::GetNodeAt(
                        &nav_manager,
                        &adjacent_position,
                        static_cast<unsigned char>(current_layer));

                    if (adjacent_node == nullptr)
                    {
                        scan = scan + DAT_0122f59c;
                    }
                    else
                    {
                        const float adjacent_step =
                            DAT_0129dcac[reinterpret_cast<CANavQuadTreeNodeOverlay*>(adjacent_node)->SizeIndex];

                        C2DVector this_position{};
                        this_position.x = scan;
                        this_position.y = this_map_x;

                        CANavQuadTreeNode* const this_node =
                            GetNodeAt(this, &this_position, current_layer);

                        if (reinterpret_cast<CANavQuadTreeNodeOverlay*>(adjacent_node)->SplitFlag == 0)
                        {
                            if (reinterpret_cast<CANavQuadTreeNodeOverlay*>(this_node)->SplitFlag == 0)
                            {
                                const auto raw_call =
                                    reinterpret_cast<int(__thiscall*)(CANavQuadTreeNode*)>(
                                        *reinterpret_cast<std::uint32_t*>(
                                            *reinterpret_cast<std::uint32_t*>(adjacent_node) + 0x14));
                                const std::uint32_t connector_key =
                                    (raw_call(adjacent_node) >> 0x10) & 0xFFFF;

                                auto* connector_storage =
                                    static_cast<CNavConnectorNode*>(::operator new(0x08));
                                CNavConnectorNode* connector_node = nullptr;
                                CNavConnectorNodeRefOverlay* ref = nullptr;

                                if (connector_storage != nullptr)
                                {
                                    connector_node = CNavConnectorNode::CNavConnectorNode(
                                        connector_storage,
                                        this_node,
                                        adjacent_node);
                                    if (connector_node != nullptr)
                                    {
                                        ref = static_cast<CNavConnectorNodeRefOverlay*>(::operator new(0x0C));
                                        if (ref != nullptr)
                                        {
                                            ref->RefCount = 1;
                                            ref->OnDie = CTCMapwho::OnDie;
                                            ref->Value = connector_node;
                                        }
                                    }
                                }

                                if (ref != nullptr)
                                {
                                    ref->RefCount = ref->RefCount + 1;
                                }

                                CNavConnectorNodeEntryOverlay stack_entry{};
                                stack_entry.Key = connector_key;
                                stack_entry.ConnectorNode = connector_node;
                                stack_entry.Ref = ref;

                                CNavConnectorNodeEntryOverlay* const current = self->ConnectorEntriesCurrent;
                                if (current == self->ConnectorEntriesEnd)
                                {
                                    Vector_InsertN_12byte(
                                        reinterpret_cast<std::byte*>(this) + 0x78,
                                        current,
                                        &stack_entry,
                                        &adjacent_map_x,
                                        1,
                                        1);
                                }
                                else
                                {
                                    if (current != nullptr)
                                    {
                                        current->Key = connector_key;
                                        current->ConnectorNode = connector_node;
                                        current->Ref = ref;
                                        if (ref != nullptr)
                                        {
                                            ref->RefCount = ref->RefCount + 1;
                                        }
                                    }

                                    self->ConnectorEntriesCurrent =
                                        reinterpret_cast<CNavConnectorNodeEntryOverlay*>(
                                            reinterpret_cast<std::byte*>(current) + 0x0C);
                                }

                                if (stack_entry.Ref != nullptr)
                                {
                                    stack_entry.Ref->RefCount = stack_entry.Ref->RefCount - 1;
                                    if (stack_entry.Ref->RefCount == 0)
                                    {
                                        stack_entry.Ref->OnDie();
                                        operator delete(stack_entry.Ref);
                                    }
                                }

                                stack_entry.ConnectorNode = nullptr;
                                stack_entry.Ref = nullptr;
                                self->Byte85 = 1;

                                if (ref != nullptr)
                                {
                                    ref->RefCount = ref->RefCount - 1;
                                    if (ref->RefCount == 0)
                                    {
                                        ref->OnDie();
                                        operator delete(ref);
                                    }
                                }

                                const float this_step =
                                    DAT_0129dcac[reinterpret_cast<CANavQuadTreeNodeOverlay*>(this_node)->SizeIndex];

                                const float* step = &adjacent_step;
                                if (this_step <= adjacent_step)
                                {
                                    step = &this_step;
                                }

                                scan = scan + *step;
                                current_layer = layer_index;
                            }
                            else
                            {
                                scan = scan + DAT_0129dcac[
                                    reinterpret_cast<CANavQuadTreeNodeOverlay*>(this_node)->SizeIndex];
                            }
                        }
                        else if (reinterpret_cast<CANavQuadTreeNodeOverlay*>(this_node)->SplitFlag == 0)
                        {
                            scan = adjacent_step + scan;
                        }
                        else
                        {
                            const float this_step =
                                DAT_0129dcac[reinterpret_cast<CANavQuadTreeNodeOverlay*>(this_node)->SizeIndex];

                            const float* step = &adjacent_step;
                            if (adjacent_step <= this_step)
                            {
                                step = &this_step;
                            }

                            scan = scan + *step;
                        }
                    }
                } while (scan < scan_max);
            }

            layer_index = current_layer + 1;
        } while (layer_index < static_cast<std::uint32_t>(layer_bytes / 0x28));
    }
}