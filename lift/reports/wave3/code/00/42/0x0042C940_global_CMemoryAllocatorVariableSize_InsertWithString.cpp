#include <cstddef>
#include <cstdint>
#include <functional>

class CCharString;
class CMemoryAllocatorVariableSize;

struct CMemoryAllocatorVariableSize_NodeOverlay {
    std::byte _pad0[0x08];
    std::uint32_t field_08;
    std::uint32_t field_0C;
    CCharString key;
};

static_assert(offsetof(CMemoryAllocatorVariableSize_NodeOverlay, field_08) == 0x08);
static_assert(offsetof(CMemoryAllocatorVariableSize_NodeOverlay, field_0C) == 0x0C);
static_assert(offsetof(CMemoryAllocatorVariableSize_NodeOverlay, key) == 0x10);

struct CMemoryAllocatorVariableSize_TreeOverlay {
    std::uint32_t headerNode;
    std::uint32_t nodeCount;
    std::less<CCharString> comparator;
};

static_assert(offsetof(CMemoryAllocatorVariableSize_TreeOverlay, headerNode) == 0x00);
static_assert(offsetof(CMemoryAllocatorVariableSize_TreeOverlay, nodeCount) == 0x04);
static_assert(offsetof(CMemoryAllocatorVariableSize_TreeOverlay, comparator) == 0x08);

extern "C" std::uint32_t RBTree__GetMaxNode(std::uint32_t node);
extern "C" std::uint32_t* RBTree_InsertString(void* outPairLike, CCharString* key);

std::uint32_t CMemoryAllocatorVariableSize::GetNoAllocatedAreas();
void CMemoryAllocatorVariableSize::InsertNode(
    std::uint32_t* result,
    std::uint32_t insertLeftOf,
    std::uint32_t parentOrHint,
    CCharString* key,
    std::uint32_t existingNode);

extern "C" std::uint32_t* __thiscall CMemoryAllocatorVariableSize_InsertWithString(
    std::uint32_t* param_1,
    std::uint32_t* param_2,
    std::uint32_t param_3,
    CCharString* param_4)
{
    auto* const tree = reinterpret_cast<CMemoryAllocatorVariableSize_TreeOverlay*>(param_1);
    auto& less = tree->comparator;

    std::uint32_t uVar3 = tree->headerNode;
    std::uint32_t uVar5 = param_3;
    std::byte local_10[4];
    std::uint32_t local_c;
    bool local_5;

    if (param_3 == reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(uVar3)->field_08) {
        if (tree->nodeCount == 0) {
            auto* const puVar4 = RBTree_InsertString(local_10, param_4);
            *param_2 = *puVar4;
            return param_2;
        }

        bool bVar1 =
            less(*param_4, reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(param_3)->key);
        if (!bVar1) {
            bVar1 =
                less(reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(param_3)->key, *param_4);
            if (!bVar1) {
                *param_2 = param_3;
                return param_2;
            }

            uVar3 = reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->GetNoAllocatedAreas();
            if (uVar3 == tree->headerNode) {
                uVar3 = 0;
                reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                    param_2, uVar3, uVar5, param_4, param_3);
                return param_2;
            }

            bVar1 =
                less(*param_4, reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(uVar3)->key);
            if (!bVar1) {
                auto* const puVar4 = RBTree_InsertString(local_10, param_4);
                *param_2 = *puVar4;
                return param_2;
            }

            if (reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(param_3)->field_0C != 0) {
                uVar5 = uVar3;
                param_3 = 0;
                reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                    param_2, uVar3, uVar5, param_4, param_3);
                return param_2;
            }

            uVar3 = 0;
            reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                param_2, uVar3, uVar5, param_4, param_3);
            return param_2;
        }
    } else {
        if (param_3 == uVar3) {
            uVar5 = reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(uVar3)->field_0C;
            const bool bVar1 =
                less(reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(uVar5)->key, *param_4);
            if (bVar1) {
                uVar3 = 0;
                reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                    param_2, uVar3, uVar5, param_4, param_3);
                return param_2;
            }

            auto* const puVar4 = RBTree_InsertString(local_10, param_4);
            *param_2 = *puVar4;
            return param_2;
        }

        local_c = RBTree__GetMaxNode(param_3);
        bool bVar1 =
            less(*param_4, reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(param_3)->key);
        if ((!bVar1) ||
            !less(reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(local_c)->key, *param_4)) {
            local_c = reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->GetNoAllocatedAreas();
            local_5 = !bVar1;

            if (!bVar1) {
                local_5 =
                    less(reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(param_3)->key, *param_4);
                if (!local_5) {
                    *param_2 = param_3;
                    return param_2;
                }

                if ((local_c == tree->headerNode) ||
                    less(*param_4, reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(local_c)->key)) {
                    if (reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(param_3)->field_0C != 0) {
                        uVar3 = local_c;
                        uVar5 = local_c;
                        param_3 = 0;
                        reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                            param_2, uVar3, uVar5, param_4, param_3);
                        return param_2;
                    }

                    uVar3 = 0;
                    reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                        param_2, uVar3, uVar5, param_4, param_3);
                    return param_2;
                }
            }

            if (bVar1 == local_5) {
                *param_2 = param_3;
                return param_2;
            }

            auto* const puVar4 = RBTree_InsertString(local_10, param_4);
            *param_2 = *puVar4;
            return param_2;
        }

        if (reinterpret_cast<CMemoryAllocatorVariableSize_NodeOverlay*>(local_c)->field_0C == 0) {
            uVar3 = 0;
            uVar5 = local_c;
            param_3 = local_c;
            reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
                param_2, uVar3, uVar5, param_4, param_3);
            return param_2;
        }

        uVar3 = param_3;
        param_3 = 0;
        reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
            param_2, uVar3, uVar5, param_4, param_3);
        return param_2;
    }

    uVar3 = param_3;
    param_3 = 0;
    reinterpret_cast<CMemoryAllocatorVariableSize*>(param_1)->InsertNode(
        param_2, uVar3, uVar5, param_4, param_3);
    return param_2;
}