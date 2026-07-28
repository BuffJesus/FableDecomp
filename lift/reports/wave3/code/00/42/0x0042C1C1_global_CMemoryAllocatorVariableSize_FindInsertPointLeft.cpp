#include <cstddef>

using ulong = unsigned long;

struct CMemoryAllocatorVariableSize;

struct RBNodeOverlay {
    ulong field_0x00;
    ulong field_0x04;
    ulong field_0x08;
    ulong field_0x0C;
    ulong field_0x10;
};

static_assert(offsetof(RBNodeOverlay, field_0x08) == 0x08);
static_assert(offsetof(RBNodeOverlay, field_0x0C) == 0x0C);
static_assert(offsetof(RBNodeOverlay, field_0x10) == 0x10);

ulong RBTree__GetMaxNode(ulong node);
ulong* RBTree_InsertLeft(ulong** insertPoint, CMemoryAllocatorVariableSize* area);
void BinaryTree_InsertNodeLeft(
    ulong* result,
    ulong leftLink,
    ulong parentLink,
    CMemoryAllocatorVariableSize* area,
    ulong rightLink);

struct CMemoryAllocatorVariableSize {
    static ulong GetNoAllocatedAreas(CMemoryAllocatorVariableSize* self);
};

ulong* __thiscall CMemoryAllocatorVariableSize_FindInsertPointLeft(
    ulong* param_1,
    ulong* param_2,
    ulong param_3,
    CMemoryAllocatorVariableSize* param_4)
{
    ulong uVar3 = param_1[0];
    ulong* local_c = param_1;
    ulong* puStack_8 = param_1;
    (void)puStack_8;

    if (param_3 == reinterpret_cast<RBNodeOverlay*>(uVar3)->field_0x08) {
        if (param_1[1] == 0) {
            ulong* puVar5 = RBTree_InsertLeft(&local_c, param_4);
            *param_2 = *puVar5;
            return param_2;
        }

        CMemoryAllocatorVariableSize* thisArea =
            reinterpret_cast<CMemoryAllocatorVariableSize*>(reinterpret_cast<RBNodeOverlay*>(param_3)->field_0x10);

        if (static_cast<long>(*reinterpret_cast<ulong*>(param_4)) < static_cast<long>(reinterpret_cast<ulong>(thisArea))) {
            uVar3 = param_3;
            ulong uVar4 = param_3;
            param_3 = 0;
            BinaryTree_InsertNodeLeft(param_2, uVar3, uVar4, param_4, param_3);
            return param_2;
        }

        if (static_cast<long>(*reinterpret_cast<ulong*>(param_4)) <= static_cast<long>(reinterpret_cast<ulong>(thisArea))) {
            *param_2 = param_3;
            return param_2;
        }

        uVar3 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(thisArea);
        ulong uVar4 = param_3;
        if (uVar3 != param_1[0]) {
            if (static_cast<long>(reinterpret_cast<RBNodeOverlay*>(uVar3)->field_0x10) <=
                static_cast<long>(*reinterpret_cast<ulong*>(param_4))) {
                ulong* puVar5 = RBTree_InsertLeft(&local_c, param_4);
                *param_2 = *puVar5;
                return param_2;
            }

            const long iVar1 = static_cast<long>(reinterpret_cast<RBNodeOverlay*>(param_3)->field_0x0C);
            if (iVar1 == 0) {
                uVar3 = 0;
                uVar4 = param_3;
            } else {
                uVar4 = uVar3;
                param_3 = 0;
            }

            BinaryTree_InsertNodeLeft(param_2, uVar3, uVar4, param_4, param_3);
            return param_2;
        }

        uVar3 = 0;
        BinaryTree_InsertNodeLeft(param_2, uVar3, uVar4, param_4, param_3);
        return param_2;
    }

    if (param_3 != uVar3) {
        ulong uVar4 = RBTree__GetMaxNode(param_3);
        const long iVar1 = static_cast<long>(*reinterpret_cast<ulong*>(param_4));
        const bool bVar2 =
            iVar1 < static_cast<long>(reinterpret_cast<RBNodeOverlay*>(param_3)->field_0x10);

        if ((!bVar2) || (iVar1 <= static_cast<long>(reinterpret_cast<RBNodeOverlay*>(uVar4)->field_0x10))) {
            uVar3 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(param_4);
            bool bVar6 = !bVar2;

            if (!bVar2) {
                bVar6 =
                    static_cast<long>(reinterpret_cast<RBNodeOverlay*>(param_3)->field_0x10) < iVar1;
                if (!bVar6) {
                    *param_2 = param_3;
                    return param_2;
                }

                if ((uVar3 == param_1[0]) ||
                    (iVar1 < static_cast<long>(reinterpret_cast<RBNodeOverlay*>(uVar3)->field_0x10))) {
                    const long leftLink =
                        static_cast<long>(reinterpret_cast<RBNodeOverlay*>(param_3)->field_0x0C);
                    if (leftLink == 0) {
                        uVar3 = 0;
                        uVar4 = param_3;
                    } else {
                        uVar4 = uVar3;
                        param_3 = 0;
                    }

                    BinaryTree_InsertNodeLeft(param_2, uVar3, uVar4, param_4, param_3);
                    return param_2;
                }
            }

            if (bVar2 == bVar6) {
                *param_2 = param_3;
                return param_2;
            }

            ulong* puVar5 = RBTree_InsertLeft(&local_c, param_4);
            *param_2 = *puVar5;
            return param_2;
        }

        if (static_cast<long>(reinterpret_cast<RBNodeOverlay*>(uVar4)->field_0x0C) == 0) {
            uVar3 = 0;
            param_3 = uVar4;
            BinaryTree_InsertNodeLeft(param_2, uVar3, param_3, param_4, param_3);
            return param_2;
        }

        uVar3 = param_3;
        const ulong parentLink = param_3;
        param_3 = 0;
        BinaryTree_InsertNodeLeft(param_2, uVar3, parentLink, param_4, param_3);
        return param_2;
    }

    const ulong uVar4 = reinterpret_cast<RBNodeOverlay*>(uVar3)->field_0x0C;
    if (static_cast<long>(*reinterpret_cast<ulong*>(param_4)) <=
        static_cast<long>(reinterpret_cast<RBNodeOverlay*>(uVar4)->field_0x10)) {
        ulong* puVar5 = RBTree_InsertLeft(&local_c, param_4);
        *param_2 = *puVar5;
        return param_2;
    }

    uVar3 = 0;
    BinaryTree_InsertNodeLeft(param_2, uVar3, uVar4, param_4, param_3);
    return param_2;
}