#include <cstddef>
#include <cstdint>

struct CEngineSceneGrid_BoundingBoxNodeOverlay {
    void* at_0x0;
    CEngineSceneGrid_BoundingBoxNodeOverlay* at_0x4;
};
static_assert(offsetof(CEngineSceneGrid_BoundingBoxNodeOverlay, at_0x0) == 0x0);
static_assert(offsetof(CEngineSceneGrid_BoundingBoxNodeOverlay, at_0x4) == 0x4);
static_assert(sizeof(CEngineSceneGrid_BoundingBoxNodeOverlay) == 0x8);

struct CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay {
    CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay* at_0x0;
    std::uint32_t at_0x4;
    std::byte at_0x8;
};
static_assert(offsetof(CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay, at_0x0) == 0x0);
static_assert(offsetof(CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay, at_0x4) == 0x4);
static_assert(offsetof(CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay, at_0x8) == 0x8);

struct CEngineSceneGrid_BoundingBoxListOwnerOverlay {
    std::byte pad_0x0[0x4];
    CEngineSceneGrid_BoundingBoxNodeOverlay* at_0x4;
};
static_assert(offsetof(CEngineSceneGrid_BoundingBoxListOwnerOverlay, at_0x4) == 0x4);

struct CEngineSceneGrid_AllocBoundingBoxNode_ContextOverlay {
    CEngineSceneGrid_BoundingBoxNodeOverlay* __thiscall AllocBoundingBoxNode(void* source);
};

void __fastcall CEngineSceneGrid_InsertBoundingBoxNode(
    CEngineSceneGrid_AllocBoundingBoxNode_ContextOverlay* context,
    void*,
    CEngineSceneGrid_BoundingBoxListOwnerOverlay* owner,
    CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay* first,
    CEngineSceneGrid_InsertBoundingBoxNode_ListEntryOverlay* last,
    std::uint32_t /*unused_stack_arg*/)
{
    for (; first != last; first = first->at_0x0) {
        CEngineSceneGrid_BoundingBoxNodeOverlay* const newNode =
            context->AllocBoundingBoxNode(&first->at_0x8);
        CEngineSceneGrid_BoundingBoxNodeOverlay* const current = owner->at_0x4;

        newNode->at_0x0 = owner;
        newNode->at_0x4 = current;
        current->at_0x0 = newNode;
        owner->at_0x4 = newNode;
    }
}