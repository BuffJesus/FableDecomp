#include <cstddef>
#include <cstdint>

struct CloneTreeNodeOverlay {
    std::uint8_t field_0x00;
    std::byte pad_0x01[3];
    CloneTreeNodeOverlay* field_0x04;
    CloneTreeNodeOverlay* field_0x08;
    CloneTreeNodeOverlay* field_0x0C;
    std::byte field_0x10[1];
};

static_assert(offsetof(CloneTreeNodeOverlay, field_0x04) == 0x04);
static_assert(offsetof(CloneTreeNodeOverlay, field_0x08) == 0x08);
static_assert(offsetof(CloneTreeNodeOverlay, field_0x0C) == 0x0C);
static_assert(offsetof(CloneTreeNodeOverlay, field_0x10) == 0x10);

struct CTCCarryableCloneTreeContext;

using TreeNodeAllocDataFn = void*(__thiscall*)(CTCCarryableCloneTreeContext*, void*);
using CloneTreeFn =
    CloneTreeNodeOverlay*(__thiscall*)(CTCCarryableCloneTreeContext*, CloneTreeNodeOverlay*, CloneTreeNodeOverlay*);

void* __thiscall TreeNode_AllocData(CTCCarryableCloneTreeContext* context, void* data);

CloneTreeNodeOverlay* __thiscall CTCCarryable_CloneTree(
    CTCCarryableCloneTreeContext* context,
    CloneTreeNodeOverlay* param_1,
    CloneTreeNodeOverlay* param_2)
{
    auto* const puVar2 =
        static_cast<CloneTreeNodeOverlay*>(TreeNode_AllocData(context, &param_1->field_0x10));
    const std::uint8_t uVar1 = param_1->field_0x00;

    puVar2->field_0x08 = nullptr;
    puVar2->field_0x0C = nullptr;
    puVar2->field_0x00 = uVar1;
    puVar2->field_0x04 = param_2;

    if (param_1->field_0x0C != nullptr) {
        puVar2->field_0x0C = CTCCarryable_CloneTree(context, param_1->field_0x0C, puVar2);
    }

    param_2 = puVar2;
    for (auto* puVar5 = param_1->field_0x08; puVar5 != nullptr; puVar5 = puVar5->field_0x08) {
        auto* const puVar4 =
            static_cast<CloneTreeNodeOverlay*>(TreeNode_AllocData(context, &puVar5->field_0x10));
        const std::uint8_t siblingValue = puVar5->field_0x00;

        puVar4->field_0x08 = nullptr;
        puVar4->field_0x0C = nullptr;
        puVar4->field_0x00 = siblingValue;
        param_2->field_0x08 = puVar4;
        puVar4->field_0x04 = param_2;

        if (puVar5->field_0x0C != nullptr) {
            puVar4->field_0x0C = CTCCarryable_CloneTree(context, puVar5->field_0x0C, puVar4);
        }

        param_2 = puVar4;
    }

    return puVar2;
}