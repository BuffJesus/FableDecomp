#include <cstddef>
#include <cstdint>

struct CTCCarryableNodeOverlay {
    std::uint8_t type_byte;                  // 0x00
    std::byte _pad01[0x03];                  // 0x01
    CTCCarryableNodeOverlay* field_04;       // 0x04
    CTCCarryableNodeOverlay* field_08;       // 0x08
    CTCCarryableNodeOverlay* field_0C;       // 0x0C
};

static_assert(offsetof(CTCCarryableNodeOverlay, type_byte) == 0x00);
static_assert(offsetof(CTCCarryableNodeOverlay, field_04) == 0x04);
static_assert(offsetof(CTCCarryableNodeOverlay, field_08) == 0x08);
static_assert(offsetof(CTCCarryableNodeOverlay, field_0C) == 0x0C);
static_assert(sizeof(CTCCarryableNodeOverlay) == 0x10);

extern "C" void* TreeNode_AllocData(void* data_start);

static CTCCarryableNodeOverlay* CTCCarryable_CloneDeep(
    CTCCarryableNodeOverlay* param_1,
    CTCCarryableNodeOverlay* param_2)
{
    auto* puVar2 = static_cast<CTCCarryableNodeOverlay*>(
        TreeNode_AllocData(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(param_1) + 0x10)));
    const std::uint8_t uVar1 = param_1->type_byte;

    puVar2->field_08 = nullptr;
    puVar2->field_0C = nullptr;
    puVar2->type_byte = uVar1;
    puVar2->field_04 = param_2;

    if (param_1->field_0C != nullptr) {
        puVar2->field_0C = CTCCarryable_CloneDeep(param_1->field_0C, puVar2);
    }

    param_2 = puVar2;
    for (auto* puVar5 = param_1->field_08; puVar5 != nullptr; puVar5 = puVar5->field_08) {
        auto* puVar4 = static_cast<CTCCarryableNodeOverlay*>(
            TreeNode_AllocData(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(puVar5) + 0x10)));
        const std::uint8_t siblingType = puVar5->type_byte;

        puVar4->field_08 = nullptr;
        puVar4->field_0C = nullptr;
        puVar4->type_byte = siblingType;
        param_2->field_08 = puVar4;
        puVar4->field_04 = param_2;

        if (puVar5->field_0C != nullptr) {
            puVar4->field_0C = CTCCarryable_CloneDeep(puVar5->field_0C, puVar4);
        }

        param_2 = puVar4;
    }

    return puVar2;
}