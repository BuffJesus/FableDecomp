#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <vector>

namespace
{
    template <typename T>
    struct VectorRaw
    {
        T* First;
        T* Last;
        T* End;
    };
    static_assert(sizeof(VectorRaw<int>) == 0x0C);
    static_assert(offsetof(VectorRaw<int>, First) == 0x0);
    static_assert(offsetof(VectorRaw<int>, Last) == 0x4);
    static_assert(offsetof(VectorRaw<int>, End) == 0x8);

    template <typename T>
    [[nodiscard]] static T* AddBytes(T* ptr, std::ptrdiff_t bytes)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uint8_t*>(ptr) + bytes);
    }

    struct ListNodeOverlay
    {
        ListNodeOverlay* Next;
        ListNodeOverlay* Prev;
        std::uint32_t Value0;
        std::uint32_t Value1;
        std::uint32_t Value2;
        std::uint32_t Value3;
    };
    static_assert(sizeof(ListNodeOverlay) == 0x18);
    static_assert(offsetof(ListNodeOverlay, Next) == 0x0);
    static_assert(offsetof(ListNodeOverlay, Prev) == 0x4);

    struct CNavigationAreaInitLayerEntryOverlay
    {
        std::uint32_t Field0;
        std::uint32_t Field4;
        void* ListObject;
        std::uint32_t Field0C;
    };
    static_assert(sizeof(CNavigationAreaInitLayerEntryOverlay) == 0x10);
    static_assert(offsetof(CNavigationAreaInitLayerEntryOverlay, Field0) == 0x0);
    static_assert(offsetof(CNavigationAreaInitLayerEntryOverlay, Field4) == 0x4);
    static_assert(offsetof(CNavigationAreaInitLayerEntryOverlay, ListObject) == 0x8);
    static_assert(offsetof(CNavigationAreaInitLayerEntryOverlay, Field0C) == 0xC);

    struct CNavigationAreaInitLayerOverlay
    {
        VectorRaw<C2DLineF> Lines;                               // +0x00
        VectorRaw<CNavigationAreaInitLayerEntryOverlay> Entries; // +0x0C
    };
    static_assert(sizeof(CNavigationAreaInitLayerOverlay) == 0x18);
    static_assert(offsetof(CNavigationAreaInitLayerOverlay, Lines) == 0x0);
    static_assert(offsetof(CNavigationAreaInitLayerOverlay, Entries) == 0xC);

    struct CNavigationAreaInitOverlay
    {
        VectorRaw<CNavigationAreaInitLayerOverlay> Layers; // +0x00
    };
    static_assert(offsetof(CNavigationAreaInitOverlay, Layers) == 0x0);

    struct CNavigationLayerEntryOverlay
    {
        std::uint32_t Field0;
        std::uint32_t Field4;
        void* ListObject;
        std::uint32_t Field0C;
    };
    static_assert(sizeof(CNavigationLayerEntryOverlay) == 0x10);
    static_assert(offsetof(CNavigationLayerEntryOverlay, Field0) == 0x0);
    static_assert(offsetof(CNavigationLayerEntryOverlay, Field4) == 0x4);
    static_assert(offsetof(CNavigationLayerEntryOverlay, ListObject) == 0x8);
    static_assert(offsetof(CNavigationLayerEntryOverlay, Field0C) == 0xC);

    struct CNavigationLayerOverlay
    {
        VectorRaw<CANavQuadTreeNode*> Nodes;             // +0x00
        std::vector<std::list<C2DLineF>> LineLists;      // +0x0C
        VectorRaw<CNavigationLayerEntryOverlay> Entries; // +0x18
        std::uint8_t ReserveLog2;                        // +0x24
        std::uint8_t Dirty;                              // +0x25
        std::byte Pad26[0x02];
    };
    static_assert(sizeof(CNavigationLayerOverlay) == 0x28);
    static_assert(offsetof(CNavigationLayerOverlay, Nodes) == 0x0);
    static_assert(offsetof(CNavigationLayerOverlay, LineLists) == 0x0C);
    static_assert(offsetof(CNavigationLayerOverlay, Entries) == 0x18);
    static_assert(offsetof(CNavigationLayerOverlay, ReserveLog2) == 0x24);
    static_assert(offsetof(CNavigationLayerOverlay, Dirty) == 0x25);

    struct CNavQuadTreeInitialiseOverlay
    {
        std::byte Pad00[0x04];
        bool Initialised;                        // +0x04
        bool Activated;                          // +0x05
        bool Initialising;                       // +0x06
        C2DVector MapPos;                        // +0x07
        float MapWidth;                          // +0x10
        float MapHeight;                         // +0x14
        long MapIndex;                           // +0x18
        std::byte* NavigationLayersFirst;        // +0x1C
        std::byte* NavigationLayersLast;         // +0x20
        std::byte* NavigationLayersEnd;          // +0x24
        std::byte Pad28[0x28];
        CNavigationPosition* ActionPointsFirst;  // +0x50
        CNavigationPosition* ActionPointsLast;   // +0x54
        CNavigationPosition* ActionPointsEnd;    // +0x58
        std::byte Pad5C[0x0C];
        void* Guard68;                           // +0x68
        void* Guard6C;                           // +0x6C
    };
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, Initialising) == 0x06);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, MapPos) == 0x07);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, MapWidth) == 0x10);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, MapHeight) == 0x14);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, MapIndex) == 0x18);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, NavigationLayersFirst) == 0x1C);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, NavigationLayersLast) == 0x20);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, NavigationLayersEnd) == 0x24);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, ActionPointsFirst) == 0x50);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, ActionPointsLast) == 0x54);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, Guard68) == 0x68);
    static_assert(offsetof(CNavQuadTreeInitialiseOverlay, Guard6C) == 0x6C);

    [[nodiscard]] static CNavigationLayerOverlay* GetLayer(
        const CNavQuadTreeInitialiseOverlay* self,
        const int byteOffset)
    {
        return reinterpret_cast<CNavigationLayerOverlay*>(self->NavigationLayersFirst + byteOffset);
    }

    using RawDeletingDestructor = void(__thiscall*)(void*, int);

    extern "C" int __ftol2();

    void __cdecl GFGetTime();

    void __thiscall std__vector_assign_buffer(void* src);
    void __thiscall Vector_AssignFromRange_12ByteElements(void* src);
    void __thiscall Vector_Assign_CopyRange_16ByteElements_v2(void* src);
    void __thiscall Vector_Assign_CopyRange_16ByteElements(void* src);

    void* __thiscall std___Move(void* first, void* last, void* dst, void* scratch, int zero);
    void __thiscall std___Copy_impl(void* first, void* last, void* dst);
    void __thiscall std___Copy_impl_v5(void* first, void* last, void* dst, void* scratch, int zero);
    void __thiscall std___Move_backward(void* first, void* last, void* scratch);

    void __thiscall Vector_Assign_NavQuadTree(CNavQuadTree* self, int count);
    void __thiscall Vector_AssignCopy_NavQuadLayer(void* dst);
    void __thiscall Std_List_Construct_From_Iterator_Range(
        void* listHead,
        void* first,
        void* last,
        void* scratch);
    void __thiscall DestroyBinaryTreeRange(void* scratch);

    void __thiscall InitialiseLines(
        CNavQuadTree* self,
        std::vector<std::list<C2DLineF>>* lineLists,
        std::vector<C2DLineF>* lines);

    void __thiscall list_pair_init(void* entry);
    CNavigationLayerEntryOverlay* __thiscall std___Move_CNavigationLayerEntry(
        void* first,
        void* last,
        void* dst,
        void* scratch);

    void __thiscall Vector_ReserveLog2_NavPos_v2(
        CNavigationLayerEntryOverlay* self,
        std::uint32_t reserveLog2);

    void __thiscall Vector_Insert_Copies_ENavigatorType(
        VectorRaw<CANavQuadTreeNode*>* self,
        CANavQuadTreeNode** oldLast,
        unsigned int count,
        CANavQuadTreeNode* const* value);

    void __thiscall CalculateNeighbours(
        CNavQuadTree* self,
        VectorRaw<CANavQuadTreeNode*>* nodes);

    void __thiscall JoinNavigationLayers(CNavQuadTree* self);
    void __thiscall CNavQuadTree_InitializeWithEvents(long mapIndex);
    bool __thiscall IsActionPointReachable(CNavQuadTree* self, CNavigationPosition* point);
    int __thiscall GetPreferabilityCostForNode(CNavQuadTree* self, CANavQuadTreeNode* node);
    void __thiscall GetMemoryUsage(
        CNavQuadTree* self,
        unsigned long* a,
        unsigned long* b,
        unsigned long* c,
        unsigned long* d);

    extern float DAT_0129dd98;
    extern float _DAT_01238164;
    extern float _DAT_01230894;
    extern void* PTR__vector_deleting_destructor__0129dcd4[];
    extern void* PTR__vector_deleting_destructor__0129dda4[];
    extern CANavQuadTreeNode DAT_013d2d2c;
}

void CNavQuadTree::Initialise(const CNavigationAreaInit& navigationAreaInit)
{
    auto* const self = reinterpret_cast<CNavQuadTreeInitialiseOverlay*>(this);
    const auto* const init = reinterpret_cast<const CNavigationAreaInitOverlay*>(&navigationAreaInit);

    CBase local_85{};
    int local_84 = 0;
    unsigned int local_80 = 0;
    int local_7c = 0;
    ENavQuadTreeNodeType local_78 = static_cast<ENavQuadTreeNodeType>(0);
    CNavigationLayerEntryOverlay* local_74 = nullptr;
    float local_70 = 0.0f;
    float local_6c = 0.0f;
    void* local_68 = nullptr;
    CNavigationLayerEntryOverlay* local_64 = nullptr;
    CNavigationLayerEntryOverlay* local_60 = nullptr;
    CNavQuadTree* local_5c = this;
    int local_58 = 0;
    std::uint32_t local_54 = 0;
    std::uint32_t local_50 = 0;
    ListNodeOverlay* local_4c = nullptr;
    void* local_44 = nullptr;
    std::uint32_t local_40 = 0;
    std::uint32_t local_3c = 0;
    void* local_38 = nullptr;
    std::uint32_t local_34 = 0;
    std::uint32_t local_30 = 0;
    void* local_2c = nullptr;
    std::uint32_t local_28 = 0;
    std::uint32_t local_24 = 0;
    std::uint8_t local_1f = 0;
    VectorRaw<unsigned __int64> local_1c{};
    unsigned long uStack_10 = 0;
    unsigned long uStack_c = 0;
    unsigned long uStack_8 = 0;
    unsigned long uStack_4 = 0;

    self->Initialising = true;
    GFGetTime();

    std__vector_assign_buffer(reinterpret_cast<std::uint8_t*>(const_cast<CNavigationAreaInit*>(&navigationAreaInit)) + 0x24);
    Vector_AssignFromRange_12ByteElements(reinterpret_cast<std::uint8_t*>(const_cast<CNavigationAreaInit*>(&navigationAreaInit)) + 0x18);
    Vector_Assign_CopyRange_16ByteElements_v2(reinterpret_cast<std::uint8_t*>(const_cast<CNavigationAreaInit*>(&navigationAreaInit)) + 0x44);
    Vector_AssignFromRange_12ByteElements(reinterpret_cast<std::uint8_t*>(const_cast<CNavigationAreaInit*>(&navigationAreaInit)) + 0x50);
    Vector_Assign_CopyRange_16ByteElements(reinterpret_cast<std::uint8_t*>(const_cast<CNavigationAreaInit*>(&navigationAreaInit)) + 0x0C);

    local_68 = std___Move(self->NavigationLayersLast, self->NavigationLayersLast, self->NavigationLayersFirst, &local_85, 0);
    std___Copy_impl(local_68, self->NavigationLayersLast, &local_85);
    self->NavigationLayersLast = static_cast<std::byte*>(local_68);

    if (self->Guard68 != self->Guard6C)
    {
        __asm
        {
            fld dword ptr [self->MapWidth]
            fmul dword ptr [DAT_0129dd98]
            call __ftol2
            mov local_68, eax
        }

        __asm
        {
            fld dword ptr [self->MapHeight]
            fmul dword ptr [DAT_0129dd98]
            call __ftol2
            mov local_58, eax
        }

        const int layerCount = static_cast<int>(init->Layers.Last - init->Layers.First);
        Vector_Assign_NavQuadTree(this, layerCount);

        local_80 = 0;
        local_84 = 0;

        for (int layerInitByteOffset = 0;
             local_80 < static_cast<unsigned int>(layerCount) || layerInitByteOffset == 0;
             layerInitByteOffset += 0x18)
        {
            local_7c = layerInitByteOffset;
            local_44 = nullptr;
            local_6c = self->MapPos.y + _DAT_01238164;
            local_40 = 0;
            local_3c = 0;
            local_70 = self->MapPos.x + _DAT_01238164;
            local_38 = nullptr;
            local_34 = 0;
            local_30 = 0;
            local_2c = nullptr;
            local_28 = 0;
            local_24 = 0;
            local_1f = 0;

            Vector_AssignCopy_NavQuadLayer(&local_44);
            std___Move_backward(local_2c, reinterpret_cast<void*>(local_28), &local_85);

            if (local_2c != nullptr)
            {
                std::free(local_2c);
            }

            DestroyBinaryTreeRange(&local_85);

            if (local_38 != nullptr)
            {
                std::free(local_38);
            }
            if (local_44 != nullptr)
            {
                std::free(local_44);
            }

            if (local_80 < static_cast<unsigned int>(layerCount))
            {
                CNavigationLayerOverlay* const layer = GetLayer(self, local_84);
                const CNavigationAreaInitLayerOverlay* const srcLayer =
                    reinterpret_cast<const CNavigationAreaInitLayerOverlay*>(
                        reinterpret_cast<const std::uint8_t*>(init->Layers.First) + layerInitByteOffset);

                InitialiseLines(
                    this,
                    &layer->LineLists,
                    reinterpret_cast<std::vector<C2DLineF>*>(const_cast<VectorRaw<C2DLineF>*>(&srcLayer->Lines)));

                const unsigned int entryCount =
                    static_cast<unsigned int>(srcLayer->Entries.Last - srcLayer->Entries.First);

                local_78 = static_cast<ENavQuadTreeNodeType>(0);
                int entryByteOffset = 0;

                if (entryCount != 0)
                {
                    do
                    {
                        const auto* const srcEntry =
                            reinterpret_cast<const CNavigationAreaInitLayerEntryOverlay*>(
                                reinterpret_cast<const std::uint8_t*>(srcLayer->Entries.First) + entryByteOffset);

                        local_54 = srcEntry->Field0;
                        local_50 = srcEntry->Field4;

                        auto* tempListHead = static_cast<ListNodeOverlay*>(std::malloc(0x18));
                        tempListHead->Next = tempListHead;
                        tempListHead->Prev = tempListHead;
                        local_4c = tempListHead;

                        auto* puVar2 = *reinterpret_cast<void***>(
                            const_cast<void*>(srcEntry->ListObject));
                        Std_List_Construct_From_Iterator_Range(
                            tempListHead,
                            *puVar2,
                            puVar2,
                            &local_85);

                        if (layer->Entries.Last == layer->Entries.End)
                        {
                            const int oldCount =
                                static_cast<int>(layer->Entries.Last - layer->Entries.First);

                            local_60 = reinterpret_cast<CNavigationLayerEntryOverlay*>(oldCount);
                            local_74 = reinterpret_cast<CNavigationLayerEntryOverlay*>(1);

                            CNavigationLayerEntryOverlay** growth = &local_74;
                            if (local_60 != nullptr)
                            {
                                growth = &local_60;
                            }

                            local_64 = reinterpret_cast<CNavigationLayerEntryOverlay*>(
                                reinterpret_cast<int>(*growth) + oldCount);

                            if (local_64 == nullptr)
                            {
                                local_74 = nullptr;
                            }
                            else
                            {
                                local_74 = static_cast<CNavigationLayerEntryOverlay*>(
                                    std::malloc(static_cast<std::size_t>(reinterpret_cast<int>(local_64)) * 0x10));
                            }

                            CNavigationLayerEntryOverlay* const inserted =
                                std___Move_CNavigationLayerEntry(
                                    layer->Entries.First,
                                    layer->Entries.Last,
                                    local_74,
                                    &local_85);

                            if (inserted != nullptr)
                            {
                                list_pair_init(&local_54);
                                tempListHead = local_4c;
                            }

                            local_60 = AddBytes(inserted, 0x10);

                            std___Move_backward(layer->Entries.First, layer->Entries.Last, &local_85);

                            if (layer->Entries.First != nullptr)
                            {
                                std::free(layer->Entries.First);
                            }

                            layer->Entries.First = local_74;
                            layer->Entries.Last = local_60;
                            layer->Entries.End =
                                AddBytes(local_74, reinterpret_cast<int>(local_64) * 0x10);
                        }
                        else
                        {
                            if (layer->Entries.Last != nullptr)
                            {
                                list_pair_init(&local_54);
                                tempListHead = local_4c;
                            }

                            layer->Entries.Last = AddBytes(layer->Entries.Last, 0x10);
                        }

                        ListNodeOverlay* node = tempListHead->Next;
                        while (node != tempListHead)
                        {
                            ListNodeOverlay* const next = node->Next;
                            std::free(node);
                            node = next;
                        }

                        tempListHead->Next = tempListHead;
                        tempListHead->Prev = tempListHead;
                        std::free(tempListHead);

                        layer->Dirty = 1;
                        local_78 = static_cast<ENavQuadTreeNodeType>(static_cast<unsigned int>(local_78) + 1U);
                        entryByteOffset += 0x10;
                        this = local_5c;
                    } while (static_cast<unsigned int>(local_78) < entryCount);
                }

                if (layer->Dirty != 0)
                {
                    Vector_ReserveLog2_NavPos_v2(layer->Entries.First, layer->ReserveLog2);
                    layer->Dirty = 0;
                }
            }

            CNavigationLayerOverlay* const layer = GetLayer(self, local_84);
            const unsigned int requiredNodeCount =
                static_cast<unsigned int>(local_58 * reinterpret_cast<int>(local_68));
            const unsigned int existingNodeCount =
                static_cast<unsigned int>(layer->Nodes.Last - layer->Nodes.First);

            CANavQuadTreeNode* local_64_node = nullptr;
            if (requiredNodeCount < existingNodeCount)
            {
                layer->Nodes.Last = layer->Nodes.First + requiredNodeCount;
            }
            else
            {
                Vector_Insert_Copies_ENavigatorType(
                    &layer->Nodes,
                    layer->Nodes.Last,
                    requiredNodeCount - existingNodeCount,
                    &local_64_node);
            }

            unsigned int nodeIndex = 0;
            if ((layer->Nodes.Last - layer->Nodes.First) != 0)
            {
                do
                {
                    local_78 = static_cast<ENavQuadTreeNodeType>(0);
                    local_1c.First = nullptr;
                    local_1c.Last = nullptr;
                    local_1c.End = nullptr;

                    CBase* createdNode = static_cast<CBase*>(::operator new(0x20));
                    if (createdNode != nullptr)
                    {
                        NHeroInformationScreens::CBase::CBase(createdNode);
                        *reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x04) = local_70;
                        *reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x08) = local_6c;
                        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x10) = 0;
                        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x14) = 0;
                        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x18) = 0;
                        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x1C) = 0;
                        createdNode[0x0C] = static_cast<CBase>(0);
                        createdNode[0x0D] = static_cast<CBase>(local_80);
                        createdNode[0x0E] = static_cast<CBase>(1);
                        createdNode[0x0F] = static_cast<CBase>(0);
                        *reinterpret_cast<void***>(createdNode) = PTR__vector_deleting_destructor__0129dcd4;

                        reinterpret_cast<CNavQuadTreeNode*>(createdNode)->CNavQuadTreeNode::Initialise(
                            this,
                            &local_78,
                            &local_1c);
                    }

                    layer->Nodes.First[nodeIndex] = reinterpret_cast<CANavQuadTreeNode*>(createdNode);

                    if (local_78 == static_cast<ENavQuadTreeNodeType>(2))
                    {
                        auto* const oldNode = layer->Nodes.First[nodeIndex];
                        if (oldNode != nullptr)
                        {
                            reinterpret_cast<RawDeletingDestructor>((**reinterpret_cast<void***>(oldNode)))(oldNode, 1);
                        }
                        layer->Nodes.First[nodeIndex] = &DAT_013d2d2c;
                    }
                    else if (local_78 == static_cast<ENavQuadTreeNodeType>(1))
                    {
                        local_85 = static_cast<CBase>(GetPreferabilityCostForNode(this, layer->Nodes.First[nodeIndex]));

                        auto* const oldNode = layer->Nodes.First[nodeIndex];
                        if (oldNode != nullptr)
                        {
                            reinterpret_cast<RawDeletingDestructor>((**reinterpret_cast<void***>(oldNode)))(oldNode, 1);
                        }

                        createdNode = static_cast<CBase*>(::operator new(0x20));
                        if (createdNode != nullptr)
                        {
                            NHeroInformationScreens::CBase::CBase(createdNode);
                            *reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x04) = local_70;
                            *reinterpret_cast<float*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x08) = local_6c;
                            createdNode[0x0D] = static_cast<CBase>(local_80);
                            createdNode[0x0C] = static_cast<CBase>(0);
                            createdNode[0x0E] = static_cast<CBase>(1);
                            createdNode[0x0F] = static_cast<CBase>(0);
                            *reinterpret_cast<void***>(createdNode) = PTR__vector_deleting_destructor__0129dda4;
                            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x10) = 0;
                            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(createdNode) + 0x14) = 0;
                            createdNode[0x18] = static_cast<CBase>(0);
                            createdNode[0x1C] = local_85;
                        }

                        layer->Nodes.First[nodeIndex] = reinterpret_cast<CANavQuadTreeNode*>(createdNode);
                    }

                    ++nodeIndex;
                    if ((static_cast<int>(nodeIndex) % reinterpret_cast<int>(local_68)) == 0)
                    {
                        local_70 = self->MapPos.x + _DAT_01238164;
                        local_6c = local_6c + _DAT_01230894;
                    }
                    else
                    {
                        local_70 = local_70 + _DAT_01230894;
                    }

                    if (local_1c.First != nullptr)
                    {
                        std::free(local_1c.First);
                    }
                } while (nodeIndex < static_cast<unsigned int>(layer->Nodes.Last - layer->Nodes.First));
            }

            CalculateNeighbours(this, &layer->Nodes);
            ++local_80;
            local_84 += 0x28;
        }

        JoinNavigationLayers(this);
        CNavQuadTree_InitializeWithEvents(self->MapIndex);

        CNavigationPosition* point = self->ActionPointsFirst;
        if (point != self->ActionPointsLast)
        {
            CNavigationPosition* next = AddBytes(point, 0x0C);
            do
            {
                if (IsActionPointReachable(this, point))
                {
                    point = AddBytes(point, 0x0C);
                    next = AddBytes(next, 0x0C);
                }
                else
                {
                    if (next != self->ActionPointsLast)
                    {
                        std___Copy_impl_v5(next, self->ActionPointsLast, point, &local_85, 0);
                    }
                    self->ActionPointsLast = AddBytes(self->ActionPointsLast, -0x0C);
                }
            } while (point != self->ActionPointsLast);
        }

        GetMemoryUsage(this, &uStack_c, &uStack_10, &uStack_4, &uStack_8);
    }

    self->Initialising = false;
}