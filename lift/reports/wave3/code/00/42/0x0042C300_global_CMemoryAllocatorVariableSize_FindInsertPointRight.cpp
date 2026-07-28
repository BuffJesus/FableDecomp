#include <cstddef>
#include <cstdint>

class CMemoryAllocatorVariableSize;

struct FindInsertPointRightOverlay {
    std::uint32_t u00;
    std::uint32_t u04;
    std::uint32_t u08;
    std::uint32_t u0C;
    std::uint32_t u10;
};

static_assert(offsetof(FindInsertPointRightOverlay, u08) == 0x08);
static_assert(offsetof(FindInsertPointRightOverlay, u0C) == 0x0C);
static_assert(offsetof(FindInsertPointRightOverlay, u10) == 0x10);

struct CMemoryAllocatorVariableSizeKeyOverlay {
    std::int32_t valueAt00;
};

static_assert(offsetof(CMemoryAllocatorVariableSizeKeyOverlay, valueAt00) == 0x00);

extern std::uint32_t __cdecl RBTree__GetMaxNode(std::uint32_t node);
extern std::uint32_t* __cdecl RBTree_InsertRight(std::uint32_t** insertContext, CMemoryAllocatorVariableSize* value);
extern void __cdecl BinaryTree_InsertNodeRight(
    std::uint32_t* outNode,
    std::uint32_t leftOrNull,
    std::uint32_t parentNode,
    CMemoryAllocatorVariableSize* value,
    std::uint32_t rightOrNull);

class CMemoryAllocatorVariableSize {
public:
    std::uint32_t GetNoAllocatedAreas();
};

std::uint32_t* __thiscall CMemoryAllocatorVariableSize_FindInsertPointRight(
    std::uint32_t* param_1,
    std::uint32_t* param_2,
    std::uint32_t param_3,
    CMemoryAllocatorVariableSize* param_4)
{
    auto* const valueOverlay = reinterpret_cast<CMemoryAllocatorVariableSizeKeyOverlay*>(param_4);

    std::uint32_t uVar3 = *param_1;
    std::uint32_t* insertContext[2]{param_1, param_1};
    std::uint32_t uVar4;
    std::uint32_t* puVar5;
    std::int32_t iVar1;
    bool bVar2;
    bool bVar6;

    if (param_3 == reinterpret_cast<FindInsertPointRightOverlay*>(uVar3)->u08) {
        if (param_1[1] == 0) {
            goto LAB_0042c426;
        }

        auto* const thisPtr =
            reinterpret_cast<CMemoryAllocatorVariableSize*>(
                reinterpret_cast<FindInsertPointRightOverlay*>(param_3)->u10);

        if (valueOverlay->valueAt00 < static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(thisPtr))) {
            goto LAB_0042c32d;
        }

        if (valueOverlay->valueAt00 <= static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(thisPtr))) {
            goto LAB_0042c343;
        }

        uVar3 = thisPtr->GetNoAllocatedAreas();
        uVar4 = param_3;
        if (uVar3 != *param_1) {
            if (static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(uVar3)->u10) <=
                valueOverlay->valueAt00) {
                goto LAB_0042c426;
            }

            iVar1 = static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(param_3)->u0C);
            goto joined_r0x0042c402;
        }
    } else {
        if (param_3 != uVar3) {
            uVar4 = RBTree__GetMaxNode(param_3);
            iVar1 = valueOverlay->valueAt00;
            bVar2 = iVar1 < static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(param_3)->u10);

            if ((!bVar2) ||
                (iVar1 <= static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(uVar4)->u10))) {
                uVar3 = param_4->GetNoAllocatedAreas();
                bVar6 = !bVar2;

                if (!bVar2) {
                    bVar6 = static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(param_3)->u10) < iVar1;
                    if (!bVar6) {
                        goto LAB_0042c343;
                    }

                    if ((uVar3 == *param_1) ||
                        (iVar1 < static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(uVar3)->u10))) {
                        iVar1 = static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(param_3)->u0C);
                        goto joined_r0x0042c402;
                    }
                }

                if (bVar2 == bVar6) {
                    goto LAB_0042c343;
                }

                goto LAB_0042c426;
            }

            if (reinterpret_cast<FindInsertPointRightOverlay*>(uVar4)->u0C == 0) {
                uVar3 = 0;
                param_3 = uVar4;
                goto LAB_0042c32f;
            }

LAB_0042c32d:
            uVar3 = param_3;
            uVar4 = param_3;
            param_3 = 0;
            goto LAB_0042c32f;
        }

        uVar4 = reinterpret_cast<FindInsertPointRightOverlay*>(uVar3)->u0C;
        if (valueOverlay->valueAt00 <=
            static_cast<std::int32_t>(reinterpret_cast<FindInsertPointRightOverlay*>(uVar4)->u10)) {
            goto LAB_0042c426;
        }
    }

    uVar3 = 0;

LAB_0042c32f:
    BinaryTree_InsertNodeRight(param_2, uVar3, uVar4, param_4, param_3);
    return param_2;

joined_r0x0042c402:
    if (iVar1 == 0) {
        uVar3 = 0;
        uVar4 = param_3;
    } else {
        uVar4 = uVar3;
        param_3 = 0;
    }
    goto LAB_0042c32f;

LAB_0042c343:
    *param_2 = param_3;
    return param_2;

LAB_0042c426:
    puVar5 = RBTree_InsertRight(insertContext, param_4);
    *param_2 = *puVar5;
    return param_2;
}