#include <cstddef>
#include <cstdint>

struct HeapRecordOverlay {
    std::int32_t value0;           // 0x00
    std::int32_t value4;           // 0x04
    std::int32_t* pointer_at_0x08; // 0x08
};

static_assert(sizeof(HeapRecordOverlay) == 0x0C);
static_assert(offsetof(HeapRecordOverlay, value0) == 0x00);
static_assert(offsetof(HeapRecordOverlay, value4) == 0x04);
static_assert(offsetof(HeapRecordOverlay, pointer_at_0x08) == 0x08);

std::int32_t __fastcall Heap_SiftDown_ContainmentVolume(
    std::int32_t value0,
    std::int32_t value4,
    std::int32_t* pointer_at_0x08,
    std::uint32_t param_5);

void __fastcall Heap_OrderAndSort(
    std::int32_t param_2,
    std::uint32_t param_3,
    std::uint32_t param_5);

void __fastcall Heap_SiftDown(
    std::int32_t* param_1,
    std::int32_t param_2,
    std::uint32_t param_3,
    std::int32_t param_4,
    std::uint32_t param_5)
{
    (void)param_3;

    const std::int32_t entry_param_2 = param_2;

    if (0x10 < ((param_2 - static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(param_1))) / 0x0C)) {
        while (param_4 != 0) {
            const std::int32_t iVar6 =
                (param_2 - static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(param_1))) / 0x18;

            std::int32_t iVar1 = param_1[iVar6 * 3];
            std::int32_t iVar2 = *param_1;
            --param_4;

            std::int32_t iVar3 = *reinterpret_cast<std::int32_t*>(param_2 - 0x0C);
            std::int32_t iVar5 = iVar1;
            std::int32_t* piVar4 = param_1 + iVar6 * 3;
            std::int32_t* piVar7 = param_1;

            if (iVar2 < iVar1) {
                iVar5 = iVar2;
                piVar4 = param_1;
                piVar7 = param_1 + iVar6 * 3;
                iVar2 = iVar1;
            }

            if (iVar3 <= iVar2) {
                piVar7 = piVar4;
                if (iVar5 < iVar3) {
                    piVar7 = reinterpret_cast<std::int32_t*>(param_2 - 0x0C);
                }
            }

            const auto* selected = reinterpret_cast<const HeapRecordOverlay*>(piVar7);
            iVar1 = selected->value0;
            iVar3 = selected->value4;
            piVar4 = selected->pointer_at_0x08;

            if (piVar4 != nullptr) {
                ++*piVar4;
            }

            param_2 = Heap_SiftDown_ContainmentVolume(iVar1, iVar3, piVar4, param_5);
            Heap_SiftDown(reinterpret_cast<std::int32_t*>(param_2), entry_param_2, 0, param_4, param_5);

            if (((param_2 - static_cast<std::int32_t>(reinterpret_cast<std::intptr_t>(param_1))) / 0x0C) < 0x11) {
                return;
            }
        }

        Heap_OrderAndSort(param_2, 0, param_5);
    }
}